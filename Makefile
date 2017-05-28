DEPDIR = .d
ODIR = obj
SDIR = src
DIRS =
$(shell mkdir -p $(DEPDIR) >/dev/null)
$(shell find $(SDIR) -type d -exec mkdir -p $(ODIR)/\{\} >/dev/null \;)
$(shell find $(SDIR) -type d -exec mkdir -p $(DEPDIR)/\{\} >/dev/null \;)

DEPFLAGS = -MT $@ -MD -MP -MF $(DEPDIR)/$*.Td

CC = g++-4.9
CFLAGS = -std=c++1y -Wall -O2 -Wfatal-errors

COMPILE = $(CC) $(DEPFLAGS) $(CFLAGS) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

_SRC = $(wildcard $(SDIR)/*.cpp) $(wildcard $(SDIR)/*/*.cpp)
_OBJ = $(_SRC:.cpp=.o)
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(OBJ): $(ODIR)/%.o: %.cpp %.h $(DEPDIR)/%.d
	$(COMPILE) -o $@ $<
	$(POSTCOMPILE)

all: main.cpp $(OBJ)
	$(CC) -o bin/$@ main.cpp $(OBJ) $(CFLAGS)

debug: main.cpp $(OBJ)
	$(CC) -g -o bin/$@ main.cpp $(OBJ) $(CFLAGS)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

.PHONY: clean
clean:
	rm -rf obj/*
	rm -rf .d/*
	rm -rf bin/*
