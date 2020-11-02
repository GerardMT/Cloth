#include "collider_polygon.h"

ColliderPolygon::ColliderPolygon(const vector<glm::vec3> &points, float bouncing, float friction)
{
    points_ = points;

    bouncing_ = bouncing;
    friction_ = friction;

    glm::vec3 n = glm::normalize(glm::cross(points[1] - points[0], points[2] - points[0]));
    collider_plane_ = ColliderPlane(n, points[0], bouncing, friction);
}

unsigned char vec3MaxDim(glm::vec3 v)
{
    if (v[0] >= v[1] && v[0] >= v[2]) {
        return 0;
    } else if (v[1] >= v[0] && v[1] >= v[2]) {
        return 1;
    } else {
        return 2;
    }
}

bool ColliderPolygon::collide(Particle &p)
{
    glm::vec3 l_v = p.pos_ - p.pos_pre_;

    if (glm::dot(l_v, collider_plane_.n_) == 0) { // Line and plane are parallel
        return false;
    }

    float l_d = glm::dot(points_[0] - p.pos_pre_, collider_plane_.n_) / glm::dot(l_v, collider_plane_.n_);

    glm::vec3 inter = p.pos_pre_ + l_v * l_d;

    unsigned char dim = vec3MaxDim(inter);
    unsigned int n_inter= 0;

    glm::vec3 p_1_proj = points_[0];
    p_1_proj[dim] = 0;

    for (unsigned int i = 0; i < points_.size() - 1; ++i) {
        glm::vec3 p_2_proj = points_[i + 1];
        p_2_proj[dim] = 0;

        float div = (p_2_proj[dim] - p_1_proj[dim]);
        if (div == 0.0f) { // Collinear
            return false;
        }

        float a = (inter[dim] - points_[i][dim]) / div;

        if (a >= 0.0f || a < 1.0f) {
            n_inter++;
        }

        p_1_proj = p_2_proj;
    }

    return n_inter % 2 == 1;
}

void ColliderPolygon::correct(float dt, Particle &p)
{
    collider_plane_.correct(dt, p);
}

void ColliderPolygon::transform(glm::mat4 m)
{
    // TODO
}
