# RayTracer
Another ray tracer based on Shirley's raytracing series. 
![renderer](https://user-images.githubusercontent.com/30566358/221235731-e96baeca-29c4-46c1-aaef-30538fe5a26b.jpg)
### Structure
I made a few design choices that differ from Peter Shirley's original implementation.
- I use Eigen library for primitive type definitions (such as colors, points, and rays). This is useful since Eigen already provides many accelerated operations on vector and matrix types used in ray tracing. 
- I implemented a GUI using OpenGL and ImGUI for easier rendering.
### Download
```
git clone --recurse-submodules git@github.com:maturk/RayTracer.git
cd RayTracer
bash build.sh
```
### Useful commands
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
