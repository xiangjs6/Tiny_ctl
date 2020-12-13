######################################
#
##
######################################

#target you can change test to what you want
#run-time library name，lib*.so
TARGET  := libtiny_ctl.so

#compile and lib parameter

CC      := gcc
LIBS    := -lpthread
LDFLAGS :=
DEFINES :=
INCLUDE := -I.
CFLAGS  := -g -Wall -O3 $(DEFINES) $(INCLUDE)
CXXFLAGS:= $(CFLAGS) -DHAVE_CONFIG_H
SHARE   := -fPIC -shared -o

#i think you should do anything here


#source file
SOURCE  := $(wildcard *.c) $(wildcard ./auto_release_pool/*.c)
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
