EXE = atomic_append \
	  bad_exclusive_open \
	  large_file multi_descriptors-01 \
	  multi_descriptors-02 \
	  multi_descriptors-03 \
	  t_readv \
	  t_truncate \
	  t_writev

all: $(EXE)


atomic_append:atomic_append.c
	gcc -o atomic_append atomic_append.c get_num.c error_functions.c

bad_exclusive_open: bad_exclusive_open.c
	gcc -o bad_exclusive_open bad_exclusive_open.c get_num.c error_functions.c

large_file:large_file.c
	gcc -o large_file large_file.c get_num.c error_functions.c

multi_descriptors-01:multi_descriptors-01.c
	gcc -o multi_descriptors-01 multi_descriptors-01.c get_num.c error_functions.c
	./multi_descriptors-01

multi_descriptors-02:multi_descriptors-02.c
	gcc -o multi_descriptors-02 multi_descriptors-02.c get_num.c error_functions.c
	xxd -b example.txt
	./multi_descriptors-02

multi_descriptors-03:multi_descriptors-03.c
	gcc -o multi_descriptors-03 multi_descriptors-03.c get_num.c error_functions.c
	xxd -b example.txt
	./multi_descriptors-03

t_readv:t_readv.c
	gcc -o t_readv t_readv.c get_num.c error_functions.c

t_truncate:t_truncate.c
	gcc -o t_truncate t_truncate.c get_num.c error_functions.c
	touch tmp.txt
	ls -l tmp.txt
	./t_truncate tmp.txt 1000
	ls -l tmp.txt
	./t_truncate tmp.txt 128
	ls -l tmp.txt
	rm tmp.txt

t_writev:t_writev.c
	gcc -o t_writev t_writev.c get_num.c error_functions.c
	./t_writev
	cat buccaneer.txt
clean:
	rm -rf $(EXE) *.o
