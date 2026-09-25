// bits and symbols are separate buffers so the original bit sequence
// survives modulation/noise/demodulation for later BER comparison.
// symbols are double so noise (in utils.cpp) can add continuous-valued
// Gaussian samples without truncation.
// TODO: implement 16QAM modulation (4 bits/symbol -> 16-point constellation)
void modulate_16QAM(const int* bits, double* symbols, int length){
}

// TODO: implement 16QAM demodulation
void demodulate_16QAM(const double* symbols, int* bits_out, int length){
}
