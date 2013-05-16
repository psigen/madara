/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
package com.madara;


/**
 * Holder for static method to set global debug level.
 * Debug extends MadaraJNI to insure the library is loaded
 * prior to invocation of {@link #setDebugLevel(DebugLevel) setDebugLevel}
 */
public class Debug extends MadaraJNI
{
	private static native void jni_setDebugLevel(int debug);
	
	/**
	 * Sets the global debug level
	 * @param debug {@link com.madara.Debug.DebugLevel DebugLevel} to set
	 */
	public static void setDebugLevel(DebugLevel debug)
	{
		jni_setDebugLevel(debug.value());
	}
	
	/**
	 * private constructor because we do not need any thing but static here
	 */
	private Debug()
	{
		
	}

	/**
	 * Enumeration of valid debug levels
	 */
	public static enum DebugLevel
	{
		MADARA_LOG_EMERGENCY(0),
		MADARA_LOG_TERMINAL_ERROR(1),
		MADARA_LOG_NONFATAL_ERROR(2),
		MADARA_LOG_ERROR(3),
		MADARA_LOG_WARNING(4),
		MADARA_LOG_MAJOR_EVENT(5),
		MADARA_LOG_MINOR_EVENT(6),
		MADARA_LOG_EVENT_TRACE(7),
		MADARA_LOG_MAJOR_DEBUG_INFO(8),
		MADARA_LOG_TRACE(9),
		MADARA_LOG_DETAILED_TRACE(10);
		
		private int num;
		private DebugLevel(int num)
		{
			this.num = num;
		}
		
		
		/**
		 * @return int value of this {@link com.madara.Debug.DebugLevel DebugLevel}
		 */
		public int value()
		{
			return num;
		}
		
		
		/**
		 * Converts an int to a {@link com.madara.Debug.DebugLevel DebugLevel}
		 * @param val value to convert
		 * @return {@link com.madara.Debug.DebugLevel DebugLevel} or null if the int is invalid
		 */
		public static DebugLevel getDebugLevel(int val)
		{
			for (DebugLevel t : values())
			{
				if (t.value() == val)
					return t;
			}
			return null;
		}
	}
}
