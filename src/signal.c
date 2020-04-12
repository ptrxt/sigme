#include "signal.h"
#include "signal_type.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <glib.h>


static SignalTypeDetails* signalTypes[kMaxSigType] = {0};

typedef struct SignalCtrl_t {
    GMutex mutex;
    uint32_t ref_cnt;
} SignalCtrl;

struct Signal_t {
    SignalCtrl ctrl;
    SignalType type;
    void *data;
};

int signal_type_register(SignalTypeDetails* signalTypeDetails) {
    if (signalTypes[signalTypeDetails->type] == 0) {
        signalTypes[signalTypeDetails->type] = signalTypeDetails;
        return 0;
    }

    return 1;
}

Signal* signal_new(SignalType type, void* data) {
    // Check first that the signal type is registered
    if (!signalTypes[type]) {
        return 0;
    }

    Signal *signal = (Signal*)malloc(sizeof(Signal));
    signal->ctrl.ref_cnt = 0;
    g_mutex_init (&signal->ctrl.mutex);
    signal->type = type;
    signal->data = signalTypes[type]->create(data);
    return signal;
}

void signal_lock(Signal* signal) {
    g_mutex_lock(&signal->ctrl.mutex);
}

void signal_unlock(Signal* signal) {
    g_mutex_unlock(&signal->ctrl.mutex);
}

void signal_ref(Signal* signal) {
    signal->ctrl.ref_cnt++;
}

static int unref_and_free(Signal* signal) {
    if (--signal->ctrl.ref_cnt == 0) {
        // No more references - delete signal
        printf("free signal %s\n", signalTypes[signal->type]->name());
        free(signal->data);
        free(signal);
        return 0;
    }
    return 1;
}

void signal_unref(Signal* signal) {
    if (unref_and_free(signal)) {
        // There are references left to signal, unlock it
        signal_unlock(signal);
    }
}

int signal_length(Signal* signal) {
    return signalTypes[signal->type]->length(signal);
}

SignalType signal_type(Signal* signal) {
    return signal->type;
}

void *signal_data(Signal* signal) {
    return signal->data;
}

const char* signal_name(Signal* signal) {
    return signalTypes[signal->type]->name();
}
