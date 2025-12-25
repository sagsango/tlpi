ALL=demo_inotify dnotify inotify_dtree rand_dtree
all:$(ALL)

demo_inotify:
	gcc -o demo_inotify demo_inotify.c  get_num.c error_functions.c

dnotify:
	gcc -o dnotify dnotify.c  get_num.c error_functions.c

inotify_dtree:
	gcc -o inotify_dtree inotify_dtree.c  get_num.c error_functions.c

rand_dtree:
	gcc -o rand_dtree rand_dtree.c  get_num.c error_functions.c

clean:
	rm $(ALL)
