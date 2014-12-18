# ---------------------------------------------------------------------
# 	MAKEFILE : CAST software
# ---------------------------------------------------------------------


CPPFLAGS = -fPIC -g -O2

# Defining the dirs structure

CAST_LIB = $(CAST_PATH)/lib
CAST_SRC = $(CAST_PATH)/src
CAST_OBJ = $(CAST_PATH)/obj
CAST_INC = $(CAST_PATH)/inc
CAST_BIN = $(CAST_PATH)/bin

INC_DIRS = -I$(CAST_PATH)/inc

# set of objets used in compilation
OBJS =  $(CAST_OBJ)/castConversion.o $(CAST_OBJ)/castDetector.o $(CAST_OBJ)/castExposure.o $(CAST_OBJ)/castGas.o $(CAST_OBJ)/castLike.o $(CAST_OBJ)/castMagnet.o $(CAST_OBJ)/castTracking.o $(CAST_OBJ)/castProfile.o $(CAST_OBJ)/castConfig.o
BINARIES =  $(CAST_BIN)/testSim $(CAST_BIN)/singleDetector $(CAST_BIN)/density2mass  $(CAST_BIN)/conversionStudy $(CAST_BIN)/simulateData

#defining compiling targets
all: Wellcome castConfig castTracking castExposure castMagnet castDetector castGas castConversion castGas castLike castProfile libCAST programs
#programs: singleDetector density2mass conversionStudy
programs: conversionStudy testSim simulateData

castConversion: $(CAST_OBJ)/castConversion.o
castDetector: $(CAST_OBJ)/castDetector.o
castExposure: $(CAST_OBJ)/castExposure.o
castGas     : $(CAST_OBJ)/castGas.o
castConfig  : $(CAST_OBJ)/castConfig.o
castLike    : $(CAST_OBJ)/castLike.o
castMagnet  : $(CAST_OBJ)/castMagnet.o
castProfile : $(CAST_OBJ)/castProfile.o
castTracking: $(CAST_OBJ)/castTracking.o
libCAST: $(CAST_LIB)/libCAST.so
testSim: $(CAST_BIN)/testSim
stepSize: $(CAST_BIN)/stepSize
conversionStudy: $(CAST_BIN)/conversionStudy
simulateData: $(CAST_BIN)/simulateData
singleDetector: $(CAST_BIN)/singleDetector
density2mass: $(CAST_BIN)/density2mass

Wellcome:
	@echo -e "\033[40m\033[0;32m -------------------------------------------------------------------- \033[0m"
	@echo -e "\033[40m\033[0;32m Makefile script to compile the CAST analysis library and binaries \033[0m"
	@echo -e "\033[40m\033[0;32m 2nd of November 2012 \033[0m"
	@echo -e "\033[40m\033[0;32m -------------------------------------------------------------------- \033[0m"
	@echo -e "\033[40m\033[0;33m ----------------------------------   \033[0m"
	@echo -e "\033[40m\033[0;33m  Cenk Yildiz \033[0m"
	@echo -e "\033[40m\033[0;33m  cenk.yildiz@cern.ch   \033[0m"
	@echo -e "\033[40m\033[0;33m  Nice color stuff taken from Javier Galan's makefiles   \033[0m"
	@echo -e "\033[40m\033[0;33m ----------------------------------  \033[0m"
	@echo

# TARGETS

$(CAST_OBJ)/castConfig.o: $(CAST_SRC)/castConfig.cxx $(CAST_INC)/castConfig.h makefile
	@echo -e "\033[40m\033[0;31m  Compiling castConfig object ........\033[0m"
	@g++ $(CPPFLAGS) -c $(INC_DIRS) -o $(CAST_OBJ)/castConfig.o $(CAST_SRC)/castConfig.cxx

