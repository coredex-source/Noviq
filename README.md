# Litecode
Just a simple interpreter based programming language written in C.

## Wiki
[Click Here To Go To The Wiki](https://github.com/coredex-source/litecode/blob/main/wiki.txt)

## Building and using
### Requirements:
  - GCC (Only GCC on windows.)
  - make
### Build using:
- MacOS/Linux
```
make all
```
- Windows
```
gcc -o litecode.exe litecode.c lexer/lexer_interpret.c lexer/lexer_display.c
```
### Run using:
- MacOS/Linux:
```
./litecode -e filename.ls
```
- Windows:
```
litecode.exe -e filename.ls
```
