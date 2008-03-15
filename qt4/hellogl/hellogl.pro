HEADERS = glwidget.h window.h
SOURCES = glwidget.cpp main.cpp window.cpp
QT += opengl

# install
target.path = hellogl
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS hellogl.pro
sources.path = .
INSTALLS += target sources

