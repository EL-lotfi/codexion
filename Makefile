CF = coders/actions.c  coders/cleaner.c  coders/coder.c  coders/coder.h  coders/main.c  \
	 coders/monitor.c  coders/routine.c  coders/scheduler.c  coders/tools.c
CFLAGS = -Wall -Wextra -Werror

NAME = codexion


all : $(NAME)

$(NAME) : $(CF)
	gcc CF $(CFLAGS)

