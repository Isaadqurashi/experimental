
CXXFLAGS=-std=c++17 -O2

OBJS=main.o other_file.o

TARGET=per_file_template_test

all:


main.o: main.cpp logv2.h
other_file.o: other_file.cpp logv2.h

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@  $(OBJS)

test: $(TARGET)
	./$(TARGET)

clean:
	rm $(TARGET) $(OBJS)

compile_commands.json:
	compiledb make

compiledb: compile_commands.json
