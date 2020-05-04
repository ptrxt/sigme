# sigme
sigme is a simple framework for distributing signals between modules in an (threaded) client application. 

## Dependencies
sigme requires GLib: https://developer.gnome.org/about/
When compiling, link to the following libs: pthread, gthread-2.0, gio-2.0, gobject-2.0, glib-2.0.

## Overview
At its core, there is a **signal manager** polling signal sources for new signals and forwards them to the signal receivers. A signal source is part of the client application and implements the **SignalSource** interface. A signal receiver is also part of the client application and implements the **SignalReceiver** interface. 
![overview](/doc/overview.png)

At application start up, sources and receivers should register with the signal manager:
```
int main() {
   SignalSource *source = temp_sensor_init();
   SignalReceiver *receiver = display_unit_init();
   signal_manager_register_source(source);
   signal_manager_register_receiver(receiver);
   ...
   while(1) {
      signal_manager_run();
      delay(1000);
   }
}
```
During registration, signal manager queries, through the SignalReceiver interface, receivers for the signal types they want to subscribe on.
## Signals
The signal sources produces **signals**. A signal is a chunk of dynamically allocated memory created by calling *signal_new()*. This memory block is a control block used for signal lifetime management. It also contains a pointer to another, by the application, dynamically allocated memory block: the application signal payload. It is created by the signal source and passed as a pointer to the call to *signal_new()*.

<p align="center">
  <img src="/doc/signal2.png">
</p>

### Signal registration
Like receivers and sources, the used signal types must be registered at startup. This ties a **SignalType** identifier to an instance of **SignalTypeDetails** interface.

![signal_registration](/doc/signal_registration2.png)


### Sending signals
To send a signal, the application should allocate memory for holding the desired application signal type, populate it and pass it to *signal_new()* which will allocate a control block for the signal. The return value from *signal_new()* is a pointer to a new signal, which should be delivered to the framework when it polls the source. If the source don't have anything to send, it just returns NULL in the *poll()* call.
```
temp_sensor.c:

static SignalSource tempSource;

static Signal* pollTempSensor() {
    TempSignal* ts = (TempSignal*)malloc(sizeof(TempSignal));
    ts->id = 0;
    ts->value = 23;
    return signal_new(kTempSignal, ts);
}

SignalSource *temp_sensor_init(void) {
    tempSource.poll = pollTempSensor;
    return &tempSource;
}
```
The new signal will be delivered to receivers of that signal type. Eventually the framework will free the memory associated with the signal. 

### Receiving signals
Signals are delivered to signal receivers through the *receive()* function. The receiver has two options:
 - extract the signal payload and use it now or later and return **PROCESSING_DONE** to indicate it has no further interest in keeping the signal memory allocated.
 - save the signal pointer for later processing and return **PROCESSING_PENDING** to indicate the signal should not be de-allocated yet.

#### Signal reference counting
When a receiver returns **PROCESS_PENDING** to the *receive()* call, a reference counter is incremented for the signal. As long as the counter is > 0, the signal will not be de-allocated. Later when the receiver has finished processing the signal and has no further interest in it, it should call *signal_unref()* to decrement the counter. When the reference count reaches 0, the memory allocated for the signal is freed by the framework.

#### Signal mutex
When signal processing is performed later outside the *receive()* call, the application must lock the signal mutex before using the signal pointer. This is done by calling *signal_lock()* which is a blocking call. Once the thread gets the lock, it is resumed and can begin using the signal. When signal processing is done, and if the thread does not need access to the signal pointer anymore, it should call *signal_unref()* to decrement the reference count. Otherwise, if the thread still wants to keep its reference to the signal, it should call *signal_unlock()* to relese the signal mutex. 

