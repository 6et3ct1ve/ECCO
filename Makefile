TARGET = ecco
CC = gcc

CFLAGS = -O2 -march=native -fomit-frame-pointer -s -Iinclude/ecco -Iinclude/tinyargs

LIBS = -lgmp -lssl -lcrypto

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SRC = $(wildcard $(SRCDIR)/*.c)
TINYARGS = $(wildcard include/tinyargs/*.c)
SOURCES = $(SRC) $(TINYARGS)

SRC_O = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TINYARGS_O = $(TINYARGS:include/tinyargs/%.c=$(OBJDIR)/%.o)
OBJECTS = $(SRC_O) $(TINYARGS_O)

EXECUTABLE = $(BINDIR)/$(TARGET)

.PHONY: all clean

all: $(BINDIR) $(OBJDIR) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $^ $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -v -c $< -o $@

$(OBJDIR)/%.o: include/tinyargs/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -v -c $< -o $@

$(BINDIR): ; @mkdir -p $@
$(OBJDIR): ; @mkdir -p $@

clean:
	@rm -f $(OBJECTS) $(EXECUTABLE)