#ifndef _SIGNAL_H_
#define _SIGNAL_H_

// signal_definitions.h must be defined by the application and
// should define the ESignalsTypes: enum ESignalsTypes {..., ..., kMaxSigType};
#include "signal_definitions.h"

#include <stdint.h>

typedef struct Signal_t Signal;
typedef enum ESignalsTypes SignalType;


/**
 * Create new signal
 *
 * Signal sources calls this function to create a new signal.
 * Uses dynamic memory allocation. Calls the create function for
 * the given signal type.
 *
 * @param type, signal type to create
 * @param data, payload to be attached to the signal, will be forwarded to the signaltype create function
 * @return pointer to dynamically allocated signal
 */
Signal* signal_new(SignalType type, void* data);

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
void signal_lock(Signal* signal);

/**
 * Release signal lock
 *
 * Should be called if the application is not done with
 * processing and wants to keep the reference to the signal.
 */
void signal_unlock(Signal* signal);

/**
 * Decrement signal reference count
 *
 * Should be called by the signal receiver when signal processing
 * is done. When signal reference count is zero, it will be de-allocated.
 * If signal is locked and unref is called, no need to call unlock.
 */
void signal_unref(Signal* signal);

/**
 * Get length of attached payload
 */
int signal_length(Signal* signal);

/**
 * Get signal type of attached payload
 */
SignalType signal_type(Signal* signal);

/**
 * Get signal payload
 */
void *signal_data(Signal* signal);

/**
 * Get signal name
 */
const char* signal_name(Signal* signal);


#endif /* _SIGNAL_H_ */
