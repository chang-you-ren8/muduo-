.PHONY:all clean

RM=rm -rf
MKDIR=mkdir -p

include_dir= -I.. 
obj_dir=./obj
bin_dir=./bin
src_dir=..


CXX=g++
CXXFLAGS=-Wall -g $(include_dir) -std=c++11
LDFLAGS=-lpthread


all_bin=Timestamp_test SingleInstance_test Logger_test LogFile_test AsyncLogger_test
all_bin:=$(addprefix $(bin_dir)/, $(all_bin))
all:$(all_bin)

obj=Timestamp_test.o Timestamp.o
obj:=$(addprefix $(obj_dir)/, $(obj))
$(bin_dir)/Timestamp_test:$(obj)
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ $^ 


obj=SingleInstance_test.o Thread.o 
obj:=$(addprefix $(obj_dir)/, $(obj))
$(bin_dir)/SingleInstance_test:$(obj)	
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ $^ $(LDFLAGS)

obj=Logger.o Logger_test.o Timestamp.o LogStream.o
obj:=$(addprefix $(obj_dir)/, $(obj))
$(bin_dir)/Logger_test:$(obj)
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ $^ 

obj=LogFile_test.o LogFile.o Logger.o LogStream.o Timestamp.o  AppendFile.o
obj:=$(addprefix $(obj_dir)/, $(obj))
$(bin_dir)/LogFile_test:$(obj)
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ $^

obj=AsyncLogger_test.o LogFile.o Logger.o LogStream.o Timestamp.o AppendFile.o AsyncLogger.o Thread.o
obj:=$(addprefix $(obj_dir)/, $(obj))
$(bin_dir)/AsyncLogger_test:$(obj)
	@$(MKDIR) $(dir $@)
	$(CXX) -g -o $@ $^ $(LDFLAGS) 

$(obj_dir)/%.o:%.cc
	@$(MKDIR) $(dir $@)
	$(CXX)  -o $@ -c $< $(CXXFLAGS)

$(obj_dir)/%.o:$(src_dir)/%.cc
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	$(RM) $(obj_dir) $(bin_dir)


