
#include "com_madara_containers_IntegerVector.h"
#include "madara/knowledge_engine/containers/Integer_Vector.h"

namespace containers = Madara::Knowledge_Engine::Containers;
namespace engine = Madara::Knowledge_Engine;

/*
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_IntegerVector
 * Signature: ()J
 */
jlong JNICALL Java_com_madara_containers_IntegerVector_jni_1IntegerVector__
  (JNIEnv * env, jobject)
{
  return (jlong) new containers::Integer_Vector ();
}

/*
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_IntegerVector
 * Signature: (J)J
 */
jlong JNICALL Java_com_madara_containers_IntegerVector_jni_1IntegerVector__J
  (JNIEnv * env, jobject, jlong cptr)
{
  return (jlong) new containers::Integer_Vector (*(containers::Integer_Vector *)cptr);
}

/*
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_freeIntegerVector
 * Signature: (J)V
 */
void JNICALL Java_com_madara_containers_IntegerVector_jni_1freeIntegerVector
  (JNIEnv * env, jclass, jlong cptr)
{
  delete (containers::Integer_Vector *) cptr;
}

/*
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_set
 * Signature: (JIJ)V
 */
void JNICALL Java_com_madara_containers_IntegerVector_jni_1set
  (JNIEnv * env, jobject, jlong cptr, jint index, jlong value)
{
  containers::Integer_Vector * current = (containers::Integer_Vector *) cptr;
  if (current)
    current->set (index, value);
}

/*
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_getName
 * Signature: (J)Ljava/lang/String;
 */
jstring JNICALL Java_com_madara_containers_IntegerVector_jni_1getName
  (JNIEnv * env, jobject, jlong cptr)
{
  jstring result;

  containers::Integer_Vector * current = (containers::Integer_Vector *) cptr;
  if (current)
    result = env->NewStringUTF(current->get_name ().c_str ());

  return result;
}

/*
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_setName
 * Signature: (JJJLjava/lang/String;)V
 */
void JNICALL Java_com_madara_containers_IntegerVector_jni_1setName
  (JNIEnv * env, jobject, jlong cptr, jlong type, jlong context, jstring name)
{
  containers::Integer_Vector * current = (containers::Integer_Vector *) cptr;

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
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_get
 * Signature: (JI)Ljava/lang/String;
 */
jlong JNICALL Java_com_madara_containers_IntegerVector_jni_1get
  (JNIEnv * env, jobject, jlong cptr, jint index)
{
  jlong result (0);

  containers::Integer_Vector * current = (containers::Integer_Vector *) cptr;
  if (current)
    result = (*current) [index];

  return result;
}

/*
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_toRecord
 * Signature: (JI)D
 */
jlong JNICALL Java_com_madara_containers_IntegerVector_jni_1toRecord__JI
  (JNIEnv * env, jobject, jlong cptr, jint index)
{
  Madara::Knowledge_Record * result (0);

  containers::Integer_Vector * current = (containers::Integer_Vector *) cptr;
  if (current)
    result = new Madara::Knowledge_Record (current->to_record (index));

  return (jlong) result;
}

/*
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_toRecord
 * Signature: (J)D
 */
jlong JNICALL Java_com_madara_containers_IntegerVector_jni_1toRecord__J
  (JNIEnv * env, jobject, jlong cptr)
{
  Madara::Knowledge_Record * result (0);

  containers::Integer_Vector * current = (containers::Integer_Vector *) cptr;
  if (current)
    result = new Madara::Knowledge_Record (current->to_record ());

  return (jlong) result;
}
