# Noviq
- The name Noviq is make up of two words: Nova(which means new) and unique.
- So, Nova + unique = Noviq.
- Noviq is just a simple interpreter based programming language written in C.

## Wiki
[Click Here To Go To The Wiki](https://github.com/coredex-source/noviq/blob/main/wiki.txt)

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
gcc -o noviq.exe noviq.c lexer/lexer_interpret.c lexer/lexer_display.c
```
### Run using:
- MacOS/Linux:
```
./noviq -e filename.nvq
```
- Windows:
```
noviq.exe -e filename.nvq
```
