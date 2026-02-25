make

jgraph make_pattern.jgr | convert -density 300 - -quality 100 pattern.jpg

./make_frames.sh

./make_gif