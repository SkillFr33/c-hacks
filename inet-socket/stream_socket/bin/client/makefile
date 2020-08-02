CC = gcc
CC_FLAGS = -Wall
BIN = client

CLTDIR = ../../src/client/
UTLDIR = ../../src/util/
OBJDIR = ../../obj/

all: mkdir_obj $(BIN)

$(BIN): $(OBJDIR)cmain.o $(OBJDIR)util.o $(OBJDIR)client.o
	$(CC) $(CC_FLAGS) $^ -o $@

$(OBJDIR)cmain.o: $(CLTDIR)main.c
	$(CC) $(CC_FLAGS) -c $^ -o $@

$(OBJDIR)client.o: $(CLTDIR)client.c
	$(CC) $(CC_FLAGS) -c $^ -o $@

$(OBJDIR)util.o: $(UTLDIR)util.c
	$(CC) $(CC_FLAGS) -c $^ -o $@

mkdir_obj:
	@mkdir -p $(OBJDIR)

clean:
	@rm -f $(BIN)
	@rm -rf $(OBJDIR)

.PHONY: all clean