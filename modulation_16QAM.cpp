#include "modulation.hpp"
#include <cmath>

// bits and symbols are separate buffers so the original bit sequence
// survives modulation/noise/demodulation for later BER comparison.
// symbols are double so noise (in utils.cpp) can add continuous-valued
// Gaussian samples without truncation.
//
// Symbols are stored interleaved: symbols[2k] = I, symbols[2k+1] = Q.
// Each double carries 2 bits, so length (number of bits) must be a
// multiple of 4 and the symbol buffer holds length / 2 doubles.
//
// Each axis is Gray-coded 4-PAM: 00 -> -3, 01 -> -1, 11 -> +1, 10 -> +3,
// scaled by 1/sqrt(10) for unit average symbol energy.
static const double QAM16_SCALE = 1.0 / std::sqrt(10.0);

void modulate_16QAM(const int* bits, double* symbols, int length){
    for(int i = 0; i < length / 2; i++){
        int b0 = bits[2 * i];
        int b1 = bits[2 * i + 1];
        double level;
        if(b0 == 0){
            level = (b1 == 0) ? -3.0 : -1.0;
        }
        else{
            level = (b1 == 0) ? 3.0 : 1.0;
        }
        symbols[i] = level * QAM16_SCALE;
    }
}

// Decision thresholds per axis are 0 and +-2 (before scaling): the sign
// gives the first bit, and whether the sample is inside +-2 gives the second.
void demodulate_16QAM(const double* symbols, int* bits_out, int length){
    for(int i = 0; i < length / 2; i++){
        double y = symbols[i] / QAM16_SCALE;
        bits_out[2 * i] = (y > 0.0) ? 1 : 0;
        bits_out[2 * i + 1] = (std::fabs(y) < 2.0) ? 1 : 0;
    }
}
