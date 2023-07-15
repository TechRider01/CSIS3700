#include "Sampler.h" 
#include <random>
#include <iostream>
using namespace std;   

Sampler::Sampler(uint32_t n) {
	elements = new uint32_t[n];
	for(int i = 0; i < n; i++) {
		elements[i] = i;
	}
	this->n = n;
	rd = new random_device;
	mt = new mt19937((*rd)());
}

Sampler::~Sampler() {
	delete[] elements;
	delete mt;
	delete rd;
}

uint32_t Sampler::getSample() {
	uniform_int_distribution<>
		dis(0, n-1);
	uint32_t
		r,
		e;
	if(n == 0) {
		throw underflow_error("Out of bounds sampler is empty");
	}
	
	r = dis(*mt);
	e = elements[r];
	n--;
	elements[r] = elements[n];
	
	return e;
}	

    
