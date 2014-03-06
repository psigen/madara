package com.madara.transport;

import com.madara.KnowledgeList;
import com.madara.KnowledgeMap;
import com.madara.KnowledgeRecord;
import com.madara.MadaraVariables;

/**
 * User: jdroot
 * Date: 3/5/14
 * Time: 11:42 AM
 */
public interface AggregateFilter
{
    /**
     * Java implementation of a MADARA Aggregate Filter. <b>DO NOT</b> invoke methods on an instance of
     * {@link com.madara.KnowledgeBase KnowledgeBase} in this method
     */
    public KnowledgeRecord filter(KnowledgeMap map, TransportContext context, MadaraVariables variables);
}
