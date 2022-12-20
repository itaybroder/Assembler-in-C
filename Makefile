maman14: main.o dataNode.o node.o lables.o second_pass.o string_operations.o file_creation.o
	gcc -Wall -ansi -pedantic main.o dataNode.o node.o lables.o second_pass.o string_operations.o file_creation.o -o maman14 -lm

main.o:	main.c
	gcc -c -Wall -ansi -pedantic main.c -o main.o

dataNode.o: dataNode.c
	gcc -c -Wall -ansi -pedantic dataNode.c -o dataNode.o

node.o: node.c
	gcc -c -Wall -ansi -pedantic node.c -o node.o

lables.o: lables.c
	gcc -c -Wall -ansi -pedantic lables.c -o lables.o

second_pass.o: second_pass.c
	gcc -c -Wall -ansi -pedantic second_pass.c -o second_pass.o

string_operations.o: string_operations.c
	gcc -c -Wall -ansi -pedantic string_operations.c -o string_operations.o -lm

file_creation.o: file_creation.c
	gcc -c -Wall -ansi -pedantic file_creation.c -o file_creation.o

