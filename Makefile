CXX = g++
SOURCEDIR = src
INCLUDEDIR = "./inc"
LIBDIR =
OBJSDIR = obj
FLAGS = -lsfml-graphics -lsfml-window -lsfml-system
REBUILDABLES = $(OBJSDIR) 

TARGET = DataVisual
SOURCES = $(wildcard $(SOURCEDIR)/*.cpp) $(wildcard $(SOURCEDIR)/**/*.cpp)
OBJS = $(SOURCES:$(SOURCEDIR)/%.cpp=$(OBJSDIR)/%.o)
DEPS = $(OBJS:(OBJSDIR)/%.o=(OBJSDIR)/%.d)
MKDIR = mkdir

RELEASE = FALSE
ifeq ($(OS),Windows_NT) 
	FLAGS += -lcomdlg32 -lole32 -lcomctl32 -loleaut32 -luuid -DWIN32
	OBJS += $(OBJSDIR)/icon.res
	INCLUDEDIR += "C:/SFML/include"
	LIBDIR += "C:/SFML/lib"
	RMDIR = rd /s /q
else     
	MKDIR += -p
	RMDIR = rm -rf
	UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        FLAGS += -DLINUX
    endif
endif

ifeq ($(BUILD),debug)   
	# "Debug" build - no optimization, and debugging symbols
	FLAGS += -Og -g -DDEBUG
else
	# "Release" build - optimization, and no debug symbols
	FLAGS += -Ofast -s
	ifeq ($(OS),Windows_NT) 
		FLAGS += -static -mwindows
	endif
endif


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

debug: 
	make "BUILD=debug"

build: $(TARGET)

# Run program
run: 
ifeq ($(OS),Windows_NT) 
	$(HIDE).\$(TARGET).exe
else
	$(HIDE)./$(TARGET)
endif

dir:
ifeq ($(OS),Windows_NT) 
	if not exist $(OBJSDIR) $(MKDIR) $(OBJSDIR)
	if not exist $(OBJSDIR)\Array $(MKDIR) $(OBJSDIR)\Array
	if not exist $(OBJSDIR)\Graphics $(MKDIR) $(OBJSDIR)\Graphics
	if not exist $(OBJSDIR)\GUI $(MKDIR) $(OBJSDIR)\GUI
	if not exist $(OBJSDIR)\Queue $(MKDIR) $(OBJSDIR)\Queue
	if not exist $(OBJSDIR)\SLL  $(MKDIR) $(OBJSDIR)\SLL 
	if not exist $(OBJSDIR)\DLL  $(MKDIR) $(OBJSDIR)\DLL 
	if not exist $(OBJSDIR)\CLL  $(MKDIR) $(OBJSDIR)\CLL 
	if not exist $(OBJSDIR)\Stack $(MKDIR) $(OBJSDIR)\Stack
	if not exist $(OBJSDIR)\States $(MKDIR) $(OBJSDIR)\States
else 
	$(MKDIR) $(OBJSDIR)
	$(MKDIR) $(OBJSDIR)/Array
	$(MKDIR) $(OBJSDIR)/Graphics
	$(MKDIR) $(OBJSDIR)/GUI
	$(MKDIR) $(OBJSDIR)/Queue
	$(MKDIR) $(OBJSDIR)/SLL 
	$(MKDIR) $(OBJSDIR)/DLL
	$(MKDIR) $(OBJSDIR)/CLL
	$(MKDIR) $(OBJSDIR)/Stack
	$(MKDIR) $(OBJSDIR)/States
endif

$(TARGET): $(OBJS)
	$(HIDE)echo Linking $@
	$(HIDE)$(CXX) -o $@ $^ $(FLAGS) $(LIBDIR:%=-L%)

$(OBJSDIR)/%.o: $(SOURCEDIR)/%.cpp
	$(HIDE)echo Building $@ from $<
	$(HIDE)$(CXX) -o $@ -MMD -c $< $(INCLUDEDIR:%=-I%)

$(OBJSDIR)/icon.res: $(SOURCEDIR)/icon.rc
	@echo Building $@ from $<
	$(HIDE)windres $< -O coff -o $@

-include $(DEPS)

clean:
ifeq ($(OS),Windows_NT) 
	$(HIDE)if exist $(REBUILDABLES) $(RMDIR) $(REBUILDABLES) && echo Removing $(REBUILDABLES)
else
	$(HIDE)$(RMDIR) $(REBUILDABLES) && echo Removing $(REBUILDABLES)
endif
	$(HIDE)echo Clean done

rebuild: clean all