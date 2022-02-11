CXX=g++
CXXFLAGS= -g -Wall -std=c++11
LDFLAGS=-lpthread

objs=Test.o
bin=Test 

$(bin):$(objs)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

%.o:%.cc 
	$(CXX) -o $@  -c $< $(CXXFLAGS)

.PHONY:clean
clean:
	rm $(objs) $(bin)