/*********************************************************************
 * Usage of this software requires acceptance of the MADARA CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/madara/wiki/CMULicense
 *********************************************************************/
package com.madara.containers;

import java.util.Arrays;
import com.madara.MadaraJNI;
import com.madara.KnowledgeBase;
import com.madara.Variables;
import com.madara.KnowledgeRecord;

/**
 * A facade for a dynamically typed vector within a knowledge base.
 **/

public class Vector extends MadaraJNI
{	
  private native long jni_Vector();
  private native long jni_Vector(long cptr);
  private static native void jni_freeVector(long cptr);
  private native void jni_set(long cptr, int index, java.lang.String value);
  private native void jni_set(long cptr, int index, double value);
  private native void jni_set(long cptr, int index, long type, long value);
  private native java.lang.String jni_getName(long cptr);
  private native void jni_setName(long cptr, long type, long kb, java.lang.String name);
  private native long jni_get(long cptr, int index);
  private native long jni_toRecord(long cptr, int index);
  private native Object[] jni_toArray(long cptr);
  private native long jni_size(long cptr);
  private native void jni_resize(long cptr, long length);
  private native void jni_modify(long cptr);
  private native void jni_modifyIndex(long cptr, int index);

  private boolean manageMemory = true;
  
  /**
   * Default constructor
   **/
  public Vector()
  {
    setCPtr(jni_Vector());
  }

  /**
   * Copy constructor
   * @param input  instance to copy
   **/
  public Vector(Vector input)
  {
    setCPtr(jni_Vector(input.getCPtr()));
  }

  /**
   * Creates a java object instance from a C/C++ pointer
   *
   * @param cptr C pointer to the object
   * @return a new java instance of the underlying pointer
   */
  public static Vector fromPointer(long cptr)
  {
    Vector ret = new Vector();
    ret.manageMemory = true;
    ret.setCPtr(cptr);
    return ret;
  }

  /**
   * Creates a java object instance from a C/C++ pointer
   *
   * @param cptr C pointer to the object
   * @param shouldManage  if true, manage the pointer
   * @return a new java instance of the underlying pointer
   */
  public static Vector fromPointer(long cptr, boolean shouldManage)
  {
    Vector ret = new Vector();
    ret.manageMemory=shouldManage;
    ret.setCPtr(cptr);
    return ret;
  }

  /**
   * Gets the value
   *
   * @param  index   index for the new value
   * @return   current value
   */
  public KnowledgeRecord get(int index)
  {
    return KnowledgeRecord.fromPointer(jni_toRecord(getCPtr(), index));
  }

  /**
   * Gets the name of the variable
   *
   * @return  name of the variable within the context
   */
  public java.lang.String getName()
  {
    return jni_getName(getCPtr());
  }

  /**
   * Resizes the vector
   *
   * @param  length   new number of elements of the vector
   */
  public void resize (long length)
  {
    jni_resize(getCPtr(), length);
  }
  
  /**
   * Sets the value
   *
   * @param  index   index for the new value
   * @param  value   new value
   */
  public void set(int index, java.lang.String value)
  {
    jni_set(getCPtr(), index, value);
  }

  /**
   * Sets the value
   *
   * @param  index   index for the new value
   * @param  value   new value
   */
  public void set(int index, double value)
  {
    jni_set(getCPtr(), index, value);
  }

  /**
   * Sets the value
   *
   * @param  index   index for the new value
   * @param  value   new value
   */
  public void set(int index, long value)
  {
    jni_set(getCPtr(), index, 0, value);
  }

  /**
   * Sets the value
   *
   * @param  index   index for the new value
   * @param  value   new value
   */
  public void set(int index, KnowledgeRecord value)
  {
    jni_set(getCPtr(), index, 1, value.getCPtr ());
  }

  /**
   * Mark the vector as modified. The vector retains the same values
   * but will resend all values as if they had been modified.
   **/
  public void modify()
  {
    jni_modify(getCPtr());
  }
  
  /**
   * Mark an element as modified. The element retains the same value
   * but will resend the value as if it had been modified.
   * @param index  the element index
   **/
  public void modify(int index)
  {
    jni_modifyIndex(getCPtr(), index);
  }
  
  /**
   * Sets the name and knowledge base being referred to
   *
   * @param  kb      the knowledge base that contains the name
   * @param  name    the variable name
   */
  public void setName(KnowledgeBase kb, java.lang.String name)
  {
    jni_setName(getCPtr(), 0, kb.getCPtr (), name);
  }

  /**
   * Sets the name and knowledge base being referred to
   *
   * @param  vars    the variables facade that contains the name
   * @param  name    the variable name
   */
  public void setName(Variables vars, java.lang.String name)
  {
    jni_setName(getCPtr(), 1, vars.getCPtr (), name);
  }

  /**
   * Returns the size of the vector
   *
   * @return  the number of elements in the vector
   */
  public long size ()
  {
    return jni_size(getCPtr());
  }
  
  /**
   * Returns a value at the specified index
   *
   * @param  index  the index 
   * @return the value at the index as a knowledge record
   */
  public KnowledgeRecord toRecord(int index)
  {
    return KnowledgeRecord.fromPointer(jni_toRecord(getCPtr(), index));
  }

  /**
   * Returns a value at the specified index
   *
   * @return the vector as an array of records
   */
  public com.madara.KnowledgeRecord[] toArray()
  {
    Object[] objs = jni_toArray(getCPtr());
    KnowledgeRecord[] records = new KnowledgeRecord[objs.length];
    for (int i = 0; i < objs.length; ++i)
    {
      records[i] = (KnowledgeRecord)objs[i];
    }
    return records;
  }

  /**
   * Deletes the C instantiation. To prevent memory leaks, this <b>must</b> be
   * called before an instance gets garbage collected
   */
  public void free()
  {
    if (manageMemory)
    {
      jni_freeVector(getCPtr());
    }
  }
}

