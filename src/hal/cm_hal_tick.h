/**
 * @file cm_hal_tick.h
 * Provide access to the system tick with 1 millisecond resolution
 */

#ifndef CM_HAL_TICK_H
#define CM_HAL_TICK_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdbool.h>

/*********************
 *      DEFINES
 *********************/
#ifndef CM_ATTRIBUTE_TICK_INC
#define CM_ATTRIBUTE_TICK_INC
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

//! @cond Doxygen_Suppress

#if !CM_TICK_CUSTOM
/**
 * You have to call this function periodically
 * @param tick_period the call period of this function in milliseconds
 */
CM_ATTRIBUTE_TICK_INC void cm_tick_inc(uint32_t tick_period);
#endif

//! @endcond

/**
 * Get the elapsed milliseconds since start up
 * @return the elapsed milliseconds
 */
uint32_t cm_tick_get(void);

/**
 * Get the elapsed milliseconds since a previous time stamp
 * @param prev_tick a previous time stamp (return value of cm_tick_get() )
 * @return the elapsed milliseconds since 'prev_tick'
 */
uint32_t cm_tick_elaps(uint32_t prev_tick);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*CM_HAL_TICK_H*/
