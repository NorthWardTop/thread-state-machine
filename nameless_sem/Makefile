CC = g++
CCFLG = -g -o
RM = rm -f
OBJGEN = server_select

#获取所有.c文件
SRC = $(wildcard *.c)

#所有的.c换成.o
OBJS = $(patsubst %.c,%.o,$(SRC))

# $@(自动目标文件名) $^(自动所有依赖文件名) $<(第一个依赖的文件名) 
$(OBJGEN):$(OBJS)
	$(CC) $(CCFLG) $@ $^

.PHONY:
clean:
	$(RM) $(OBJS)
 
