# RayTracer
Another ray tracer based on Shirley's raytracing series. 
![renderer](https://user-images.githubusercontent.com/30566358/221235731-e96baeca-29c4-46c1-aaef-30538fe5a26b.jpg)
### Structure
I made a few design choices that differ from Peter Shirley's original implementation.
- I use Eigen library for primitive type definitions (such as colors, points, and rays). This is useful since Eigen already provides many accelerated operations on vector and matrix types used in ray tracing. 
- I implemented a GUI using OpenGL and ImGUI for easier rendering. I also implemented basic parallelisation with OpenMP (which caused the installation for MacOS to be a bit more difficult).
### Download
```
git clone --recurse-submodules git@github.com:maturk/RayTracer.git
cd RayTracer
```
### Installation
#### Linux
Simply running 
```
bash build.sh
```
should work :)
#### MacOS
Some prerequisites:
```
brew install llvm
brew install libomp
```
I set the path to clang++ in CMakeLists.txt to <em>"/opt/homebrew/opt/llvm/bin/clang++"</em>. Make sure this is the same directory indicated when installing llvm with ``brew install llvm``. 
Then run:
```
bash build.sh
```
### Useful commands
Run the main GUI with
```
./build/RayTracer
```
Build the project with
```
bash build.sh
```
Convert "saved" ppm image to jpg if you have ffmpeg
```
bash convert.sh
```
### Resources
 - for raytracing: https://raytracing.github.io/books/RayTracingInOneWeekend.html
 - for the GUI: https://learnopengl.com
