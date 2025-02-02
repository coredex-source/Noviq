```diff
@@ The language is currently in PreAlpha development stages. @@
@@ A lot of missing features and bugs are to be expected.    @@
```
# Noviq
- The name Noviq is make up of two words: Nova(which means new) and unique.
- So, Nova + unique = Noviq.
- Noviq is just a simple interpreter based programming language written in C.

## Wiki
[Click Here To Go To The Wiki](https://coredex-source.github.io/Noviq-site/wiki/introduction.html)

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
gcc -o noviq.exe noviq.c lexer/lexer_interpret.c lexer/lexer_display.c -lm
```
### Run using:
- MacOS/Linux:
```
./noviq -e filename.nvq
```
- Windows (cmd):
```
noviq.exe -e filename.nvq
```
