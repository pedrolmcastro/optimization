ZIP := 1.zip

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

run_all: $(BIN)
	@echo -n $(BIN) | xargs -n 1 -d' ' -I{} sh -c 'echo {} && ./{}'

run_perf:
	@eval './profile.py $(BIN)'

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/%.out: $(OBJDIR)/%.o
	@mkdir -p $(dir $@)
	@$(CC) $(LDFLAGS) $(LDLIBS) $< -o $@


zip:
	@zip $(ZIP) $(SRC) $(INC) Makefile

clean:
	@$(RM) $(ZIP) $(OBJ) $(DEP) $(BIN)


.PHONY: clean zip all run_all run_perf
.SECONDARY: $(OBJS)


-include $(DEP)
