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

// Random number generators
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

// Clamp function
inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Specular reflection equation
Vector3f reflect(const Vector3f& v, const Vector3f& n) {
    return v - 2*v.dot(n)*n;
}

// Refraction (Snell's law)
Vector3f refract(const Vector3f& uv, const Vector3f& n, double etai_over_etat) {
    auto cos_theta = fmin((-uv).dot(n), 1.0);
    Vector3f r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    Vector3f r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.squaredNorm())) * n;
    return r_out_perp + r_out_parallel;
}

RAYTRACER_NAMESPACE_END

#endif /* __RAYTRACER_COMMON_H */