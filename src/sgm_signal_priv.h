#ifndef _SGM_SIGNAL_PRIV_H_
#define _SGM_SIGNAL_PRIV_H_

#include "sgm_signal.h"
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
void sgm_signal_ref(Signal* signal);
uint32_t sgm_signal_refcount(Signal* signal);

/**
 * Create new signal
 *
 * Signal sources calls this function to create a new signal.
 * Uses dynamic memory allocation.
 *
 * @param type, signal type to create
 * @param data, the dynamically allocated payload to be attached to the signal
 * @return pointer to dynamically allocated signal
 */
Signal* sgm_signal_new(SignalType type, SignalPayload data);

#endif /* _SGM_SIGNAL_PRIV_H_ */
