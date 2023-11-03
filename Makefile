cicpoffs:
	${CC} ulockmgr.c -c $(shell pkg-config fuse3 libattr --cflags --libs) -fPIC ${CFLAGS}
	${CXX} --std=c++17 ulockmgr.o cicpps.cpp fuse_launcher_gpl2.cpp -o cicpoffs $(shell pkg-config fuse3 libattr --cflags --libs) -fPIC ${CXXFLAGS}

install:
	install -Dm755 cicpoffs ${DESTDIR}/usr/bin/cicpoffs

clean:
	rm -f ulockmgr.o cicpoffs

