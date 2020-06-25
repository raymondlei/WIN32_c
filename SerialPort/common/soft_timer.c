
#include <stdint.h>
#include <stdbool.h>

#include "soft_timer.h"

/*
 * -----------------------------------------------------------------------------
 * Private function prototypes
 * -----------------------------------------------------------------------------
 */
static void _changeRate(SoftTimer_st* handle, uint16_t ticks_per_sec);
static void _enable(SoftTimer_st* handle);
static void _disable(SoftTimer_st* handle);

static inline bool _rawTickAligned(SoftTimer_st* handle);
static void _updateCounter(SoftTimer_st* handle);

/*
 * -----------------------------------------------------------------------------
 * Public functions implementation
 * -----------------------------------------------------------------------------
 */
void SoftTimer_default(SoftTimer_st* handle, uint16_t rate)
{
    handle->count_type = eUpCounting;

    _changeRate(handle, rate);
    SoftTimer_Reset(handle);
    _enable(handle);
}

void SoftTimer_default_downCount(SoftTimer_st* handle, uint16_t rate, uint16_t reload_value)
{
    handle->count_type = eDownCounting;
    handle->reload_value = reload_value;

    _changeRate(handle, rate);
    SoftTimer_Reset(handle);
    _enable(handle);
}

void SoftTimer_Tick(SoftTimer_st* handle)
{
    if(handle->enabled)
    {
        if(_rawTickAligned(handle))
        {
            _updateCounter(handle);
        }

        handle->raw_ticks++;
    }
}

void SoftTimer_Pause(SoftTimer_st* handle)
{
    //keep raw_tick values
    _disable(handle);
}

void SoftTimer_Resume(SoftTimer_st* handle)
{
    _enable(handle);
}

void SoftTimer_Reset(SoftTimer_st* handle)
{
    if(handle->count_type == eUpCounting)
    {
        handle->pseudo_unit = 0; //reset
    }
    else
    {
        handle->pseudo_unit = handle->reload_value;  //reload
    }

    handle->raw_ticks = 0;
}

bool SoftTimer_isTimeout(SoftTimer_st* handle)
{
    return handle->pseudo_unit > handle->reload_value;
}

bool SoftTimer_hasElapsed(SoftTimer_st* handle, uint16_t seconds)
{
    return (handle->raw_ticks > (seconds * handle->ticks_per_second));
}

uint32_t SoftTimer_totalElapsedTicks(SoftTimer_st* handle)
{
    return handle->raw_ticks;
}

/*
 * -----------------------------------------------------------------------------
 * Private functions implementation
 * -----------------------------------------------------------------------------
 */
static inline bool _rawTickAligned(SoftTimer_st* handle)
{
    return ((handle->raw_ticks + 1) % handle->ticks_per_second == 0);
}
static void _changeRate(SoftTimer_st* handle, uint16_t ticks_per_sec)
{
    handle->ticks_per_second = ticks_per_sec;
}

static void _enable(SoftTimer_st* handle)
{
    handle->enabled = true;
}

static void _disable(SoftTimer_st* handle)
{
    handle->enabled = false;
}

static void _updateCounter(SoftTimer_st* handle)
{
    switch(handle->count_type)
    {
    default:  //fall-through
    case eUpCounting: handle->pseudo_unit++;
        break;
    case eDownCounting:
        if(handle->pseudo_unit >= 1)
        {
            handle->pseudo_unit--;
        }
        break;
    }

    handle->on_tick_handler(); //invoke function
}
