#ifndef _SGM__H_
#define _SGM__H_

#include <glib.h>
#include "sgm_signal.h"
#include "sgm_receiver.h"
#include "sgm_source.h"

void sgm_add_source(SignalSource *source);
void sgm_add_receiver(SignalReceiver *receiver);
void sgm_run(void);

#endif
