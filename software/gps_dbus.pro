INCLUDEPATH += \
	inc \
	ext/velib/inc

target = $$(TARGET)

equals(target,bpp3) {
	CONFIG(debug, debug|release) {
		TARGET = obj/bpp3-linux-arm-gnueabi-debug/gps_dbus
		target.sources = obj/bpp3-linux-arm-gnueabi-debug
		target.path += /opt/gps
	}
	CONFIG(release, debug|release) {
		TARGET = obj/bpp3-linux-arm-gnueabi-release/gps_dbus
		target.sources = obj/bpp3-linux-arm-gnueabi-release
		target.path += /opt/color-control/gps
	}
	target.files = gps_dbus
} else {
	CONFIG(debug, debug|release) {
		TARGET = obj/pc-linux-i686-unknown-debug/gps_dbus
		target.sources = obj/pc-linux-i686-unknown-debug
	}
	CONFIG(release, debug|release) {
		TARGET = obj/pc-linux-i686-unknown-release/gps_dbus
		target.sources = obj/pc-linux-i686-unknown-release
	}
	target.files = gps_dbus
	target.path += /root
}

INSTALLS += target

SOURCES += \
	ext/velib/task/task/platform_init.c \
	ext/velib/task/task/main_libevent.c \
	ext/velib/src/plt/serial.c \
	ext/velib/src/plt/posix_serial.c \
	ext/velib/src/plt/posix_ctx.c \
	src/platform/pc/console.c \
	src/task.c \
	src/platform/pc/serial_hal_pc.c \
	src/values_dbus_service.c \
	src/gps.c \
	src/gps_fh.c \
	ext/velib/src/types/ve_variant_print.c \
	ext/velib/src/types/ve_variant.c \
	ext/velib/src/types/ve_str.c \
	ext/velib/src/types/ve_item.c \
	ext/velib/src/types/ve_dbus_item.c

OTHER_FILES += \
	velib_path.mk \
	rules.mk \
	Makefile \
	src/rules.mk \
	src/platform/rules.mk \
	src/platform/pc/rules.mk

HEADERS += \
	src/platform/pc/velib/velib_config_app.h \
	inc/velib/velib_config_app_common.h \
	inc/serial_hal.h \
	inc/version.h \
	inc/values.h \
	inc/gps.h

DEFINES += \
	CFG_WITH_LIBEVENT DEBUG
