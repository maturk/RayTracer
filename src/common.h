#if !defined(__RAYTRACER_COMMON_H)
#define __RAYTRACER_COMMON_H

/* Convenience definitions */
#define RAYTRACER_NAMESPACE_BEGIN namespace raytracer {
#define RAYTRACER_NAMESPACE_END }


// Common includes
#include <Eigen/Core>

RAYTRACER_NAMESPACE_BEGIN

// Common primitive typedefs
typedef Eigen::Matrix<float, 3, 1> Point3f;
typedef Eigen::Matrix<float, 3, 1> Vector3f;
typedef Eigen::Matrix<float, 3, 1> Color;


Point3f test(2,2,2);

RAYTRACER_NAMESPACE_END

#endif /* __RAYTRACER_COMMON_H */