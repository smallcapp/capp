
OBJS=umd.o user.o cli.o flow.o timer.o cfg.o
__TARGET=umd
TARGET=$(__TARGET)
LIBS_DEPEND=-lubacktrace -lappkey -ljson-c
CFLAGS+=

.PHONY:all
all:$(TARGET)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)

$(TARGET):$(OBJS)
	${CC} ${CFLAGS} ${LDFLAGS} $(LIBS_DEPEND) -o $(TARGET) $(OBJS)
%.o:%.c
	${CC} -c ${CFLAGS} $< -o $@