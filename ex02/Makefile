NAME = PmergeMe

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = $(wildcard *.cpp)

RM = rm -fr

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) -o $(NAME) $(SRC)

clean:
	$(RM) $(NAME)

fclean:
	$(RM) $(NAME)

re: clean all
