package com.madara.transport;

import com.madara.KnowledgeRecord;
import com.madara.MadaraVariables;

import java.util.List;

/**
 * User: jdroot
 * Date: 3/5/14
 * Time: 11:45 AM
 */
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
