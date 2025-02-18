CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 #-fsanitize=address
INCLUDES = -Iincludes
INC = includes/minishell.h
BINDIR = bin

PARSING_DIR = parsing
EXECUTION_DIR = execution
BUILTINS_DIR = $(EXECUTION_DIR)/builtins

PARSING_SRC = $(wildcard $(PARSING_DIR)/*.c)
PARSING_OBJ = $(patsubst $(PARSING_DIR)/%.c,$(BINDIR)/%.o,$(PARSING_SRC))

EXECUTION_SRC = $(wildcard $(EXECUTION_DIR)/*.c)
EXECUTION_OBJ = $(patsubst $(EXECUTION_DIR)/%.c,$(BINDIR)/%.o,$(EXECUTION_SRC))

BUILTINS_SRC = $(wildcard $(BUILTINS_DIR)/*.c)
BUILTINS_OBJ = $(patsubst $(BUILTINS_DIR)/%.c,$(BINDIR)/%.o,$(BUILTINS_SRC))

TARGET = minishell

.PHONY: all clean

all: $(TARGET)

$(BINDIR)/%.o: $(PARSING_DIR)/%.c $(INC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -I $(shell brew --prefix readline)/include -c $< -o $@

$(BINDIR)/%.o: $(EXECUTION_DIR)/%.c $(INC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -I $(shell brew --prefix readline)/include -c $< -o $@

$(BINDIR)/%.o: $(BUILTINS_DIR)/%.c $(INC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -I $(shell brew --prefix readline)/include -c $< -o $@

$(TARGET): $(PARSING_OBJ) $(EXECUTION_OBJ) $(BUILTINS_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -L$(shell brew --prefix readline)/lib -lreadline $^ -o $@

clean:
	rm -rf $(BINDIR)

fclean: clean
	rm -f $(TARGET)

re: clean all
