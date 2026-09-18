void modulate_bpsk(int* signal, int length){
    for(int i = 0; i < length; i++){
        if(signal[i] == 0){
            signal[i] += 1;
        }
        else{
            signal[i] -= 1;
        }

    }
}

void demodulate_bpsk(int* signal, int length){
    for(int i = 0; i < length; i++){
        if(signal[i] > 0){
            signal[i] = 0;
        }
        else{
            signal[i]  = 1;
        }
    }
}
