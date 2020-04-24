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

The size and format of the signal payload is not known by the framework, so during signal creation, *signal_new()* will make a callout to a *create()* function supplied by the application. In this function, the client code should allocate and populate a memory block for the payload.
Since each **SignalType** payload may be different from the others, there must be one *create* function for each **SignalType**. This is handled by registration of a **SignalTypeDetails** structure for each SignalType.

### Signal registration
Like receivers and sources, the used signal types must be registered at startup. This ties a **SignalType** identifier to an instance of **SignalTypeDetails** interface, which includes a *create()* function for creation of the signal. So when a signal source creates a new signal with *signal_new(type, data)*, the framework knows which interface to call for allocation of the payload part of the signal. This gives the application full control over its signal types.

![signal_registration](/doc/signal_registration.png)


### Sending signals
Signals are not actually sent, they are delivered when the signal manager in the framework polls the sources for signals. The signal manager calls the *poll()* function in the **SignalSource** interface for each registered signal source. 
