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
The signal sources produces **signals**. A signal is a chunk of dynamically allocated memory created by calling *signal_new()*. This function actually leads to allocation of two blocks of memory. The first block is a control block used for signal lifetime management. The second block is the signal payload seen by the client application. 
<p align="center">
  <img src="/doc/signal2.png">
</p>

The memory block for the signal payload, is allocated by the application and passed as a pointer to the *signal_new()* call. 

### Signal registration
Like receivers and sources, the used signal types must be registered at startup. This ties a **SignalType** identifier to an instance of **SignalTypeDetails** interface.

![signal_registration](/doc/signal_registration.png)


### Sending signals
To send a signal, the application allocates memory for holding the desired signal type, populates it and passes it to *signal_new()* which will allocate a new signal and attach the creates a new signal in the *poll()* function. If the source don't have anything to send, it just returns NULL. 
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


