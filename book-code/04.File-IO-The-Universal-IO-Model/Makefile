EXE = copy seek_io open

all:$(EXE)

copy:copy.c
	gcc -o copy copy.c error_functions.c get_num.c

seek_io:seek_io.c
	gcc -o seek_io seek_io.c get_num.c error_functions.c

open:open.c
	gcc -o open open.c error_functions.c get_num.c

clean:
	rm -rf $(EXE) *.o 
