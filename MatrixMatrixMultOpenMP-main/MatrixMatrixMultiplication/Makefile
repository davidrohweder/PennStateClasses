CC=g++
CFLAGS= -I. -O3 -mavx -fopenmp
DEPS=hw4_part2.hpp
LIB=lib_%.a

cpp_files=$(wildcard *.cpp)
a_files=$(patsubst %.cpp,$(LIB),$(cpp_files))

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS);

$(LIB): %.o
	ar rcs $@ $<

all: $(a_files)

.PHONY: clean
clean:
	rm -f $(wildcard *.o) $(wildcard *.a)
