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
truePartialTextFile := $(projectPath)/trueText/TruePartial.txt
truePartialPlusGuessTextFile := $(projectPath)/trueText/TruePartialPlusGuess.txt

all: cppWriterExecutable researchExecutable 

cppWriterExecutable: $(cppWriterFile)
	g++ -Wextra -Werror -Wall -o $(cppWriterExecutable) $(cppWriterFile) -l sqlite3 -g

researchExecutable: $(researchFile)
	g++ -Wextra -Werror -Wall -o $(researchExecutable) $(researchFile) -l sqlite3 -g

gtests: $(testFile) $(cppWriterFile)
	g++ -Wextra -Werror -O3 -std=c++17 -pthread -g -D GTEST \
    -I"$(projectPath)/googleCode/googletest/googletest/include" -I"$(projectPath)/googleCode/googletest/googlemock/include" \
    -o $(projectPath)/build/a \
    $(testFile) \
    $(cppWriterFile) \
    $(projectPath)/googleCode/bin/libgtest.a $(projectPath)/googleCode/bin/libgtest_main.a \
	-l sqlite3 \
	&& $(projectPath)/build/a

reset: clearDB clearPartialText

clearDB:
	sqlite3 db/CppWriterDB.db "delete from current_true_word"

clearPartialText:
	truncate -s 0 $(truePartialTextFile)
	truncate -s 0 $(truePartialPlusGuessTextFile)

clean:
	rm -f $(cppWriterExecutable)
	rm -f $(researchExecutable)
	rm -f $(projectPath)/build/a