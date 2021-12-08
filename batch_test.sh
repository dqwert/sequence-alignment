rm -rf output/*
mkdir -p output

rm -rf sequence_alignment

make clean
make sequence_alignment

max=15
for (( i=1; i <= max; i++ ))
do
    echo "processing data/input${i}.txt"
    ./sequence_alignment basic "data/input${i}.txt"
    mv output.txt output/basic${i}.txt

    ./sequence_alignment efficient "data/input${i}.txt"
    mv output.txt output/efficient${i}.txt
done

make clean