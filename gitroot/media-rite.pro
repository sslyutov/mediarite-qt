# ------------------------------------------------------------------------
# author Sergey Slyutov
# date September 12, 2022
# media-rite media application for common purposes.
# ------------------------------------------------------------------------

TEMPLATE = app 

TARGET = mediarite

TARGET_EXT = .exe #use this tp stop qmake adding a number following TARGET filename.

VERSION = 1.0.1.0

QMAKE_TARGET_PRODUCT = "media-rite qt"

QMAKE_TARGET_COPYRIGHT =  "Sergey Slyutov" 

QMAKE_TARGET_DESCRIPTION = "media application for common purposes"

QT += widgets core gui multimedia multimediawidgets

CONFIG += c++20

CONFIG += debug

DESTDIR_DEBUG = ./DEBUG

DETIDIR_RELEASE = ./RELEASE

QMAKE_CXXFLAGS += /MP

RESOURCES = media-rite.qrc

#RC_FILE = media-rite.rc

RC_ICONS = ./resources/sss_b2_copy.ico

include (media-rite.pri)
