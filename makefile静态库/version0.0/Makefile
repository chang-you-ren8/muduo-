CXX=g++
CXXFLAGS=-g -Wall

RM=rm -rf 
AR=ar r 			#r 表示如果静态库中已经存在相同名字的目标文件，那么使用新的替代原来的 

STATIC_LIB=libtest.a
SRC=sub.cc add.cc
OBJ=$(patsubst %.cc, %.o, $(SRC))

.PHONY:all
all:test 

test:test.o $(STATIC_LIB)
	$(CXX) -o $@ $< $(STATIC_LIB) $(CXXFLAGS)
	$(RM) test.o

# ar r libtest.a sub.o add.o
# 静态库命名规范:libname.a 其中name表示库的名字
#libtest.a 包含sub.o add.o
#生成库之后sub.o, add.o可以有选择的删除，因为静态库就是目标文件的一个副本
# ar t libtest.a 可以查看libtest.a中包含的目标文件
# ar tv libtest.a 输出目标文件的详细信息
# ar d libtest.a sub.o 从libtest.a中删除sub.o
$(STATIC_LIB):$(OBJ)
	$(AR) $@ $^
	$(RM) $^

%.o:%.cc
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY:clean
clean:
	$(RM) $(STATIC_LIB) test