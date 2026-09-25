// Noise addition, SNR, and BER calculations shared across modulation schemes.

// Adds AWGN (additive white Gaussian noise) to a modulated symbol stream.
// symbols: clean modulated symbols (input)
// noisy_symbols: output buffer, same length, receives symbols + noise
// snr_db: desired signal-to-noise ratio in decibels
void add_noise(const double* symbols, double* noisy_symbols, int length, double snr_db);

// Measures the achieved SNR (in decibels) between a clean and a noisy
// symbol stream of the same length.
double calculate_snr(const double* symbols, const double* noisy_symbols, int length);

// Computes the bit error rate: the fraction of mismatched bits between
// the originally transmitted bits and the recovered (demodulated) bits.
double calculate_ber(const int* original_bits, const int* recovered_bits, int length);
