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
#define LOG_NIDEBUG 1

#include <log/log.h>

#include "Power.h"
#include "performance.h"

using ::aidl::android::hardware::power::Boost;

namespace aidl {
namespace android {
namespace hardware {
namespace power {
namespace impl {

extern "C" int perf_hint_enable_with_type(int hint_id, int duration, int type);

static bool processInteractionBoost(int32_t durationMs) {
    ALOGI("processing interaction boost with duration: %i", durationMs);
    perf_hint_enable_with_type(VENDOR_HINT_SCROLL_BOOST, durationMs, SCROLL_VERTICAL);
    return true;
}

bool setBoostOverride(Boost type, int32_t durationMs) {
    int ret = false;
    switch (type) {
        case Boost::INTERACTION:
            ret = processInteractionBoost(durationMs);
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

}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace android
}  // namespace aidl
