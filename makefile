#  Compiler
CC = gcc

#  Compiler Flags
CFLAGS = -Wall -ansi -O3

#  Sources
SOURCES = gestor.c leparque.c parque.c graph.c graph_pos.c escreve_saida.c lecarro.c LinkedList.c lerest.c estaciona.c heap.c

#  Objects
OBJECTS = gestor.o leparque.o parque.o graph.o graph_pos.o escreve_saida.o lecarro.o LinkedList.o lerest.o estaciona.o heap.o

autopark: $(OBJECTS)
	gcc -o $@ $(OBJECTS)

leparque.o: parque.h

graph.o: graph.h heap.h

graph_pos.o: parque.h graph.h

parque.o: parque.h LinkedList.h

lecarro.o: parque.h

LinkedList.o: LinkedList.h item.h

escreve_saida.o:

lerest.o: parque.h

estaciona.o: parque.h

heap.o: heap.h 


clean::
	rm -f *.o core a.out autopark *~

depend::
	makedepend $(SOURCES)
# DO NOT DELETE
