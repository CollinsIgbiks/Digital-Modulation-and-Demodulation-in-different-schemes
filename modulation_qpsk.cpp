// bits and symbols are separate buffers so the original bit sequence
// survives modulation/noise/demodulation for later BER comparison.
// symbols are double so noise (in utils.cpp) can add continuous-valued
// Gaussian samples without truncation.
// TODO: implement QPSK modulation (2 bits/symbol -> I/Q pair)
void modulate_qpsk(const int* bits, double* symbols, int length){
}

// TODO: implement QPSK demodulation
void demodulate_qpsk(const double* symbols, int* bits_out, int length){
}
