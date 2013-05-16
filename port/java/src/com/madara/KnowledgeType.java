/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
package com.madara;


/**
 * Type of {@link com.madara.KnowledgeRecord KnowledgeRecord}
 */
public enum KnowledgeType
{
	INTEGER (0),
	STRING (1),
	DOUBLE (2),
	UNKNOWN (3),
	XML(4),
	TEXT_FILE (5),
	IMAGE_JPEG (50);
	
	private int num;
	private KnowledgeType(int num)
	{
		this.num = num;
	}
	
	/**
	 * @return int value of this {@link com.madara.KnowledgeType KnowledgeType}
	 */
	public int value()
	{
		return num;
	}
	
	/**
	 * Converts an int to a {@link com.madara.KnowledgeType KnowledgeType}
	 * @param val value to convert
	 * @return {@link com.madara.KnowledgeType KnowledgeType} or null if the int is invalid
	 */
	public static KnowledgeType getType(int val)
	{
		for (KnowledgeType t : values())
		{
			if (t.value() == val)
				return t;
		}
		return null;
	}
}
