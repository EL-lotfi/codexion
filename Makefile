#############################################################################################################
#                                                                                                           #
#                                               VARIABLES   												#
#																											#
#############################################################################################################
NAME = codexion

CF = coders/actions.c  coders/cleaner.c  coders/coder.c coders/main.c  \
	 coders/monitor.c  coders/routine.c  coders/scheduler.c  coders/tools.c

OBF = $(CF:.c=.o)

CFLAGS = -Wall -Wextra -Werror -g

#############################################################################################################
#                                                                                                           #
#                                         		  RULES 													#
#																											#
#############################################################################################################

all : $(NAME)

$(NAME) : $(OBF)
	gcc $(OBF) -o $@

clean :
	rm -f $(OBF)

re : fclean all

fclean : clean
	rm -rf $(NAME)
