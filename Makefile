ARCH := $(shell uname -m | sed 's/i686/32/; s/x86_64/64/')

all: umbreon.so umbreon.so.i686
umbreon.so: umbreon.c
	gcc -std=gnu99 -O0 umbreon.c readlink.c -Wall -Wno-unused-result -shared -fPIC -Iinc -ldl  -lcrypt -Wl,--build-id=none  -o umbreon.so 
	strip umbreon.so
umbreon.so.i686: umbreon.c
	gcc -m32 -std=gnu99 -O0 umbreon.c readlink.c -Wall -Wno-unused-result -shared -fPIC -Iinc -ldl -lcrypt -Wl,--build-id=none  -o umbreon.so.i686
	# For some insane reason, 32 bit machines will eat the library when compiling it with optimization... totally OK to strip it afterward though.
	strip umbreon.so.i686
clean:
	rm -f umbreon.so umbreon.so.i686 unfuck whirlpool-gen

distclean:
	rm -f umbreon.so unfuck whirlpool-gen .rk* .old inc/config.h.bak umbreon.so.i686
	cp inc/config.h.dist inc/config.h
