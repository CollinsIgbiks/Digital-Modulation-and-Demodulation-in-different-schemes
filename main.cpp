#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "modulation.hpp"
#include "utils.hpp"

using namespace std;

typedef void (*ModulateFn)(const int*, double*, int);
typedef void (*DemodulateFn)(const double*, int*, int);
typedef double (*TheoryFn)(double);

double q_function(double x){
    return 0.5 * erfc(x / sqrt(2.0));
}

// The SNR used throughout is the one add_noise() sets: average power per
// stored double / per-dimension noise variance. With unit symbol energy
// this equals Es/N0 for QPSK and 16-QAM, and 2*Eb/N0 for BPSK.

// BPSK and QPSK: BER = Q(sqrt(SNR)).
double theoretical_ber_bpsk_qpsk(double snr_db){
    double snr_linear = pow(10.0, snr_db / 10.0);
    return q_function(sqrt(snr_linear));
}

// Gray-coded 16-QAM, exact per-axis 4-PAM result:
// BER = (3Q(a) + 2Q(3a) - Q(5a)) / 4, with a = sqrt(SNR / 5).
double theoretical_ber_16qam(double snr_db){
    double snr_linear = pow(10.0, snr_db / 10.0);
    double a = sqrt(snr_linear / 5.0);
    return (3.0 * q_function(a) + 2.0 * q_function(3.0 * a) - q_function(5.0 * a)) / 4.0;
}

// Sweeps SNR for one scheme, printing a table and writing ber_<name>.csv.
// bits_per_double: how many bits each stored double carries (1 for BPSK
// and QPSK, 2 for 16-QAM), which sets the symbol buffer length.
void run_ber_sweep(const string& name, ModulateFn modulate, DemodulateFn demodulate,
                   TheoryFn theory_fn, int bits_per_double, const vector<int>& bits,
                   double snr_min_db, double snr_max_db, double snr_step_db){
    int num_bits = static_cast<int>(bits.size());
    int num_doubles = num_bits / bits_per_double;
    vector<double> symbols(num_doubles);
    vector<double> noisy_symbols(num_doubles);
    vector<int> recovered_bits(num_bits);

    modulate(bits.data(), symbols.data(), num_bits);

    string csv_name = "ber_" + name + ".csv";
    ofstream csv(csv_name);
    csv << "snr_db,measured_snr_db,ber,theoretical_ber\n";

    cout << "\n" << name << " BER vs SNR (" << num_bits << " bits per point)\n";
    cout << setw(8) << "SNR(dB)" << setw(14) << "measured(dB)"
         << setw(14) << "BER" << setw(14) << "theory" << "\n";

    for(double snr_db = snr_min_db; snr_db <= snr_max_db + 1e-9; snr_db += snr_step_db){
        add_noise(symbols.data(), noisy_symbols.data(), num_doubles, snr_db);
        demodulate(noisy_symbols.data(), recovered_bits.data(), num_bits);

        double measured_snr = calculate_snr(symbols.data(), noisy_symbols.data(), num_doubles);
        double ber = calculate_ber(bits.data(), recovered_bits.data(), num_bits);
        double theory = theory_fn(snr_db);

        cout << fixed << setprecision(2) << setw(8) << snr_db
             << setw(14) << measured_snr
             << scientific << setprecision(3) << setw(14) << ber
             << setw(14) << theory << "\n";
        csv << snr_db << "," << measured_snr << "," << ber << "," << theory << "\n";
    }

    cout << "Results written to " << csv_name << "\n";
}

int main(){
    const int num_bits = 1000000;  // multiple of 4, so it divides into 16-QAM symbols
    const double snr_step_db = 1.0;

    // Random source bits, shared by every scheme and SNR point.
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> bit_dist(0, 1);
    vector<int> bits(num_bits);
    for(int i = 0; i < num_bits; i++){
        bits[i] = bit_dist(rng);
    }

    run_ber_sweep("bpsk", modulate_bpsk, demodulate_bpsk, theoretical_ber_bpsk_qpsk,
                  1, bits, 0.0, 12.0, snr_step_db);
    run_ber_sweep("qpsk", modulate_qpsk, demodulate_qpsk, theoretical_ber_bpsk_qpsk,
                  1, bits, 0.0, 12.0, snr_step_db);
    // 16-QAM needs about 6.5 dB more SNR for the same BER, so sweep further.
    run_ber_sweep("16qam", modulate_16QAM, demodulate_16QAM, theoretical_ber_16qam,
                  2, bits, 0.0, 18.0, snr_step_db);
    return 0;
}
