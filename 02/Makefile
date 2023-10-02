ZIP := 02.zip

SRCDIR := src
BINDIR := bin
OBJDIR := obj
INCDIR := include

DOC := $(shell find . -name "*.pdf")
SRC := $(shell find $(SRCDIR) -name "*.c")
INC := $(shell find $(INCDIR) -name "*.h")
DEP := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.d,$(SRC))
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

BIN := $(BINDIR)/main.out

CC := gcc-11
LDLIBS += -lm
CFLAGS += -I$(INCDIR) -std=c11 -O0 -Werror -Wall -Wextra -pedantic -MMD -MP


all: $(BIN)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJ)
	@mkdir -p $(dir $@)
	@$(CC) $(LDFLAGS) $(LDLIBS) $^ -o $@


run:
	@$(BIN)

zip:
	@zip $(ZIP) $(SRC) $(INC) $(DOC) Makefile

clean:
	@$(RM) $(ZIP) $(OBJ) $(DEP) $(BIN)


.SECONDARY: $(OBJ)
.PHONY: all run zip clean


-include $(DEP)
