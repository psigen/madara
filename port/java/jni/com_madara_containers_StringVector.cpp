
#include "com_madara_containers_StringVector.h"
#include "madara/knowledge_engine/containers/String_Vector.h"

namespace containers = Madara::Knowledge_Engine::Containers;
namespace engine = Madara::Knowledge_Engine;

/*
 * Class:     com_madara_containers_StringVector
 * Method:    jni_StringVector
 * Signature: ()J
 */
jlong JNICALL Java_com_madara_containers_StringVector_jni_1StringVector__
  (JNIEnv * env, jobject)
{
  return (jlong) new containers::String_Vector ();
}

/*
 * Class:     com_madara_containers_StringVector
 * Method:    jni_StringVector
 * Signature: (J)J
 */
jlong JNICALL Java_com_madara_containers_StringVector_jni_1StringVector__J
  (JNIEnv * env, jobject, jlong cptr)
{
  return (jlong) new containers::String_Vector (*(containers::String_Vector *)cptr);
}

/*
 * Class:     com_madara_containers_StringVector
 * Method:    jni_freeStringVector
 * Signature: (J)V
 */
void JNICALL Java_com_madara_containers_StringVector_jni_1freeStringVector
  (JNIEnv * env, jclass, jlong cptr)
{
  delete (containers::String_Vector *) cptr;
}

/*
 * Class:     com_madara_containers_StringVector
 * Method:    jni_set
 * Signature: (JILjava/lang/String;)V
 */
void JNICALL Java_com_madara_containers_StringVector_jni_1set
  (JNIEnv * env, jobject, jlong cptr, jint index, jstring value)
{
  containers::String_Vector * current = (containers::String_Vector *) cptr;
  if (current)
  {
    const char * str_value = env->GetStringUTFChars(value, 0);

    current->set (index, str_value);

    env->ReleaseStringUTFChars(value, str_value);
  }
}

/*
 * Class:     com_madara_containers_StringVector
 * Method:    jni_getName
 * Signature: (J)Ljava/lang/String;
 */
jstring JNICALL Java_com_madara_containers_StringVector_jni_1getName
  (JNIEnv * env, jobject, jlong cptr)
{
  jstring result;

  containers::String_Vector * current = (containers::String_Vector *) cptr;
  if (current)
    result = env->NewStringUTF(current->get_name ().c_str ());

  return result;
}

/*
 * Class:     com_madara_containers_StringVector
 * Method:    jni_setName
 * Signature: (JJJLjava/lang/String;)V
 */
void JNICALL Java_com_madara_containers_StringVector_jni_1setName
  (JNIEnv * env, jobject, jlong cptr, jlong type, jlong context, jstring name)
{
  containers::String_Vector * current = (containers::String_Vector *) cptr;

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
 * Class:     com_madara_containers_StringVector
 * Method:    jni_get
 * Signature: (JI)Ljava/lang/String;
 */
jstring JNICALL Java_com_madara_containers_StringVector_jni_1get
  (JNIEnv * env, jobject, jlong cptr, jint index)
{
  jstring result;

  containers::String_Vector * current = (containers::String_Vector *) cptr;
  if (current)
    result = env->NewStringUTF((*current)[index].c_str ());

  return result;
}

/*
 * Class:     com_madara_containers_StringVector
 * Method:    jni_toRecord
 * Signature: (JI)J
 */
jlong JNICALL Java_com_madara_containers_StringVector_jni_1toRecord__JI
  (JNIEnv * env, jobject, jlong cptr, jint index)
{
  Madara::Knowledge_Record * result (0);

  containers::String_Vector * current = (containers::String_Vector *) cptr;
  if (current)
    result = new Madara::Knowledge_Record (current->to_record (index));

  return (jlong) result;
}

