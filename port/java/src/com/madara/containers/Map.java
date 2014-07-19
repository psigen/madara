/**
 * Copyright (c) 2013-2014 Carnegie Mellon University. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following acknowledgments and disclaimers.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 * 
 * 3. The names �Carnegie Mellon University,� "SEI� and/or �Software
 *  Engineering Institute" shall not be used to endorse or promote products
 *  derived from this software without prior written permission. For written
 *  permission, please contact permission@sei.cmu.edu.
 * 
 * 4. Products derived from this software may not be called "SEI" nor may "SEI"
 *  appear in their names without prior written permission of
 *  permission@sei.cmu.edu.
 * 
 * 5. Redistributions of any form whatsoever must retain the following
 *  acknowledgment:
 * 
 *    This material is based upon work funded and supported by the Department
 *    of Defense under Contract No. FA8721-05-C-0003 with Carnegie Mellon
 *    University for the operation of the Software Engineering Institute, a
 *    federally funded research and development center. Any opinions,
 *    findings and conclusions or recommendations expressed in this material
 *    are those of the author(s) and do not necessarily reflect the views of
 *    the United States Department of Defense.
 * 
 *    NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING
 *    INSTITUTE MATERIAL IS FURNISHED ON AN �AS-IS� BASIS. CARNEGIE MELLON
 *    UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR
 *    IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF
 *    FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS
 *    OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES
 *    NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT,
 *    TRADEMARK, OR COPYRIGHT INFRINGEMENT.
 * 
 *    This material has been approved for public release and unlimited
 *    distribution.
 **/
package com.madara.containers;

import com.madara.MadaraJNI;
import com.madara.KnowledgeBase;
import com.madara.Variables;
import com.madara.KnowledgeRecord;

public class Map extends MadaraJNI
{	
  private native long jni_Map();
  private native long jni_Map(long cptr);
  private static native void jni_freeMap(long cptr);
  private native void jni_set(long cptr,
    java.lang.String key, java.lang.String value);
  private native void jni_set(long cptr,
    java.lang.String key, double value);
  private native void jni_set(long cptr,
    java.lang.String key, long type, long value);
  private native java.lang.String jni_getName(long cptr);
  private native void jni_setName(long cptr,
    long type, long kb, java.lang.String name);
  private native long jni_get(long cptr, java.lang.String key);
  private native long jni_toRecord(long cptr, java.lang.String key);

  public Map()
  {
    setCPtr(jni_Map());
  }

  public Map(Map input)
  {
    setCPtr(jni_Map(input.getCPtr()));
  }

  /**
   * Gets the value
   *
   * @return   current value
   */
  public KnowledgeRecord get(java.lang.String key)
  {
    return KnowledgeRecord.fromPointer(jni_toRecord(getCPtr(), key));
  }

  /**
   * Gets the name of the variable
   *
   * @param  name of the variable within the context
   */
  public java.lang.String getName()
  {
    return jni_getName(getCPtr());
  }

  /**
   * Sets the value
   *
   * @param  value   new value
   */
  public void set(java.lang.String key, java.lang.String value)
  {
    jni_set(getCPtr(), key, value);
  }

  /**
   * Sets the value
   *
   * @param  value   new value
   */
  public void set(java.lang.String key, double value)
  {
    jni_set(getCPtr(), key, value);
  }

  /**
   * Sets the value
   *
   * @param  value   new value
   */
  public void set(java.lang.String key, long value)
  {
    jni_set(getCPtr(), key, 0, value);
  }

  /**
   * Sets the value
   *
   * @param  value   new value
   */
  public void set(java.lang.String key, KnowledgeRecord value)
  {
    jni_set(getCPtr(), key, 1, value.getCPtr ());
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
   * Returns a value at the specified index
   *
   * @param  index  the index 
   * @return the value at the index as a knowledge record
   */
  public KnowledgeRecord toRecord(java.lang.String key)
  {
    return KnowledgeRecord.fromPointer(jni_toRecord(getCPtr(), key));
  }

  /**
   * Deletes the C instantiation. To prevent memory leaks, this <b>must</b> be
   * called before an instance of WaitSettings gets garbage collected
   */
  public void free()
  {
    jni_freeMap(getCPtr());
  }
}

