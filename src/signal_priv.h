#ifndef _SIGNAL_PRIV_H_
#define _SIGNAL_PRIV_H_

#include "signal.h"
#include <stdint.h>

/**
 * Increment signal reference count
 *
 * Should (normally) not be used by client application.
 * The framework calls this function for each receiver it delivers
 * the signal to. Each receiver should call signal_unref when signal
 * processing is done. This will decrement the reference count,
 * and when zero, the allocated memory for the signal will be free'd.
 *
 * @param signal
 */
void signal_ref(Signal* signal);
uint32_t signal_refcount(Signal* signal);

#endif /* _SIGNAL_PRIV_H_ */
