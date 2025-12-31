# Makefile
# Criado por Matheus Leme Da Silva

VERSION := 0.1
TARGET := potato

CC ?= gcc
CFLAGS := -std=c11 -Wall -Wextra -Werror
DEFS := -DVERSION=\"$(VERSION)\"
LIBS :=

BUILDDIR := $(CURDIR)/build
SRCDIR := $(CURDIR)/src

MODE ?= release

ifeq ($(MODE),debug)
CFLAGS += -O0 -g3 -DDEBUG
else
CFLAGS += -O2 -DNDEBUG
endif

BASEDIR := $(BUILDDIR)/$(MODE)

BINDIR := $(BASEDIR)/bin
OBJDIR := $(BASEDIR)/obj
DEPDIR := $(BASEDIR)/dep
DIRS := $(BINDIR) $(OBJDIR) $(DEPDIR)

TARGETFILE = $(BINDIR)/$(notdir $(TARGET))

SRC := \
	   main.c \
	   potato.c \
	   util.c \
	   lexer/token.c \
	   lexer/lexer.c \
	   parser/parser.c

SRC := $(addprefix $(SRCDIR)/,$(SRC))
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
DEP := $(patsubst $(SRCDIR)/%.c,$(DEPDIR)/%.d,$(SRC))

all: release

clean:
	rm -rf $(BUILDDIR)

example: $(TARGETFILE)
	@echo "example.ptt content:" > example.log
	@cat example.zyn >> example.log
	@echo "Running: $(TARGETFILE) --help:" >> example.log
	@$(TARGETFILE) --help >> example.log
	@echo "Running: $(TARGETFILE) --version" >> example.log
	@$(TARGETFILE) --version >> example.log
	@echo "Running: $(TARGETFILE) --run-file example.zyn" >> example.log
	@$(TARGETFILE) --run-file example.ptt >> example.log
	@cat example.log

build: $(TARGETFILE)

release:
	$(MAKE) MODE=release build

debug:
	$(MAKE) MODE=debug build

$(DIRS):
	mkdir -p $@

$(TARGETFILE): $(OBJ) $(LIBS) | $(BINDIR)
	mkdir -p $(dir $@)
	$(CC) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR) $(DEPDIR)
	mkdir -p $(dir $@) $(dir $(DEPDIR)/$*)
	$(CC) $(CFLAGS) $(DEFS) -c $< -o $@ -MMD -MF $(DEPDIR)/$*.d

.PHONY: all clean example build release debug
-include $(DEP)
