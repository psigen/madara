/*********************************************************************
 * Usage of this software requires acceptance of the MADARA CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/madara/wiki/CMULicense
 *********************************************************************/
package com.madara;

import java.util.HashMap;

import com.madara.transport.TransportSettings;
import com.madara.transport.TransportType;

/**
 * This class provides a distributed knowledge base to users
 **/
public class KnowledgeBase extends MadaraJNI
{
  //Native Methods
  private native long jni_KnowledgeBase();
  private native long jni_KnowledgeBase(String host, int transport, String domain);
  private native long jni_KnowledgeBase(String host, long config);
  private native long jni_KnowledgeBase(long original);
  private native long jni_evaluate(long cptr, String expression, long evalSettings);
  private native long jni_evaluate(long cptr, long expression, long evalSettings);
  private native void jni_evaluateNoReturn(long cptr, String expression, long evalSettings);
  private native void jni_evaluateNoReturn(long cptr, long expression, long evalSettings);
  private native long jni_compile(long cptr, String expression);
  private native void jni_defineFunction(long cptr, String name, MadaraFunction function);
  private native void jni_defineFunction(long cptr, String name, String expression);
  private native void jni_print(long cptr, String statement);
  private native void jni_print(long cptr);
  private native void jni_clear(long cptr);
  private native boolean jni_exists(long cptr, String statement);
  private native void jni_sendModifieds(long cptr);
  private native void jni_sendModifieds(long cptr, long evalSettings);
  private native long jni_get(long cptr, String name);
  private static native void jni_setInteger(long cptr, String name, long value);
  private static native void jni_setDouble(long cptr, String name, double value);
  private static native void jni_setString(long cptr, String name, String value);
  private static native void jni_setIntegerArray(long cptr, String name, long[] value);
  private static native void jni_setDoubleArray(long cptr, String name, double[] value);
  private static native void jni_setIntegerSettings(long cptr, String name, long value, long settings);
  private static native void jni_setDoubleSettings(long cptr, String name, double value, long settings);
  private static native void jni_setStringSettings(long cptr, String name, String value, long settings);
  private static native void jni_setIntegerArraySettings(long cptr, String name, long[] value, long settings);
  private static native void jni_setDoubleArraySettings(long cptr, String name, double[] value, long settings);
  private native void jni_freeKnowledgeBase(long cptr);
  private native long jni_wait(long cptr, String expression, long waitSettings);
  private native long jni_wait(long cptr, long expression, long waitSettings);
  private native void jni_waitNoReturn(long cptr, String expression, long waitSettings);
  private native void jni_waitNoReturn(long cptr, long expression, long waitSettings);
  private native long[] jni_toKnowledgeList(long cptr, String subject, int start, int end);
  private native void jni_toKnowledgeMap(long cptr, String expression, MapReturn ret);

  private static HashMap<Long, KnowledgeBase> knowledgeBases = new HashMap<Long, KnowledgeBase>();
  private HashMap<String, MadaraFunction> callbacks = new HashMap<String, MadaraFunction>();

  private boolean manageMemory = true;

  /**
   * Used to determine if we are inside the context of a MadaraFunction
   **/
  private final Object CONTEXT_LOCK = new Object();

  /**
   * Creates a KnowledgeBase with default settings
   **/
  public KnowledgeBase()
  {
    setCPtr(jni_KnowledgeBase());
    knowledgeBases.put(getCPtr(), this);
  }

  /**
   * Creates a KnowledgeBase
   *
   * @param host    hostname/ip of this machine
   * @param transport to use for knowledge dissemination
   * @param domain  knowledge domain we want to join
   **/
  public KnowledgeBase(String host, TransportType transport, String domain)
  {
    setCPtr(jni_KnowledgeBase(host, transport.value(), domain));
    knowledgeBases.put(getCPtr(), this);
  }

