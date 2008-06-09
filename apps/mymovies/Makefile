# replace this with an scons script

all: build

build: resources.qrc addeditmoviedlg.ui
	rm -f *.pyc
	pyrcc4 resources.qrc -o qrc_resources.py
	pyuic4 addeditmoviedlg.ui -o ui_addeditmoviedlg.py

clean:
	rm -f *.pyc
	rm -f qrc_resources.py ui_addeditmoviedlg.py

.PHONY: clean

	
