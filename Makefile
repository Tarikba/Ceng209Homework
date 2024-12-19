CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = DungeonAdventure

SCR_DIR = .
OBJ_DIR = obj 

SRCS = DungeonAdventure.c structures.h
OBJS = $(OBJ_DIR)/$(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

	$(OBJ_DIR)/%.o: $(SCR_DIR)/%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

.PHONY: clean $(OBJ_DIR)