NAME := ft_strace
SRC_DIR := src
SRCS := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJDIR := obj
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

CC := gcc
CFLAGS := -Wall -Wextra -Werror

all: $(NAME)

de: CFLAGS += -g -DDEBUG_ME
de: fclean all

san: CFLAGS += -fsanitize=address
san: de

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJDIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)
	rm -rf $(OBJDIR)

re: fclean all

.PHONY: all clean fclean re debug de