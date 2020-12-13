CC = gcc
C_FLAGS = -g -Wall
INCLUDE = -I..
LIBS = -L.. -ltiny_ctl -lpthread
#wildcard对cpp文件进行展开
SRCS := $(wildcard *.c)
#将SRCS中后缀.c替换成空，即可执行程序的名字与c名字相同
TARGET  := $(patsubst %.c, %, $(SRCS))
OBJDIR = .

#可移动到指定的OBJDIR目录
#mv $@ $(OBJDIR)
.c:
	$(CC) $(C_FLAGS) -o $@ $< $(INCLUDE) $(LIBS)

all: $(TARGET) 

clean:
	rm -rf $(TARGET)
