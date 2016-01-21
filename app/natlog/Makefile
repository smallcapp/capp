CC = gcc -std=gnu99
CFLAGS = -I/usr/src/linux-headers-3.2.0-imq/include
OBJS = ntlg_main.o ntlg_hash.o ntlg_list.o 
TARGET = natlog 

all:$(TARGET)

$(TARGET):$(OBJS) 
	$(CC) $(OBJS) $(CFLAGS) -Wall -o $@

%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm *.o $(TARGET)
