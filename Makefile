PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

CC :=gcc
CFLAGS := -Werror -Wall
LDFLAGS := -lrt -lpthread -g
OBJS  := SendingProcess AnsweringProcess

all: SendingProcess AnsweringProcess

%: %.c
	$(CC) $(CFLAGS) $@.c util.c -o $@ $(LDFLAGS)
run:
	./SendingProcess

clean:
	rm -fr ASS5 $(OBJS)NAME = littlearrow
NCURSES = $(shell ncursesw5-config --cflags --libs)

$(NAME): $(NAME).c utils.c
		gcc -Wall -Werror -std=gnu11 -pthread -o $(NAME) $(NAME).c utils.c $(NCURSES)


