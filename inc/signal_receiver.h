#ifndef _SIGNAL_RECEIVER_H_
#define _SIGNAL_RECEIVER_H_

#include "signal.h"


/**
 * SignalReceiver interface
 *
 * The application receives signals by registering a signal receiver.
 * Each receiver can be associated with several signal types. The framework
 * queries the types during receiver registration using the get_signal_types
 * function. Then signals is delivered using the receive function.
 */
typedef struct {
    /**
     * Receive signal
     *
     * The framework delivers signals to the receiver using this function.
     * Executed in the context of signal manager and the signal is locked
     * by that thread. So if the receive function places the signal in a
     * queue to be handled by another thread, that thread must acquire the lock
     * by calling signal_lock() before processing the signal to be sure signal
     * manager thread has released the signal.
     */
    void (*receive)(Signal* signal);

    /**
     * Request signals to be subscribed
     *
     * During registration of the signal receiver, the framework uses this
     * function to query the signal types to deliver to the receiver.
     *
     * @param signals, array to be populated by the wanted signal types
     * @param max, max length of the signals array
     * @param len, should be set to the number of types added to the array
     */
    void (*get_signal_types)(SignalType signals[], unsigned max, unsigned *len);

    /**
     * Get the name of the receiver
     */
    const char* (*name)(void);
} SignalReceiver;


#endif /* _SIGNAL_RECEIVER_H_ */
