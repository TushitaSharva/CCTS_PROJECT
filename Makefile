CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Ilib/googletest/googletest/include -pthread
LDFLAGS = -pthread

# Source files
SRCS = NodeList.cpp
TEST_SRCS = test/test_nodelist.cpp
OBJS = $(SRCS:.cpp=.o)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# GoogleTest build
GTEST_DIR = lib/googletest
GTEST_MAIN = $(GTEST_DIR)/build/lib/libgtest_main.a
GTEST = $(GTEST_DIR)/build/lib/libgtest.a

all: gtest build

# Build GoogleTest
gtest:
	cd $(GTEST_DIR) && cmake -B build && cmake --build build

build: $(OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o test_runner $(OBJS) $(TEST_OBJS) $(GTEST) $(GTEST_MAIN) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TEST_OBJS) test_runner

test: build
	./test_runner
