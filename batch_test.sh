rm sequence_alignment_test

g++ -O2 -std=c++17 sequence_alignment.cpp SequenceAlignment.hpp SequenceAlignment.cpp

max=15
for (( i=1; i <= max; i++ ))
do
    ./a.out basic "data/input$i.txt"
    mv output.txt output/basic$i.txt

    ./a.out efficient "data/input$i.txt"
    mv output.txt output/efficient$i.txt
done
