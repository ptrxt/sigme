#ifndef SGM_SIGNAL_H_
#define SGM_SIGNAL_H_

// The file: signal_definitions.h, must be defined by the application and
// should contain a definition of the enum ESignalsTypes: enum ESignalsTypes {..., ..., kMaxSigType};
#include "signal_definitions.h"

#include <stdint.h>

typedef struct Signal_t Signal;
typedef enum ESignalsTypes SignalType;
typedef void* SignalPayload;



/**
 * Lock signal mutex
 *
 * In threaded applications the signal manager and signal receivers
 * might be running in different threads and synchronization is needed.
 * In case signal is not processed immediately in the receive() function,
 * but is processed later, the lock must be called before signal processing.
 * This is a blocking operation, the current thread will block until it gets
 * the signal lock.
 *
 * @note No need to lock if signal processing is done within the receive()
 * function. It is already locked by signal_manager.
 */
void sgm_signal_lock(Signal* signal);

/**
 * Release signal lock
 *
 * Should be called if the application is done with
 * processing and wants to keep the reference to the signal for
 * further processing later.
 */
void sgm_signal_unlock(Signal* signal);

/**
 * Decrement signal reference count
 *
 * Should be called by the signal receiver when signal processing
 * is done and no further signal processing will be done. When
 * signal reference count is zero, it will be de-allocated.
 * If signal is locked and unref is called, no need to call unlock.
 */
void sgm_signal_unref(Signal* signal);

/**
 * Get signal type of attached payload
 */
SignalType sgm_signal_type(Signal* signal);

/**
 * Get signal payload
 */
SignalPayload sgm_signal_data(Signal* signal);



#endif /* _SIGNAL_H_ */
