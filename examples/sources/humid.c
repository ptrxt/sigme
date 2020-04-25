#include "humid.h"
#include "signal_source.h"
#include "signal.h"
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

static Signal* fetchHumidity() {
    HumiditySignal* h = (HumiditySignal*)malloc(sizeof(HumiditySignal));
    h->humidity = humidSensorRead();
    h->id = humidSensorId();
    printf("new signal humidity\n");
    return signal_new(kHumiditySignal, h);
}

SignalSource *humid_init(void) {
    humidSource.poll = fetchHumidity;
    return &humidSource;
}



