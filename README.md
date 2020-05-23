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
    // Fetch the payload and cast it to a TempSignal
    TempSignal *t = (TempSignal*)(sgm_signal_payload(signal));
    
    // write it to stdout
    printf("temp receiver received %d\n", t->temp);
    
    // We are done with this signal
    return PROCESSING_DONE;
}
```
Note how the receive function returns the value PROCESSING_DONE. This indicates that the receiver has done its signal processing and the sigme framework can de-allocate signal memory if no other receivers needs it. If instead the receiver had pushed the signal pointer to a queue for later processing in another thread, it must return PROCESSING_PENDING. 
