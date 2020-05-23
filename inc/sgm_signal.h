#ifndef SGM_SIGNAL_H_
#define SGM_SIGNAL_H_

// The file: signal_definitions.h, must be defined by the application and
// should contain a definition of the enum ESignalsTypes: enum ESignalsTypes {..., ..., kMaxSigType};
#include "signal_definitions.h"

#include <stdint.h>

typedef struct Signal_t Signal;
typedef enum ESignalTypes SignalType;
typedef void* SignalPayload;



/**
 * Start deferred processing of signal
 *
 * Shall be called in signal processing thread if the signal
 * was not handled in the receive function. E.g if the receive
 * method pushes the signal on a queue for later processing, the
 * thread pulling from the queue must call this function before
 * the signal processing is started.
 *
 * This is a blocking operation, the current thread will block until the
 * specified signal is released by other threads.
 *
 * @note: if the signal is processed in the receive function of
 * the SignalReceiver, there is no need to call this function.
 *
 * @note: also, if the signal contents are copied and saved for
 * later processing, there is no need to call this function
 */
void sgm_signal_process(Signal* signal);

/**
 * End signal processing, leave it open for processing later
 *
 * Should be called if the application currently is done with
 * processing but wants to keep the reference to the signal for
 * further processing later.
 *
 * @note Should only be called if sgm_signal_process() was called
 */
void sgm_signal_complete_later(Signal* signal);

/**
 * End signal processing
 *
 * Should be called by the signal receiver when signal processing
 * is done and no further signal processing will be done. Allocated
 * memory can be de-allocated by the framework.
 *
 * * @note Should only be called if sgm_signal_process() was called
 */
void sgm_signal_complete(Signal* signal);

/**
 * Get signal type of attached payload
 */
SignalType sgm_signal_type(Signal* signal);

/**
 * Get signal payload
 */
SignalPayload sgm_signal_payload(Signal* signal);



#endif /* _SIGNAL_H_ */
