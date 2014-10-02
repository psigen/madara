/*********************************************************************
 * Usage of this software requires acceptance of the MADARA CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/madara/wiki/CMULicense
 *********************************************************************/
package com.madara.transport;

import com.madara.KnowledgeList;
import com.madara.KnowledgeMap;
import com.madara.KnowledgeRecord;
import com.madara.Variables;

public interface AggregateFilter
{
  /**
   * Java implementation of a MADARA Aggregate Filter. <b>DO NOT</b> invoke methods on an instance of
   * {@link com.madara.KnowledgeBase KnowledgeBase} in this method
   * @param map  a map of all variable names to values
   * @param context  information about current state of transport layer
   * @param variables facade for current knowledge base
   **/
  public KnowledgeRecord filter(KnowledgeMap map, TransportContext context, Variables variables);
}

