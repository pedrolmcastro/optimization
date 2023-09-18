ZIP := 01.zip

SRCDIR := src
OBJDIR := obj
BINDIR := bin
INCDIR := include

SRC := $(shell find $(SRCDIR) -name "*.c")
INC := $(shell find $(INCDIR) -name "*.h")
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
DEP := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.d,$(SRC))
BIN := $(patsubst $(SRCDIR)/%.c,$(BINDIR)/%.out,$(SRC))

CC := gcc
LDLIBS += -lm
CFLAGS += -I$(INCDIR) -std=c11 -Werror -Wall -Wextra -pedantic -MMD -MP


all: $(BIN)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/%.out: $(OBJDIR)/%.o
	@mkdir -p $(dir $@)
	@$(CC) $(LDFLAGS) $(LDLIBS) $< -o $@


run: $(BIN)
	@echo -n $(BIN) | xargs -n 1 -d' ' -I{} sh -c 'echo {} && ./{}'

perf:
	@eval './profiling.py $(BIN)'


zip:
	@zip $(ZIP) $(SRC) $(INC) Makefile

clean:
	@$(RM) $(ZIP) $(OBJ) $(DEP) $(BIN)


.SECONDARY: $(OBJS)
.PHONY: clean zip all run perf


-include $(DEP)
