TARGET = a.out

all:
	# $(CC) -o a.out main.c $(OBJECTS)
	gcc -g -o $(TARGET) -Wall -Wextra -std=c99 -O3  main.c list.c vertice.c apiparte3.c APIG23.* 


run: 
	time ./$(TARGET) < ../grafos-practia/Octo.txt
	time ./$(TARGET) < ../grafos-practia/Q53.txt
	time ./$(TARGET) < ../grafos-practia/8suCBQ10.txt
clean:
	rm $(TARGET) 

.PHONY:
	clean