  /**
   * Creates a KnowledgeBase
   *
   * @param host   hostname/ip of this machine
   * @param config transport settings to use for dissemination
   **/
  public KnowledgeBase(String host, TransportSettings config)
  {
    setCPtr(jni_KnowledgeBase(host, config.getCPtr()));
    knowledgeBases.put(getCPtr(), this);
  }

  /**
   * Copy constructor
   *
   * @param original knowledge base to copy from
   **/
  public KnowledgeBase(KnowledgeBase original)
  {
    setCPtr(jni_KnowledgeBase(original.getCPtr()));
    knowledgeBases.put(getCPtr(), this);
  }

  /**
   * Creates a java object instance from a C/C++ pointer
   *
   * @param cptr C pointer to the object
   * @return a new java instance of the underlying pointer
   **/
  public static KnowledgeBase fromPointer(long cptr)
  {
    KnowledgeBase ret = new KnowledgeBase();
    ret.manageMemory = true;
    ret.setCPtr(cptr);
    return ret;
  }

  /**
   * Creates a java object instance from a C/C++ pointer
   *
   * @param cptr C pointer to the object
   * @param shouldManage  if true, manage the pointer
   * @return a new java instance of the underlying pointer
   **/
  public static KnowledgeBase fromPointer(long cptr, boolean shouldManage)
  {
    KnowledgeBase ret = new KnowledgeBase();
    ret.manageMemory=shouldManage;
    ret.setCPtr(cptr);
    return ret;
  }

  /**
   * Evaluates an expression.
   * <br>The returned KnowledgeRecord <b>must</b> be freed  ({@link com.madara.KnowledgeRecord#free () KnowledgeRecord.free ()})
   * at some point. If it is to be ignored, consider using {@link #evaluateNoReturn (String)}
   *
   * @param expression KaRL expression to evaluate
   * @return value of expression
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public KnowledgeRecord evaluate(String expression)
  {
    return evaluate(expression, EvalSettings.DEFAULT_EVAL_SETTINGS);
  }

  /**
   * Evaluates an expression.
   *
   * @param expression KaRL expression to evaluate
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void evaluateNoReturn(String expression)
  {
    evaluateNoReturn(expression, EvalSettings.DEFAULT_EVAL_SETTINGS);
  }

  /**
   * Evaluates an expression.
   * The returned KnowledgeRecord <b>must</b> be freed  ({@link com.madara.KnowledgeRecord#free () KnowledgeRecord.free ()}) at some point. If
   * it is to be ignored, consider using {@link #evaluateNoReturn (String,EvalSettings)}
   *
   * @param expression   KaRL expression to evaluate
   * @param evalSettings Settings for evaluating and printing
   * @return value of expression
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public KnowledgeRecord evaluate(String expression, EvalSettings evalSettings)
  {
    checkContextLock();
    return KnowledgeRecord.fromPointer(jni_evaluate(getCPtr(), expression, evalSettings.getCPtr()));
  }

  /**
   * Evaluates an expression.
   *
   * @param expression   KaRL expression to evaluate
   * @param evalSettings evalSettings Settings for evaluating and printing
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void evaluateNoReturn(String expression, EvalSettings evalSettings)
  {
    checkContextLock();
    jni_evaluateNoReturn(getCPtr(), expression, evalSettings.getCPtr());
  }

  /**
   * Evaluates an expression.
   * The returned KnowledgeRecord <b>must</b> be freed  ({@link com.madara.KnowledgeRecord#free () KnowledgeRecord.free ()}) at some point. If
   * it is to be ignored, consider using {@link #evaluateNoReturn (CompiledExpression)}
   *
   * @param expression KaRL expression to evaluate  (result of {@link #compile (String)})
   * @return value of expression
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public KnowledgeRecord evaluate(CompiledExpression expression)
  {
    return evaluate(expression, EvalSettings.DEFAULT_EVAL_SETTINGS);
  }

  /**
   * Evaluates an expression.
   *
   * @param expression KaRL expression to evaluate  (result of {@link #compile (String)})
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void evaluateNoReturn(CompiledExpression expression)
  {
    evaluateNoReturn(expression, EvalSettings.DEFAULT_EVAL_SETTINGS);
  }

  /**
   * Evaluates an expression.
   * The returned KnowledgeRecord <b>must</b> be freed  ({@link com.madara.KnowledgeRecord#free () KnowledgeRecord.free ()}) at some point. If
   * it is to be ignored, consider using {@link #evaluateNoReturn (CompiledExpression,EvalSettings)}
   *
   * @param expression   KaRL expression to evaluate  (result of {@link #compile (String)})
   * @param evalSettings Settings for evaluating and printing
   * @return value of expression
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public KnowledgeRecord evaluate(CompiledExpression expression, EvalSettings evalSettings)
  {
    checkContextLock();
    return KnowledgeRecord.fromPointer(jni_evaluate(getCPtr(), expression.getCPtr(), evalSettings.getCPtr()));
  }

  /**
   * Evaluates an expression.
   *
   * @param expression   KaRL expression to evaluate  (result of {@link #compile (String)})
   * @param evalSettings Settings for evaluating and printing
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void evaluateNoReturn(CompiledExpression expression, EvalSettings evalSettings)
  {
    checkContextLock();
    jni_evaluateNoReturn(getCPtr(), expression.getCPtr(), evalSettings.getCPtr());
  }

  /**
   * Compiles a KaRL expression into an expression tree.
   *
   * @param expression expression to compile
   * @return {@link com.madara.KnowledgeBase.CompiledExpression CompiledExpression}: compiled, optimized expression tree
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public CompiledExpression compile(String expression)
  {
    checkContextLock();
    return new CompiledExpression(jni_compile(getCPtr(), expression));
  }

  /**
   * <b>Currently unsupported</b><br>
   * Defines a function.
   *
   * @param name   name of the function
   * @param function Implementation of MadaraFunction
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void defineFunction(String name, MadaraFunction function)
  {
    checkContextLock();
//    callbacks.put(name, function);
    jni_defineFunction(getCPtr(), name, function);
  }

  /**
   * Defines a MADARA KaRL function.
   *
   * @param name     name of the function
   * @param expression KaRL function body
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void defineFunction(String name, String expression)
  {
    checkContextLock();
    jni_defineFunction(getCPtr(), name, expression);
  }

  /**
   * Clears the knowledge base.
   *
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void clear()
  {
    checkContextLock();
    jni_clear(getCPtr());
  }

  /**
   * Checks to see if record exists
   * @param name name of record to check
   * @return true if the record exists, false otherwise
   **/
  public boolean exists(String name)
  {
    return jni_exists(getCPtr(), name);
  }

