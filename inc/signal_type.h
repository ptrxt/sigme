#ifndef _SIGNAL_TYPE_H_
#define _SIGNAL_TYPE_H_

#include "signal.h"


/**
 * SignalTypeDetails interface
 *
 * The application shall register an instance of this struct for each
 * signal type used.
 */
typedef struct {
    /**
     * Unique signal type identifier
     *
     * Enum value define in signal_definitions.h
     */
    SignalType type;

    /**
     * Returns length in bytes of the signal payload
     */
    int (*length)(Signal* signal);

    /**
     * Get the signal types name
     */
    const char* (*name)(void);      // Returns signal type's name
} SignalTypeDetails;


/**
 * Register signal type
 *
 * Once registered the signal type is available for sources and receivers
 *
 * @param signalTypeDetails, instantiation of the struct for the new signal type
 */
int signal_type_register(SignalTypeDetails* signalTypeDetails);


#endif /* _SIGNAL_TYPE_H_ */
