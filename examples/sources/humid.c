#include "humid.h"
#include "signal_source.h"
#include "signal.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

static SignalSource humidSource;

static Signal* fetchHumidity() {
    uint32_t data = 85;
    printf("new signal humidity\n");
    return signal_new(kHumiditySignal, &data);
}

SignalSource *humid_init(void) {
    humidSource.poll = fetchHumidity;
    return &humidSource;
}
