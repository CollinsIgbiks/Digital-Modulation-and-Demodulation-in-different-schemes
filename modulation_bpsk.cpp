//change data types
int modulate_bpsk(int* signal){
    for(int i = 0; i <= sizeof(signal) - 1; i++){
        if(signal[i] == 0){
            signal[i] += 1;
        }
        else{
            signal[i] -= 1;
        }

    }
    int result;
    return result;
}

int demodulate_bpsk(int* signal){
    for(int i = 0; i <= sizeof(signal) - 1; i++){
        if(signal[i] > 0){
            signal[i] = 0;
        }
        else{
            signal[i]  = 1;
        }
    }
}