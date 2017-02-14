all:
	gcc -c -o lib/dashgl.o lib/dashgl.c -lGL -lGLEW -lpng
	gcc main.c lib/dashgl.o -lGL -lGLEW -lglut -lm -lpng

run:
	./a.out

clean:
	rm a.out
	rm lib/dashgl.o
