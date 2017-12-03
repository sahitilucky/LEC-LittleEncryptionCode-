all:Lineartrial Difftrial linapp diffapp encryption decryption

linapp:
	gcc -o linapp linearapproxtable.c
diffapp:
	gcc -o diffapp propratio.c
Lineartrial:
	gcc -o Lineartrial Lineartrial.c -lm
Difftrial:
	gcc -o Difftrial Differentialtrial.c -lm

decryption:decryptfuncs.o maindec.o 
	gcc -o  decryption maindec.o decryptfuncs.o

decryptfuncs.o:decryptfuncs.c decryptfuncs.h
	gcc -g -c -o decryptfuncs.o decryptfuncs.c

maindec.o:Decryption.c decryptfuncs.h
	gcc -g -c -o maindec.o Decryption.c -lm


encryption:mixcolumns.o mainenc.o 
	gcc -o  encryption mainenc.o mixcolumns.o -lrt

mixcolumns.o:mixcolumns.c mixcolumns.h
	gcc -g -c -o mixcolumns.o mixcolumns.c

mainenc.o:Encryption.c mixcolumns.h
	gcc -g -c -o mainenc.o Encryption.c -lm -lrt

clean:
	rm Lineartrial Difftrial linapp diffapp encryption decryption
	rm mixcolumns.o mainenc.o maindec.o decryptfuncs.o 
	
