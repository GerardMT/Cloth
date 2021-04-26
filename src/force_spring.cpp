#include "force_spring.h"

ForceSpring::ForceSpring(Particle &p1, Particle &p2, float k_e, float k_d, float length_coef)
{
    p1_ = &p1;
    p2_ = &p2;

    k_e_ = k_e;
    k_d_ = k_d;

    length_ = glm::length(p2.pos_ - p1.pos_) * length_coef;
}

void ForceSpring::apply() const
{
    glm::vec3 dir = glm::normalize(p2_->pos_ - p1_->pos_);

    glm::vec3 f_p1 = (k_e_ * (glm::length(p2_->pos_ - p1_->pos_) - length_) + k_d_ * glm::dot(p2_->vel_ - p1_->vel_, dir)) * dir;
    p1_->force_ += f_p1;
    p2_->force_ += -f_p1;
}
