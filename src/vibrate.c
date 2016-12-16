#include <vibrate.h>

AppTimer* vibration_timer;


void vibrate()
{
    static const uint32_t segments[] = { 700, 300, 300, 200, 300 };
    VibePattern pat = {
        .durations = segments,
        .num_segments = ARRAY_LENGTH(segments),
    };
    vibes_enqueue_custom_pattern(pat);
}

void register_vibration_timer()
{
    //vibrate();
    //vibration_timer = app_timer_register(20000, register_vibration_timer, NULL);
}

void cancel_vibration_timer()
{
    //app_timer_cancel(vibration_timer);
}