# sigme
sigme is a simple framework for distributing signals between modules in an (threaded) client application. At its core, there is a **signal manager** polling signal sources for new signals and forwards them to the signal receivers. A signal source is part of the client application and implements the **SignalSource** interface. A signal receiver is also part of the client application and implements the **SignalReceiver** interface. 
![overview](/doc/overview.png)
