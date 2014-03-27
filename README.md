keisan-kun-lingr
================

lingr.cpp
---------
"lingr" というバイナリ名で g++ でコンパイル, 生成.

background.cpp
--------------
適当なバイナリ名で g++ でコンパイル, 生成.
手動で裏で起動.

その後
------
"lingr" は POST された文字列データを input.txt に入力を書き出します.
"background" は input.txt ファイルの状況を監視し, 変更を検出すると裏で起動した wolfram に入力として中身を与え, std::cout で結果を出力します.
結果, 標準出力に wolfram で処理した内容が送られるのですが, <b>まだ動作確認を行っていません</b>.