  /**
   * Retrieves a knowledge value.
   *
   * @param name knowledge name
   * @return value of knowledge
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public KnowledgeRecord get(String name)
  {
    checkContextLock();
    return KnowledgeRecord.fromPointer(jni_get(getCPtr(), name));
  }

  /**
   * Sets a knowledge value to a specified value.
   *
   * @param name  knowledge name
   * @param value value to set
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void set(String name, long value)
  {
    checkContextLock();
    jni_setInteger(getCPtr(), name, value);
  }

  /**
   * Sets a knowledge value to a specified value.
   *
   * @param name  knowledge name
   * @param value value to set
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void set(String name, double value)
  {
    checkContextLock();
    jni_setDouble(getCPtr(), name, value);
  }

  /**
   * Sets a knowledge value to a specified value.
   *
   * @param name  knowledge name
   * @param value value to set
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void set(String name, String value)
  {
    checkContextLock();
    jni_setString(getCPtr(), name, value);
  }

  /**
   * Sets a knowledge value to a specified value.
   *
   * @param name  knowledge name
   * @param value value to set
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void set(String name, double[] value)
  {
    checkContextLock();
    jni_setDoubleArray(getCPtr(), name, value);
  }

  /**
   * Sets a knowledge value to a specified value.
   *
   * @param name  knowledge name
   * @param value value to set
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void set(String name, long[] value)
  {
    checkContextLock();
    jni_setIntegerArray(getCPtr(), name, value);
  }

  
  
  /**
   * Sets a knowledge value to a specified value.
   *
   * @param name  knowledge name
   * @param value value to set
   * @param settings settings for evaluating the set command
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void set(String name, long value, EvalSettings settings)
  {
    checkContextLock();
    jni_setIntegerSettings(getCPtr(), name, value, settings.getCPtr());
  }

  /**
   * Sets a knowledge value to a specified value.
   *
   * @param name  knowledge name
   * @param value value to set
   * @param settings settings for evaluating the set command
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void set(String name, double value, EvalSettings settings)
  {
    checkContextLock();
    jni_setDoubleSettings(getCPtr(), name, value, settings.getCPtr());
  }

  /**
   * Sets a knowledge value to a specified value.
   *
   * @param name  knowledge name
   * @param value value to set
   * @param settings settings for evaluating the set command
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void set(String name, String value, EvalSettings settings)
  {
    checkContextLock();
    jni_setStringSettings(getCPtr(), name, value, settings.getCPtr());
  }

  /**
   * Sets a knowledge value to a specified value.
   *
   * @param name  knowledge name
   * @param value value to set
   * @param settings settings for evaluating the set command
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void set(String name, double[] value, EvalSettings settings)
  {
    checkContextLock();
    jni_setDoubleArraySettings(getCPtr(), name, value, settings.getCPtr());
  }

  /**
   * Sets a knowledge value to a specified value.
   *
   * @param name  knowledge name
   * @param value value to set
   * @param settings settings for evaluating the set command
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void set(String name, long[] value, EvalSettings settings)
  {
    checkContextLock();
    jni_setIntegerArraySettings(getCPtr(), name, value, settings.getCPtr());
  }

  
  /**
   * Sends all modifications to global variables since last send modifieds,
   * eval, wait, or set statement.
   **/
  public void sendModifieds ()
  {
    checkContextLock();
    jni_sendModifieds(getCPtr());
  }
  
