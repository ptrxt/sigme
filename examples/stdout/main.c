/*
 ============================================================================
 Name        : main.c
 Description : Example application with one signal source and one signal receiver.

 Signal source:
 humid.c: Fakes humidity sensor readings. Produces signals of type kHumiditySignal which is
          a constant uint32_t value.

 Receiver:
 stdout_broadcaster.c: Consumes signals of type kHumiditySignal and prints them to stdout.
 The broadcaster uses a worker thread to pop signals from an GAsyncQueue, pushed by the
 signal manager running in the main thread.


 ============================================================================
 */

#include "sgm_signal.h"
#include "sgm.h"
#include "humid.h"
#include "stdout_broadcaster.h"
#include <glib.h>
#include <stdio.h>

#define G_SOURCE_FUNC(f) ((GSourceFunc) (void (*)(void)) (f))


static gboolean timer_callback(void* data) {
    sgm_run();
    return G_SOURCE_CONTINUE;
}

static void setup_timer(GMainContext *context) {
    GSource *timer = g_timeout_source_new(5000);
    g_source_set_callback(timer, G_SOURCE_FUNC(timer_callback), NULL, NULL);
    g_source_attach(timer, context);
}

int main(void) {
    GMainLoop *loop = g_main_loop_new (NULL, FALSE);
    GMainContext *context = g_main_loop_get_context(loop);
    GThread* t;

    // Create timeout for executing signal manager periodically
    setup_timer(context);

    sgm_add_source(humid_init());
    sgm_add_receiver(stdout_broadcaster_init(&t));

    g_main_loop_run (loop);
    g_thread_join(t);
}
