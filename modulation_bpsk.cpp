// bits and symbols are separate buffers so the original bit sequence
// survives modulation/noise/demodulation for later BER comparison.
// symbols are double so noise (in utils.cpp) can add continuous-valued
// Gaussian samples without truncation.
void modulate_bpsk(const int* bits, double* symbols, int length){
    for(int i = 0; i < length; i++){
        if(bits[i] == 0){
            symbols[i] = 1.0;
        }
        else{
            symbols[i] = -1.0;
        }
    }
}

void demodulate_bpsk(const double* symbols, int* bits_out, int length){
    for(int i = 0; i < length; i++){
        if(symbols[i] > 0.0){
            bits_out[i] = 0;
        }
        else{
            bits_out[i] = 1;
        }
    }
}
