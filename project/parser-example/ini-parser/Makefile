CXXFLAGS := -O3 \
	    -Wall -Wextra -pedantic \
	    -Winit-self \
	    -Wshadow \
	    -Wformat=2 \
	    -Wwrite-strings \
	    -DNDEBUG \
	    ${CXXFLAGS}

ifeq (${CXX}, g++)
	CXXFLAGS += -Wno-missing-field-initializers
endif

ifeq (${CXX}, clang++)
	CXXFLAGS += -Wimplicit-fallthrough
endif

CXX := ${CXX} -std=c++14

all: ini
ini: test.o ini.o
	${CXX} ${CXXFLAGS} $^ -o $@

.cpp.o:
	${CXX} ${CXXFLAGS} $^ -o $@

ini.cc: ini.rl
	ragel -C $< -o $@

clean:
	rm ini *.o

.PHONY: clean
