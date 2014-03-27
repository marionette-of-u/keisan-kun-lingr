#include <unistd.h>
#include <netdb.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <string>
#include <map>

#define R 0
#define W 1

char pipebuf[0x1000];

int popen2(const char *command, int *pfd){
  int pipe_c2p[2], pipe_p2c[2];
  pid_t pid;

  if(pipe(pipe_c2p) < 0){
    perror("popen2");
    return -1;
  }

  if(pipe(pipe_p2c) < 0){
    perror("popen2");
    close(pipe_c2p[R]), close(pipe_c2p[W]);
    return -1;
  }

  if((pid = fork()) < 0){
    perror("popen2");
    close(pipe_c2p[R]), close(pipe_c2p[W]);
    close(pipe_p2c[R]), close(pipe_p2c[W]);
    return -1;
  }

  if(pid == 0){
    close(pipe_p2c[W]), close(pipe_c2p[R]);
    dup2(pipe_p2c[R], STDIN_FILENO);
    dup2(pipe_c2p[W], STDOUT_FILENO);
    close(pipe_p2c[R]), close(pipe_c2p[W]);
    if(execlp("sh", "sh", "-c", command, NULL) < 0){
      perror("popen2");
      close(pipe_p2c[R]), close(pipe_c2p[W]);
      exit(-1);
    }
  }

  close(pipe_p2c[R]), close(pipe_c2p[W]);
  pfd[R] = pipe_c2p[R];
  pfd[W] = pipe_p2c[W];

  return pid;
}

size_t read_line(int rfd, char terminal_symbol = '\n'){
  size_t i;
  char c;
  i = 0;
  do{
    if(i > sizeof(pipebuf)){
      sprintf(pipebuf, ">_<\n");
      i = 4;
      break;
    }
    read(rfd, (void*)&c, 1);
    pipebuf[i++] = c;
  }while(c != terminal_symbol);
  pipebuf[i] = '\0';
  printf(pipebuf);
  return i;
}

int pfd[2];
int count = 0;

size_t w2(std::string command){
  read_line(pfd[R], '=');
  write(pfd[W], command.c_str(), command.size());
  read_line(pfd[R], '\n');
  return read_line(pfd[R], '\n');
}

int main(int argc, char *argv[]) {
  popen2("wolfram", pfd);
  for(int i = 0; i < 4; ++i){ read_line(pfd[R]); }
  while(true){
    sleep(0);

    {
      std::ifstream ifile("input.txt");
      if(ifile.fail()){ continue; }
      std::string str;
      ifile >> str;
      if(ifile.fail() || str.empty()){ continue; }

      ++count;

      if(count >= 128){ 
        count = 0;
        static const char etx[] = { 3, 0 }, quit[] = "quit";
        w2(etx);
        w2(quit);
        popen2("wolfram", pfd);
        for(int i = 0; i < 4; ++i){ read_line(pfd[R]); }
      }

      w2(str);
    }

    { std::ofstream ofile("input.txt", std::ios::out | std::ios::trunc); }
  }
  return 0;
}
