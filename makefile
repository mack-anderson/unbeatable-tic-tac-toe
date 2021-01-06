all: alphatoe show_node count_nodes

ttt1.o: ttt.h ttt1.c
	gcc  -std=c99 -Wall -pedantic -c ttt1.c -o ttt1.o 

ttt2.o: ttt.h ttt2.c
	gcc  -std=c99 -Wall -pedantic -c ttt2.c -o ttt2.o 

alphatoe.o: alphatoe.c ttt.h ttt1.o ttt2.o
	gcc  -std=c99 -Wall -pedantic -c alphatoe.c -o alphatoe.o

alphatoe: alphatoe.o ttt1.o ttt2.o
	gcc  -std=c99 -Wall -pedantic alphatoe.o ttt1.o ttt2.o -o alphatoe

show_node.o: show_node.c ttt.h
	gcc  -std=c99 -Wall -pedantic -c show_node.c -o show_node.o

show_node: show_node.o ttt1.o ttt2.o
	gcc  -std=c99 -Wall -pedantic show_node.o ttt1.o ttt2.o -o show_node

count_nodes.o: count_nodes.c ttt1.o ttt2.o ttt.h
	gcc  -std=c99 -Wall -pedantic -c count_nodes.c -o count_nodes.o

count_nodes: count_nodes.o ttt1.o ttt2.o
	gcc  -std=c99 -Wall -pedantic count_nodes.o ttt1.o ttt2.o -o count_nodes

clean:
	rm *.o alphatoe show_node count_nodes
