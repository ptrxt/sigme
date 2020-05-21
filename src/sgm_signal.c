#include "sgm_signal_priv.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <glib.h>


typedef struct SignalCtrl_t {
    GMutex mutex;
    uint32_t ref_cnt;
} SignalCtrl;

struct Signal_t {
    SignalCtrl ctrl;
    SignalType type;
    void *data;
};

typedef struct {
    SignalType type;
    int (*length)(Signal* signal);
} SignalTypeDetails;

static SignalTypeDetails signalTypes[kMaxSigType] = {0};

int sgm_add_signal(SignalType type, int (*length)(Signal*)) {
    if (signalTypes[type].type != type) {
        signalTypes[type].type = type;
        signalTypes[type].length = length;
        return 0;
    }
    return 1;
}
Signal* sgm_signal_new(SignalType type, void* data) {
    // Check first that the signal type is registered
    if (!signalTypes[type].type == type)
        return 0;

    Signal *signal = (Signal*)malloc(sizeof(Signal));
    signal->ctrl.ref_cnt = 0;
    g_mutex_init (&signal->ctrl.mutex);
    signal->type = type;
    signal->data = data;
    return signal;
}

void sgm_signal_lock(Signal* signal) {
    g_mutex_lock(&signal->ctrl.mutex);
}

void sgm_signal_unlock(Signal* signal) {
    g_mutex_unlock(&signal->ctrl.mutex);
}

void sgm_signal_ref(Signal* signal) {
    signal->ctrl.ref_cnt++;
}

static int unref_and_free(Signal* signal) {
    if (--signal->ctrl.ref_cnt == 0) {
        // No more references - delete signal
        printf("free signal type %d\n", signal->type);
        if (signal->data != NULL) {
            free(signal->data);
        }
        free(signal);
        return 0;
    }
    return 1;
}

void sgm_signal_unref(Signal* signal) {
    if (unref_and_free(signal)) {
        // There are references left to signal, unlock it
        sgm_signal_unlock(signal);
    }
}

int sgm_signal_length(Signal* signal) {
    return signalTypes[signal->type].length(signal);
}

SignalType sgm_signal_type(Signal* signal) {
    return signal->type;
}

void *sgm_signal_data(Signal* signal) {
    return signal->data;
}

uint32_t sgm_signal_refcount(Signal* signal) {
    return signal->ctrl.ref_cnt;
}
