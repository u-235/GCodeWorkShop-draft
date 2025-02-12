

PROJECT_ROOT_PATH = $${PWD}

CONFIG(debug, debug|release) {
    BUILD_FLAG = debug
    CONFIG *= warn_on
} else {
    BUILD_FLAG = release
    # Suppress debug message
    DEFINES *= QT_NO_DEBUG_OUTPUT # QT_NO_INFO_OUTPUT
}

APP_BINS = $$shadowed($${PROJECT_ROOT_PATH})/bin

CONFIG *= c++11
CONFIG -= debug_and_release debug_and_release_target

# moc doesn't detect Q_OS_LINUX correctly, so add this to make it work
linux*:DEFINES *= __linux__

!CONFIG(staticlib) {
    DESTDIR = $$APP_BINS
}

!defined(PREFIX, var) {
    unix:PREFIX = /usr/local/
    win32:PREFIX = $$(programfiles)
}

#------------------#
# Link modules     #
#------------------#

contains(MODULES, kdiff3) {
    INCLUDEPATH += $${PROJECT_ROOT_PATH}/3rdparty
    LIBS += -L$$shadowed($${PROJECT_ROOT_PATH}/3rdparty/kdiff3)
    LIBS += -lkdiff3
    # QTextCodec in Qt6
    greaterThan(QT_MAJOR_VERSION, 5): QT *= core5compat
}

contains(MODULES, qtsingleapplication) {
    INCLUDEPATH += $${PROJECT_ROOT_PATH}/3rdparty/include
    LIBS += -L$$shadowed($${PROJECT_ROOT_PATH}/3rdparty/qtsingleapplication)
    LIBS += -lqtsingleapplication
}

contains(MODULES, addons) {
    MODULES *= basic
    INCLUDEPATH += $${PROJECT_ROOT_PATH}/addons/include
    LIBS += -laddons
    LIBS += -L$$shadowed($${PROJECT_ROOT_PATH}/addons)
}

contains(MODULES, GCodeShared) {
    INCLUDEPATH += $${PROJECT_ROOT_PATH}/gcodeshared \
                   $${PROJECT_ROOT_PATH}/gcodeshared/include \
                   $$shadowed($${PROJECT_ROOT_PATH}/gcodeshared)
    LIBS += -L$$shadowed($${PROJECT_ROOT_PATH}/gcodeshared)
    LIBS += -lgcodeshared
    # QTextCodec in Qt6
    greaterThan(QT_MAJOR_VERSION, 5): QT *= core5compat
}

contains(MODULES, GCodeWorkShop) {
    INCLUDEPATH += $${PROJECT_ROOT_PATH}/gcodeworkshop/include
#   LIBS += -L$$shadowed($${PROJECT_ROOT_PATH}/gcodeworkshop)
#   LIBS += -lgcodeworkshop
}

contains(MODULES, GCodeFileServer) {
    INCLUDEPATH += $${PROJECT_ROOT_PATH}/gcodefileserver
#   LIBS += -L$$shadowed($${PROJECT_ROOT_PATH}/gcodefileserver)
#   LIBS += -lgcodefileserver
}

contains(MODULES, basic) {
    INCLUDEPATH += $${PROJECT_ROOT_PATH}/3rdparty/basic
    LIBS += -L$$shadowed($${PROJECT_ROOT_PATH}/3rdparty/basic)
    LIBS += -lbasic
}


#------------------#
# iwyu support     #
#------------------#

# https://doc.qt.io/qt-5/qmake-manual.html
# https://wiki-qt-io-staging.herokuapp.com/Undocumented_QMake

defineReplace(iwyu_out) {
    return ($${OUT_PWD}/$$basename(1).log)
}

contains(USE, check_iwyu) {
    linux*:IWYU_FLAGS += -fPIE

    IWYU_FLAGS += $(DEFINES) -w $(INCPATH)

    for(include, IWYU_INCLUDES) {
        IWYU_FLAGS += -I$$include
    }

    IWYU_EXTRA_FLAGS += -Xiwyu --max_line_length=120 -Xiwyu --mapping_file=$${PROJECT_ROOT_PATH}/tools/iwyu.imp

    iwyu.output_function  = iwyu_out
    iwyu.commands = $${PROJECT_ROOT_PATH}/tools/iwyu_comp.sh $${IWYU_EXTRA_FLAGS} $${IWYU_FLAGS} ${QMAKE_FILE_NAME}
    iwyu.input = SOURCES
    iwyu.CONFIG += no_link target_predeps
    QMAKE_EXTRA_COMPILERS += iwyu
}


#
# function findFiles(dir, basename)
# Returns a list of files with this basename.
# The function searches for dir and subdir recursively.
#
defineReplace(findFiles) {
    dir = $$1
    base_name = $$2

    file_list = $$files($$dir/$$base_name)
    path_list = $$files($$dir/*)

    for(path, path_list) {
        file_list += $$findFiles($$path, $$base_name)
    }

    return ($$file_list)
}


defineReplace(getVersion) {
    version_file = gcodeshared/include/version.h
    !exists ($${PROJECT_ROOT_PATH}/$${version_file}): error(In function getVersion: File \"$${version_file}\" not found)
    version_header = $$cat($${PROJECT_ROOT_PATH}/$${version_file}, lines)

    for (str, version_header) {
        sub_str=$$split(str, " ")

        contains (sub_str, GCODEWORKSHOP_VERSION) {
        ver_str=$$find(sub_str, '".*"')
            !isEmpty(ver_str): return ($$split(ver_str, '"'))
        }
    }

    error(In function getVersion: GCODEWORKSHOP_VERSION not found in $${version_file})
}
