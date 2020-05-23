# sigme
sigme is a simple framework for distributing signals between modules in an (threaded) client application. 

## Dependencies
sigme requires GLib: https://developer.gnome.org/about/

When compiling, link to the following libs: pthread, gthread-2.0, gio-2.0, gobject-2.0, glib-2.0.

## Quick tutorial
To give a quick introduction on how to use sigme, we will guide you through an example application that reads temperature values from file and writes them to stdout. Steps covered are:
 * Signal definition
 * Signal source
 * Signal receiver
 * Main loop

### Signal definition
First we create the mandatory headerfile *signal_definitions.h* in which we define the enum ESignalTypes and also defines a struct for use as a temperature signal:
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

### Signal source
Next thing to do is to implement a signal source that creates temperature signals. Signal sources implements the SignalSource interface which consists of one function: poll(). 

```
temp_source.c:

#include "signal_definitions.h"
#include "sgm_signal.h"
#include <stdlib.h>

// The SignalSource instance
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
### Signal receiver
The signal sent from the source wil be delivered to receivers registered for that signal type. Signal receivers implements the SignalReceiver interface, which consists of two functions: get_signal_types() and receive().
