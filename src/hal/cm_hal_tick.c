/**
 * @file cm_hal_tick.c
 * Provide access to the system tick with 1 millisecond resolution
 */

/*********************
 *      INCLUDES
 *********************/
#include "cm_hal_tick.h"
#include <stddef.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
#if !CM_TICK_CUSTOM
    static uint32_t sys_time = 0;
    static volatile uint8_t tick_irq_flag;
#endif

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

#if !CM_TICK_CUSTOM
/**
 * You have to call this function periodically
 * @param tick_period the call period of this function in milliseconds
 */
CM_ATTRIBUTE_TICK_INC void cm_tick_inc(uint32_t tick_period)
{
    tick_irq_flag = 0;
    sys_time += tick_period;
}
#endif

/**
 * Get the elapsed milliseconds since start up
 * @return the elapsed milliseconds
 */
uint32_t cm_tick_get(void)
{
    /*If `cm_tick_inc` is called from an interrupt while `sys_time` is read
     *the result might be corrupted.
     *This loop detects if `cm_tick_inc` was called while reading `sys_time`.
     *If `tick_irq_flag` was cleared in `cm_tick_inc` try to read again
     *until `tick_irq_flag` remains `1`.*/
    uint32_t result;
    do {
        tick_irq_flag = 1;
        result        = sys_time;
    } while(!tick_irq_flag); /*Continue until see a non interrupted cycle*/

    return result;
}

/**
 * Get the elapsed milliseconds since a previous time stamp
 * @param prev_tick a previous time stamp (return value of cm_tick_get() )
 * @return the elapsed milliseconds since 'prev_tick'
 */
uint32_t cm_tick_elaps(uint32_t prev_tick)
{
    uint32_t act_time = cm_tick_get();

    /*If there is no overflow in sys_time simple subtract*/
    if(act_time >= prev_tick) {
        prev_tick = act_time - prev_tick;
    }
    else {
        prev_tick = UINT32_MAX - prev_tick + 1;
        prev_tick += act_time;
    }

    return prev_tick;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
