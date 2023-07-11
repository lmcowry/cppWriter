# targets: prerequisites
# 	command
#variable := value
#variable = value

projectPath := .
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
    -I"$(projectPath)/googleCode/googletest/googletest/include" -I"$(projectPath)/googleCode/googletest/googlemock/include" \
    -o $(projectPath)/build/a \
    $(testFile) \
    $(cppWriterFile) \
    $(projectPath)/googleCode/bin/libgtest.a $(projectPath)/googleCode/bin/libgtest_main.a \
	-l sqlite3 \
	&& $(projectPath)/build/a

clean:
	rm -f $(cppWriterExecutable)
	rm -f $(researchExecutable)
	rm -f $(projectPath)/build/a