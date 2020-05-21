#include "sgm_signal.h"
#include "sgm_signal_priv.h"
#include "sgm_type.h"
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

int sgm_signal_type_register(SignalTypeDetails* signalTypeDetails) {
    if (signalTypes[signalTypeDetails->type] == 0) {
        signalTypes[signalTypeDetails->type] = signalTypeDetails;
        return 0;
    }

    return 1;
}

Signal* sgm_signal_new(SignalType type, void* data) {
    // Check first that the signal type is registered
    if (!signalTypes[type]) {
        return 0;
    }

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
        printf("free signal %s\n", signalTypes[signal->type]->name());
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
    return signalTypes[signal->type]->length(signal);
}

SignalType sgm_signal_type(Signal* signal) {
    return signal->type;
}

void *sgm_signal_data(Signal* signal) {
    return signal->data;
}

const char* sgm_signal_name(Signal* signal) {
    return signalTypes[signal->type]->name();
}

uint32_t sgm_signal_refcount(Signal* signal) {
    return signal->ctrl.ref_cnt;
}
