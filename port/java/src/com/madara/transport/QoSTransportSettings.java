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
    private native long jni_QoSTransportSettings();
    private native long jni_QoSTransportSettings(long cptr);
    private static native void jni_freeQoSTransportSettings(long cptr);

    private native void jni_addRebroadcastFilter(long cptr, int type, TransportFilter filter);
    private native void jni_addRebroadcastFilter(long cptr, AggregateFilter filter);

    private native void jni_addSendFilter(long cptr, int type, TransportFilter filter);
    private native void jni_addSendFilter(long cptr, AggregateFilter filter);

    private native void jni_addReceiveFilter(long cptr, int type, TransportFilter filter);
    private native void jni_addReceiveFilter(long cptr, AggregateFilter filter);

    private native void jni_setRebroadcastTtl(long cptr, int ttl);
    private native int jni_getRebroadcastTtl(long cptr);

    private native void jni_enableParticipantTtl(long cptr, int ttl);
    private native int jni_getParticipantTtl(long cptr);

    private native void jni_setSendBandwidthLimit(long cptr, int limit);
    private native int jni_getSendBandwidthLimit(long cptr);

    private native void jni_setTotalBandwidthLimit(long cptr, int limit);
    private native int jni_getTotalBandwidthLimit(long cptr);

    private native void jni_setDeadline(long cptr, int deadline);
    private native int jni_getDeadline(long cptr);

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
     * Sets the rebroadcast time-to-live for all sent packets
     * @param ttl the time-to-live
     */
    public void setRebroadcastTtl(int ttl)
    {
        jni_setRebroadcastTtl(getCPtr(), ttl);
    }

    /**
     * Gets the rebroadcast time-to-live for all sent packets
     * @return the rebroadcast time-to-live
     */
    public int getRebroadcastTtl()
    {
        return jni_getRebroadcastTtl(getCPtr());
    }
 
    /**
     * Enables participation in rebroadcasts up to a certain ttl value
     * @param ttl the time-to-live
     */
    public void enableParticipantTtl(int ttl)
    {
        jni_enableParticipantTtl(getCPtr(), ttl);
    }

    /**
     * Gets the rebroadcast time-to-live for all rebroadcasted packets
     * @return the rebroadcast time-to-live
     */
    public int getParticipantTtl()
    {
        return jni_getParticipantTtl(getCPtr());
    }

    /**
     * Sets the send bandwidth limit
     * @param limit  the bandwidth limit for sending packets
     */
    public void setSendBandwidthLimit(int limit)
    {
        jni_setSendBandwidthLimit(getCPtr(), limit);
    }

    /**
     * Gets the send bandwidth limit
     * @return the bandwidth limit for sending packets
     */
    public int getSendBandwidthLimit()
    {
        return jni_getSendBandwidthLimit(getCPtr());
    }

    /**
     * Sets the total bandwidth limit
     * @param limit  the bandwidth limit for all packets
     */
    public void setTotalBandwidthLimit(int limit)
    {
        jni_setTotalBandwidthLimit(getCPtr(), limit);
    }

    /**
     * Gets the total bandwidth limit
     * @return the bandwidth limit for all packets
     */
    public int getTotalBandwidthLimit()
    {
        return jni_getTotalBandwidthLimit(getCPtr());
    }

    /**
     * Sets the deadline (seconds)
     * @param deadline  the maximum lifetime for all packets in seconds
     */
    public void setDeadline(int deadline)
    {
        jni_setDeadline(getCPtr(), deadline);
    }

    /**
     * Gets the deadline (seconds)
     * @return the maximum lifetime for all packets in seconds
     */
    public int getDeadline()
    {
        return jni_getDeadline(getCPtr());
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

