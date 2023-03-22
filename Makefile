CXX = g++
SOURCEDIR = src
INCLUDEDIR = inc
BINDIR = bin
OBJSDIR = obj
FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
REBUILDABLES = $(OBJSDIR) $(BINDIR)

TARGET = DataVisual
SOURCES = Main Application State StateStack TitleState Utility MenuState
OBJS = $(SOURCES:%=$(OBJSDIR)/%.o)

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
	$(HIDE)$(CXX) -o $@ -c $< -I $(INCLUDEDIR)

clean:
	$(HIDE)echo Removing $(REBUILDABLES)
	$(HIDE)rm -rf $(REBUILDABLES)
	$(HIDE)echo Clean done