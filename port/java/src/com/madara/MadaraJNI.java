/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
package com.madara;


import com.madara.transport.AggregateFilter;
import com.madara.transport.TransportContext;
import com.madara.transport.TransportFilter;

/**
 * Abstract class that insures loading of libMADARA.so and holds the C pointer
 */
public abstract class MadaraJNI
{
    static
    {
        System.loadLibrary("ACE");
        System.loadLibrary("MADARA");
    }

    /**
     * C pointer to an object
     */
    private long cptr = 0;

    /**
     * Set the C pointer to the object
     *
     * @param cptr C Pointer
     */
    protected void setCPtr(long cptr)
    {
        this.cptr = cptr;
    }


    /**
     * @return The C pointer of this object for passing to JNI functions
     */
    public long getCPtr()
    {
        return cptr;
    }


    /**
     * @return &lt;ClassName&gt;[&lt;C Pointer&gt;]
     * @see java.lang.Object#toString ()
     */
    public String toString()
    {
        return getClass().getName() + "[" + cptr + "]";
    }

    private static long functionCallback(MadaraFunction func, long[] args, long vars)
    {
        MadaraVariables _vars = MadaraVariables.fromPointer(vars);
        KnowledgeList _args = new KnowledgeList(args);
        KnowledgeRecord ret = func.execute(_args, _vars);
        return ret == null ? 0 : ret.getCPtr();
    }

    private static long filterCallback(TransportFilter func, long[] args, long vars)
    {
        MadaraVariables _vars = MadaraVariables.fromPointer(vars);
        KnowledgeList _args = new KnowledgeList(args);
        KnowledgeRecord ret = func.filter(_args, _vars);
        return ret == null ? 0 : ret.getCPtr();
    }

    private static long aggregateFilterCallback(AggregateFilter func, String[] keys, long[] vals, long ctx, long vars)
    {
        KnowledgeMap map = new KnowledgeMap(keys, vals, false);
        TransportContext context = TransportContext.fromPointer(ctx);
        MadaraVariables variables = MadaraVariables.fromPointer(vars);

        KnowledgeRecord ret = func.filter(map, context, variables);
        if (ret != null && !ret.isNew())
            ret = ret.clone();
        return ret == null ? 0 : ret.getCPtr();
    }

}