$(CAST_OBJ)/castTracking.o: makefile
	@echo -e "\033[40m\033[0;31m  Compiling castTracking object ........\033[0m"
	@g++ $(CPPFLAGS) -c $(INC_DIRS) -o $(CAST_OBJ)/castTracking.o $(CAST_SRC)/castTracking.cxx

$(CAST_OBJ)/castDetector.o: makefile $(CAST_SRC)/castDetector.cxx $(CAST_INC)/castDetector.h
	@echo -e "\033[40m\033[0;31m  Compiling castDetector object ........\033[0m"
	@g++ $(CPPFLAGS) -c $(INC_DIRS) -o $(CAST_OBJ)/castDetector.o $(CAST_SRC)/castDetector.cxx

$(CAST_OBJ)/castExposure.o: makefile $(CAST_SRC)/castExposure.cxx $(CAST_INC)/castExposure.h
	@echo -e "\033[40m\033[0;31m  Compiling castExposure object ........\033[0m"
	@g++ $(CPPFLAGS) -c $(INC_DIRS) -o $(CAST_OBJ)/castExposure.o $(CAST_SRC)/castExposure.cxx

$(CAST_OBJ)/castMagnet.o: $(CAST_SRC)/castMagnet.cxx $(CAST_INC)/castMagnet.h makefile
	@echo -e "\033[40m\033[0;31m  Compiling castMagnet object ........\033[0m"
	@g++ $(CPPFLAGS) -c $(INC_DIRS) -o $(CAST_OBJ)/castMagnet.o $(CAST_SRC)/castMagnet.cxx

$(CAST_OBJ)/castGas.o: $(CAST_SRC)/castConfig.cxx $(CAST_INC)/castConfig.h $(CAST_SRC)/castMagnet.cxx $(CAST_INC)/castMagnet.h $(CAST_SRC)/castGas.cxx $(CAST_INC)/castGas.h makefile
	@echo -e "\033[40m\033[0;31m  Compiling castGas object ........\033[0m"
	@g++ $(CPPFLAGS) -c $(INC_DIRS) -o $(CAST_OBJ)/castGas.o $(CAST_SRC)/castGas.cxx
 
$(CAST_OBJ)/castConversion.o : $(CAST_SRC)/castConfig.cxx $(CAST_INC)/castConfig.h $(CAST_SRC)/castMagnet.cxx $(CAST_INC)/castMagnet.h $(CAST_SRC)/castGas.cxx $(CAST_INC)/castGas.h $(CAST_SRC)/castConversion.cxx $(CAST_INC)/castConversion.h   makefile
	@echo -e "\033[40m\033[0;31m  Compiling castConversion object ........\033[0m"
	@g++ $(CPPFLAGS) -c $(INC_DIRS) -o $(CAST_OBJ)/castConversion.o $(CAST_SRC)/castConversion.cxx

$(CAST_OBJ)/castProfile.o : $(CAST_SRC)/castConfig.cxx $(CAST_INC)/castConfig.h $(CAST_SRC)/castGas.cxx $(CAST_INC)/castGas.h $(CAST_INC)/castProfile.h $(CAST_SRC)/castProfile.cxx  makefile
	@echo -e "\033[40m\033[0;31m  Compiling castProfile object ........\033[0m"
	@g++ $(CPPFLAGS) -c $(INC_DIRS) -o $(CAST_OBJ)/castProfile.o $(CAST_SRC)/castProfile.cxx

$(CAST_OBJ)/castLike.o: $(CAST_SRC)/castConfig.cxx $(CAST_INC)/castConfig.h $(CAST_SRC)/castConversion.cxx $(CAST_INC)/castConversion.h $(CAST_SRC)/castDetector.cxx $(CAST_INC)/castDetector.h $(CAST_SRC)/castGas.cxx $(CAST_INC)/castGas.h $(CAST_SRC)/castExposure.cxx $(CAST_INC)/castExposure.h $(CAST_SRC)/castMagnet.cxx $(CAST_INC)/castMagnet.h $(CAST_SRC)/castTracking.cxx $(CAST_INC)/castTracking.h $(CAST_INC)/castLike.h $(CAST_SRC)/castLike.cxx $(CAST_INC)/castProfile.h $(CAST_SRC)/castProfile.cxx  makefile
	@echo -e "\033[40m\033[0;31m  compiling castLike object ........\033[0m"
	@g++ $(CPPFLAGS) -c $(INC_DIRS) -o $(CAST_OBJ)/castLike.o $(CAST_SRC)/castLike.cxx

