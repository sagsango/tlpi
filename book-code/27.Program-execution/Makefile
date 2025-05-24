EXEC=closeonexec hello_world simple_system t_execl
.phony: clean
closeonexec:closeonexec.c
	gcc -o closeonexec closeonexec.c get_num.c error_functions.c

hello_world:hello_world.c
	gcc -o hello_world hello_world.c

simple_system:simple_system.c
	gcc -o simple_system simple_system.c get_num.c error_functions.c  print_wait_status.c

t_execl:t_execl.c
	gcc -o t_execl t_execl.c get_num.c error_functions.c

t_execle:t_execle.c
	gcc -o t_execle t_execle.c get_num.c error_functions.c

clean:
	rm $(EXEC) *.o
