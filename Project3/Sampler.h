#include <iostream>
#include <stdexcept>
#include <random>
using namespace std;

#ifndef SAMPLER_H
#define SAMPLER_H

class Sampler {
	public:
		Sampler(uint32_t n);
		~Sampler();
		uint32_t getSample();

	private:
		uint32_t
			*elements,
			n;
		random_device
			*rd;
		mt19937
			*mt;
};

#endif
