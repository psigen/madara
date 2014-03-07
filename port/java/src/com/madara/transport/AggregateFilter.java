/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
package com.madara.transport;

import com.madara.KnowledgeList;
import com.madara.KnowledgeMap;
import com.madara.KnowledgeRecord;
import com.madara.MadaraVariables;

public interface AggregateFilter
{
    /**
     * Java implementation of a MADARA Aggregate Filter. <b>DO NOT</b> invoke methods on an instance of
     * {@link com.madara.KnowledgeBase KnowledgeBase} in this method
     */
    public KnowledgeRecord filter(KnowledgeMap map, TransportContext context, MadaraVariables variables);
}
