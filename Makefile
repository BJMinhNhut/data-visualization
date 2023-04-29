CXX = g++
SOURCEDIR = src
INCLUDEDIR = inc
BINDIR = bin
OBJSDIR = obj
FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -O2  
REBUILDABLES = $(OBJSDIR) $(BINDIR)

TARGET = DataVisual
SOURCES = $(wildcard $(SOURCEDIR)/*.cpp) $(wildcard $(SOURCEDIR)/**/*.cpp)
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

# Run program
run: build 
	$(HIDE)./$(BINDIR)/$(TARGET)

dir:
	$(HIDE)[ -d $(BINDIR) ] && echo $(BINDIR)/ found || echo Make directory $(BINDIR)/
	$(HIDE)mkdir -p $(BINDIR)
	$(HIDE)[ -d $(OBJSDIR) ] && echo $(OBJSDIR)/ found || echo Make directory $(OBJSDIR)/
	$(HIDE)mkdir -p $(OBJSDIR)
	$(HIDE)[ -d $(OBJSDIR)/GUI ] && echo $(OBJSDIR)/GUI/ found || echo Make directory $(OBJSDIR)/GUI/
	$(HIDE)mkdir -p $(OBJSDIR)/GUI/
	$(HIDE)[ -d $(OBJSDIR)/Graphics ] && echo $(OBJSDIR)/Graphics/ found || echo Make directory $(OBJSDIR)/Graphics/
	$(HIDE)mkdir -p $(OBJSDIR)/Graphics/
	$(HIDE)[ -d $(OBJSDIR)/States ] && echo $(OBJSDIR)/States/ found || echo Make directory $(OBJSDIR)/States/
	$(HIDE)mkdir -p $(OBJSDIR)/States/
	$(HIDE)[ -d $(OBJSDIR)/SLL ] && echo $(OBJSDIR)/SLL/ found || echo Make directory $(OBJSDIR)/SLL/
	$(HIDE)mkdir -p $(OBJSDIR)/SLL/
	$(HIDE)[ -d $(OBJSDIR)/Stack ] && echo $(OBJSDIR)/Stack/ found || echo Make directory $(OBJSDIR)/Stack/
	$(HIDE)mkdir -p $(OBJSDIR)/Stack/
	$(HIDE)[ -d $(OBJSDIR)/Queue ] && echo $(OBJSDIR)/Queue/ found || echo Make directory $(OBJSDIR)/Queue/
	$(HIDE)mkdir -p $(OBJSDIR)/Queue/

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