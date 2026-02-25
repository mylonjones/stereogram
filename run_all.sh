make

echo "creating pattern"
jgraph make_pattern.jgr | convert -density 300 - -quality 100 pattern.jpg

echo "creating stereogram frames"
for ((i = 0; i < 10; i++)); do
    sed -i "/offset =/c\\\t\toffset = ${i}*2*pi\/60;\\\\" ferris_wheel.jgr
    jgraph ferris_wheel.jgr | convert -density 300 - -quality 100 shape.jpg
    ./shift_pixels $i
done

./make_gif