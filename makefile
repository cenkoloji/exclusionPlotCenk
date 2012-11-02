# ---------------------------------------------------------------------
# 	MAKEFILE : CAST software
# ---------------------------------------------------------------------

# All "-I /usr/include/shift/"  added by Cenk. (There was an error)

CPPFLAGS = -fPIC -g -Wall

# Defining the dirs structure

CAST_LIB = $(CAST_PATH)/lib
CAST_SRC = $(CAST_PATH)/src
CAST_OBJ = $(CAST_PATH)/obj
CAST_INC = $(CAST_PATH)/inc
CAST_BIN = $(CAST_PATH)/bin

INC_DIRS = -I$(CAST_PATH)/inc

# set of objets used in compilation
OBJS =  $(CAST_OBJ)/castConversion.o $(CAST_OBJ)/castDetector.o $(CAST_OBJ)/castExposure.o $(CAST_OBJ)/castGas.o $(CAST_OBJ)/castLike.o $(CAST_OBJ)/castMagnet.o $(CAST_OBJ)/castTracking.o
BINARIES =  $(CAST_BIN)/testLike


#defining compiling targets
all: Wellcome castTracking castExposure castMagnet castDetector castGas castConversion castGas castLike libCAST programs
programs: testLike

castConversion: $(CAST_OBJ)/castConversion.o
castDetector: $(CAST_OBJ)/castDetector.o
castExposure: $(CAST_OBJ)/castExposure.o
castGas     : $(CAST_OBJ)/castGas.o
castLike    : $(CAST_OBJ)/castLike.o
castMagnet  : $(CAST_OBJ)/castMagnet.o
castTracking: $(CAST_OBJ)/castTracking.o
libCAST: $(CAST_LIB)/libCAST.so
testLike: $(CAST_BIN)/testLike

Wellcome:
	@echo -e "\033[40m\033[0;1;32m -------------------------------------------------------------------- \033[0m"
	@echo -e "\033[40m\033[0;1;32m Makefile script to compile the CAST analysis library and binaries \033[0m"
	@echo -e "\033[54m\033[0;1;32m 2nd of November 2012 \033[0m"
	@echo -e "\033[40m\033[0;1;32m -------------------------------------------------------------------- \033[0m"
	@echo -e "\033[47m\033[0;1;38m ----------------------------------   \033[0m"
	@echo -e "\033[47m\033[0;1;38m  Cenk Yildiz \033[0m"
	@echo -e "\033[47m\033[0;1;38m  cenk.yildiz@cern.ch   \033[0m"
	@echo -e "\033[47m\033[0;1;38m  Nice color stuff taken from Javier Galan's makefiles   \033[0m"
	@echo -e "\033[47m\033[0;1;38m ----------------------------------  \033[0m"
	@echo

# TARGETS

$(CAST_OBJ)/castTracking.o:
	@echo -e "\033[40m\033[0;31m  Compiling castTracking object ........\033[0m"
	g++ $(CPPFLAGS) -c $(INC_DIRS) -o $(CAST_OBJ)/castTracking.o $(CAST_SRC)/castTracking.cxx

$(CAST_OBJ)/castDetector.o:
	@echo -e "\033[40m\033[0;31m  Compiling castDetector object ........\033[0m"
	g++ $(CPPFLAGS) -c $(INC_DIRS) -o $(CAST_OBJ)/castDetector.o $(CAST_SRC)/castDetector.cxx

$(CAST_OBJ)/castExposure.o:
	@echo -e "\033[40m\033[0;31m  Compiling castExposure object ........\033[0m"
	g++ $(CPPFLAGS) -c $(INC_DIRS) -o $(CAST_OBJ)/castExposure.o $(CAST_SRC)/castExposure.cxx

$(CAST_OBJ)/castMagnet.o:
	@echo -e "\033[40m\033[0;31m  Compiling castMagnet object ........\033[0m"
	g++ $(CPPFLAGS) -c $(INC_DIRS) -o $(CAST_OBJ)/castMagnet.o $(CAST_SRC)/castMagnet.cxx

