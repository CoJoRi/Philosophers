# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jrinaudo <jrinaudo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/29 09:00:39 by jrinaudo          #+#    #+#              #
#    Updated: 2025/02/12 11:16:55 by jrinaudo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#            # # #     #      #   #    #            ####                       #
#            #     #   #      #   #    #           #    #                      #
#            #     #   #      #   #    #          #      #                     #
#            # # #     ########   #    #          #      #                     #
#            #         #      #   #    #          #      #                     #
#            #         #      #   #    #          #      #                     #
#            #         #      #   #    #           #    #                      #
#            #         #      #   #    #######      ####                       #
# **************************************************************************** #

################################################################################
#                                     CONFIG                                   #
################################################################################
NAME	:= philo
CC		:= gcc
FLAGS	:= -Wall -Wextra -Werror -g

################################################################################
#                                 PROGRAM'S SRCS                               #
################################################################################
SRCS	:=	free.c init.c init2.c main.c philo.c philo2.c utils1.c medic.c
OBJS	:= $(SRCS:.c=.o)

.c.o:
	@${CC} ${FLAGS} -c $< -o ${<:.c=.o}
################################################################################
#                                  Makefile  objs                              #
################################################################################
CLR_RMV		:= \033[0m
RED		    := \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
CYAN 		:= \033[1;36m
RM		    := rm -f

${NAME}:	${OBJS}
			@echo "$(GREEN)Compilation ${CLR_RMV}of ${YELLOW}$(NAME) ${CLR_RMV}..."
			@${CC} ${FLAGS} -o ${NAME} ${OBJS}
			@echo "$(GREEN)$(NAME) created[0m ✔️"

all:		${NAME}

bonus:		all

val:		${NAME}
			@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) 4 410 200 200

hel:		${NAME}
			@valgrind --tool=helgrind ./$(NAME) 4 8000 100 100 4

run:		${NAME}
			@./$(NAME) $(ARGS)
#to execute: make run ARGS="4 400 200 200"

gdb:		${NAME}
			@gdb -tui ./$(NAME)

clean:
			@ ${RM} *.o */*.o */*/*.o
			@ echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)objs ✔️"

fclean:		clean
			@ ${RM} ${NAME}
			@ echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)binary ✔️"

re:			fclean all

.PHONY:		all clean fclean re bonus run gdb val hel