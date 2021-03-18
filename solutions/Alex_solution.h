#pragma once

#include <stdexcept>
#include "fake_crypto.h"


std::tuple<vector_t, vector_t, vector_t> FakeSign(const std::string& msg, RWEPublicKey pubkey) {
    vector_t c = {};
    vector_t z1 = {};
    vector_t z2 = {};
    vector_t y1 = {};
    vector_t y2 = {};
    vector_t w  = {};

		vector_t akey = pubkey.getAVector();
		vector_t skey;
		int m = pubkey.getModule();
		//setting the whole skey with 1
		for (int i = 0; i < 128; i++)
		{
			skey[i] = 1;
		}
    vector_t asekey = pubkey.getASEVector();
		//computing a proper ekey considering that skey = 1 
		vector_t ekey = (asekey - akey) % m;
	 	auto msg_hash = ComputeSimpleHash(msg);

		std::random_device rd;
   	std::mt19937 mt(rd());
   	std::uniform_int_distribution<int> uniform_distribution;

		//randomizing the y1 and y2
		uniform_distribution = std::uniform_int_distribution<int>(-(m / 2) + 1, m / 2);
    auto generator = [&uniform_distribution, &mt]() -> int {
        return uniform_distribution(mt);
    };
    std::generate(y1.begin(), y1.end(), generator);
    std::generate(y2.begin(), y2.end(), generator);
 
    // Compute w
    w = (akey * y1 + y2) % m;
    // Compute c
    c = (w + msg_hash) % m;
		//Compute z1 z2
		z1 = (skey * c + y1) % m;
		z2 = (ekey * c + y2) % m;

    return std::make_tuple(c, z1, z2);
}
