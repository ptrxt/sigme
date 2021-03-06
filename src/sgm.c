#include "sgm.h"
#include "sgm_signal.h"
#include "sgm_signal_priv.h"
#include "signal_definitions.h"
#include <glib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

#define MAX_N_SOURCES   10
#define MAX_N_RECEIVERS 10
#define SIGNAL_MANAGER_NOT_INITIALISED  0
#define SIGNAL_MANAGER_INITIALISED      1

typedef struct {
    SignalSource *sources[MAX_N_SOURCES];
    SignalReceiver* sigTypeToReceiver[kMaxSigType][MAX_N_RECEIVERS];
    uint8_t init;
} SignalManager;

static SignalManager manager = {.init = SIGNAL_MANAGER_NOT_INITIALISED};

static void init() {
    if (manager.init == SIGNAL_MANAGER_INITIALISED) return;

    for (int i = 0; i < MAX_N_SOURCES; i++) {
        manager.sources[i] = 0;
    }

    for (int i = 0; i < kMaxSigType; i++) {
        for (int j = 0; j < MAX_N_RECEIVERS; j++) {
            manager.sigTypeToReceiver[i][j] = 0;
        }
    }

    manager.init = SIGNAL_MANAGER_INITIALISED;
}

static void send_to_receivers(Signal* signal) {
    SignalReceiver **receiver = &manager.sigTypeToReceiver[sgm_signal_type(signal)][0];
    for (int j = 0; j < MAX_N_RECEIVERS && *receiver != 0; j++, receiver++) {
        printf("signal manager:\t\tforward to receiver\n");

        ReceiveReturn result = (*receiver)->receive(signal, (*receiver)->context);
        if (result == PROCESSING_PENDING) {
            sgm_signal_ref(signal);
        }
    }
}

static void pollSources(void) {
    for (int i = 0; i<MAX_N_SOURCES && manager.sources[i] != 0; i++) {
        SignalType type;
        SignalPayload data = NULL;
        manager.sources[i]->poll(&type, &data);
        if (data != NULL) {
            Signal *signal = sgm_signal_new(type, data);

            time_t t;
            time(&t);
            printf("---------------------------\n%ssignal manager:\t\treceive signal type %d ..\n",ctime(&t), sgm_signal_type(signal));
            sgm_signal_lock(signal);
            sgm_signal_ref(signal);
            send_to_receivers(signal);
            sgm_signal_unref(signal);
        }
    }
}

void sgm_add_source(SignalSource *source) {
    if (manager.init == SIGNAL_MANAGER_NOT_INITIALISED) {
        init();
    }
    for (int i=0; i<MAX_N_SOURCES; i++) {
        if (manager.sources[i] == 0) {
            manager.sources[i] = source;
            break;
        }
    }
}

void sgm_add_receiver(SignalReceiver *receiver) {
    if (manager.init == SIGNAL_MANAGER_NOT_INITIALISED) {
        init();
    }
    SignalType requested[kMaxSigType];
    unsigned n_signals = 0;

    if (!receiver->get_signal_types) {
        printf("Error: receiver has not set get_signal_types pointer!\n");
        exit(1);
    }

    receiver->get_signal_types(requested, kMaxSigType, &n_signals);
    for (int type = 0; type < n_signals; type++) {
        for (int j = 0; j<MAX_N_RECEIVERS; j++) {
            if (manager.sigTypeToReceiver[type][j] == 0) {
                manager.sigTypeToReceiver[type][j] = receiver;
                printf("registered as receiver of signal type: %d\n", type);
                break;
            }
        }
    }
}

void sgm_run(void) {
    if (manager.init == SIGNAL_MANAGER_NOT_INITIALISED) {
        init();
    }
    pollSources();
}
