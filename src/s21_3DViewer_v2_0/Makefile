SHELL=/bin/sh
GXX=g++
CFLAGS=-g -Wall -Wextra -Werror -std=c++17 -O2
DIST=3DViewer_v2_0

CXX_H          := h
CXX_EXT        := cc

DIR_MODEL   := Model
DIR_CONT   := Controller
DIR_VIEW   := View

HEADERS        = $(wildcard $(DIR_MODEL)*/*.$(CXX_H) $(DIR_CONT)*/*.$(CXX_H) $(DIR_VIEW)*/*.$(CXX_H))
CC_FILE      = $(wildcard $(DIR_MODEL)*/*.$(CXX_EXT) $(DIR_CONT)*/*.$(CXX_EXT) $(DIR_VIEW)*/*.$(CXX_EXT))

CC_FILE_MODEL = $(wildcard $(DIR_MODEL)*/*.$(CXX_EXT))

# NAME OF PC CORE
UNAME_S        := $(shell uname -s)

ifeq ($(UNAME_S),Linux) # Linux
	OPEN_CMD          = xdg-open
	TEST_CHECK_LIB    = -lgtest
	ADD_LIB           = -lm -pthread
endif
# FIX
ifeq ($(UNAME_S),Darwin) # MacOS
	OPEN_CMD          := open
	TEST_CHECK_LIB    := -lgtest
	ADD_LIB           := -lm -pthread
endif



all: clean dist tests install dvi

install:
	rm -rf ${PWD}/../build
	mkdir ${PWD}/../build
	qmake6 3DViewer_v2_0.pro -o ${PWD}/../build
	$(MAKE) -C ${PWD}/../build

uninstall:
	rm -rf ${PWD}/../build

tests: clean
	$(GXX) $(CFLAGS)  ${CC_FILE_MODEL} Tests/tests.cc $(TEST_CHECK_LIB) $(ADD_LIB) -o test 
	./test

dist:
	rm -rf ${HOME}/Desktop/${DIST}
	mkdir ${HOME}/Desktop/${DIST}
	cp -r ${PWD}/* ${HOME}/Desktop/${DIST}
	tar -czvf ${HOME}/Desktop/${DIST}.tar.gz ${HOME}/Desktop/${DIST}
	rm -rf ${HOME}/Desktop/${DIST}

dvi:
	doxygen Doxyfile
	open html/index.html

clean:
	rm -rf test *.dSYM ${HOME}/Desktop/${DIST}.tar.gz 3DViewer_v2_0.pro.* latex html

style_check:
	clang-format --style=Google -n *.cc ${CC_FILE} ${HEADERS}

style_fix:
	clang-format --style=Google -i *.cc ${CC_FILE} ${HEADERS}

