/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 24, 2017 - 17:17:44
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/phongmaterial.h>
#include <labraytracer/util.h>

namespace inviwo {

PhongMaterial::PhongMaterial(const vec3& color, const double reflectance, const double shininess,
    const vec3& ambientMaterialColor, const vec3& diffuseMaterialColor, const vec3& specularMatierialColor)
    : Material(color, reflectance) {

    constexpr double LightIntensity = 100.0;

    shininess_ = shininess;
    ambientMaterialColor_   = Util::scalarMult(LightIntensity, ambientMaterialColor);
    diffuseMaterialColor_   = Util::scalarMult(LightIntensity, diffuseMaterialColor);
    specularMatierialColor_ = Util::scalarMult(LightIntensity, specularMatierialColor);
}

vec4 PhongMaterial::shade(const RayIntersection& intersection, const Light& light) const {
    // get normal and light direction
    vec3 N = intersection.getNormal();
    vec3 L = Util::normalize(light.getPosition() - intersection.getPosition());

    double cosNL = std::max(double(dot(N, L)), double(0));

    // Programming Task 2: Extend this method.
    // This method currently implements a Lambert's material with ideal
    // diffuse reflection.
    // Your task is to implement a Phong shading model.
    //
    // Hints:
    //
    // 1. This function should return the sum of diffuse and specular parts (no ambient part)
    // 2. The used light color for each type (diffuse/specular) from the light source
    //    is the light color divided by the quadratic distance of the light source from
    //    the point of intersection. (quadratic falloff)
    // 3. The view vector V is the direction of the ray which intersects the object.
    // 4. The rest of the terms as per the slides are as follows
    // 5. You have to take into account shininess_ (p), material colors, light colors
    //    light, view, reflection and normal vector.


	//distance 
	vec3 distanceVector = light.getPosition() - intersection.getPosition();
    double distance = length(distanceVector);
    double distanceSquared = pow(distance, 2);

    // Specular (glossy) part.
    vec3 N_normalized = Util::normalize(N);
    vec3 N2 = Util::scalarMult(2, N_normalized);

    double tmp1 = std::max(double(dot(N_normalized, L)), double(0.0));
    vec3 R0 = Util::scalarMult(tmp1, N2);

    vec3 R = -(R0 - L);
    vec3 R_normalized = Util::normalize(R);

    vec3 V = intersection.getRay().getDirection();
    vec3 V_normalized = Util::normalize(V);

    double tmp2 = std::max(double(dot(R_normalized, V_normalized)), double(0.0));
    vec3 c_specular = vec3(
      specularMatierialColor_[0] * light.getSpecularColor()[0] * pow(tmp2, shininess_) / distanceSquared,
      specularMatierialColor_[1] * light.getSpecularColor()[1] * pow(tmp2, shininess_) / distanceSquared,
      specularMatierialColor_[2] * light.getSpecularColor()[2] * pow(tmp2, shininess_) / distanceSquared
    );

    // Diffuse part.

    vec3 c_diffuse = vec3(
      (diffuseMaterialColor_[0] * light.getDiffuseColor()[0] * cosNL) / distanceSquared,
      (diffuseMaterialColor_[1] * light.getDiffuseColor()[1] * cosNL) / distanceSquared,
      (diffuseMaterialColor_[2] * light.getDiffuseColor()[2] * cosNL) / distanceSquared
    );

    // Sum up the two parts and return.
    vec3 c_sum = c_specular + c_diffuse;
    // vec3 c_sum = c_specular + Util::scalarMult(cosNL, this->color());
    return vec4(c_sum, 1.0);
    // return vec4(Util::scalarMult(cosNL, this->color()), 1.0);
}

} // namespace
