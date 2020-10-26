/*
 * Copyright (C) 2020 The LineageOS Project
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * *    * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define LOG_TAG "QTI PowerHAL"
#define LOG_NIDEBUG 0

#include <log/log.h>
#include <time.h>

#include "performance.h"
#include "power-common.h"
#include "utils.h"

const int kMinInteractiveDuration = 250;  /* ms */
const int kMaxInteractiveDuration = 5000; /* ms */
const int kMaxLaunchDuration = 5000;      /* ms */

static bool processInterActionBoost(int32_t durationMs) {
    static struct timespec s_previous_boost_timespec;
    static int s_previous_duration = 0;
    static int interaction_handle = -1;

    struct timespec cur_boost_timespec;
    long long elapsed_time;
    int duration = kMinInteractiveDuration;

    int input_duration = durationMs;
    if (input_duration > duration) {
        duration = (input_duration > kMaxInteractiveDuration) ? kMaxInteractiveDuration
                                                              : input_duration;
    }

    clock_gettime(CLOCK_MONOTONIC, &cur_boost_timespec);

    elapsed_time = calc_timespan_us(s_previous_boost_timespec, cur_boost_timespec);
    // don't hint if it's been less than 250ms since last boost
    // also detect if we're doing anything resembling a fling
    // support additional boosting in case of flings
    if (elapsed_time < 250000 && duration <= 750) {
        return true;
    }
    s_previous_boost_timespec = cur_boost_timespec;
    s_previous_duration = duration;

    interaction_handle = perf_hint_enable_with_type(VENDOR_HINT_SCROLL_BOOST,
                                                    duration, SCROLL_VERTICAL);
    if (!CHECK_HANDLE(interaction_handle)) {
        ALOGE("Failed to perform interaction boost");
        return false;
    }
    return true;
}

bool setBoostOverride(Boost type, int32_t durationMs) {
    int ret = false;
    switch (type) {
        case Boost::INTERACTION:
            ret = processInterActionBoost(durationMs);
            break;
        default:
            break;
    }
    return ret;
}

bool isBoostSupportedOverride(Boost type) {
    int ret = false;
    switch (type) {
        case Boost::INTERACTION:
            ret = true;
            break;
        default:
            break;
    }
    return ret;
}

int set_interactive_override(int on) {
    return HINT_HANDLED; /* Don't excecute this code path, not in use */
}
