#ifndef _STDOUT_BROADCASTER_H_
#define _STDOUT_BROADCASTER_H_

#include "sgm_receiver.h"
#include <glib.h>

SignalReceiver* stdout_broadcaster_init(GThread **thread);

#endif
