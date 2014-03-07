/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
package com.madara.transport;

import com.madara.KnowledgeRecord;
import com.madara.MadaraVariables;

import java.util.List;

public interface TransportFilter
{
    /**
     * Java implementation of a MADARA filter.
     * @param args      {@link java.util.List List&lt;KnowledgeRecord&gt;} of arguments passed to the function
     * @param variables Local access to evaluate and compile methods
     * @return A {@link com.madara.KnowledgeRecord KnowledgeRecord} containing the result of the function
     */
    public KnowledgeRecord filter(List<KnowledgeRecord> args, MadaraVariables variables);
}
