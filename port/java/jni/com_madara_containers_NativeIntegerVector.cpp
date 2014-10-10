
#include "com_madara_containers_NativeIntegerVector.h"
#include "madara/knowledge_engine/containers/Native_Integer_Vector.h"

namespace engine = Madara::Knowledge_Engine;
namespace containers = engine::Containers;

/*
 * Class:     com_madara_containers_NativeIntegerVector
 * Method:    jni_NativeIntegerVector
 * Signature: ()J
 */
MADARA_Export jlong JNICALL Java_com_madara_containers_NativeIntegerVector_jni_1NativeIntegerVector__
  (JNIEnv * env, jobject)
{
  return (jlong) new containers::Native_Integer_Vector ();
}

/*
 * Class:     com_madara_containers_NativeIntegerVector
 * Method:    jni_NativeIntegerVector
 * Signature: (J)J
 */
MADARA_Export jlong JNICALL Java_com_madara_containers_NativeIntegerVector_jni_1NativeIntegerVector__J
  (JNIEnv * env, jobject, jlong cptr)
{
  return (jlong) new containers::Native_Integer_Vector (
    *(containers::Native_Integer_Vector *)cptr);
}

/*
 * Class:     com_madara_containers_NativeIntegerVector
 * Method:    jni_freeNativeIntegerVector
 * Signature: (J)V
 */
MADARA_Export void JNICALL Java_com_madara_containers_NativeIntegerVector_jni_1freeNativeIntegerVector
  (JNIEnv * env, jclass, jlong cptr)
{
  delete (containers::Native_Integer_Vector *) cptr;
}

/*
 * Class:     com_madara_containers_NativeIntegerVector
 * Method:    jni_set
 * Signature: (JIJ)V
 */
MADARA_Export void JNICALL Java_com_madara_containers_NativeIntegerVector_jni_1set
  (JNIEnv * env, jobject, jlong cptr, jint index, jdouble value)
{
  containers::Native_Integer_Vector * current = (containers::Native_Integer_Vector *) cptr;
  if (current)
    current->set (index, value);
}

/*
 * Class:     com_madara_containers_NativeIntegerVector
 * Method:    jni_getName
 * Signature: (J)Ljava/lang/String;
 */
MADARA_Export jstring JNICALL Java_com_madara_containers_NativeIntegerVector_jni_1getName
  (JNIEnv * env, jobject, jlong cptr)
{
  jstring result;

  containers::Native_Integer_Vector * current = (containers::Native_Integer_Vector *) cptr;
  if (current)
    result = env->NewStringUTF(current->get_name ().c_str ());

  return result;
}

/*
 * Class:     com_madara_containers_NativeIntegerVector
 * Method:    jni_setName
 * Signature: (JJJLjava/lang/String;)V
 */
MADARA_Export void JNICALL Java_com_madara_containers_NativeIntegerVector_jni_1setName
  (JNIEnv * env, jobject, jlong cptr, jlong type, jlong context, jstring name)
{
  containers::Native_Integer_Vector * current =
    (containers::Native_Integer_Vector *) cptr;

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
 * Class:     com_madara_containers_NativeIntegerVector
 * Method:    jni_get
 * Signature: (JI)J
 */
MADARA_Export jlong JNICALL Java_com_madara_containers_NativeIntegerVector_jni_1get
  (JNIEnv * env, jobject, jlong cptr, jint index)
{
  jlong result (0);

  containers::Native_Integer_Vector * current =
    (containers::Native_Integer_Vector *) cptr;
  if (current)
    result = (*current) [index];

  return result;
}

/*
 * Class:     com_madara_containers_NativeIntegerVector
 * Method:    jni_toRecord
 * Signature: (JI)J
 */
MADARA_Export jlong JNICALL Java_com_madara_containers_NativeIntegerVector_jni_1toRecord__JI
  (JNIEnv * env, jobject, jlong cptr, jint index)
{
  Madara::Knowledge_Record * result (0);

  containers::Native_Integer_Vector * current =
    (containers::Native_Integer_Vector *) cptr;
  if (current)
    result = new Madara::Knowledge_Record (current->to_record (index));

  return (jlong) result;
}

/*
 * Class:     com_madara_containers_NativeIntegerVector
 * Method:    jni_toRecord
 * Signature: (J)J
 */
MADARA_Export jlong JNICALL Java_com_madara_containers_NativeIntegerVector_jni_1toRecord__J
  (JNIEnv * env, jobject, jlong cptr)
{
  Madara::Knowledge_Record * result (0);

  containers::Native_Integer_Vector * current =
    (containers::Native_Integer_Vector *) cptr;
  if (current)
    result = new Madara::Knowledge_Record (current->to_record ());

  return (jlong) result;
}

/*
 * Class:     com_madara_containers_NativeIntegerVector
 * Method:    jni_toArray
 * Signature: (J)[Ljava/lang/Object;
 */
MADARA_Export jobjectArray JNICALL Java_com_madara_containers_NativeIntegerVector_jni_1toArray
  (JNIEnv * env, jobject, jlong cptr)
{
  jclass kr_class = env->FindClass ("com/madara/KnowledgeRecord");
  jobjectArray list;
  if (kr_class && cptr != 0)
  {
    jmethodID method = env->GetStaticMethodID (kr_class,
      "fromPointer", "(J)Lcom/madara/KnowledgeRecord;");
    Madara::Knowledge_Vector records;
    containers::Native_Integer_Vector * current =
      (containers::Native_Integer_Vector *) cptr;
    current->copy_to (records);
    jsize size = (jsize)records.size ();

    list = env->NewObjectArray ((jsize)records.size (), kr_class, 0);

    if (method)
    {
      for (jsize i = 0; i < size; ++i)
      {
        std::cout << "record[" << i << "] = " << records[i] << "\n";
        jobject result = env->CallStaticObjectMethod (
          kr_class, method, (jlong)records[i].clone ());
        env->SetObjectArrayElement (list, i, result);
      }
    }
  }
  return list;
}

/*
 * Class:     com_madara_containers_NativeIntegerVector
 * Method:    jni_size
 * Signature: (J)J
 */
MADARA_Export jlong JNICALL Java_com_madara_containers_NativeIntegerVector_jni_1size
  (JNIEnv * env, jobject, jlong cptr)
{
  jlong result (0);

  containers::Native_Integer_Vector * current =
    (containers::Native_Integer_Vector *) cptr;

  if (current)
    result = (jlong) current->size ();

  return (jlong) result;
}

/*
 * Class:     com_madara_containers_NativeIntegerVector
 * Method:    jni_resize
 * Signature: (JJ)V
 */
MADARA_Export void JNICALL Java_com_madara_containers_NativeIntegerVector_jni_1resize
  (JNIEnv * env, jobject, jlong cptr, jlong length)
{
  containers::Native_Integer_Vector * current =
    (containers::Native_Integer_Vector *) cptr;

  if (current)
    current->resize (length);
}

void JNICALL Java_com_madara_containers_NativeIntegerVector_jni_1modify
  (JNIEnv *, jobject, jlong cptr)
{
  containers::Native_Integer_Vector * current =
    (containers::Native_Integer_Vector *) cptr;

  if (current)
    current->modify ();
}
