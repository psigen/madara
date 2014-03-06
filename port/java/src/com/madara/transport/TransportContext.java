package com.madara.transport;

import com.madara.KnowledgeMap;
import com.madara.KnowledgeRecord;
import com.madara.MadaraJNI;

/**
 * User: jdroot
 * Date: 3/5/14
 * Time: 11:43 AM
 */
public class TransportContext extends MadaraJNI
{
    private TransportContext(long cptr)
    {
        setCPtr(cptr);
    }

    public static TransportContext fromPointer(long cptr)
    {
        return new TransportContext(cptr);
    }

    /**
     * Adds a record to the list that should be appended to send or rebroadcast.
     * @param key key of the record to add
     * @param record the record to add
     */
    public void addRecord(String key, KnowledgeRecord record)
    {

    }

    /**
     * Clears records added through filtering operations.
     */
    public void clearRecords()
    {

    }

    /**
     * Gets the current timestamp
     * @return the current time stamp
     */
    public long getCurrentTime()
    {
        return 0;
    }

    /**
     * Gets the current network domain
     * @return the network domain
     */
    public String getDomain()
    {
        return "";
    }

    /**
     * Gets the message timestamp
     * @return the message timestamp
     */
    public long getMessageTime()
    {
        return 0;
    }

    /**
     * Get operation that the context is performing
     * @return
     */
    public long getOperation()
    {
        return 0;
    }

    /**
     * Returns the current message originator
     * @return the current message originator
     */
    public String getOriginator()
    {
        return "";
    }

    /**
     * Gets the receive bandwidth in bytes per second
     * @return the receive bandwidth in bytes per second
     */
    public long getReceiveBandwidth()
    {
        return 0;
    }

    /**
     * Returns the additional records stored in the context
     * @return the additional records stored in the context
     */
    public KnowledgeMap getRecords()
    {
        return null;
    }

    /**
     * Gets the send/rebroadcast bandwidth in bytes per second
     * @return the send/rebroadcast bandwidth in bytes per second
     */
    public long getSendBandwidth()
    {
        return 0;
    }
}
