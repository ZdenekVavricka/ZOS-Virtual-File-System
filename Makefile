#Použité překladač
CC = gcc
CFLAGS = -lm -g

BIN_DIR = bin

# Binární soubory projektu
BIN = file_system.exe
OBJ = main.o commands.o vfs.o utilities.o

# Návody pro sestavení projektu
all: clean $(BIN_DIR) $(BIN)

$(BIN): $(BIN_DIR)/main.o $(BIN_DIR)/commands.o $(BIN_DIR)/vfs.o $(BIN_DIR)/utilities.o
	$(CC) $^ -o $@ $(CFLAGS)

$(BIN_DIR)/main.o: src/main.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BIN_DIR)/commands.o: src/commands.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BIN_DIR)/vfs.o: src/vfs.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BIN_DIR)/utilities.o: src/utilities.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BIN_DIR):
	mkdir $@

clean:
	rm -f $(BIN)
	rm -f $(BIN_DIR)/*.o
