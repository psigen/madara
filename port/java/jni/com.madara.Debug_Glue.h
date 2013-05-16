/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#ifndef _Included_com_madara_Debug_Glue
#define _Included_com_madara_Debug_Glue

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_madara_Debug
 * Method:    jni_setDebugLevel
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_madara_Debug_jni_1setDebugLevel
  (JNIEnv *, jclass, jint);

#ifdef __cplusplus
}
#endif
#endif
