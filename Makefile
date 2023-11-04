cicpoffs:
	${CXX} --std=c++17 cicpps.cpp fuse_launcher_gpl2.cpp -o cicpoffs -O2 -Wall $(shell pkg-config fuse3 libattr --cflags --libs) -lulockmgr -fPIC ${CXX_FLAGS}

install:
	install -Dm755 cicpoffs ${DESTDIR}/usr/bin/cicpoffs

clean:
	rm -f cicpoffs

