TARGET := ../bin/test_zmq
CC := gcc
INCLUDE:= -I ../inc

LIBS:= -L ../lib/ -Wl,-Bstatic -lzmq  -Wl,-Bdynamic -lpthread -lstdc++ -lm

CSRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(CSRCS))

$(TARGET): $(OBJS)
	$(CC) $+ -o $@  $(LIBS)
%.o:%.c
	$(CC) -c -g $< -o $@ $(INCLUDE)

clean:
	rm -rf $(OBJS) $(TARGET)
