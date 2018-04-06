all:
	pgcc -fast -ta=tesla,cuda8.0 -Minfo=all,ccff main.c

host: 
	pgcc -fast -ta=host -Minfo=all,ccff main.c

multicore: 
	pgcc -fast -ta=multicore -Minfo=all,ccff main.c

run: all
	./main.exe

_run:
	./a.exe
