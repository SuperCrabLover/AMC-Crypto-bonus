#pragma once

#include <stdexcept>
#include "fake_crypto.h"

std::tuple<vector_t, vector_t, vector_t> FakeSign(const std::string &message, RWEPublicKey public_key) {
    auto a_vector = public_key.getAVector();
    auto ase_vector = public_key.getASEVector();
    auto module = public_key.getModule();
    
    vector_t c = {};
    vector_t z1 = {};
    vector_t z2 = {};
    vector_t y1 = {};
    vector_t y2 = {};
    vector_t w = {};
    vector_t s_vector = {};
    vector_t e_vector = {};
    
    auto message_hash = ComputeSimpleHash(message);
    
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> uniform_distribution;

    // Uniform distribution without zeros
    uniform_distribution = std::uniform_int_distribution<int>(-(module / 2) + 1, module / 2);
    auto generator = [&uniform_distribution, &mt]() -> int {
        return uniform_distribution(mt);
    };
    std::generate(y1.begin(), y1.end(), generator);
    std::generate(y2.begin(), y2.end(), generator);

    // Compute w
    w = (a_vector * y1 + y2) % module;

    // Compute c
    c = (w + message_hash) % module;
    
    uniform_distribution = std::uniform_int_distribution<int>(1, module);
    std::generate(s_vector.begin(), s_vector.end(), generator);
    
    e_vector = (ase_vector - a_vector * s_vector) % module;
    
    z1 = (s_vector * c + y1) % module;
    z2 = (e_vector * c + y2) % module;

    return std::make_tuple(c, z1, z2);
}
