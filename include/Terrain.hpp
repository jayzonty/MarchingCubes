#pragma once

#include "FastNoiseLite/FastNoiseLite.h"

#include <glm/glm.hpp>

struct Terrain
{
    FastNoiseLite noise[9];
    int seed[9] = { -9999, 100, 2500, -2300, 500, 12345, -12345, 10, 9999 };
    float amplitudes[9] = { 2.0f, 1.0f, 5.0f, 3.0f, 0.5f, 3.0f, 8.0f, 0.75f, 1.0f };
    float frequencyFactors[9] = { 1.0f, 0.2f, 0.5f, 3.25f, 2.0f, 0.2f, 0.25f, 0.1f, 3.0f };

    Terrain()
    {
        for (int i = 0; i < 9; ++i)
        {
            noise[i].SetSeed(seed[i]);
            noise[i].SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
        }
    }

    float DensityFunction(float x, float y, float z)
    {
        float density = glm::clamp(-y, 0.0f, 1.0f) * 5.0f;
        for (int i = 0; i < 9; ++i)
        {
            density += noise[i].GetNoise(x * frequencyFactors[i], y * frequencyFactors[i], z * frequencyFactors[i]) * amplitudes[i];
        }
        return density;
    }
};