$(CAST_OBJ)/castGas.o: $(CAST_SRC)/castMagnet.cxx $(CAST_INC)/castMagnet.h
	@echo -e "\033[40m\033[0;31m  Compiling castGas object ........\033[0m"
	g++ $(CPPFLAGS) -c $(INC_DIRS) -o $(CAST_OBJ)/castGas.o $(CAST_SRC)/castGas.cxx

$(CAST_OBJ)/castConversion.o : $(CAST_SRC)/castMagnet.cxx $(CAST_INC)/castMagnet.h $(CAST_SRC)/castGas.cxx $(CAST_INC)/castGas.h
	@echo -e "\033[40m\033[0;31m  Compiling castConversion object ........\033[0m"
	g++ $(CPPFLAGS) -c $(INC_DIRS) -o $(CAST_OBJ)/castConversion.o $(CAST_SRC)/castConversion.cxx

$(CAST_OBJ)/castLike.o: $(CAST_SRC)/castConversion.cxx $(CAST_INC)/castConversion.h $(CAST_SRC)/castDetector.cxx $(CAST_INC)/castDetector.h $(CAST_SRC)/castGas.cxx $(CAST_INC)/castGas.h $(CAST_SRC)/castExposure.cxx $(CAST_INC)/castExposure.h $(CAST_SRC)/castMagnet.cxx $(CAST_INC)/castMagnet.h $(CAST_SRC)/castTracking.cxx $(CAST_INC)/castTracking.h
	@echo -e "\033[40m\033[0;31m  compiling castlike object ........\033[0m"
	g++ $(CPPFLAGS) -c $(INC_DIRS) -o $(CAST_OBJ)/castLike.o $(CAST_SRC)/castLike.cxx

$(CAST_LIB)/libCAST.so: $(CAST_SRC)/castConversion.cxx $(CAST_INC)/castConversion.h $(CAST_SRC)/castDetector.cxx $(CAST_INC)/castDetector.h $(CAST_SRC)/castGas.cxx $(CAST_INC)/castGas.h $(CAST_SRC)/castExposure.cxx $(CAST_INC)/castExposure.h $(CAST_SRC)/castMagnet.cxx $(CAST_INC)/castMagnet.h $(CAST_SRC)/castTracking.cxx $(CAST_INC)/castTracking.h $(CAST_SRC)/castLike.cxx $(CAST_INC)/castLike.h
	@echo -e "\033[40m\033[0;31m  creating libCAST.so shared object........\033[0m"
	g++ -shared $(CPPFLAGS) $(INC_DIRS) -o $(CAST_LIB)/libCAST.so $(OBJS)

$(CAST_BIN)/testLike: $(CAST_SRC)/castConversion.cxx $(CAST_INC)/castConversion.h $(CAST_SRC)/castDetector.cxx $(CAST_INC)/castDetector.h $(CAST_SRC)/castGas.cxx $(CAST_INC)/castGas.h $(CAST_SRC)/castExposure.cxx $(CAST_INC)/castExposure.h $(CAST_SRC)/castMagnet.cxx $(CAST_INC)/castMagnet.h $(CAST_SRC)/castTracking.cxx $(CAST_INC)/castTracking.h $(CAST_SRC)/castLike.cxx $(CAST_INC)/castLike.h $(CAST_LIB)/libCAST.so $(CAST_SRC)/testLike.cxx
	@echo -e "\033[40m\033[0;31m  compiling testLike program........\033[0m"
	g++ $(CPPFLAGS)  $(INC_DIRS) -o $(CAST_BIN)/testLike $(CAST_SRC)/testLike.cxx $(CAST_LIB)/libCAST.so

clean:
	@echo -e "\033[40m\033[0;34m  Removing objects ........\033[0m"
	@rm $(OBJS)
	@rm $(CAST_LIB)/libCAST.so
	@rm $(BINARIES)
