# Compiler
CC = gcc

# Compiler flags
CFLAGS = -pthread -Wall -Wextra

# Targets
SERVER = server
CLIENT = client

# Source files
SERVER_SRC = server.c
CLIENT_SRC = client.c

# Default target: build both server and client
all: $(SERVER) $(CLIENT)

# Build server
$(SERVER): $(SERVER_SRC)
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $(SERVER)

# Build client
$(CLIENT): $(CLIENT_SRC)
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o $(CLIENT)

# Clean compiled binaries
clean:
	rm -f $(SERVER) $(CLIENT)

# Run server
run-server: $(SERVER)
	./$(SERVER)

# Run client
run-client: $(CLIENT)
	./$(CLIENT)
