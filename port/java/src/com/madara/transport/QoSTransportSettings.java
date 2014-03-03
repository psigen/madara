package com.madara.transport;

import com.madara.KnowledgeType;
import com.madara.MadaraFunction;

/**
 * User: jdroot
 * Date: 3/3/14
 * Time: 10:32 AM
 */
public class QoSTransportSettings extends TransportSettings
{
    private native long jni_QoSTransportSettings();
    private native long jni_QoSTransportSettings(long cptr);
    private static native void jni_freeQoSTransportSettings(long cptr);

    public QoSTransportSettings()
    {
        setCPtr(jni_QoSTransportSettings());
    }

    public QoSTransportSettings(QoSTransportSettings transportSettings)
    {
        setCPtr(jni_QoSTransportSettings(transportSettings.getCPtr()));
    }


    public void addRebroadcastFilter(KnowledgeType type, MadaraFunction function)
    {

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
