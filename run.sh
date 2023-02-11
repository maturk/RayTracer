# run main program and save render to output file


mkdir -p out
rm -f out/image.ppm
./build/RayTracer >> out/image.ppm
ffmpeg -i out/image.ppm out/image.jpg
