
#include "com_madara_containers_Vector.h"
#include "madara/knowledge_engine/containers/Vector.h"
#include <iostream>

namespace engine = Madara::Knowledge_Engine;
namespace containers = engine::Containers;

/*
 * Class:     com_madara_containers_Vector
 * Method:    jni_Vector
 * Signature: ()J
 */
jlong JNICALL Java_com_madara_containers_Vector_jni_1Vector__
  (JNIEnv * env, jobject)
{
  return (jlong) new containers::Vector ();
}

/*
 * Class:     com_madara_containers_Vector
 * Method:    jni_Vector
 * Signature: (J)J
 */
jlong JNICALL Java_com_madara_containers_Vector_jni_1Vector__J
  (JNIEnv * env, jobject, jlong cptr)
{
  return (jlong) new containers::Vector (*(containers::Vector *)cptr);
}

/*
 * Class:     com_madara_containers_Vector
 * Method:    jni_freeVector
 * Signature: (J)V
 */
void JNICALL Java_com_madara_containers_Vector_jni_1freeVector
  (JNIEnv * env, jclass, jlong cptr)
{
  delete (containers::Vector *) cptr;
}

/*
 * Class:     com_madara_containers_Vector
 * Method:    jni_set
 * Signature: (JILjava/lang/String;)V
 */
MADARA_Export void JNICALL Java_com_madara_containers_Vector_jni_1set__JILjava_lang_String_2
  (JNIEnv * env, jobject, jlong cptr, jint index, jstring value)
{
  containers::Vector * current = (containers::Vector *) cptr;
  if (current)
  {
    const char * str_value = env->GetStringUTFChars(value, 0);

    current->set (index, str_value);

    env->ReleaseStringUTFChars(value, str_value);
  }
}

/*
 * Class:     com_madara_containers_Vector
 * Method:    jni_set
 * Signature: (JID)V
 */
MADARA_Export void JNICALL Java_com_madara_containers_Vector_jni_1set__JID
  (JNIEnv * env, jobject, jlong cptr, jint index, jdouble value)
{
  containers::Vector * current = (containers::Vector *) cptr;
  if (current)
  {
    current->set (index, value);
  }
}

/*
 * Class:     com_madara_containers_Vector
 * Method:    jni_set
 * Signature: (JIJJ)V
 */
MADARA_Export void JNICALL Java_com_madara_containers_Vector_jni_1set__JIJJ
  (JNIEnv * env, jobject, jlong cptr, jint index, jlong type, jlong value)
{
  containers::Vector * current = (containers::Vector *) cptr;
  if (current)
  {
    if (type == 0)
    {
      current->set (index, (Madara::Knowledge_Record::Integer)value);
    }
    else
    {
      Madara::Knowledge_Record * record = (Madara::Knowledge_Record *) cptr;
      
      if (record->type () == Madara::Knowledge_Record::DOUBLE)
      {
        current->set (index, record->to_double ());
      }
      else if (record->type () == Madara::Knowledge_Record::DOUBLE_ARRAY)
      {
        current->set (index, record->to_doubles ());
      }
      else if (record->type () == Madara::Knowledge_Record::INTEGER)
      {
        current->set (index, record->to_integer ());
      }
      else if (record->type () == Madara::Knowledge_Record::INTEGER_ARRAY)
      {
        current->set (index, record->to_integers ());
      }
      else if (record->is_binary_file_type ())
      {
        size_t size;
        unsigned char * buffer = record->to_unmanaged_buffer (size);
        current->set_file (index, buffer, size);
        delete [] buffer;
      }
      else if (record->type () == Madara::Knowledge_Record::STRING)
      {
        current->set (index, record->to_string ());
      }
      else if (record->is_string_type ())
      {
        current->set (index, record->to_string ());
      }
    }
  }
}

/*
 * Class:     com_madara_containers_Vector
 * Method:    jni_getName
 * Signature: (J)Ljava/lang/String;
 */
jstring JNICALL Java_com_madara_containers_Vector_jni_1getName
  (JNIEnv * env, jobject, jlong cptr)
{
  jstring result;

  containers::Vector * current = (containers::Vector *) cptr;
  if (current)
    result = env->NewStringUTF(current->get_name ().c_str ());

  return result;
}

/*
 * Class:     com_madara_containers_Vector
 * Method:    jni_setName
 * Signature: (JJJLjava/lang/String;)V
 */
void JNICALL Java_com_madara_containers_Vector_jni_1setName
  (JNIEnv * env, jobject, jlong cptr, jlong type, jlong context, jstring name)
{
  containers::Vector * current = (containers::Vector *) cptr;

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
 * Class:     com_madara_containers_Vector
 * Method:    jni_get
 * Signature: (JI)J
 */
jlong JNICALL Java_com_madara_containers_Vector_jni_1get
  (JNIEnv * env, jobject, jlong cptr, jint index)
{
  Madara::Knowledge_Record * result (0);

  containers::Vector * current = (containers::Vector *) cptr;
  if (current)
    result = new Madara::Knowledge_Record (current->to_record (index));

  return (jlong) result;
}

/*
 * Class:     com_madara_containers_Vector
 * Method:    jni_resize
 * Signature: (JJ)V
 */
MADARA_Export void JNICALL Java_com_madara_containers_Vector_jni_1resize
  (JNIEnv * env, jobject, jlong cptr, jlong length)
{
  containers::Vector * current = (containers::Vector *) cptr;
  if (current)
    current->resize (length);
}

/*
 * Class:     com_madara_containers_Vector
 * Method:    jni_size
 * Signature: (J)J
 */
jlong JNICALL Java_com_madara_containers_Vector_jni_1size
  (JNIEnv * env, jobject, jlong cptr)
{
  jlong result (0);

  containers::Vector * current = (containers::Vector *) cptr;
  if (current)
    result = (jlong) current->size ();

  return (jlong) result;
}

/*
 * Class:     com_madara_containers_Vector
 * Method:    jni_toRecord
 * Signature: (JI)J
 */
jlong JNICALL Java_com_madara_containers_Vector_jni_1toRecord
  (JNIEnv * env, jobject, jlong cptr, jint index)
{
  Madara::Knowledge_Record * result (0);

  containers::Vector * current = (containers::Vector *) cptr;
  if (current)
    result = new Madara::Knowledge_Record (current->to_record (index));

  return (jlong) result;
}

/*
 * Class:     com_madara_containers_Vector
 * Method:    jni_toArray
 * Signature: (JI)[Ljava/lang/Object;
 */
jobjectArray JNICALL Java_com_madara_containers_Vector_jni_1toArray
  (JNIEnv * env, jobject, jlong cptr)
{
  jclass kr_class = env->FindClass ("com/madara/KnowledgeRecord");
  jobjectArray list;
  if (kr_class && cptr != 0)
  {
    jmethodID method = env->GetStaticMethodID (kr_class,
      "fromPointer", "(J)Lcom/madara/KnowledgeRecord;");
    Madara::Knowledge_Vector records;
    containers::Vector * current = (containers::Vector *) cptr;
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
