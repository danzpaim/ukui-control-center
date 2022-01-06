TEMPLATE = subdirs

CONFIG += ordered

include($$PWD/log/log.pri)

SUBDIRS = \
    registeredQDbus \
    plugins\
    shell
 #    tastenbrett \


TRANSLATIONS += \
    shell/res/i18n/zh_CN.ts \

# Automating generation .qm files from .ts files
CONFIG(release, debug|release) {
    !system($$PWD/translate_generation.sh): error("Failed to generate translation")
}

isEmpty(PREFIX) {
    PREFIX = /usr
}

qm_files.path = $${PREFIX}/share/ukui-control-center/shell/res/i18n/
qm_files.files = shell/res/i18n/*.qm

icon_files.path = $${PREFIX}/share/ukui-control-center/shell/res/secondaryleftmenu/
icon_files.files = shell/res/secondaryleftmenu/*.svg

search_file.path = $${PREFIX}/share/ukui-control-center/shell/res/
search_file.files = shell/res/search.xml


INSTALLS += \
    qm_files \
    icon_files\
    search_file
