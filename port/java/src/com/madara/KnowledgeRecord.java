/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
package com.madara;


/**
 * This class encapsulates an entry in a KnowledgeBase.
 */
public class KnowledgeRecord extends MadaraJNI
{
	//Constructors
	private native long jni_KnowledgeRecord(String str);
	private native long jni_KnowledgeRecord(double str);
	private native long jni_KnowledgeRecord(long str);
	
	//Getters
	private native long jni_toLongValue(long cptr);
	private native String jni_toStringValue(long cptr);
	private native double jni_toDoubleValue(long cptr);
	
	private native int jni_getType(long cptr);
	
	//Free
	private native void jni_freeKnowledgeRecord(long cptr);
	
	
	/**
	 * No reason to ever create this without a pointer
	 */
	private KnowledgeRecord()
	{
		
	}
	
	
	/**
	 * Constructor for long/integer values
	 * @param lng value to set
	 */
	public KnowledgeRecord(long lng)
	{
		setCPtr(jni_KnowledgeRecord(lng));
	}
	
	
	/**
	 * Constructor for string values
	 * @param str value to set
	 */
	public KnowledgeRecord(String str)
	{
		setCPtr(jni_KnowledgeRecord(str));
	}
	
	
	/**
	 * Constructor for double values
	 * @param dbl value to set
	 */
	public KnowledgeRecord(double dbl)
	{
		setCPtr(jni_KnowledgeRecord(dbl));
	}
	
	
	/**
	 * Converts the value to a long
	 * @return current long value
	 */
	public long toLongValue()
	{
		return jni_toLongValue(getCPtr());
	}
	
	
	/**
	 * Converts the value to a float/double 
	 * @return current double value
	 */
	public double toDoubleValue()
	{
		return jni_toDoubleValue(getCPtr());
	}
	
	
	/**
	 * Converts the value to a String
	 * @return current string value
	 */
	public String toStringValue()
	{
		return jni_toStringValue(getCPtr());
	}
	
	
	/**
	 * @return the {@link com.madara.KnowledgeType KnowledgeType} of the value
	 */
	public KnowledgeType getType()
	{
		return KnowledgeType.getType(jni_getType(getCPtr()));
	}
	
	/**
	 * Deletes the C instantiation. To prevent memory leaks, this <b>must</b> be called
	 * before an instance of KnowledgeRecord gets garbage collected
	 */
	public void free()
	{
		jni_freeKnowledgeRecord(getCPtr());
	}
	
	public String toString()
	{
		String ret = null;
		switch (getType())
		{
			case INTEGER:
				ret = "" + toLongValue(); break;
			case DOUBLE:
				ret = "" + toDoubleValue(); break;
			case STRING:
				ret = toStringValue(); break;
			default:
				ret = "Unknown"; break;
		}
		return ret;
	}
	
	/**
	 * Creates a {@link com.madara.KnowledgeRecord KnowledgeRecord} from a pointer
	 * @param cptr C pointer to a KnowledgeRecord object
	 * @return new KnowledgeRecord
	 */
	public static KnowledgeRecord fromPointer(long cptr)
	{
		KnowledgeRecord ret = new KnowledgeRecord();
		ret.setCPtr(cptr);
		return ret;
	}
}