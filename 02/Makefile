ZIP := 02.zip

SRCDIR := src
BINDIR := bin
OBJDIR := obj
OUTDIR := out
INCDIR := include
SRPDIR := scripts

DOC := $(shell find . -name "*.pdf")
MON := $(shell find . -name "gmon.out")
SRP := $(shell find $(SRPDIR) -name "*")
SRC := $(shell find $(SRCDIR) -name "*.c")
INC := $(shell find $(INCDIR) -name "*.h")
DEP := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.d,$(SRC))
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

BIN := $(BINDIR)/main.out

CC := gcc
LDLIBS += -lm
LDFLAGS += -pg
CFLAGS += -I$(INCDIR) -std=c11 -pg -O0 -Werror -Wall -Wextra -pedantic -MMD -MP


# Execution config variables
REPEATS := 10


all: $(BIN)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJ)
	@mkdir -p $(dir $@)
	@$(CC) $(LDFLAGS) $(LDLIBS) $^ -o $@


run: $(BIN)
	@$(BIN)


requirements:
	@apt install graphviz
	@pip install -r $(SRPDIR)/requirements.txt

profile: $(BIN)
	@python3 $(SRPDIR)/profile.py $(REPEATS) $(BIN) $(MON)

graph:
	@mkdir -p $(OUTDIR)/dots $(OUTDIR)/pngs
	@$(foreach i, 0 1 2 3 4 5 6 7 8 9,                                  \
		gprof2dot $(OUTDIR)/reports/$(i).out > $(OUTDIR)/dots/$(i).dot; \
		dot -Tpng $(OUTDIR)/dots/$(i).dot -o $(OUTDIR)/pngs/$(i).png;)


zip:
	@zip $(ZIP) $(SRC) $(INC) $(SRP) $(DOC) Makefile

clean:
	@$(RM) -r $(OUTDIR) $(OBJDIR) $(BINDIR) $(ZIP) $(MON)


.SECONDARY: $(OBJ)
.PHONY: all run requirements profile graph zip clean

-include $(DEP)
