/*
 * Copyright (C) 2013 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "AsuspecKeyHandler"

#include "JNIHelp.h"
#include "jni.h"
#include <utils/Log.h>
#include <utils/misc.h>

#include <android_runtime/AndroidRuntime.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>

namespace asuspec {

#define ASUSPEC_DEV "/dev/asuspec"

// copied from drivers/input/asusec/asuspec.h
#define ASUSDEC_TP_ON   1
#define ASUSDEC_TP_OFF  0
#define ASUSPEC_IOC_MAGIC   0xf4
#define ASUSDEC_TP_CONTROL      _IOR(ASUSPEC_IOC_MAGIC, 5,  int)


JNIEXPORT jboolean JNICALL asuspec_KeyHandler_nativeToggleTouchpad
  (JNIEnv *env, jclass cls, jboolean status) {
    ALOGD("Switching touchpad %d\n", status);

    int fd = open(ASUSPEC_DEV, O_RDONLY | O_NONBLOCK);

    if (fd < 0) {
        ALOGE("Could  open device %s\n", ASUSPEC_DEV);
        return -1;
    }

    int on = (status == 0) ? ASUSDEC_TP_OFF : ASUSDEC_TP_ON;
    int success = ioctl(fd, ASUSDEC_TP_CONTROL, on);

    if (success != 0) {
        ALOGE("Error calling ioctl, %d\n", success);
    }

    close(fd);

    ALOGD("Touchpad is %d\n", on);
    return (jboolean) ((on == 1) ? true : false);
}

static JNINativeMethod sMethods[] = {
     /* name, signature, funcPtr */
    {"nativeToggleTouchpad", "(Z)Z", (void*)asuspec_KeyHandler_nativeToggleTouchpad},
};

int register_asuspec_KeyHandler(JNIEnv* env)
{
    return jniRegisterNativeMethods(env, "com/cyanogenmod/asuspec/KeyHandler", sMethods, NELEM(sMethods));
}

} /* namespace asuspec */
