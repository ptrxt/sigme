#ifndef _SGM_MANAGER_H_
#define _SGM_MANAGER_H_

#include <glib.h>
#include "sgm_receiver.h"
#include "sgm_source.h"

void sgm_manager_register_source(SignalSource *source);
void sgm_manager_register_receiver(SignalReceiver *receiver);
void sgm_manager_run(void);

#endif
