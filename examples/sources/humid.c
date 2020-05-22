#include "humid.h"
#include "sgm_source.h"
#include "sgm_signal.h"
#include "app_signals.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

static SignalSource humidSource;

//Fake humidity sensor
static uint32_t humidSensorRead(void) {
    return 85;
}

static uint32_t humidSensorId(void) {
    return 0;
}

//static Signal* fetchHumidity() {
//    HumiditySignal* h = (HumiditySignal*)malloc(sizeof(HumiditySignal));
//    h->humidity = humidSensorRead();
//    h->id = humidSensorId();
//    printf("new signal humidity\n");
//    return sgm_signal_new(kHumiditySignal, h);
//}

static void fetchHumidity(SignalType *type, void** data) {
    HumiditySignal* h = (HumiditySignal*)malloc(sizeof(HumiditySignal));
    h->humidity = humidSensorRead();
    h->id = humidSensorId();
    printf("new signal humidity\n");
    *type = kHumiditySignal;
    *data = (void*)h;
}

SignalSource *humid_init(void) {
    humidSource.poll = fetchHumidity;
    return &humidSource;
}



