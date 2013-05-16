/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
package com.madara;

/**
 * Encapsulates settings for a wait statement.
 */
public class WaitSettings extends EvalSettings
{

	/**
	 * 
	 */
	public static final WaitSettings DEFAULT_WAIT_SETTINGS = getDefaultWaitSettings(0);
	
	
	private static native long jni_getDefaultWaitSettings(int which);
	private static WaitSettings getDefaultWaitSettings(int which)
	{
		return new WaitSettings(jni_getDefaultWaitSettings(which));
	}
	
	//Constructors
	private native long jni_waitSettings();
	private native long jni_waitSettings(long oldPtr);
	
	//Setters/Getters
	private native void jni_setPollFrequency(long cptr, double freq);
	private native double jni_getPollFrequency(long cptr);
	
	private native void jni_setMaxWaitTime(long cptr, double maxWaitTime);
	private native double jni_getMaxWaitTime(long cptr);
	
	
	/**
	 * Default constructor
	 */
	public WaitSettings()
	{
		setCPtr(jni_waitSettings());
	}
	
	/**
	 * Copy constructor
	 * @param waitSettings
	 */
	public WaitSettings(WaitSettings waitSettings)
	{
		setCPtr(jni_waitSettings(waitSettings.getCPtr()));
	}
	
	
	/**
	 * C pointer constructor
	 * @param cptr
	 */
	protected WaitSettings(long cptr)
	{
		super(cptr);
	}
	
	
	/**
	 * @param freq Frequency to poll an expression for truth. 
	 */
	public void setPollFrequency(double freq)
	{
		if (!constant)
			jni_setPollFrequency(getCPtr(), freq);
	}
	
	
	/**
	 * @return current poll frequency
	 */
	public double getPollFrequency()
	{
		return jni_getPollFrequency(getCPtr());
	}
	
	
	/**
	 * @param maxWaitTime Maximum time to wait for an expression to become true. 
	 */
	public void setMaxWaitTime(double maxWaitTime)
	{
		if (!constant)
			jni_setMaxWaitTime(getCPtr(), maxWaitTime);
	}
	
	
	/**
	 * @return current max wait time
	 */
	public double getMaxWaitTime()
	{
		return jni_getMaxWaitTime(getCPtr());
	}
	
}