  /**
   * Sends all modifications to global variables since last send modifieds,
   * eval, wait, or set statement.
   * @param settings   settings to use for considering records to send
   **/
  public void sendModifieds (EvalSettings settings)
  {
    checkContextLock();
    jni_sendModifieds(getCPtr(), settings.getCPtr());
  }

  /**
   * Prints a statement with variable expansion
   *
   * @param statement The statement to print
   **/
  public void print(String statement)
  {
    jni_print(getCPtr(), statement);
  }
  
  /**
   * Prints all knowledge in knowledge base
   **/
  public void print()
  {
    jni_print(getCPtr());
  }
  
  /**
   * Deletes the C++ instantiation. To prevent memory leaks, this <b>must</b> be called
   * before an instance of KnowledgeBase gets garbage collected
   *
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void free()
  {
    checkContextLock();
    if (manageMemory)
    {
      jni_freeKnowledgeBase(getCPtr());
    }
  }

  /**
   * Waits for an expression to be non-zero.
   * <br><br>The returned KnowledgeRecord <b>must</b> be freed  ({@link com.madara.KnowledgeRecord#free () KnowledgeRecord.free ()})
   * at some point. If it is to be ignored, consider using {@link #waitNoReturn (String)}
   *
   * @param expression KaRL expression to evaluate
   * @return value of expression
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public KnowledgeRecord wait(String expression)
  {
    return wait(expression, WaitSettings.DEFAULT_WAIT_SETTINGS);
  }

  /**
   * Waits for an expression to be non-zero.
   * <br><br>The returned KnowledgeRecord <b>must</b> be freed  ({@link com.madara.KnowledgeRecord#free () KnowledgeRecord.free ()})
   * at some point. If it is to be ignored, consider using {@link #waitNoReturn (CompiledExpression)}
   *
   * @param expression KaRL expression to evaluate  (result of {@link #compile (String)})
   * @return value of expression
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public KnowledgeRecord wait(CompiledExpression expression)
  {
    return wait(expression, WaitSettings.DEFAULT_WAIT_SETTINGS);
  }

  /**
   * Waits for an expression to be non-zero.
   * <br>Provides additional settings for fine-tuning the time to wait and atomic print statements.
   * <br><br>The returned KnowledgeRecord <b>must</b> be freed  ({@link com.madara.KnowledgeRecord#free () KnowledgeRecord.free ()})
   * at some point. If it is to be ignored, consider using {@link #waitNoReturn (String,WaitSettings)}
   *
   * @param expression KaRL expression to evaluate
   * @param settings   Settings for waiting
   * @return value of expression
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public KnowledgeRecord wait(String expression, WaitSettings settings)
  {
    checkContextLock();
    return KnowledgeRecord.fromPointer(jni_wait(getCPtr(), expression, settings.getCPtr()));
  }

  /**
   * Waits for an expression to be non-zero.
   * <br>Provides additional settings for fine-tuning the time to wait and atomic print statements.
   * <br><br>The returned KnowledgeRecord <b>must</b> be freed  ({@link com.madara.KnowledgeRecord#free () KnowledgeRecord.free ()})
   * at some point. If it is to be ignored, consider using {@link #waitNoReturn (CompiledExpression,WaitSettings)}
   *
   * @param expression KaRL expression to evaluate  (result of {@link #compile (String)})
   * @param settings   Settings for waiting
   * @return value of expression
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public KnowledgeRecord wait(CompiledExpression expression, WaitSettings settings)
  {
    checkContextLock();
    return new KnowledgeRecord(jni_wait(getCPtr(), expression.getCPtr(), settings.getCPtr()));
  }

  /**
   * Waits for an expression to be non-zero.
   *
   * @param expression KaRL expression to evaluate
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void waitNoReturn(String expression)
  {
    waitNoReturn(expression, WaitSettings.DEFAULT_WAIT_SETTINGS);
  }

  /**
   * Waits for an expression to be non-zero.
   *
   * @param expression KaRL expression to evaluate  (result of {@link #compile (String)})
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void waitNoReturn(CompiledExpression expression)
  {
    waitNoReturn(expression, WaitSettings.DEFAULT_WAIT_SETTINGS);
  }

  /**
   * Waits for an expression to be non-zero.
   *
   * @param expression KaRL expression to evaluate
   * @param settings   Settings for waiting
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void waitNoReturn(String expression, WaitSettings settings)
  {
    checkContextLock();
    jni_waitNoReturn(getCPtr(), expression, settings.getCPtr());
  }

  /**
   * Waits for an expression to be non-zero.
   *
   * @param expression KaRL expression to evaluate  (result of {@link #compile (String)})
   * @param settings   Settings for waiting
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public void waitNoReturn(CompiledExpression expression, WaitSettings settings)
  {
    checkContextLock();
    jni_waitNoReturn(getCPtr(), expression.getCPtr(), settings.getCPtr());
  }

  /**
   * Fills a {@link com.madara.KnowledgeList KnowledgeList} with
   * {@link com.madara.KnowledgeRecord KnowledgeRecords} that begin with
   * a common subject and have a finite range of integer values.
   * <br><br>The returned {@link com.madara.KnowledgeList KnowledgeList}
   * <b>must</b> be freed using {@link com.madara.KnowledgeList#free KnowledgeList.free ()}
   * before the object goes out of scope.
   *
   * @param subject The common subject of the variable names. For instance,
   *        if we are looking for a range of vars like "var0", "var1",
   *        "var2", then the common subject would be "var".
   * @param start   An inclusive start index
   * @param end   An inclusive end index
   * @return {@link com.madara.KnowledgeList KnowledgeList} containing the
   * requested {@link com.madara.KnowledgeRecord KnowledgeRecords}
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public KnowledgeList toKnowledgeList(String subject, int start, int end)
  {
    checkContextLock();
    return new KnowledgeList(jni_toKnowledgeList(getCPtr(), subject, start, end));
  }

  /**
   * Fills a {@link com.madara.KnowledgeMap KnowledgeMap} with
   * {@link com.madara.KnowledgeRecord KnowledgeRecords} that match an expression.
   * <br><br>The returned {@link com.madara.KnowledgeMap KnowledgeMap}
   * <b>must</b> be freed using {@link com.madara.KnowledgeMap#free KnowledgeMap.free ()}
   * before the object goes out of scope.
   *
   * @param expression An expression that matches the variable names that
   *           are of interest. Wildcards may only be at the end
   * @return {@link com.madara.KnowledgeMap KnowledgeMap} containing the
   * requested {@link com.madara.KnowledgeRecord KnowledgeRecords}
   * @throws KnowledgeBaseLockedException If called from a MadaraFunction
   **/
  public KnowledgeMap toKnowledgeMap(String expression)
  {
    checkContextLock();
    MapReturn jniRet = new MapReturn();
    jni_toKnowledgeMap(getCPtr(), expression, jniRet);

    return new KnowledgeMap(jniRet.keys, jniRet.vals);
  }

