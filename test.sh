experiment(){
    input_file=$1
    res_file=$2
    bit=$3
    for sz in 0 2 4 8 16 32 64 128 256 512 1024 1200 1400 1600 1800 2048 2200 2400 2600 2800 3000 3200 3400 3600 4096 8192 16384 32768 65536 131072
    do
        echo $sz
        echo $sz >> $res_file
        ./obj/lz78_test $input_file $sz $bit >> $res_file
        echo >> $res_file
    done
}

experiments(){
    type=$1
    for bit in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
    do
        echo "$type - $bit"
        experiment "./data/${type}.${type}" "./result/${bit}_${type}.txt" $bit
    done
}

# show compression efficiency under different symbol bits
experiments txt
experiments html
experiments docx
experiments bmp
experiments mat