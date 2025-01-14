all:
	gcc -o noviq noviq.c lexer/lexer_interpret.c lexer/lexer_display.c -lm

clean:
	rm -f noviq