$(CAST_LIB)/libCAST.so: $(CAST_SRC)/castConversion.cxx $(CAST_INC)/castConversion.h $(CAST_SRC)/castDetector.cxx $(CAST_INC)/castDetector.h $(CAST_SRC)/castGas.cxx $(CAST_INC)/castGas.h $(CAST_SRC)/castExposure.cxx $(CAST_INC)/castExposure.h $(CAST_SRC)/castMagnet.cxx $(CAST_INC)/castMagnet.h $(CAST_SRC)/castTracking.cxx $(CAST_INC)/castTracking.h $(CAST_SRC)/castLike.cxx $(CAST_INC)/castLike.h $(CAST_INC)/castProfile.h $(CAST_SRC)/castProfile.cxx  $(CAST_SRC)/castConfig.cxx $(CAST_INC)/castConfig.h  makefile
	@echo -e "\033[40m\033[0;36m  creating libCAST.so shared object........\033[0m"
	@g++ -shared $(CPPFLAGS) $(INC_DIRS) -o $(CAST_LIB)/libCAST.so $(OBJS)

$(CAST_BIN)/testSim: $(CAST_SRC)/castConversion.cxx $(CAST_INC)/castConversion.h $(CAST_SRC)/castDetector.cxx $(CAST_INC)/castDetector.h $(CAST_SRC)/castGas.cxx $(CAST_INC)/castGas.h $(CAST_SRC)/castExposure.cxx $(CAST_INC)/castExposure.h $(CAST_SRC)/castMagnet.cxx $(CAST_INC)/castMagnet.h $(CAST_SRC)/castTracking.cxx $(CAST_INC)/castTracking.h $(CAST_SRC)/castLike.cxx $(CAST_INC)/castLike.h $(CAST_LIB)/libCAST.so $(CAST_SRC)/testSim.cxx $(CAST_SRC)/castConfig.cxx $(CAST_INC)/castConfig.h  makefile
	@echo -e "\033[40m\033[0;32m  compiling testSim program........\033[0m"
	@g++ $(CPPFLAGS)  $(INC_DIRS) -o $(CAST_BIN)/testSim $(CAST_SRC)/testSim.cxx $(CAST_LIB)/libCAST.so

$(CAST_BIN)/singleDetector: $(CAST_SRC)/castConversion.cxx $(CAST_INC)/castConversion.h $(CAST_SRC)/castDetector.cxx $(CAST_INC)/castDetector.h $(CAST_SRC)/castGas.cxx $(CAST_INC)/castGas.h $(CAST_SRC)/castExposure.cxx $(CAST_INC)/castExposure.h $(CAST_SRC)/castMagnet.cxx $(CAST_INC)/castMagnet.h $(CAST_SRC)/castTracking.cxx $(CAST_INC)/castTracking.h $(CAST_SRC)/castLike.cxx $(CAST_INC)/castLike.h $(CAST_LIB)/libCAST.so $(CAST_SRC)/singleDetector.cxx $(CAST_SRC)/castConfig.cxx $(CAST_INC)/castConfig.h  makefile
	@echo -e "\033[40m\033[0;32m  compiling singleDetector program........\033[0m"
	@g++ $(CPPFLAGS)  $(INC_DIRS) -o $(CAST_BIN)/singleDetector $(CAST_SRC)/singleDetector.cxx $(CAST_LIB)/libCAST.so

