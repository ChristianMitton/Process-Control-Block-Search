all: my_ps

my_ps: my_ps.c
	gcc -g -o my_ps my_ps.c

clean:
	rm -rf my_ps
