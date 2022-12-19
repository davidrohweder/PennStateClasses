CC=g++
CFLAGS= -I. -O3 -mavx
DEPS = hw3_part1.hpp
OBJ = hw3_part1.o
LIB = lib_sd_thread.a

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB): $(OBJ)
	ar rcs lib_sd_thread.a $<

.PHONY: clean
clean:
	rm -f $(OBJ) $(LIB)
