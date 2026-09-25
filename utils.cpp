#include "utils.hpp"
#include <cmath>
#include <random>
#include <limits>

void add_noise(const double* symbols, double* noisy_symbols, int length, double snr_db){
    // Average signal power (assumes real-valued symbols, e.g. BPSK's +-1).
    double signal_power = 0.0;
    for(int i = 0; i < length; i++){
        signal_power += symbols[i] * symbols[i];
    }
    signal_power /= length;

    // Convert the desired SNR (dB) into a noise power/std-dev to sample from.
    double snr_linear = std::pow(10.0, snr_db / 10.0);
    double noise_power = signal_power / snr_linear;
    double noise_stddev = std::sqrt(noise_power);

    static std::mt19937 rng(std::random_device{}());
    std::normal_distribution<double> gaussian(0.0, noise_stddev);

    for(int i = 0; i < length; i++){
        noisy_symbols[i] = symbols[i] + gaussian(rng);
    }
}

double calculate_snr(const double* symbols, const double* noisy_symbols, int length){
    double signal_power = 0.0;
    double noise_power = 0.0;
    for(int i = 0; i < length; i++){
        signal_power += symbols[i] * symbols[i];
        double noise = noisy_symbols[i] - symbols[i];
        noise_power += noise * noise;
    }
    signal_power /= length;
    noise_power /= length;

    if(noise_power == 0.0){
        return std::numeric_limits<double>::infinity();
    }
    return 10.0 * std::log10(signal_power / noise_power);
}

double calculate_ber(const int* original_bits, const int* recovered_bits, int length){
    int errors = 0;
    for(int i = 0; i < length; i++){
        if(original_bits[i] != recovered_bits[i]){
            errors++;
        }
    }
    return static_cast<double>(errors) / length;
}