$(CAST_BIN)/density2mass:  $(CAST_SRC)/castGas.cxx $(CAST_INC)/castGas.h $(CAST_SRC)/castMagnet.cxx $(CAST_INC)/castMagnet.h $(CAST_LIB)/libCAST.so $(CAST_SRC)/density2mass.cxx makefile
	@echo -e "\033[40m\033[0;32m  compiling density2mass program........\033[0m"
	@g++ $(CPPFLAGS)  $(INC_DIRS) -o $(CAST_BIN)/density2mass $(CAST_SRC)/density2mass.cxx $(CAST_LIB)/libCAST.so

$(CAST_BIN)/stepSize: $(CAST_SRC)/castConversion.cxx $(CAST_INC)/castConversion.h $(CAST_SRC)/castDetector.cxx $(CAST_INC)/castDetector.h $(CAST_SRC)/castGas.cxx $(CAST_INC)/castGas.h $(CAST_SRC)/castExposure.cxx $(CAST_INC)/castExposure.h $(CAST_SRC)/castMagnet.cxx $(CAST_INC)/castMagnet.h $(CAST_SRC)/castTracking.cxx $(CAST_INC)/castTracking.h $(CAST_SRC)/castLike.cxx $(CAST_INC)/castLike.h $(CAST_LIB)/libCAST.so $(CAST_SRC)/stepSize.cxx $(CAST_SRC)/castConfig.cxx $(CAST_INC)/castConfig.h  makefile
	@echo -e "\033[40m\033[0;32m  compiling stepSize program........\033[0m"
	@g++ $(CPPFLAGS)  $(INC_DIRS) -o $(CAST_BIN)/stepSize $(CAST_SRC)/stepSize.cxx $(CAST_LIB)/libCAST.so


$(CAST_BIN)/conversionStudy: $(CAST_SRC)/castConversion.cxx $(CAST_INC)/castConversion.h $(CAST_SRC)/castDetector.cxx $(CAST_INC)/castDetector.h $(CAST_SRC)/castGas.cxx $(CAST_INC)/castGas.h $(CAST_SRC)/castExposure.cxx $(CAST_INC)/castExposure.h $(CAST_SRC)/castMagnet.cxx $(CAST_INC)/castMagnet.h $(CAST_SRC)/castTracking.cxx $(CAST_INC)/castTracking.h $(CAST_SRC)/castLike.cxx $(CAST_INC)/castLike.h $(CAST_LIB)/libCAST.so $(CAST_SRC)/conversionStudy.cxx $(CAST_SRC)/castConfig.cxx $(CAST_INC)/castConfig.h  makefile
	@echo -e "\033[40m\033[0;32m  compiling conversionStudy program........\033[0m"
	@g++ $(CPPFLAGS)  $(INC_DIRS) -o $(CAST_BIN)/conversionStudy $(CAST_SRC)/conversionStudy.cxx $(CAST_LIB)/libCAST.so

$(CAST_BIN)/simulateData: $(CAST_SRC)/castGas.cxx $(CAST_INC)/castGas.h $(CAST_SRC)/castMagnet.cxx $(CAST_INC)/castMagnet.h $(CAST_LIB)/libCAST.so $(CAST_SRC)/simulateData.cxx $(CAST_SRC)/castConfig.cxx $(CAST_INC)/castConfig.h  makefile
	@echo -e "\033[40m\033[0;32m  compiling simulateData program........\033[0m"
	@g++ $(CPPFLAGS)  $(INC_DIRS) -o $(CAST_BIN)/simulateData $(CAST_SRC)/simulateData.cxx $(CAST_LIB)/libCAST.so

clean:
	@echo -e "\033[40m\033[0;34m  Removing objects ........\033[0m"
	@rm $(OBJS)
	@rm $(CAST_LIB)/libCAST.so
	@rm $(BINARIES)
