#pragma once

#include <ctime>
#include <random>

template <typename Engine = std::mt19937>
class Random {
    template <typename T>
    using UniformDistr    = std::uniform_real_distribution<T>;
    using UniformIntDistr = std::uniform_int_distribution<int>;

public:
    Random(int seed = std::time(nullptr)) : m_prng(seed) {}
    int getIntInRange(int low, int high) { return getNumberInRange<UniformIntDistr>(low, high); }
    float getFloatInRange(float low, float high) { return getNumberInRange<UniformDistr<float>>(low, high); }

    template <typename T>
    T getNumberInRange(T low, T high) { return getNumberInRange<UniformDistr<T>>(low, high); }

    template <typename Dist, typename T>
    T getNumberInRange(T low, T high) {
        Dist dist(low, high);
        return dist(m_prng);
    }

private:
    Engine m_prng;
};