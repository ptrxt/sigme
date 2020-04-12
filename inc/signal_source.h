#ifndef _SIGNAL_SOURCE_H_
#define _SIGNAL_SOURCE_H_

#include "signal.h"


/**
 * SignalSource interface
 *
 * The application registers signal sources for sending signals. The
 * signal manager polls the sources and forwards the signals to the
 * receivers. The source typically creates new signals using signal_new()
 * and returns the signal pointer to the manager. The signal framework will
 * free the allocated memory when signal processing is done.
 */
typedef struct {
    Signal* (*poll)();
} SignalSource;

#endif /* _SIGNAL_SOURCE_H_ */
