QT -= qt core gui

INCLUDEPATH += ext/glfw/include
INCLUDEPATH += ext/glfw/deps
HEADERS = $$files(src/*.h)
SOURCES = $$files(src/*.c)
