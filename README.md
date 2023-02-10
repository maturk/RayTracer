# RayTracer
Another ray tracer. 

### Structure
I made a few design choices that differ from Peter Shirley's original implementation.
- I use Eigen library for primitive type definitions (such as colors, points, and rays). This is useful since Eigen already provides many accelerated operations on vector and matrix types used in ray tracing. 

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
Render ouput image to file in ./out folder
```
bash run.sh
```
### Resources
 https://raytracing.github.io/books/RayTracingInOneWeekend.html