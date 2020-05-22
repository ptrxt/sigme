#include "stdout_broadcaster.h"
#include "sgm_signal.h"
#include <stdio.h>
#include <stdint.h>
#include <glib.h>

// The stdout broadcaster is a signal receiver.
typedef struct {
    SignalReceiver receiver;    // The interface through which the signal manager can push signals
    GAsyncQueue *queue;
} StdoutBroadcaster;

static StdoutBroadcaster receiver;

static ReceiveReturn pushSignal(Signal *signal) {
    g_async_queue_push(receiver.queue, signal);
    return PROCESSING_PENDING;
}

static void get_signals(SignalType signals[], unsigned max, unsigned *len) {
    signals[0] = kTempSignal;
    signals[1] = kHumiditySignal;
    *len = 2;
}

static const char* get_name(void) {
    return "stdout broadcaster";
}

static gpointer printThread(gpointer data) {
    StdoutBroadcaster *receiver = data;

    while(1) {
        Signal* signal = (Signal*)g_async_queue_pop(receiver->queue);
        sgm_signal_process(signal);
        switch (sgm_signal_type(signal)) {
            case kTempSignal:
                printf("stdout_broadcaster:\treceived %s\n", (char*)sgm_signal_data(signal));
                break;

            case kHumiditySignal:
                printf("stdout_broadcaster:\treceived %d\n", *(uint32_t*)sgm_signal_data(signal));
                break;

            default:
                printf("stdout_broadcaster:\treceived unknown signal\n");
                break;
        }

        sgm_signal_complete(signal);
    }

    return 0;
}

SignalReceiver* stdout_broadcaster_init(GThread **thread) {
    receiver.receiver.receive = pushSignal;
    receiver.receiver.get_signal_types = get_signals;
    receiver.receiver.name = get_name;
    receiver.queue = g_async_queue_new();
    GThread* t = g_thread_new("printThread", printThread, &receiver);
    *thread = t;
    return &receiver.receiver;
}


