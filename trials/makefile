#Its not a real makefile. You should run it as "source makefile"
g++ trialGas.cxx ../src/castGas.cxx ../src/castMagnet.cxx -I ../inc/ -o bin/trialGas
g++ trialMagnet.cxx ../src/castGas.cxx ../src/castMagnet.cxx -I ../inc/ -o bin/trialMagnet
g++ conversionProb.cxx ../src/castConversion.cxx ../src/castGas.cxx ../src/castMagnet.cxx -I ../inc/ -o bin/conversionProb
g++ testDetector.cxx ../src/castDetector.cxx -I ../inc/ -o bin/testDetector
g++ testExposure.cxx ../src/castExposure.cxx -I ../inc/ -o bin/testExposure

g++ testLike.cxx ../src/castDetector.cxx ../src/castConversion.cxx ../src/castMagnet.cxx ../src/../src/castGas.cxx ../src/castExposure.cxx ../src/castTracking.cxx ../src/castLike.cxx -I ../inc/ -o bin/testLike && bin/testLike

