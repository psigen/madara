/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
package com.madara.transport;

import com.madara.KnowledgeType;
import com.madara.MadaraFunction;

public class QoSTransportSettings extends TransportSettings
{
    private static native long jni_QoSTransportSettings();
    private static native long jni_QoSTransportSettings(long cptr);
    private static native void jni_freeQoSTransportSettings(long cptr);

    private static native void jni_addRebroadcastFilter(long cptr, int type, TransportFilter filter);
    private static native void jni_addRebroadcastFilter(long cptr, AggregateFilter filter);

    private static native void jni_addSendFilter(long cptr, int type, TransportFilter filter);
    private static native void jni_addSendFilter(long cptr, AggregateFilter filter);

    private static native void jni_addReceiveFilter(long cptr, int type, TransportFilter filter);
    private static native void jni_addReceiveFilter(long cptr, AggregateFilter filter);


    public QoSTransportSettings()
    {
        setCPtr(jni_QoSTransportSettings());
    }

    public QoSTransportSettings(QoSTransportSettings transportSettings)
    {
        setCPtr(jni_QoSTransportSettings(transportSettings.getCPtr()));
    }

    /**
     * Adds a filter that will be applied to certain types after receiving
     * and before rebroadcasting (if TTL > 0)
     * @param type the types to add the filter to
     * @param filter Madara callback function
     */
    public void addRebroadcastFilter(KnowledgeType type, TransportFilter filter)
    {
        jni_addRebroadcastFilter(getCPtr(), type.value(), filter);
    }

    /**
     * Adds an aggregate update filter that will be applied before
     * rebroadcasting, after individual record filters.
     * @param filter Madara callback function
     */
    public void addRebroadcastFilter(AggregateFilter filter)
    {
        jni_addRebroadcastFilter(getCPtr(), filter);
    }

    /**
     * Adds a filter that will be applied to certain types before sending
     * @param type the types to add the filter to
     * @param filter Madara callback function
     */
    public void addSendFilter(KnowledgeType type, TransportFilter filter)
    {
        jni_addSendFilter(getCPtr(), type.value(), filter);
    }

    /**
     * Adds an aggregate update filter that will be applied before sending,
     * after individual record filters.
     * @param filter Madara callback function
     */
    public void addSendFilter(AggregateFilter filter)
    {
        jni_addSendFilter(getCPtr(), filter);
    }

    /**
     * Adds an aggregate filter for a map of variables to values
     * before applying updates to the Knowledge_Base
     * @param type the types to add the filter to
     * @param filter Madara callback function
     */
    public void addReceiveFilter(KnowledgeType type, TransportFilter filter)
    {
        jni_addReceiveFilter(getCPtr(), type.value(), filter);
    }

    /**
     * Adds an aggregate update filter that will be applied after receiving,
     * after individual record filters.
     * @param filter Madara callback function
     */
    public void addReceiveFilter(AggregateFilter filter)
    {
        jni_addReceiveFilter(getCPtr(), filter);
    }

    /**
     * Deletes the C instantiation. To prevent memory leaks, this <b>must</b> be called
     * before an instance of WaitSettings gets garbage collected
     */
    public void free()
    {
        jni_freeQoSTransportSettings(getCPtr());
    }
}
