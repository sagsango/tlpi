/*
	 #include <malloc.h>

	 void *memalign(size_t alignment, size_t size);

	 The obsolete function memalign() allocates size bytes and returns a pointer to the allocated  memory.
   The memory address will be a multiple of alignment, which must be a power of two.

	 RETURN VALUE
       aligned_alloc(), memalign(), valloc(), and pvalloc() return a pointer to the allocated memory on suc-
       cess.  On error, NULL is returned, and errno is set to indicate the error.
	
	 ERRORS
       EINVAL The alignment argument was not a power of two, or was not a multiple of sizeof(void *).
       ENOMEM There was insufficient memory to fulfill the allocation request.
*/

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	if( argc < 3 ){
		printf("$./memalign alignment size");
		return 1;
	}

	size_t alignment = atol(argv[1]);
	size_t size = atol(argv[2]);
	void * ptr = memalign( alignment, size);

	if( ptr == NULL ){
		perror("memalign():");
		return 1;
	}

  for(int i=0;i<64;++i){
    unsigned long long p = 1LLU << i;
    printf("addres %% %llu  = %llu\n", p, ((long long)ptr) % p);
  }

  /*
   * aligned_alloc(0x100, 0x100);
   * 	alignment must be a power of two.
   * 	size must be a multiple of alignment.
   *
   * memalign is removed from posix now, so use aligned_alloc
   */

  printf("Address: %lld, alignment:%ld, Address%%alignment:%lld\n", (long long) ptr, alignment, ( (long long) ptr ) % alignment);

	return 0;

}

