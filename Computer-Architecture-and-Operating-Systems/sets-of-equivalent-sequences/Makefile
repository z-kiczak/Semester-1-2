FLAGZ=-Wall -Wextra -Wno-implicit-fallthrough -std=gnu17 -fPIC -O2
LINKZ=-Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc -Wl,--wrap=reallocarray -Wl,--wrap=free -Wl,--wrap=strdup -Wl,--wrap=strndup

seq.o: seq.c seq.h
	gcc $(FLAGZ) -c seq.c
 
memory_tests.o: memory_tests.c memory_tests.h
	gcc $(FLAGZ) -c memory_tests.c
 
seq_example.o: seq_example.c seq.h memory_tests.h
	gcc $(FLAGZ) -c seq_example.c

test: libseq.so seq_example.o
	gcc $(LINKZ) -o test seq_example.o libseq.so

libseq.so: seq.o memory_tests.o
	gcc -shared $(LINKZ) -o libseq.so seq.o memory_tests.o

 
clean:
	rm -f *.o libseq.so test