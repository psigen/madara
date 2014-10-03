/*********************************************************************
 * Usage of this software requires acceptance of the MADARA CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/madara/wiki/CMULicense
 *********************************************************************/
package com.madara.transport.filters;

import com.madara.KnowledgeList;
import com.madara.KnowledgeMap;
import com.madara.KnowledgeRecord;
import com.madara.Variables;
import com.madara.transport.TransportContext;

import java.util.Set;
import java.util.Map;

public class LogAggregate implements AggregateFilter
{		
  /**
   * Types of operations
   **/
  public String[] opTypes = {
    "IDLE_OPERATION",
    "SEND_OPERATION",
    "RECEIVE_OPERATION",
    "REBROADCAST_OPERATION"
  };
   
  /**
   * Logger of all information
   * @param map  a map of all variable names to values
   * @param context  information about current state of transport layer
   * @param variables facade for current knowledge base
   **/
  public KnowledgeRecord filter(KnowledgeMap map, TransportContext context, Variables variables)
  {
    StringBuffer buffer = new StringBuffer();
    buffer.append("Aggregate Filter args:\n");
    
    buffer.append("  Operation Type:");
   
    int index = (int) context.getOperation();
    if (index < opTypes.length)
    {
      buffer.append(opTypes[index]);
    }
    buffer.append("\n");
       
    buffer.append("  Send bandwidth:");
    buffer.append(context.getSendBandwidth());
    buffer.append(" B/s\n");
       
    buffer.append("  Recv bandwidth:");
    buffer.append(context.getReceiveBandwidth());
    buffer.append(" B/s\n");
       
    buffer.append("  Update Time:");
    buffer.append(context.getMessageTime());
    buffer.append("\n");
       
    buffer.append("  Current Time:");
    buffer.append(context.getCurrentTime());
    buffer.append("\n");
       
    buffer.append("  Domain:");
    buffer.append(context.getDomain());
    buffer.append("\n");
       
    buffer.append("  Updates:\n");
    
    for(Map.Entry<String, KnowledgeRecord> update : map.entrySet())
    {
      buffer.append("    ");
      buffer.append(update.getKey());
      buffer.append("=");
      buffer.append(update.getValue());
      buffer.append("\n");
    }
    
    buffer.append("\n");
       
    System.out.println(buffer.toString());
    
    return new KnowledgeRecord(0);
  }
}

