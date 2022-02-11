CXX=g++
CXXFLAGS=-std=c++11 -g -Wall
LDFLAGS= -lpthread


bin_dir=./bin
obj_dir=./obj
MKDIR= mkdir -p
RM=rm
RMFLAGS= -rf

.PHONY:all clean test
all_bin= Thread_test MutexLock_test ThreadPool_test
all_bin:=$(addprefix $(bin_dir)/, $(all_bin))

all:$(all_bin)

objs=Thread_test.o Thread.o
objs:=$(addprefix $(obj_dir)/, $(objs))
$(bin_dir)/Thread_test:$(objs)
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)


objs=MutexLock_test.o Thread.o
objs:=$(addprefix $(obj_dir)/, $(objs))
$(bin_dir)/MutexLock_test:$(objs)
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)
	
objs=ThreadPool_test.o ThreadPool.o  Thread.o 
objs:=$(addprefix $(obj_dir)/, $(objs))
$(bin_dir)/ThreadPool_test:$(objs)
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

$(obj_dir)/%.o:%.cc
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

test:
	@echo $(objs)
clean:
	$(RM) $(RMFLAGS) $(bin_dir) $(obj_dir)

