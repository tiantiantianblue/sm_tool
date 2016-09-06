CXX = g++-5
CC = g++-5
LDFLAGS = -L. -pthread
LDFLAGS += -static-libstdc++ -static-libgcc
LDLIBS += -lsm_hash_map
LDLIBS += -lrt
CPPFLAGS = -I/home/tian-qing/source_code/boost_1_61_0 -std=c++11 
sm_test:sm_test.o
clean:
	-rm *.o sm_test
