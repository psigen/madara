/*********************************************************************
 * Usage of this software requires acceptance of the MADARA CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/madara/wiki/CMULicense
 *********************************************************************/

package com.madara.tests;

import com.madara.KnowledgeBase;
import com.madara.KnowledgeRecord;
import com.madara.containers.Vector;

/**
 * This class is a tester for the com.madara.containers package
 */
public class ContainerTest
{
  public static void testVector ()
  {
    KnowledgeBase knowledge = new KnowledgeBase ();
    Vector list = new Vector ();
    
    System.out.println ("Testing Vector");
    
    list.setName (knowledge, "device.sensors");
    
    list.resize (4);
    
    System.out.println ("  Setting elements");
    
    list.set (0, "first element");
    list.set (1, "second element");
    list.set (2, "third element");
    list.set (3, "fourth element");
    
    System.out.println ("0: " + list.get (0).toString ());
    System.out.println ("1: " + list.get (1).toString ());
    System.out.println ("2: " + list.get (2).toString ());
    System.out.println ("3: " + list.get (3).toString ());
    
    System.out.println ("  Shipping elements to an array");
    
    KnowledgeRecord[] my_array = list.toArray ();
    
    System.out.println ("  Printing " + my_array.length + " array elements");
    
    for (int i = 0; i < my_array.length; ++i)
    {
      String result = "    ";
      result += i;
      result += ": ";
      result += my_array[i].toString ();
      
      System.out.println (result);
    }
  }
  
  public static void main (String...args) throws Exception
  {
    testVector ();
  }
}
