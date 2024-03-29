C_CPP_COMMON_COMPILE_FLAGS:= -O3 -g -Wall -Wextra -Wuninitialized -Winit-self -Wstrict-aliasing -Wfloat-equal -Wshadow -Wconversion -Werror -fpack-struct=4

C_COMPILE:=gcc -c
C_COMPILE_FLAGS:=-pedantic-errors

CPP_COMPILE:=gcc -c
CPP_COMPILE_FLAGS:=

C_LIBS=
C_LINK:=g++
C_LINK_FLAGS:=-g -lm $(C_LIBS)

ifdef WINDIR
C_TARGET_EXTENSION := .exe
C_LINK_FLAGS += 
else
C_LINK_FLAGS +=
C_TARGET_EXTENSION := 
endif	# ifdef WINDIR

HUFFMAN_C_DEPENDS:= binaryUtil.c main.c
HUFFMAN_CPP_DEPENDS:=
C_PROJECTS:=huffman

all: $(C_PROJECTS)

define upperString
$(shell echo $1 | tr [a-z] [A-Z] )
endef

define C_PROJECT_template
$2_C_SRCFILES += $1.c
$2_C_SRCFILES += $($2_C_DEPENDS)
$2_CPP_SRCFILES:=$($2_CPP_DEPENDS)
$2_C_DEPEND_OBJS:=$($2_C_DEPENDS:.c=.o)
$2_CPP_DEPEND_OBJS:=$($2_CPP_DEPENDS:.cpp=.o)
$2_C_DFILES:=$$($2_C_SRCFILES:.c=.d)
$2_CPP_DFILES:=$$($2_CPP_SRCFILES:.cpp=.d)

$2_OBJFILE:=$1.o
$2_C_OBJFILES:=$$($2_C_SRCFILES:.c=.o)
$2_CPP_OBJFILES:=$$($2_CPP_SRCFILES:.cpp=.o)

C_SRCFILES += $$($2_C_SRCFILES) $$($2_CPP_SRCFILES)
C_OBJFILES += $$($2_C_OBJFILES) $$($2_CPP_OBJFILES)
C_DFILES += $$($2_C_DFILES) $$($2_CPP_DFILES)

C_TARGETS += $1

$$($2_OBJFILE): $$($2_C_DEPEND_OBJS) $$($2_CPP_DEPEND_OBJS) $1.c
$1: $$($2_C_OBJFILES) $$($2_CPP_OBJFILES)
endef
     
$(foreach project,$(C_PROJECTS),$(eval $(call C_PROJECT_template,$(project),$(call upperString,$(project)))))

C_TARGET_EXES := $(foreach target,$(C_TARGETS),$(target)$(C_TARGET_EXTENSION))

test:
	@echo C_PROJECTS=$(C_PROJECTS)
	@echo C_TARGETS=$(C_TARGETS)
	@echo C_SRCFILES=$(C_SRCFILES)
	@echo C_OBJFILES=$(C_OBJFILES)
	@echo C_DFILES=$(C_DFILES)
	@echo C_TARGET_EXTENSION=$(C_TARGET_EXTENSION)
	@echo C_TARGET_EXES=$(C_TARGET_EXES)
	@echo C_LINK_FLAGS=$(C_LINK_FLAGS)

%.8: %.go
	@echo Go Compiling $<
	@$(GC) $<

%: %.8
	@echo Go Linking $<
	@$(LD) -o $@ $<

%.o: %.cpp
	@echo C++ Compiling $<
	@$(CPP_COMPILE) -MMD $(C_CPP_COMMON_COMPILE_FLAGS) $(CPP_COMPILE_FLAGS) -o $*.o $<

%.o: %.c
	@echo C Compiling $<
	@$(C_COMPILE) -MMD $(C_CPP_COMMON_COMPILE_FLAGS) $(C_COMPILE_FLAGS) -o $*.o $<

%: %.o
	@echo C Linking $<
	@$(C_LINK) -o $@ $^ $(C_LINK_FLAGS)

.PHONY: all clean nuke 
.SUFFIXES:            # Delete the default suffixes

FORCE:

clean: FORCE
	@$(RM) -vf $(C_OBJFILES)
	@$(RM) -vf $(C_DFILES)
	@$(RM) -vf tags
	@$(RM) -vf cscope.out
	@$(RM) -vf cscope.in.out
	@$(RM) -vf cscope.po.out

nuke: clean
	@$(RM) -vf $(C_TARGET_EXES)

sinclude $(C_DFILES)
