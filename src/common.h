#if !defined(__RAYTRACER_COMMON_H)
#define __RAYTRACER_COMMON_H

/* Convenience definitions */
#define RAYTRACER_NAMESPACE_BEGIN namespace raytracer {
#define RAYTRACER_NAMESPACE_END }

// Common includes
#include <Eigen/Core>
#include <memory>
#include <random>

RAYTRACER_NAMESPACE_BEGIN
// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Common primitive typedefs
typedef Eigen::Matrix<float, 3, 1> Point3f;
typedef Eigen::Matrix<float, 2, 1> Point2f;
typedef Eigen::Matrix<float, 3, 1> Vector3f;
typedef Eigen::Matrix<float, 3, 1> Color;

// Constants
const double M_INF = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;
const double M_EPSILON = 0.001;

// Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * M_PI / 180.0;
}

inline double random() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random(float min, float max) {
    static std::uniform_real_distribution<float> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}


inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

RAYTRACER_NAMESPACE_END

#endif /* __RAYTRACER_COMMON_H */