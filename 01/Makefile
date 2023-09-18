ZIP := 01.zip

BINDIR := bin
OBJDIR := obj
OUTDIR := out
SRCDIR := src
INCDIR := include
SRPDIR := scripts

INC := $(shell find $(INCDIR) -name "*.h")
SRC := $(shell find $(SRCDIR) -name "*.c")
SRP := $(shell find $(SRPDIR) -name "*.py")
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
DEP := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.d,$(SRC))
BIN := $(patsubst $(SRCDIR)/%.c,$(BINDIR)/%.out,$(SRC))

CC := gcc
LDLIBS += -lm
CFLAGS += -I$(INCDIR) -std=c11 -Werror -Wall -Wextra -pedantic -MMD -MP


# Execution config variables
RUNS := 10
SIZE := 512
REPEATS := 1
PROFILED := $(OUTDIR)/profiled.json


all: $(BIN)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -DSIZE=$(SIZE) -DREPEATS=$(REPEATS) -c $< -o $@

$(BINDIR)/%.out: $(OBJDIR)/%.o
	@mkdir -p $(dir $@)
	@$(CC) $(LDFLAGS) $(LDLIBS) $< -o $@


run: $(BIN)
	@echo -n $(BIN) | xargs -d' ' -I{} sh -c 'echo {} && ./{}'

profile: $(BIN)
	@python3 $(SRPDIR)/profile.py $(RUNS) $(PROFILED) $(BIN)


zip:
	@zip $(ZIP) $(SRC) $(INC) $(SRP) Makefile

clean:
	@$(RM) $(ZIP) $(OBJ) $(DEP) $(BIN)


.SECONDARY: $(OBJS)
.PHONY: all run profile zip clean


-include $(DEP)
