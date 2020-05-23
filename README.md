# sigme
sigme is a simple framework for distributing signals between modules in an (threaded) client application. 

## Dependencies
sigme requires GLib: https://developer.gnome.org/about/

When compiling, link to the following libs: pthread, gthread-2.0, gio-2.0, gobject-2.0, glib-2.0.

## Quick tutorial
To give a quick introduction on how to use sigme, we will guide you through an example application that reads temperature value from a fake sensor and writes it to stdout. Steps covered are:
 * Signal definition
 * Signal source
 * Signal receiver
 * Main loop

### Signal definition
First we create the mandatory header file *signal_definitions.h* in which we define the enum ESignalTypes and a struct for use as a temperature signal:
```
signal_definitions.h:

enum ESignalTypes {
    kTempSignal,
    kMaxSigType
};

typedef struct {
    int temp;
} TempSignal;
```
The enum values are used in calls to functions that takes a SignalType argument, and is used by the signal receiver to decode which signal type is sent. The last value in the enum, must be named kMaxSigType.
The signal definition struct can of course be placed in another header file.

### Signal source
Next thing to do is to implement a signal source that publishes temperature signals. Signal sources implements the SignalSource interface which consists of one function: poll(). 

```
temp_source.c:

#include "signal_definitions.h"
#include "sgm_signal.h"
#include <stdlib.h>

// Every signal source should declare a SignalSource instance
static SignalSource source;

SignalSource* temp_source_init(void) {
    source.poll = poll;
    return &source;
}

// Called periodically by the framework to poll for new signals.
void poll(SignalType *type, SignalPayload *data) {

    // Create and populate a new TempSignal
    TempSignal* temp = malloc(sizeof(TempSignal));
    temp->temp = read_temperature();
    
    // Set the output parameters
    *type = kTempSignal;
    *data = (SignalPayload)temp;
}
```
The signal source shall use dynamic memory allocation when creating the signal. The allocated memory will be freed by sigme when all receivers are done with it.
Note that the signal source can only deliver one signal in each poll() call. Also note that the source can create different signal types, it is not fixed to only one as in the example. You could for instance arrange to alternate two signal types every second call.

### Signal receiver
The signal sent from the source wil be delivered to receivers registered for that signal type. Signal receivers implements the SignalReceiver interface, which consists of two functions: get_signal_types() and receive().
```
// Every signal receiver shall declare a SignalReceiver instance.
static SignalReceiver receiver;

SignalReceiver* temp_receiver_init(void) {
    receiver.get_signal_types = getSignals;
    receiver.receive = receive;
    return &receiver;
}

// Called by framework to fetch the signal types this receiver wants
// to subscribe on
static void getSignals(SignalType signals[], unsigned max, unsigned *len) {

    // Fill the signals array with the signal types we are interested in
    signals[0] = kTempSignal;
    
    // indicate how many we filled in
    *len = 1;
}

static ReceiveReturn receive(Signal* signal) {
    // Check if correct signal type
    if (kTempSignal == sgm_signal_type(signal)) {
    
        // Fetch the payload and cast it to a TempSignal
        TempSignal *t = (TempSignal*)(sgm_signal_payload(signal));
    
        // write it to stdout
        printf("temp receiver received %d\n", t->temp);
    }
    else {
        printf("received unexpected type\n");
    }
    
    // We are done with this signal
    return PROCESSING_DONE;
}
```
Notice how the receive function returns the value PROCESSING_DONE. This indicates that the receiver is done with its signal processing and the sigme framework can de-allocate signal memory if no other receivers needs it. If instead the receiver had for instance, pushed the signal pointer to a queue for later processing, it must return PROCESSING_PENDING. 
The example below shows how the code would be changed to use later signal processing by another thread pulling from a queue:
```
static ReceiveReturn receive(Signal* signal) {
    // push the signal to the queue for asynchronous processing in another thread
    g_async_queue_push(queue, signal);
    
    // pending indicates to framework that it shall not de-allocate the signal
    return PROCESSING_PENDING;
}

// Thread function reading from queue
static gpointer printThread(gpointer data) {
    // the queue was passed to the function in the data argument
    GAsyncQueue *q = data;

    while(1) {
        Signal* signal = (Signal*)g_async_queue_pop(q);
        
        // A signal pointer was popped from queue, wait for lock to start processing it
        sgm_signal_process(signal);
        
        switch (sgm_signal_type(signal)) {
            case kTempSignal: {
                    TempSignal *temp = (TempSignal*)sgm_signal_payload(signal);
                    printf("stdout_broadcaster:\treceived %d\n", temp->temp);
                    break;
            }

            default:
                printf("stdout_broadcaster:\treceived unknown signal\n");
                break;
        }

        // We are done with this signal
        sgm_signal_complete(signal);
    }

    return 0;
}
```
In case of asynchronus signal processing, the thread must call *sgm_signal_process(signal)* to aquire a lock for the signal. This is a blocking call. And to release the lock, the receiver calls *sgm_signal_complete()* to indicate that the signal can be de-allocated.
If the receiver is partially done with processing and will continue later, it shall instead call *sgm_signal_complete_later()*

