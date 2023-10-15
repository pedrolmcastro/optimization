ZIP := 02.zip

SRCDIR := src
BINDIR := bin
OBJDIR := obj
INCDIR := include
PROFDIR := gprof
GRAPHDIR := graph

DOC := $(shell find . -name "*.pdf")
SRC := $(shell find $(SRCDIR) -name "*.c")
INC := $(shell find $(INCDIR) -name "*.h")
PRF := $(shell find $(PROFDIR) -name "*.py")
REP := $(shell find $(PROFDIR) -name "*.out")
DEP := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.d,$(SRC))
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

BIN := $(BINDIR)/main.out
MON := $(shell find . -name "gmon.out")
NEXECS := 10

CC := gcc
LDLIBS += -lm
CFLAGS += -I$(INCDIR) -std=c11 -O0 -Werror -Wall -Wextra -pedantic -MMD -MP

all: $(BIN)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -pg $< -o $@

$(BIN): $(OBJ)
	@mkdir -p $(dir $@)
	@$(CC) $(LDFLAGS) $(LDLIBS) $^ -pg -o $@

run:
	@$(BIN)

run_gprof: all
	@python3 $(PRF) $(NEXECS) $(BIN) $(MON)

zip:
	@zip $(ZIP) $(SRC) $(INC) $(DOC) Makefile

clean:
	@$(RM) $(ZIP) $(OBJ) $(DEP) $(BIN) $(MON)
	@$(RM) -r $(sort $(dir $(wildcard gprof/*/)))
	@$(RM) -r $(sort $(dir $(wildcard graph/*/)))


NUMBERS := 0 1 2 3 4 5 6 7 8 9

graph_gprof: run_gprof
	@python3 -m pip install --break-system-packages --user graphviz
	@mkdir -p $(GRAPHDIR)/dots $(GRAPHDIR)/pngs
	@$(foreach rep, $(NUMBERS), \
		gprof2dot $(PROFDIR)/reports/$(rep).out > $(GRAPHDIR)/dots/$(rep).dot; \
		dot -Tpng $(GRAPHDIR)/dots/$(rep).dot -o $(GRAPHDIR)/pngs/$(rep).png;)


.SECONDARY: $(OBJ)
.PHONY: all run zip clean

-include $(DEP)