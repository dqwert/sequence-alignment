all: sequence_alignment basic efficient

sequence_alignment: SequenceAlignment
	g++ -std=c++11 -o sequence_alignment sequence_alignment.cpp SequenceAlignment.o

basic: SequenceAlignment
	g++ -std=c++11 -o basic basic.cpp SequenceAlignment.o

efficient: SequenceAlignment
	g++ -std=c++11 -o efficient efficient.cpp SequenceAlignment.o

SequenceAlignment:
	g++ -std=c++11 -c SequenceAlignment.hpp SequenceAlignment.cpp

clean:
	rm -rf *.out *.o *.gch efficient basic sequence_alignment *.tmp
