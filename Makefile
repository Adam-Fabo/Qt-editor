#Project: Editor and Interpret of structured functional blocks
#Subject: ICP at VUT FIT Brno
#Author: Stanislav Gabriš

AUTHORS=xfaboa00-xgabri18
SRCDIR=src
DOXYDIR=doc
EXAMPLEDIR=examples
DOXYCONFIG=Doxyfile
PROJECTFILE=ICP.pro
EXECUTABLE=ICP
VARIANT=2
TOPACK=$(SRCDIR) $(DOXYDIR) $(EXAMPLEDIR) README.txt Makefile

build: clean
	cd $(SRCDIR) ; qmake $(PROJECTFILE) ; make
run: clean build doxygen
	cd $(SRCDIR) ; ./$(EXECUTABLE)
doxygen:
	cd $(DOXYDIR) ; doxygen $(DOXYCONFIG)
pack:
	tar -czvf $(VARIANT)-$(AUTHORS).tar.gz $(TOPACK)
clean:
	cd $(SRCDIR) ; make clean ; rm -f Makefile
	cd $(DOXYDIR) ; rm -rf ./html	
