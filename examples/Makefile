# Generic C++ Makefile
# Made by Arthur van der Staaij and Aron de Jong

# To download the latest version, report issues or request new features, visit
# https://github.com/avdstaaij/cpp-init

#==============================================================================#
#----------------------------- Usage instructions -----------------------------#
#==============================================================================#

# Besides actual build targets, the following rules are provided:
# all:      Build the release binary (with FLAGS_R)
# release:  Build the release binary (with FLAGS_R)
# debug:    Build the debug binary (with FLAGS_D)
# run:      Build the release binary and run it
# rrun:     Build the release binary and run it
# drun:     Build the debug binary and run it
# c:        Clean all build byproducts
# clean:    Clean all build byproducts and the binaries

# To use "run" and "drun" with arguments, use:
# make run ARGS="arguments here"

# The rules in this Makefile store generated object files and keep track of C++
# dependencies through dependency files to minimize recompilation time.
# Because the dependency generation is fully automated using compiler tools,
# there is no need to specify a header extension (e.g. ".h").

# The variables below the "Settings" header can be altered to change the build
# behavior. Their meaning is as follows:

# CXX        The C++ compiler
# CXXFLAGS   Compiler flags, both for building object files and linking them
# FLAGS_R    Release-specific compiler flags
# FLAGS_D    Debug-specific compiler flags
# LDFLAGS    Compiler flags used when linking the the binary
# INCFLAGS   Compiler flags for file inclusion; used for building object files
# BINNAME_R  The name of the generated release binary
# BINNAME_D  The name of the genereated debug binary

# BINDIR     Directory in which the binaries are placed
# SRCDIR     Directory in which to look for C++ source files
# BLDDIR     Directory in which generated object and dependency files are placed

# SRCEXT     Extension of C++ source files
# ARGS       Text to append to binary invocations (rules "run" and "drun")
# RM         The command that is used to remove files (rules "c" and "clean")

# MKBIN_R and MKBIN_D are the commands used to build the final binaries. These
# can be changed for advanced usage, such as building libraries. Make will run
# these commands with $^ set to the list of object files and $@ set to the path
# of the binary to create.

#==============================================================================#
#---------------------------------- Settings ----------------------------------#
#==============================================================================#

CXX       = g++

CXXFLAGS  = -std=c++11 -Wall -Wextra -Wignored-qualifiers
FLAGS_R   = -O2
FLAGS_D   = -g
LDFLAGS   =
INCFLAGS  = -I ../include

BINNAME_R = examples
BINNAME_D = $(BINNAME_R)_debug

BINDIR    = .
SRCDIR    = .
BLDDIR    = build

SRCEXT    = .cpp
ARGS      =
RM        = rm -f

MKBIN_R   = $(CXX) $(CXXFLAGS) $(FLAGS_R) $^ $(LDFLAGS) -o $@
MKBIN_D   = $(CXX) $(CXXFLAGS) $(FLAGS_D) $^ $(LDFLAGS) -o $@

#==============================================================================#
#------------------ Are you sure you know what you're doing? ------------------#
#==============================================================================#

# Checks

ifeq ($(BLDDIR), .)
  $(error You MONSTER, never set BLDDIR to '.'! It would clutter your project directory)
endif


# Functions

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))


# Variables

OBJDIR ::= $(BLDDIR)/obj
DEPDIR ::= $(BLDDIR)/dep

SUBDIR_R ::= release
SUBDIR_D ::= debug

OBJDIR_R ::= $(OBJDIR)/$(SUBDIR_R)
OBJDIR_D ::= $(OBJDIR)/$(SUBDIR_D)
DEPDIR_R ::= $(DEPDIR)/$(SUBDIR_R)
DEPDIR_D ::= $(DEPDIR)/$(SUBDIR_D)

SOURCES ::= $(call rwildcard,$(SRCDIR)/,*$(SRCEXT))
OBJS_R  ::= $(patsubst $(SRCDIR)/%$(SRCEXT),$(OBJDIR_R)/%.o,$(SOURCES))
OBJS_D  ::= $(patsubst $(SRCDIR)/%$(SRCEXT),$(OBJDIR_D)/%.o,$(SOURCES))

DEPS ::= $(patsubst $(SRCDIR)/%$(SRCEXT),$(DEPDIR_R)/%.d,$(SOURCES)) \
         $(patsubst $(SRCDIR)/%$(SRCEXT),$(DEPDIR_D)/%.d,$(SOURCES))

BIN_R ::= $(BINDIR)/$(BINNAME_R)
BIN_D ::= $(BINDIR)/$(BINNAME_D)

DEPGENFLAGS_R = -MT "$@" -MMD -MP -MF $(DEPDIR_R)/$*.Td
DEPGENFLAGS_D = -MT "$@" -MMD -MP -MF $(DEPDIR_D)/$*.Td

POSTCOMPILE_R = mv -f $(DEPDIR_R)/$*.Td $(DEPDIR_R)/$*.d && touch $@
POSTCOMPILE_D = mv -f $(DEPDIR_D)/$*.Td $(DEPDIR_D)/$*.d && touch $@


# Targets

.PRECIOUS: $(BINDIR)/. $(SRCDIR)/. \
           $(addsuffix .,$(dir $(OBJS_R))) $(addsuffix .,$(dir $(OBJS_D))) \
           $(addsuffix .,$(dir $(DEPS)))

.PHONY: all run
all: release
run: rrun

.PHONY: debug release
debug: $(BIN_D)
release: $(BIN_R)

.PHONY: rrun drun
rrun: release
	$(BIN_R) $(ARGS)
drun: debug
	$(BIN_D) $(ARGS)

%/.:
	mkdir -p $@

$(BIN_R): $(OBJS_R) | $(BINDIR)/.
	$(MKBIN_R)

$(BIN_D): $(OBJS_D) | $(BINDIR)/.
	$(MKBIN_D)

$(DEPS): ;
include $(wildcard $(DEPS))

.PHONY: c
c:
	-if [ -d '$(OBJDIR)' ]; then find '$(OBJDIR)' -type f -name '*.o'  -exec $(RM) {} +; fi
	-if [ -d '$(DEPDIR)' ]; then find '$(DEPDIR)' -type f -name '*.d'  -exec $(RM) {} +; fi
	-if [ -d '$(DEPDIR)' ]; then find '$(DEPDIR)' -type f -name '*.Td' -exec $(RM) {} +; fi
	-find $(OBJDIR) $(DEPDIR) -type d -empty -exec 'rmdir' '-p' {} \; 2>/dev/null || true

.PHONY: clean
clean: c
	-$(RM) $(BIN_R)
	-$(RM) $(BIN_D)
	-if [ -d $(BINDIR) ]; then rmdir --ignore-fail-on-non-empty -p "$$(cd '$(BINDIR)'; pwd)"; fi


.SECONDEXPANSION:

$(OBJDIR_R)/%.o: $(SRCDIR)/%$(SRCEXT) $(DEPDIR_R)/%.d | $$(dir $$@). $$(dir $(DEPDIR_R)/$$*.d).
	$(CXX) $(CXXFLAGS) $(FLAGS_R) $(INCFLAGS) -c $< -o $@ $(DEPGENFLAGS_R)
	$(POSTCOMPILE_R)

$(OBJDIR_D)/%.o: $(SRCDIR)/%$(SRCEXT) $(DEPDIR_D)/%.d | $$(dir $$@). $$(dir $(DEPDIR_D)/$$*.d).
	$(CXX) $(CXXFLAGS) $(FLAGS_D) $(INCFLAGS) -c $< -o $@ $(DEPGENFLAGS_D)
	$(POSTCOMPILE_D)
