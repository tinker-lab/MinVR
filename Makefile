#.SILENT:

ifneq ($(WINDIR),)
  ARCH=WIN32
  GBUILDSTR=WIN32-i686-vc10
  ifeq ($(shell uname -o), Cygwin)
    WINCURDIR = $(shell cygpath -wm $(CURDIR))
  else
    WINCURDIR = $(CURDIR)
  endif
else
  UNAME=$(shell uname)
  ifeq ($(UNAME), Linux)
    ARCH=linux
    NPROCS = $(shell nproc)
    CUR_DIR_NAME:=$(shell basename $(CURDIR))
  else ifeq ($(UNAME), Darwin)
    ARCH=OSX
  else
    $(error The Makefile does not recognize the architecture: $(UNAME))
  endif
endif


all: gen

gen:
	mkdir -p ./build
    ifeq ($(ARCH), linux)
	  mkdir -p ./build/Release
	  mkdir -p ./build/Debug	  
	  cd ./build/Release; cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(CURDIR)/build/install ../../
	  cd ./build/Debug; cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$(CURDIR)/build/install ../../
    else ifeq ($(ARCH), WIN32)
	  cd ./build; cmake -DGBUILDSTR=$(GBUILDSTR) -DCMAKE_INSTALL_PREFIX=$(WINCURDIR)/build/install ../ -G "Visual Studio 12 Win64"
    else ifeq ($(ARCH), OSX)
	  cd ./build; cmake -DCMAKE_INSTALL_PREFIX=$(CURDIR)/build/install ../ -G Xcode
    endif

use_boost:
	mkdir -p ./build
    ifeq ($(ARCH), linux)
	  mkdir -p ./build/Release
	  mkdir -p ./build/Debug	  
	  cd ./build/Release; cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(CURDIR)/build/install -DUSE_BOOST=ON ../../
	  cd ./build/Debug; cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$(CURDIR)/build/install -DUSE_BOOST=ON ../../
    else ifeq ($(ARCH), WIN32)
	  cd ./build; cmake -DGBUILDSTR=$(GBUILDSTR) -DCMAKE_INSTALL_PREFIX=$(WINCURDIR)/build/install -DUSE_BOOST=ON ../ -G "Visual Studio 11 Win64"
    else ifeq ($(ARCH), OSX)
	  cd ./build; cmake -DCMAKE_INSTALL_PREFIX=$(CURDIR)/build/install -DUSE_BOOST=ON ../ -G Xcode
    endif

debug:
    ifeq ($(ARCH), linux) 
	  cd ./build/Debug; make -j$(NPROC)
    else
	  @echo "Open the project file to build the project on this architecture."
    endif

release opt:
    ifeq ($(ARCH), linux) 
	  cd ./build/Release; make -j$(NPROC)
    else
	  @echo "Open the project file to build the project on this architecture."
    endif

install:
    ifeq ($(ARCH), linux) 
	  cd ./build/Debug; make install -j$(NPROC)
	  cd ./build/Release; make install -j$(NPROC)
    else
	  @echo "Open the project file to run make install on this architecture."
    endif

eclipse: install
    ifeq ($(ARCH), linux)
	  mkdir -p ../$(CUR_DIR_NAME)_build
	  cd ../$(CUR_DIR_NAME)_build; cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_DEPENDENCIES=OFF -DCMAKE_INSTALL_PREFIX=$(CURDIR)/install ../$(CUR_DIR_NAME)/ -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_VERSION=4.3
    endif

clean:
    ifeq ($(ARCH), linux) 
	  cd ./build/Debug; make clean
	  cd ./build/Release; make clean
    else
	  @echo "Open the project file to run make clean on this architecture."
    endif

clobber:
	rm -rf ./build
	rm -rf ./dependencies/boost
	rm -rf ./dependencies/glfw
	rm -rf ./dependencies/vrpn
	rm -rf ./install
	rm -rf ../$(CUR_DIR_NAME)_build	  
