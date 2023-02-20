CC=gcc
CFLAGS= -g -Wall

TARGET = main

all: $(TARGET)

$(TARGET): $(TARGET).cc
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cc -l simlib -lstdc++

example1: example1.cc
	$(CC) $(CFLAGS) -o example1 example1.cc -l simlib -lstdc++

example2: example2.cc
	$(CC) $(CFLAGS) -o example2 example2.cc -l simlib -lstdc++

example3: example3.cc
	$(CC) $(CFLAGS) -o example3 example3.cc -l simlib -lstdc++

example4: example4.cc
	$(CC) $(CFLAGS) -o example4 example4.cc -l simlib -lstdc++

example5: example5.cc
	$(CC) $(CFLAGS) -o example5 example5.cc -l simlib -lstdc++

example6: example6.cc
	$(CC) $(CFLAGS) -o example6 example6.cc -l simlib -lstdc++

generators: generators.cc
	$(CC) $(CFLAGS) -o generators generators.cc -l simlib -lstdc++ -lm

simulation_methods: simulation_methods.cc
	$(CC) $(CFLAGS) -o simulation_methods simulation_methods.cc -l simlib -lstdc++

clean:
	$(RM) $(TARGET)
	$(RM) *.out

run:
	./$(TARGET)

run1:
	rm example1 if [ -f example1 ]
	make example1
	./example1
	cat example1.out

run2:
	rm example2 if [ -f example2 ]
	make example2
	./example2
	cat example2.out

run3:
	rm example3 if [ -f example3 ]
	make example3
	./example3
	cat example3.out

run4:
	rm example4 if [ -f example4 ]
	make example4
	./example4
	cat example4.out

run5:
	rm example5 if [ -f example5 ]
	make example5
	./example5
	cat example5.out

run6:
	rm example6 if [ -f example6 ]
	make example6
	./example6
	cat example6.out

generator:
	rm generators if [ -f generators ]
	make generators
	./generators

sim_methods:
	rm simulation_methods if [ -f simulation_methods ]
	make simulation_methods
	./simulation_methods
