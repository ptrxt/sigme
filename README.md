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
The signal sources produces **signals**. A signal is a chunk of dynamically allocated memory created by calling signal_new(). This function actually allocates two chunks of memory. The first is a control block used for signal lifetime management. The second block is the signal payload seen by the client application. 
![signal](/doc/signal.png)

The size and format of the signal payload is not known by the framework. During signal creation, signal_new() will make a call to a create() callback supplied by the application for that signal type. In the create() function, the application should allocate and populate a memory block for the payload.

### Signal registration
Like receivers and sources, the used signal types must be registered at startup. This ties a **SignalType** identifier to an instance of **SignalTypeDetails** interface. So when an signal source creates a new signal with signal_new(type, data), the framework knows which interface to call for allocation of the signal payload part.
