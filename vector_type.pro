TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    vec.h \
    token.h \
    lexer.h \
    symtab.h \
    parser.h \
    func.h \
    screen.h

DISTFILES += \
    grammar \
    todo
