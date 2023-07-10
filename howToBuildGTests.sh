# in general, need to (one time) build the googletests from source (could use Cmake like Google suggests, but I like knowing how to do this)
# then use those archive/binaries to build my gtests


# ----------
# from here: https://stackoverflow.com/questions/72108314/how-do-i-build-and-use-googletest-gtest-and-googlemock-gmock-with-gcc-g-or
# Clone the repo
git clone https://github.com/google/googletest.git

# Build all of gtest and gmock as static library archive `*.a` files

time g++ -Wall -Wextra -Werror -O3 -std=c++17 -pthread -c \
    -I"googletest/googletest/include" -I"googletest/googletest" \
    -I"googletest/googlemock/include" -I"googletest/googlemock" \
    googletest/googletest/src/gtest-all.cc \
    googletest/googletest/src/gtest_main.cc \
    googletest/googlemock/src/gmock-all.cc \
    googletest/googlemock/src/gmock_main.cc

# move all of the object files just created to a "bin" dir
mkdir -p bin
mv -t bin gtest-all.o gtest_main.o gmock-all.o gmock_main.o

# Use the `ar` "archive" utility to create the *.a static library archive files
# from the 4 object files above
time ar -rv bin/libgtest.a bin/gtest-all.o
time ar -rv bin/libgtest_main.a bin/gtest_main.o
time ar -rv bin/libgmock.a bin/gmock-all.o
time ar -rv bin/libgmock_main.a bin/gmock_main.o
# ----------

time ( \
    time g++ -Wall -Wextra -Werror -O3 -std=c++17 -pthread \
    -I"/home/samuel_sewall/cppWriter/gtestTry2/googletest/googletest/include" -I"/home/samuel_sewall/cppWriter/gtestTry2/googletest/googlemock/include" \
    /home/samuel_sewall/cppWriter/gtestTry2-MyTests/myown_unittest.cpp \
    /home/samuel_sewall/cppWriter/gtestTry2-MyTests/myown.cpp \
    /home/samuel_sewall/cppWriter/gtestTry2/bin/libgtest.a /home/samuel_sewall/cppWriter/gtestTry2/bin/libgtest_main.a \
    -o /home/samuel_sewall/cppWriter/gtestTry2/bin/a \
    && time /home/samuel_sewall/cppWriter/gtestTry2/bin/a \
)

# and then for my cppWriter.cpp file:
#got rid of -Wall

time ( \
    time g++ -Wextra -Werror -O3 -std=c++17 -pthread -g -D GTEST \
    -I"/home/samuel_sewall/cppWriter/gtestTry2/googletest/googletest/include" -I"/home/samuel_sewall/cppWriter/gtestTry2/googletest/googlemock/include" \
    /home/samuel_sewall/cppWriter/src/tests.cpp \
    /home/samuel_sewall/cppWriter/src/cppWriter.cpp \
    /home/samuel_sewall/cppWriter/gtestTry2/bin/libgtest.a /home/samuel_sewall/cppWriter/gtestTry2/bin/libgtest_main.a \
    -o /home/samuel_sewall/cppWriter/build/a \
    -l sqlite3 \
    && time /home/samuel_sewall/cppWriter/build/a \
)

#could get rid of the time commands, if I really wanted
#here's how to build just the executable:
g++ -o ./executable src/cppWriter.cpp -l sqlite3 -g

#here's the minimum for building and running the gtest executable
g++ -Wextra -Werror -O3 -std=c++17 -pthread -g -D GTEST \
    -I"/home/samuel_sewall/cppWriter/gtestTry2/googletest/googletest/include" -I"/home/samuel_sewall/cppWriter/gtestTry2/googletest/googlemock/include" \
    -o /home/samuel_sewall/cppWriter/build/a \
    -l sqlite3 \
    /home/samuel_sewall/cppWriter/src/tests.cpp \
    /home/samuel_sewall/cppWriter/src/cppWriter.cpp \
    /home/samuel_sewall/cppWriter/gtestTry2/bin/libgtest.a /home/samuel_sewall/cppWriter/gtestTry2/bin/libgtest_main.a \
&& /home/samuel_sewall/cppWriter/build/a