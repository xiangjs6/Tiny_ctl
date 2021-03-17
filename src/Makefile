######################################
#
##
######################################

#target you can change test to what you want
#run-time library name，lib*.so
TARGET  := ../lib/libtiny_ctl.so

#compile and lib parameter

CC      := gcc
LIBS    := -lpthread
LDFLAGS :=
DEFINES :=
INCLUDE := -I ../include
CFLAGS  := -std=c11 -pedantic -g -Wall -O3 -fPIC $(DEFINES) $(INCLUDE)
CXXFLAGS:= $(CFLAGS) -DHAVE_CONFIG_H
SHARE   := -shared -o

#i think you should do anything here


#source file
SOURCE  := $(wildcard *.c ./basic/*.c)
OBJS    := $(patsubst %.c,%.o,$(SOURCE))

.PHONY : everything objs clean veryclean rebuild

everything : $(TARGET)

all : $(TARGET)

objs : $(OBJS)

rebuild: veryclean everything

clean :
	rm -fr $(OBJS)
	rm -fr $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CXXFLAGS) $(SHARE) $@ $(OBJS) $(LDFLAGS) $(LIBS)
