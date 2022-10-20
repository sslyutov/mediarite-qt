# \brief media application for common purposes
# \file media-rite.pri
# \date September 12, 2022
# \author Sergey Slyutov

INCLUDEPATH += "./"

SRCROOT = ./source

HEADERS += $${SRCROOT}/videorecorder.h

HEADERS += $${SRCROOT}/constants.h

HEADERS += $${SRCROOT}/playthrough.h

SOURCES += $${SRCROOT}/media-rite.cpp

SOURCES += $${SRCROOT}/videorecorder.cpp

SOURCES += $${SRCROOT}/playthrough.cpp

FORMS += $${SRCROOT}/videorecorder.ui

