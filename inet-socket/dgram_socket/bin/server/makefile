CC = gcc
CC_FLAGS = -Wall
BIN = server

SRVDIR = ../../src/server/
UTLDIR = ../../src/util/
OBJDIR = ../../obj/

all: mkdir_obj $(BIN)

$(BIN): $(OBJDIR)smain.o $(OBJDIR)util.o $(OBJDIR)server.o
	$(CC) $(CC_FLAGS) $^ -o $@

$(OBJDIR)smain.o: $(SRVDIR)main.c
	$(CC) $(CC_FLAGS) -c $^ -o $@

$(OBJDIR)server.o: $(SRVDIR)server.c
	$(CC) $(CC_FLAGS) -c $^ -o $@

$(OBJDIR)util.o: $(UTLDIR)util.c
	$(CC) $(CC_FLAGS) -c $^ -o $@

mkdir_obj:
	@mkdir -p $(OBJDIR)

clean:
	@rm -f $(BIN)
	@rm -rf $(OBJDIR)

.PHONY: all clean