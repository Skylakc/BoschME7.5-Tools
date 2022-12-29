RM	= rm -f
AR	= ar rcs
ECHO	= @echo

#CASAN	= -fsanitize=address -fsanitize=undefined -fno-sanitize=alignment
#LASAN	= -lasan -lubsan

CFLAGS	+= -Wall -O2 -g -Werror -MMD $(CASAN) $(CDEFS)

CDEFS	+= -D__GIT_VERSION=\"$(GIT_VERSION)\"

GIT_VERSION = $(shell sh -c 'git describe --tags --abbrev=4 --dirty --always')
SYS	:= $(shell gcc -dumpmachine)
#SYS	:= i686-w64-mingw32

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
CC	= gcc
LD	= gcc
LDFLAGS += -L /opt/homebrew/lib -Linifile $(LASAN) $(GMP_LINK) -lgmp -Wl,-dynamic
CFLAGS  += -I /opt/homebrew/include
else
CC	= $(SYS)-gcc
LD	= $(SYS)-gcc
LDFLAGS += -Linifile $(LASAN) $(GMP_LINK) -lgmp -Wl,-Bdynamic
endif

SRC     = $(notdir $(foreach dir, ., $(wildcard $(dir)/*.c)))

ifneq (, $(findstring mingw, $(SYS)))
GMP_LINK = -Wl,-Bstatic
EXE_EXT = .exe
else ifneq (, $(findstring cygwin, $(SYS)))
EXE_EXT = -cyg.exe
endif
