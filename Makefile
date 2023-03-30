CXX = g++
SOURCEDIR = src
INCLUDEDIR = inc
BINDIR = bin
OBJSDIR = obj
FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -O2  
REBUILDABLES = $(OBJSDIR) $(BINDIR)

TARGET = DataVisual
SOURCES = $(wildcard $(SOURCEDIR)/*.cpp)
OBJS = $(SOURCES:$(SOURCEDIR)/%.cpp=$(OBJSDIR)/%.o)
DEPS = $(OBJS:(OBJSDIR)/%.o=(OBJSDIR)/%.d)

# Hide or not the calls depending of VERBOSE
VERBOSE = FALSE
ifeq ($(VERBOSE),TRUE)
    HIDE =  
else
    HIDE = @
endif
# Compile program
all: dir build
	$(HIDE)echo All done

build: $(BINDIR)/$(TARGET)

# Compile & run program
run: build
	$(HIDE)./$(BINDIR)/$(TARGET)

dir:
	$(HIDE)[ -d $(BINDIR) ] && echo $(BINDIR)/ found || echo Make directory $(BINDIR)/
	$(HIDE)mkdir -p $(BINDIR)
	$(HIDE)[ -d $(OBJSDIR) ] && echo $(OBJSDIR)/ found || echo Make directory $(OBJSDIR)/
	$(HIDE)mkdir -p $(OBJSDIR)

$(BINDIR)/$(TARGET): $(OBJS)
	$(HIDE)echo Linking $@
	$(HIDE)$(CXX) -o $@ $^ $(FLAGS)

$(OBJSDIR)/%.o: $(SOURCEDIR)/%.cpp
	$(HIDE)echo Building $@ from $<
	$(HIDE)$(CXX) -o $@ -MMD -c $< -I $(INCLUDEDIR)

-include $(DEPS)

clean:
	$(HIDE)echo Removing $(REBUILDABLES)
	$(HIDE)rm -rf $(REBUILDABLES)
	$(HIDE)echo Clean done

rebuild: clean all