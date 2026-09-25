void modulate_bpsk(const int* bits, double* symbols, int length);
void demodulate_bpsk(const double* symbols, int* bits_out, int length);
void modulate_qpsk(const int* bits, double* symbols, int length);
void demodulate_qpsk(const double* symbols, int* bits_out, int length);
void modulate_16QAM(const int* bits, double* symbols, int length);
void demodulate_16QAM(const double* symbols, int* bits_out, int length);
