#include "sgm.h"
#include "sgm_signal.h"
#include "app_signals.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

//static SignalTypeDetails humidSignal;


//static void* createHumidSignal(void* data) {
//    uint32_t* d = (uint32_t*)malloc(sizeof(uint32_t));
//    *d = *(uint32_t*)data;
//    return d;
//}

static int humidSignalLength(Signal* signal) {
    return sizeof(HumiditySignal);
}

static void logError(SignalType type) {
    printf("Error: Signal type %d already registered\n", type);
}

void install_signals(void) {
   // humidSignal.type = kHumiditySignal;
    //humidSignal.create = createHumidSignal;
   // humidSignal.length = humidSignalLength;
    //humidSignal.name = humidName;
    if (1 == sgm_add_signal(kHumiditySignal, humidSignalLength)) {
        logError(kHumiditySignal);
    }
}
