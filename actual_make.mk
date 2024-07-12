# Compiles the game (for real)

TYPE = ps-exe

CURRDIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# Source files
SRCS += $(CURRDIR)thirdparty/nugget/common/crt0/crt0.s
SRCS += $(CURRDIR)thirdparty/nugget/common/syscalls/printf.s

# Compiler flags
CPPFLAGS += -I$(CURRDIR)thirdparty/nugget/psyq/include
CPPFLAGS += -I$(CURRDIR)psyq/include

# Linker flags
LDFLAGS += -L$(CURRDIR)thirdparty/nugget/psyq/lib
LDFLAGS += -L$(CURRDIR)psyq/lib

LDFLAGS += -Wl,--start-group
LDFLAGS += -lapi
LDFLAGS += -lc
LDFLAGS += -lc2
LDFLAGS += -lcard
LDFLAGS += -lcomb
LDFLAGS += -lds
LDFLAGS += -letc
LDFLAGS += -lgpu
LDFLAGS += -lgs
LDFLAGS += -lgte
LDFLAGS += -lgun
LDFLAGS += -lhmd
LDFLAGS += -lmath
LDFLAGS += -lmcrd
LDFLAGS += -lmcx
LDFLAGS += -lpad
LDFLAGS += -lpress
LDFLAGS += -lsio
LDFLAGS += -lsnd
LDFLAGS += -lspu
LDFLAGS += -ltap
LDFLAGS += -lcd
LDFLAGS += -Wl,--end-group

include $(CURRDIR)thirdparty/nugget/common.mk
