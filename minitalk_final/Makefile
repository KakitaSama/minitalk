SERVER=server
CLIENT=client
SERVERB=server_bonus
CLIENTB=client_bonus
CFLAGS= -Wall -Wextra -Werror

SRCSSV=server.c
SRCSBSV=server_bonus.c
SRCSCL=client.c
SRCSBCL=client_bonus.c


all: $(SERVER) $(CLIENT)

$(SERVER) : $(SRCSSV:.c=.o)
	cc $^ -o $@
$(CLIENT) : $(SRCSCL:.c=.o)
	cc $^ -o $@

$(SERVERB) : $(SRCSBSV:.c=.o)
	cc $^ -o $@
$(CLIENTB) : $(SRCSBCL:.c=.o)
	cc $^ -o $@


$(OBJS):%.o:%.c so_long.h
	cc $(CFLAGS) -c $< -o $@
$(OBJSB):%.o:%.c so_long_bonus.h
	cc $(CFLAGS) -c $< -o $@

bonus : $(SERVERB) $(CLIENTB)
clean :
	rm -rf $(SRCSSV:.c=.o) $(SRCSCL:.c=.o) $(SRCSBSV:.c=.o) $(SRCSBCL:.c=.o)
fclean: clean
	rm -rf $(SERVER)
	rm -rf $(CLIENT)
	rm -rf $(SERVERB)
	rm -rf $(CLIENTB)
re : fclean all