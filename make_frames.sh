


for ((i = 0; i < 10; i++)); do
    sed -i "/offset =/c\\\t\toffset = ${i}*2*pi\/60;\\\\" ferris_wheel.jgr
    jgraph ferris_wheel.jgr | convert -density 300 - -quality 100 shape.jpg
    ./shift_pixels $i
done
