# targets: prerequisites
# 	command
#variable := value
#variable = value

projectPath := /home/samuel_sewall/cppWriter
cppWriterFile := $(projectPath)/src/cppWriter.cpp
researchFile := $(projectPath)/src/research.cpp
testFile := $(projectPath)/src/tests.cpp
cppWriterExecutable := $(projectPath)/build/cppWriterExecutable.o
researchExecutable := $(projectPath)/build/researchExecutable.o

all: cppWriterExecutable researchExecutable 

cppWriterExecutable: $(cppWriterFile)
	g++ -o $(cppWriterExecutable) $(cppWriterFile) -l sqlite3 -g

researchExecutable: $(researchFile)
	g++ -o $(researchExecutable) $(researchFile) -l sqlite3 -g

gtests: $(testFile) $(cppWriterFile)
	g++ -Wextra -Werror -O3 -std=c++17 -pthread -g -D GTEST \
    -I"$(projectPath)/gtestTry2/googletest/googletest/include" -I"$(projectPath)/gtestTry2/googletest/googlemock/include" \
    -o $(projectPath)/build/a \
    -l sqlite3 \
    $(testFile) \
    $(cppWriterFile) \
    $(projectPath)/gtestTry2/bin/libgtest.a $(projectPath)/gtestTry2/bin/libgtest_main.a \
	&& $(projectPath)/build/a

clean:
	rm -f $(cppWriterExecutable)
	rm -f $(researchExecutable)
	rm -f $(projectPath)/build/a