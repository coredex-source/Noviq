all:
	gcc -o litecode litecode.c lexer/lexer_interpret.c lexer/lexer_display.c

clean:
	rm -f litecode