  /**
   * Sets the log level to dictate the detail of MADARA logging.
   *
   * @param logLevel The log level to set
   **/
  public static void setLogLevel(MadaraLog.MadaraLogLevel logLevel)
  {
    MadaraLog.setLogLevel(logLevel);
  }

  /**
   * Checks the CONTEXT_LOCK to see if the current thread is allowed
   * to invoke member methods. Access will be denied if the a {@link com.madara.MadaraFunction MadaraFunction}
   * is in in stack trace.
   *
   * @throws KnowledgeBaseLockedException
   * @see MadaraFunction
   **/
  private void checkContextLock()
  {
    if (Thread.holdsLock(CONTEXT_LOCK))
    {
      throw new KnowledgeBaseLockedException();
    }
  }

  /**
   * callBack is called directly from JNI to allow execution of Java code inside a MADARA defined function
   *
   * @param name The name of the defined function
   * @param ptr  Pointer to the KnowledgeBase this function was defined in
   * @param args Pointers to the KnowledgeRecords passed in to the function
   * @param vars Pointer to the Variables object to allow execution of evaluate statements
   * @return Pointer to a KnowledgeRecord
   **/
  private static long callBack(String name, long ptr, long[] args, long vars)
  {
    KnowledgeBase knowledge = knowledgeBases.get(ptr);
    MadaraFunction callback = knowledge.callbacks.get(name);

    Variables _vars = Variables.fromPointer(vars);
    KnowledgeList _args = new KnowledgeList(args);

    KnowledgeRecord ret = null;

    synchronized (knowledge.CONTEXT_LOCK)
    {
      ret = callback.execute(_args, _vars);
    }

    return ret == null ? 0 : ret.getCPtr();
  }

  /**
   * Compiled, optimized KaRL logic.
   **/
  public static class CompiledExpression extends MadaraJNI
  {

    private native void jni_freeCompiledExpression(long cptr);

    /**
     * Create a Java representation of a compiled expression
     *
     * @param cptr pointer to a C++ instance of CompiledExpression
     **/
    public CompiledExpression(long cptr)
    {
      setCPtr(cptr);
    }

    /**
     * Deletes the C++ instantiation. To prevent memory leaks, this <b>must</b> be called
     * before an instance of KnowledgeBase gets garbage collected
     **/
    public void free()
    {
      jni_freeCompiledExpression(getCPtr());
    }
  }

  /**
   * KnowledgeBaseLockedException will be thrown if methods are called from inside a MadaraFunction
   **/
  public class KnowledgeBaseLockedException extends RuntimeException
  {
    private static final long serialVersionUID = 2818595961365992639L;

    private KnowledgeBaseLockedException()
    {
      super(KnowledgeBase.this + " is currently locked. Are you invoking it from inside a MadaraFunction?");
    }
  }

  static class MapReturn
  {
    public String[] keys;
    public long[] vals;
  }
}

