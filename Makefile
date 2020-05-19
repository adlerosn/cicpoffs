CXX := g++
CXX_FLAGS :=

cicpoffs:
	${CXX} --std=c++17 cicpps.cpp fuse_launcher_gpl2.cpp -o cicpoffs -O2 -Wall $(shell pkg-config fuse --cflags --libs) -lulockmgr -fPIC ${CXX_FLAGS}

clean:
	rm -f cicpoffs
