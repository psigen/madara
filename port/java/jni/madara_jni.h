
#ifndef _Included_com_madara_JNI
#define _Included_com_madara_JNI

#include <jni.h>
#include "madara/MADARA_export.h"

MADARA_Export jint JNI_OnLoad(JavaVM *vm, void *reserved);
MADARA_Export void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved);

MADARA_Export JNIEnv* madara_jni_get_env();

MADARA_Export JavaVM* madara_jni_jvm();

MADARA_Export jclass madara_jni_class();
MADARA_Export jmethodID madara_jni_function_callback();
MADARA_Export jmethodID madara_jni_filter_callback();

#endif