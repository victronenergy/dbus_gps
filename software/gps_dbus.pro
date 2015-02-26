INCLUDEPATH += \
    inc \
    ext/velib/inc

target = $$(TARGET)
CONFIG += link_pkgconfig

equals(target,ccgx) {
    CONFIG(debug, debug|release) {
        TARGET = obj/ccgx-linux-arm-gnueabi-debug/gps_dbus
        target.sources = obj/ccgx-linux-arm-gnueabi-debug
        target.path += /opt/gps
    }
    CONFIG(release, debug|release) {
        TARGET = obj/ccgx-linux-arm-gnueabi-release/gps_dbus
        target.sources = obj/ccgx-linux-arm-gnueabi-release
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

    INCLUDEPATH += src/platform/pc

    target.files = gps_dbus
    target.path += /root
}

INSTALLS += target

SOURCES += \
    ext/velib/src/plt/posix_ctx.c \
    ext/velib/src/plt/posix_serial.c \
    ext/velib/src/plt/serial.c \
    ext/velib/src/types/ve_dbus_item.c \
    ext/velib/src/types/ve_item.c \
    ext/velib/src/types/ve_stamp.c \
    ext/velib/src/types/ve_str.c \
    ext/velib/src/types/ve_variant.c \
    ext/velib/src/types/ve_variant_print.c \
    ext/velib/src/utils/ve_logger.c \
    ext/velib/src/utils/ve_arg.c \
    ext/velib/task/task/main_libevent.c \
    ext/velib/task/task/platform_init.c \
    src/gps.c \
    src/gps_fh.c \
    src/platform/pc/console.c \
    src/platform/pc/serial_hal_pc.c \
    src/task.c \
    src/values_dbus_service.c \

OTHER_FILES += \
    Makefile \
    rules.mk \
    src/platform/pc/rules.mk \
    src/platform/rules.mk \
    src/rules.mk \
    velib_path.mk \

HEADERS += \
    inc/dev_reg.h \
    inc/gps.h \
    inc/serial_hal.h \
    inc/values.h \
    inc/velib/velib_config_app_common.h \
    inc/version.h \
    src/platform/pc/velib/velib_config_app.h \


DEFINES += \
    CFG_WITH_LIBEVENT DEBUG

unix {
    DEFINES += DBUS
    PKGCONFIG += dbus-1 libevent libevent_pthreads
}
