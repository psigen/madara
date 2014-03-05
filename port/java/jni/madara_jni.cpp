
#include "madara_jni.h"

static JavaVM* madara_JVM = NULL;
static jclass madara_JNI = NULL;
static jmethodID madara_JNI_function_callback = NULL;
static jmethodID madara_JNI_filter_callback = NULL;

MADARA_Export jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK)
    {
        return JNI_ERR;
    }

    madara_JNI = (jclass)env->NewGlobalRef(env->FindClass("com/madara/MadaraJNI"));
    madara_JNI_function_callback = env->GetStaticMethodID(madara_JNI, "functionCallback", "(Lcom/madara/MadaraFunction;[JJ)J" );
    madara_JNI_filter_callback = env->GetStaticMethodID(madara_JNI, "filterCallback", "(Lcom/madara/transport/TransportFilter;[JJ)J" );

    madara_JVM = vm;

    return env->GetVersion();
}

MADARA_Export void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved)
{
    JNIEnv* env;
    vm->GetEnv((void**)&env, JNI_VERSION_1_6);
    env->DeleteGlobalRef(madara_JNI);
    madara_JVM = NULL;
    madara_JNI_function_callback = NULL;
    madara_JNI_filter_callback = NULL;
}

MADARA_Export JNIEnv* madara_jni_get_env()
{
    JNIEnv* env;
    madara_JVM->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (env == 0) //Thread is not attached
    {
        madara_JVM->AttachCurrentThread((void**)&env, NULL);
    }
    return env;
}

MADARA_Export JavaVM* madara_jni_jvm()
{
    return madara_JVM;
}

MADARA_Export jclass madara_jni_class()
{
    return madara_JNI;
}

MADARA_Export jmethodID madara_jni_function_callback()
{
    return madara_JNI_function_callback;
}

MADARA_Export jmethodID madara_jni_filter_callback()
{
    return madara_JNI_filter_callback;
}