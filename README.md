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
}
```
