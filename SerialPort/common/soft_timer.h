#ifndef SOFT_TIMER_H_
#define SOFT_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

/*
 * -----------------------------------------------------------------------------
 * Type definition
 * -----------------------------------------------------------------------------
 */
typedef void (*SoftTimer_OnTick_fp)(void);

typedef enum
{
    eDownCounting = 0,
    eUpCounting = 1

}counter_enum;

typedef struct
{
    uint16_t pseudo_unit, reload_value;
    uint16_t ticks_per_second;
    uint32_t raw_ticks;
    bool enabled;
    counter_enum count_type;

    SoftTimer_OnTick_fp on_tick_handler;
}SoftTimer_st;

#ifdef __cplusplus
extern "C"
{
#endif
/*
 * -----------------------------------------------------------------------------
 * Public function prototypes
 * -----------------------------------------------------------------------------
 */
    extern void SoftTimer_default(SoftTimer_st* handle, uint16_t rate);
    extern void SoftTimer_default_downCount(SoftTimer_st* handle, uint16_t rate, uint16_t reload_value);

    extern void SoftTimer_Tick(SoftTimer_st* handle);

    extern void SoftTimer_Pause(SoftTimer_st* handle);
    extern void SoftTimer_Resume(SoftTimer_st* handle);
    extern void SoftTimer_Reset(SoftTimer_st* handle);

    extern bool SoftTimer_isTimeout(SoftTimer_st* handle);
    extern bool SoftTimer_hasElapsed(SoftTimer_st* handle, uint16_t seconds);
    extern uint32_t SoftTimer_totalElapsedTicks(SoftTimer_st* handle);

#ifdef __cplusplus
}
#endif

#endif /* SOFT_TIMER_H_ */
