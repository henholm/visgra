/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:24:56
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/sphere.h>
#include <labraytracer/util.h>

namespace inviwo {

Sphere::Sphere(const vec3& center, const double& radius) {
    center_ = center;
    radius_ = radius;
}

bool Sphere::closestIntersection(const Ray& ray, double maxLambda,
                                 RayIntersection& intersection) const {
    // Programming TASK 1: implement this method
    // Your code should compute the intersection between a ray and a sphere;

    // If you detect an intersection, the return type should look similar to this:
    // if(rayIntersectsSphere)
    // {
    //   intersection = RayIntersection(ray,shared_from_this(),lambda,normalVec,uvw);
    //   return true;
    // }
    //
    // Hints:
    // lambda is the distance form the ray origin to an the intersection point.
    // Ray origin p_r : ray.getOrigin()
    // Ray direction t_r : ray.getDirection()
    // If you need the intersection point, use ray.pointOnRay(lambda)
    // You can ignore the uvw (texture coordinates)

    const vec3 r0 = ray.getOrigin();
    const vec3 rd = ray.getDirection();

    float t = dot(center_ - r0, rd); // Center of intersection points
    vec3 p = r0 + rd * t;

    // Using equation of circle: x^2+y^2 = r^2 to find distance between t and intersection points.
    float y = length(center_ - p);

    // Is there an intersection?
    if (y < radius_) {
        float x = sqrt(radius_ * radius_ - (y * y));
        float lambda1 = t - x;
        // float lambda2 = t + x;

        // Which point to use? Using 1.
        vec3 p1 = r0 + rd * lambda1;
        // vec3 p2 = r0 + rd * lambda2;

        // Taken from plane code
        if (lambda1 < 0 || lambda1 + Util::epsilon > maxLambda) {
            return false;
        }

        const vec3 normal_ = p1 - center_;

        const vec3 uvw(0, 0, 0);
        intersection = RayIntersection(ray, shared_from_this(), lambda1, normal_, uvw);
        return true;
    }

    return false;
}

bool Sphere::anyIntersection(const Ray& ray, double maxLambda) const {
    RayIntersection temp;
    return closestIntersection(ray, maxLambda, temp);
}

void Sphere::drawGeometry(std::shared_ptr<BasicMesh> mesh,
                          std::vector<BasicMesh::Vertex>& vertices) const {
    auto indexBuffer = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::None);

    int lat = 8;
    int lon = 10;

    for (int i = 0; i < lat - 1; i++) {
        float theta1 = float(i * M_PI) / (lat - 1);
        float theta2 = float((i + 1) * M_PI) / (lat - 1);

        for (int j = 0; j < lon - 1; j++) {
            float phi1 = float(j * 2 * M_PI) / (lon - 1);
            float phi2 = float((j + 1) * 2 * M_PI) / (lon - 1);

            vec3 v1 = vec3(radius_ * sin(theta1) * cos(phi1), radius_ * sin(theta1) * sin(phi1),
                           radius_ * cos(theta1)) +
                      center_;
            vec3 v2 = vec3(radius_ * sin(theta2) * cos(phi1), radius_ * sin(theta2) * sin(phi1),
                           radius_ * cos(theta2)) +
                      center_;
            vec3 v3 = vec3(radius_ * sin(theta2) * cos(phi2), radius_ * sin(theta2) * sin(phi2),
                           radius_ * cos(theta2)) +
                      center_;

            Util::drawLineSegment(v1, v2, vec4(0.2, 0.2, 0.2, 1), indexBuffer.get(), vertices);
            Util::drawLineSegment(v2, v3, vec4(0.2, 0.2, 0.2, 1), indexBuffer.get(), vertices);
        }
    }
}

}  // namespace inviwo
