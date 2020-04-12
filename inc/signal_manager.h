#ifndef _SIGNAL_MANAGER_H_
#define _SIGNAL_MANAGER_H_

#include <glib.h>
#include "signal_receiver.h"
#include "signal_source.h"

void signal_manager_register_source(SignalSource *source);
void signal_manager_register_receiver(SignalReceiver *receiver);
void signal_manager_run(void);

#endif
