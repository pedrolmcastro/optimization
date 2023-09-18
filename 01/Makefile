ZIP := 01.zip

SRCDIR := src
OBJDIR := obj
BINDIR := bin
OUTDIR := out
INCDIR := include
SRPDIR := scripts

SRP := $(shell find $(SRPDIR) -name "*")
SRC := $(shell find $(SRCDIR) -name "*.c")
INC := $(shell find $(INCDIR) -name "*.h")
DEP := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.d,$(SRC))
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
BIN := $(patsubst $(SRCDIR)/%.c,$(BINDIR)/%.out,$(SRC))

CC := gcc-11
LDLIBS += -lm
CFLAGS += -I$(INCDIR) -std=c11 -O0 -Werror -Wall -Wextra -pedantic -MMD -MP


# Execution config variables
RUNS := 15
SIZE := 512
REPEATS := 1
PROFILING := $(OUTDIR)/profiling.json


all: $(BIN)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -DSIZE=$(SIZE) -DREPEATS=$(REPEATS) -c $< -o $@

$(BINDIR)/%.out: $(OBJDIR)/%.o
	@mkdir -p $(dir $@)
	@$(CC) $(LDFLAGS) $(LDLIBS) $< -o $@


run: $(BIN)
	@echo -n $(BIN) | xargs -d' ' -I{} sh -c 'echo {} && ./{}'


requirements:
	@pip install -r $(SRPDIR)/requirements.txt

profile: $(BIN)
	@python3 $(SRPDIR)/profile.py $(RUNS) $(PROFILING) $(BIN)

stats:
	@python3 $(SRPDIR)/stats.py $(PROFILING)


zip:
	@zip $(ZIP) $(SRC) $(INC) $(SRP) Makefile

clean:
	@$(RM) $(ZIP) $(OBJ) $(DEP) $(BIN)


.SECONDARY: $(OBJS)
.PHONY: all run profile stats zip clean


-include $(DEP)
