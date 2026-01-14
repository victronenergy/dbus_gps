DBUS = 1

T = gps_dbus$(EXT)

TARGETS += $T
INSTALL_BIN += $T

SUBDIRS += ext/velib
$T_DEPS += $(call subtree_tgts,$(d)/ext/velib)

SUBDIRS += ext/veutil
$T_DEPS += $(call subtree_tgts,$(d)/ext/veutil)

SUBDIRS += src
$T_DEPS += $(call subtree_tgts,$(d)/src)

INCLUDES += src/platform/pc

DEFINES += DBUS
override CFLAGS += $(shell pkg-config --cflags dbus-1)
$T_LIBS += -ldl `pkg-config --libs dbus-1` -levent -lm

ifdef WINDOWS
CFLAGS += -DUNICODE -D_UNICODE
$T_LIBS += -lwinmm -lShlwapi
endif

INCLUDES += inc
INCLUDES += ext/velib/inc
INCLUDES += ext/veutil/inc
