#include "modulation.hpp"
#include <cmath>

// bits and symbols are separate buffers so the original bit sequence
// survives modulation/noise/demodulation for later BER comparison.
// symbols are double so noise (in utils.cpp) can add continuous-valued
// Gaussian samples without truncation.
//
// Symbols are stored interleaved: symbols[2k] = I, symbols[2k+1] = Q.
// length is the number of bits (must be even); the symbol buffer holds
// the same number of doubles (length / 2 complex symbols).
//
// Gray mapping, unit symbol energy: bit 2k drives I, bit 2k+1 drives Q,
// 0 -> +1/sqrt(2), 1 -> -1/sqrt(2).
static const double QPSK_AMPLITUDE = 1.0 / std::sqrt(2.0);

void modulate_qpsk(const int* bits, double* symbols, int length){
    for(int i = 0; i < length; i++){
        if(bits[i] == 0){
            symbols[i] = QPSK_AMPLITUDE;
        }
        else{
            symbols[i] = -QPSK_AMPLITUDE;
        }
    }
}

// Each axis is decided independently (Gray mapping), so a sign test on
// I and on Q recovers the two bits of each symbol.
void demodulate_qpsk(const double* symbols, int* bits_out, int length){
    for(int i = 0; i < length; i++){
        if(symbols[i] > 0.0){
            bits_out[i] = 0;
        }
        else{
            bits_out[i] = 1;
        }
    }
}
