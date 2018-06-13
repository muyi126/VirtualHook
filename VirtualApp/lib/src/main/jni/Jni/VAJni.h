//
// VirtualApp Native Project
//

#ifndef NDK_CORE_H
#define NDK_CORE_H

#include "Vhook.h"
#include "Helper.h"
#include "Foundation/VMPatch.h"
#include <Foundation/IOUniformer.h>

extern alias_ref<jclass> nativeEngineClass;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved);
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved);



#endif //NDK_CORE_H
