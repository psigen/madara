/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#ifndef _Included_com_madara_Debug
#define _Included_com_madara_Debug

#include <jni.h>
#include "madara/MADARA_export.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_madara_Debug
 * Method:    jni_setDebugLevel
 * Signature: (I)V
 */
MADARA_Export void JNICALL Java_com_madara_Debug_jni_1setDebugLevel
  (JNIEnv *, jclass, jint);

#ifdef __cplusplus
}
#endif
#endif
