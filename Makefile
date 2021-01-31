all: myshell

myshell: myshell.c islem tekrar
	gcc -o myshell myshell.c

islem : islem.c topla cikar
	gcc -o islem  islem.c

tekrar: tekrar.c 
	gcc -o tekrar tekrar.c

topla : topla.c 
	gcc -o topla topla.c

cikar : cikar.c 
	gcc -o cikar cikar.c
