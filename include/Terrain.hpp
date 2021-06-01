#pragma once

#include "FastNoiseLite/FastNoiseLite.h"

struct Terrain
{
    FastNoiseLite noise;
    int seed;

    void SetSeed(int seed)
    {
        noise.SetSeed(seed);
    }

    float DensityFunction(float x, float y, float z)
    {
        /*float sample1 = noise.GetNoise(x * 2.0f, z * 2.0f);
        float sample2 = noise.GetNoise(x * 5.0f, y * 0.5f) * 0.5f;
        return -y + (sample1 + sample2);*/
        float density = noise.GetNoise(x * 2.0f, y * 2.0f, z * 2.0f);
        return density;
    }
};
