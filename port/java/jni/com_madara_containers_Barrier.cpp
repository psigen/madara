#include "com_madara_containers_Barrier.h"
#include "madara/knowledge_engine/containers/Barrier.h"

namespace engine = Madara::Knowledge_Engine;
namespace containers = engine::Containers;

/*
 * Class:     com_madara_containers_Barrier
 * Method:    jni_Barrier
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_madara_containers_Barrier_jni_1Barrier__
  (JNIEnv *, jobject)
{
  return (jlong) new containers::Barrier ();
}


/*
 * Class:     com_madara_containers_Barrier
 * Method:    jni_Barrier
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_containers_Barrier_jni_1Barrier__J
  (JNIEnv *, jobject, jlong cptr)
{
  return (jlong) new containers::Barrier (*(containers::Barrier *)cptr);
}


/*
 * Class:     com_madara_containers_Barrier
 * Method:    jni_freeBarrier
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_madara_containers_Barrier_jni_1freeBarrier
  (JNIEnv *, jclass, jlong cptr)
{
  delete (containers::Barrier *) cptr;
}


/*
 * Class:     com_madara_containers_Barrier
 * Method:    jni_set
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_madara_containers_Barrier_jni_1set
  (JNIEnv *, jobject, jlong cptr, jlong value)
{
  containers::Barrier * current = (containers::Barrier *) cptr;
  if (current)
    *current = value;
}


/*
 * Class:     com_madara_containers_Barrier
 * Method:    jni_getName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_madara_containers_Barrier_jni_1getName
  (JNIEnv * env, jobject, jlong cptr)
{
  jstring result;

  containers::Barrier * current = (containers::Barrier *) cptr;
  if (current)
  {
    result = env->NewStringUTF(current->get_name ().c_str ());
  }

  return result;
}

/*
 * Class:     com_madara_containers_Barrier
 * Method:    jni_setName
 * Signature: (JJJLjava/lang/String;V
 */
JNIEXPORT void JNICALL Java_com_madara_containers_Barrier_jni_1setName
  (JNIEnv * env, jobject, jlong cptr, jlong type, jlong context, jstring name)
{
  containers::Barrier * current = (containers::Barrier *) cptr;

  if (current)
  {
    const char * str_name = env->GetStringUTFChars(name, 0);

    if (type == 0)
    {
      engine::Knowledge_Base * kb = (engine::Knowledge_Base *) context;
      current->set_name (str_name, *kb);
    }
    else if (type == 1)
    {
      engine::Variables * vars = (engine::Variables *) context;
      current->set_name (str_name, *vars);
    }

    env->ReleaseStringUTFChars(name, str_name);
  }
}


/*
 * Class:     com_madara_containers_Barrier
 * Method:    jni_toString
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_madara_containers_Barrier_jni_1toString
  (JNIEnv * env, jobject, jlong cptr)
{
  jstring result;

  containers::Barrier * current = (containers::Barrier *) cptr;
  if (current)
  {
    result = env->NewStringUTF(current->to_string ().c_str ());
  }

  return result;
}

/*
 * Class:     com_madara_containers_Barrier
 * Method:    jni_toDouble
 * Signature: (J)D
 */
JNIEXPORT jdouble JNICALL Java_com_madara_containers_Barrier_jni_1toDouble
  (JNIEnv *, jobject, jlong cptr)
{
  jdouble result (0.0);

  containers::Barrier * current = (containers::Barrier *) cptr;
  if (current)
  {
    result = current->to_double ();
  }

  return result;
}


/*
 * Class:     com_madara_containers_Barrier
 * Method:    jni_toLong
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_containers_Barrier_jni_1toLong
  (JNIEnv *, jobject, jlong cptr)
{
  jlong result (0);

  containers::Barrier * current = (containers::Barrier *) cptr;
  if (current)
  {
    result = current->to_integer ();
  }

  return result;
}


/*
 * Class:     com_madara_containers_Barrier
 * Method:    jni_next
 * Signature: (J)J
 */
JNIEXPORT void JNICALL Java_com_madara_containers_Barrier_jni_1next
  (JNIEnv *, jobject, jlong cptr)
{
  containers::Barrier * current = (containers::Barrier *) cptr;
  current->next ();
}


/*
 * Class:     com_madara_containers_Barrier
 * Method:    jni_isDone
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_madara_containers_Barrier_jni_1isDone
  (JNIEnv *, jobject, jlong cptr)
{
  containers::Barrier * current = (containers::Barrier *) cptr;
  return current->is_done ();
}


/*
 * Class:     com_madara_containers_Barrier
 * Method:    jni_modify
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_madara_containers_Barrier_jni_1modify
  (JNIEnv *, jobject, jlong cptr)
{
  containers::Barrier * current = (containers::Barrier *) cptr;
  current->modify ();
}


/*
 * Class:     com_madara_containers_Barrier
 * Method:    jni_resize
 * Signature: (JII)V
 */
JNIEXPORT void JNICALL Java_com_madara_containers_Barrier_jni_1resize
  (JNIEnv *, jobject, jlong cptr, jint id, jint participants)
{
  containers::Barrier * current = (containers::Barrier *) cptr;
  current->resize (id, participants);
}
