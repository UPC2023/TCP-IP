src += main.c 
src += broadcast.c
src += pack_unpack.c
src += create_socket.c
src += filelist.c
src += key_scan_process.c
src += tcp_send_file.c
src += udp_process.c
src += userlist.c

obj = $(src:%.c=%.o)

CC = gcc

OPTION = -lpthread -g -Wall
EXEC_NAME = ipmsg


ipmsg:$(obj)
	@echo make ...
	$(CC) $^ -o $(EXEC_NAME) $(FLAG) $(OPTION)
	@echo make over
	@echo =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=ol
%.o:%.c
	$(CC) -c -o $@ $< $(FLAG)

.PHONY:clean
clean:
	@echo clean ...
	rm $(EXEC_PATH) *.o -rf
	@echo -----clean over------
