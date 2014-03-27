#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory>
#include <fstream>
#include <iostream>

int main(){
  int content_length = atoi(getenv("CONTENT_LENGTH"));
  if(content_length <= 0 || content_length > 4096){ return 1; }
  char *buffer = new char[content_length + 1];
  std::unique_ptr<char> buffer_scoped_guard(buffer);
  char c;
  int i = 0;
  for(; (c = getchar()) != EOF; ++i){
    if(c == '\r' || c == '\n'){ --i; continue; }
    buffer[i] = c;
  }
  buffer[i] = '\0';
  std::ofstream log("logfile.txt");
  log << buffer << std::endl;
  std::cout << "Content-type:text/html\r\n\r\ntest" << std::endl;

  /*
  while(true){
    sleep(0);
    {
      std::ofstream ofile("input.txt", std::ios::out | std::ios::trunc);
      if(ofile.fail()){ continue; }
      ofile << buffer;
      if(ofile.fail()){ continue; }
    }
    break;
  }
  */
  return 0;
}

