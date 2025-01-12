all:
	gcc -o litecode litecode.c lexar/lexar_interpret.c lexar/lexar_display.c

clean:
	rm -f litecode