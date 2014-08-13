/*********************************************************************
 * Usage of this software requires acceptance of the MADARA CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/madara/wiki/CMULicense
 *********************************************************************/
package com.madara.transport;

import com.madara.KnowledgeRecord;
import com.madara.Variables;

import java.util.List;

public interface TransportFilter
{
  /**
   * Java implementation of a MADARA filter.
   * @param args    {@link java.util.List List&lt;KnowledgeRecord&gt;} of arguments passed to the function
   * @param variables Local access to evaluate and compile methods
   * @return A {@link com.madara.KnowledgeRecord KnowledgeRecord} containing the result of the function
   */
  public KnowledgeRecord filter(List<KnowledgeRecord> args, Variables variables);
}

