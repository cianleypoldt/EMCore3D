#pragma once
#include "common/constants.h"

class ParticleGroup
{
    ParticleGroup(int particle_count);
    ~ParticleGroup();

    void allocate();

    const int particle_count;
  private:
    real * x_pos;
    real * y_pos;
    real * z_pos;
};
