#ifndef _SGM_SOURCE_H_
#define _SGM_SOURCE_H_

#include "sgm_signal.h"


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
    /**
     * Poll source for new signal
     *
     * @param[out] type, should be set to a value from the ESignalTypes enum
     *             indicating which signal type to be sent
     * @param[out] data, pointer to the signal payload
     */
    void (*poll)(SignalType* type, SignalPayload* data);
} SignalSource;

#endif /* _SGM_SOURCE_H_ */
