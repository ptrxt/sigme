# sigme
sigme is a simple framework for distributing signals between modules in an (threaded) client application. At its core, there is a **signal manager** polling signal sources for new signals and forwards them to the signal receivers. A signal source is part of the client application and implements the **SignalSource** interface. A signal receiver is also part of the client application and implements the **SignalReceiver** interface. 
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


