# Generated Makefile. При изменении этого файла вручную удалите данную строку
CSOURCES = $(filter-out lex.yy.c y.tab.c,$(wildcard *.c))
CPPSOURCES = $(filter-out lex.yy.cpp y.tab.cpp %_unittest.cpp,$(wildcard *.cpp))
GRMSOURCES = $(wildcard *.grm)
MSOURCES = $(wildcard *.m)
M1SOURCE = $(firstword $(filter %domains.m, $(MSOURCES)))
M1HEADER = $(patsubst %.m,%.h,$(M1SOURCE))
M2SOURCE = $(firstword $(filter-out %domains.m, $(MSOURCES)))
M2GENER = $(patsubst %.m,%.cpp,$(M2SOURCE))
LEXSOURCES = $(wildcard *.lex)
CHEADERS = $(filter-out y.tab.h %domains.h,$(wildcard *.h))
CPPHEADERS = $(wildcard *.hpp)
ifneq (,$(LEXSOURCES))
ifneq (,$(strip $(CPPSOURCES) $(MSOURCES)))
LEXGENER = lex.yy.cpp
else
LEXGENER = lex.yy.c
endif
else
LEXGENER =
endif
ifneq (,$(GRMSOURCES))
ifneq (,$(strip $(CPPSOURCES) $(MSOURCES)))
GRMGENER = y.tab.cpp
else
GRMGENER = y.tab.c
endif
GRMHEADER = y.tab.h
else
GRMGENER =
GRMHEADER =
endif
ifeq ($(TARGET),debug)
CCFLAGS = -g -O0
else
CCFLAGS = -O1  
endif
GCC = gcc -std=c11
GPP = g++ -std=c++14
ifneq (,$(strip $(CPPSOURCES) $(MSOURCES)))
CC = $(GPP)
LIBS = conio2 bgi gdi32 comdlg32 uuid oleaut32 ole32
OBJS = $(sort $(patsubst %.c,$(TARGET)/%.o,$(CSOURCES)) $(patsubst %.cpp,$(TARGET)/%.o,$(CPPSOURCES) $(LEXGENER) $(GRMGENER) $(M2GENER)))
else
CC = $(GCC)
LIBS = conio2
OBJS = $(sort $(patsubst %.c,$(TARGET)/%.o,$(CSOURCES) $(LEXGENER) $(GRMGENER)) $(patsubst %.cpp,$(TARGET)/%.o,$(CPPSOURCES)))
endif

CCCS = -finput-charset=windows-1251 -fexec-charset=windows-1251
HS = $(sort $(CHEADERS) $(M1HEADER) $(GRMHEADER))

all: $(TARGET)/program.exe

$(TARGET)/program.exe $(OBJS): | $(TARGET)

$(TARGET):
	mkdir $(TARGET)

$(TARGET)/lex.yy.o: | $(LEXGENER)

$(TARGET)/y.tab.o: | $(GRMGENER)

$(LEXGENER): $(LEXSOURCES)
	flex -o$(LEXGENER) $<

$(GRMGENER) $(GRMHEADER): $(GRMSOURCES)
	bison -o$(GRMGENER) -d $<
ifneq ($(GRMGENER),y.tab.c)
	-del y.tab.h
	ren y.tab.hpp y.tab.h
endif

$(M1HEADER): $(M1SOURCE)
	memphis2 -h $<

$(M2GENER): $(M2SOURCE)
	memphis2 $<

$(TARGET)/%.o: %.cpp $(CPPHEADERS) $(HS)
	$(GPP) $(CCFLAGS) $(if $(filter uc%,$<),$(CCCS),) -o $@ -c $<

$(TARGET)/%.o: %.c $(HS)
	$(GCC) $(CCFLAGS) $(if $(filter uc%,$<),$(CCCS),) -o $@ -c $<

$(TARGET)/program.exe: $(OBJS) Makefile
	$(CC) $(CCFLAGS) -Wl,--stack,83886080 -o $(TARGET)/program $(OBJS) "C:\Users\smbdsbrain\Desktop\MinIDE\SciTE/../lib/initansicp.o" $(addprefix -l,$(LIBS))

clean:
	-rmdir /q /s release
	-rmdir /q /s debug

