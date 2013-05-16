/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
package com.madara;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import com.madara.transport.Settings;
import com.madara.transport.TransportType;

/**
 * This class provides a distributed knowledge base to users
 */
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
	
	private native void jni_defineFunction(long cptr, String name);
	private native void jni_defineFunction(long cptr, String name, String expression);
	private native void jni_defineFunction(long cptr, String name, long expression);
	
	private native void jni_clear(long cptr);
	
	private native long jni_get(long cptr, String name);
	private native void jni_set(long cptr, String name, long record);
	
	private native long jni_wait(long cptr, String expression, long waitSettings);
	private native long jni_wait(long cptr, long expression, long waitSettings);
	private native void jni_waitNoReturn(long cptr, String expression, long waitSettings);
	private native void jni_waitNoReturn(long cptr, long expression, long waitSettings);
	
	private native void jni_freeKnowledgeBase(long cptr);
	
	private static HashMap<Long, KnowledgeBase> knowledgeBases = new HashMap<Long, KnowledgeBase>();
	private HashMap<String, MadaraFunction> callbacks = new HashMap<String, MadaraFunction>();
	
	
	/**
	 * Used to determine if we are inside the context of a DefinedFunction
	 */
	private final Object CONTEXT_LOCK = new Object();
	
	
	/**
	 * Creates a KnowledgeBase with default settings
	 */
	public KnowledgeBase()
	{
		setCPtr(jni_KnowledgeBase());
		knowledgeBases.put(getCPtr(), this);
	}
	
	
	/**
	 * Creates a KnowledgeBase
	 * @param hostname/ip of this machine
	 * @param transport to use for knowledge dissemination
	 * @param domain knowledge domain we want to join
	 */
	public KnowledgeBase(String host, TransportType transport, String domain)
	{
		setCPtr(jni_KnowledgeBase(host, transport.value(), domain));
		knowledgeBases.put(getCPtr(), this);
	}
	
	
	/**
	 * Creates a KnowledgeBase
	 * @param host hostname/ip of this machine
	 * @param config transport settings to use for dissemination
	 */
	public KnowledgeBase(String host, Settings config)
	{
		setCPtr(jni_KnowledgeBase(host, config.getCPtr()));
		knowledgeBases.put(getCPtr(), this);
	}
	
	
	/**
	 * Copy constructor
	 * @param original
	 */
	public KnowledgeBase(KnowledgeBase original)
	{
		setCPtr(jni_KnowledgeBase(original.getCPtr()));
		knowledgeBases.put(getCPtr(), this);
	}
	
	
	/**
	 * Evaluates an expression.
	 * <br/>The returned KnowledgeRecord <b>must</b> be freed ({@link com.madara.KnowledgeRecord#free() KnowledgeRecord.free()}) 
	 * at some point. If it is to be ignored, consider using {@link #evaluateNoReturn(String)}
	 * @param expression KaRL expression to evaluate
	 * @return value of expression
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public KnowledgeRecord evaluate(String expression) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		return evaluate(expression, EvalSettings.DEFAULT_EVAL_SETTINGS);
	}
	
	
	/**
	 * Evaluates an expression.
	 * @param expression KaRL expression to evaluate
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public void evaluateNoReturn(String expression) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		evaluateNoReturn(expression, EvalSettings.DEFAULT_EVAL_SETTINGS);
	}
	
	
	/**
	 * Evaluates an expression.
	 * The returned KnowledgeRecord <b>must</b> be freed ({@link com.madara.KnowledgeRecord#free() KnowledgeRecord.free()}) at some point. If
	 * it is to be ignored, consider using {@link #evaluateNoReturn(String,EvalSettings)}
	 * @param expression KaRL expression to evaluate
	 * @param evalSettings Settings for evaluating and printing
	 * @return value of expression
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public KnowledgeRecord evaluate(String expression, EvalSettings evalSettings) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		return KnowledgeRecord.fromPointer(jni_evaluate(getCPtr(), expression, evalSettings.getCPtr()));
	}
	
	
	/**
	 * Evaluates an expression.
	 * @param expression KaRL expression to evaluate
	 * @param evalSettings evalSettings Settings for evaluating and printing
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public void evaluateNoReturn(String expression, EvalSettings evalSettings) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		jni_evaluateNoReturn(getCPtr(), expression, evalSettings.getCPtr());
	}
	
	
	/**
	 * Evaluates an expression.
	 * The returned KnowledgeRecord <b>must</b> be freed ({@link com.madara.KnowledgeRecord#free() KnowledgeRecord.free()}) at some point. If
	 * it is to be ignored, consider using {@link #evaluateNoReturn(CompiledExpression)}
	 * @param expression KaRL expression to evaluate (result of {@link #compile(String)})
	 * @return value of expression
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public KnowledgeRecord evaluate(CompiledExpression expression) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		return evaluate(expression, EvalSettings.DEFAULT_EVAL_SETTINGS);
	}
	
	
	/**
	 * Evaluates an expression.
	 * @param expression KaRL expression to evaluate (result of {@link #compile(String)})
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public void evaluateNoReturn(CompiledExpression expression) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		evaluateNoReturn(expression, EvalSettings.DEFAULT_EVAL_SETTINGS);
	}
	
	
	/**
	 * Evaluates an expression.
	 * The returned KnowledgeRecord <b>must</b> be freed ({@link com.madara.KnowledgeRecord#free() KnowledgeRecord.free()}) at some point. If
	 * it is to be ignored, consider using {@link #evaluateNoReturn(CompiledExpression,EvalSettings)}
	 * @param expression KaRL expression to evaluate (result of {@link #compile(String)})
	 * @param evalSettings Settings for evaluating and printing
	 * @return value of expression
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public KnowledgeRecord evaluate(CompiledExpression expression, EvalSettings evalSettings) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		return KnowledgeRecord.fromPointer(jni_evaluate(getCPtr(), expression.getCPtr(), evalSettings.getCPtr()));
	}
	
	
	/**
	 * Evaluates an expression.
	 * @param expression KaRL expression to evaluate (result of {@link #compile(String)})
	 * @param evalSettings Settings for evaluating and printing
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public void evaluateNoReturn(CompiledExpression expression, EvalSettings evalSettings) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		jni_evaluateNoReturn(getCPtr(), expression.getCPtr(), evalSettings.getCPtr());
	}
	
	
	/**
	 * Compiles a KaRL expression into an expression tree.
	 * @param expression expression to compile
	 * @return {@link com.madara.KnowledgeBase.CompiledExpression CompiledExpression}: compiled, optimized expression tree
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public CompiledExpression compile(String expression) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		return new CompiledExpression(jni_compile(getCPtr(), expression));
	}
	
	
	/**
	 * Defines a function.
	 * @param name name of the function
	 * @param function Implementation of MadaraFunction
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public void defineFunction(String name, MadaraFunction function) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		callbacks.put(name, function);
		jni_defineFunction(getCPtr(), name);
	}
	
	
	/**
	 * Defines a MADARA KaRL function.
	 * @param name name of the function
	 * @param expression KaRL function body
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public void defineFunction(String name, String expression) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		jni_defineFunction(getCPtr(), name, expression);
	}
	
	
	/**
	 *  Defines a MADARA KaRL function.
	 * @param name name of the function
	 * @param expression KaRL function body
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public void defineFunction(String name, CompiledExpression expression) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		jni_defineFunction(getCPtr(), name, expression.getCPtr());
	}
	
	
	/**
	 * Clears the knowledge base.
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public void clear() throws KnowledgeBaseLockedException
	{
		checkContextLock();
		jni_clear(getCPtr());
	}
	
	
	/**
	 * Retrieves a knowledge value.
	 * @param name knowledge name
	 * @return value of knowledge
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public KnowledgeRecord get(String name) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		return KnowledgeRecord.fromPointer(jni_get(getCPtr(), name));
	}
	
	
	/**
	 * Sets a knowledge value to a specified value.
	 * @param name knowledge name
	 * @param record value to set
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public void set(String name, KnowledgeRecord record) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		jni_set(getCPtr(), name, record.getCPtr());
	}
	
	
	/**
	 * Sets a knowledge value to a specified value.
	 * @param name knowledge name
	 * @param value value to set
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public void set(String name, long value) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		KnowledgeRecord kr = new KnowledgeRecord(value);
		set(name, kr);
		kr.free();
	}
	
	/**
	 * Sets a knowledge value to a specified value.
	 * @param name knowledge name
	 * @param value value to set
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public void set(String name, double value) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		KnowledgeRecord kr = new KnowledgeRecord(value);
		set(name, kr);
		kr.free();
	}
	
	/**
	 * Sets a knowledge value to a specified value.
	 * @param name knowledge name
	 * @param value value to set
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public void set(String name, String value) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		KnowledgeRecord kr = new KnowledgeRecord(value);
		set(name, kr);
		kr.free();
	}
	
	/**
	 * Deletes the C++ instantiation. To prevent memory leaks, this <b>must</b> be called
	 * before an instance of KnowledgeBase gets garbage collected
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public void free() throws KnowledgeBaseLockedException
	{
		checkContextLock();
		jni_freeKnowledgeBase(getCPtr());
	}
	
	
	/**
	 * Waits for an expression to be non-zero. 
	 * <br/><br/>The returned KnowledgeRecord <b>must</b> be freed ({@link com.madara.KnowledgeRecord#free() KnowledgeRecord.free()}) 
	 * at some point. If it is to be ignored, consider using {@link #waitNoReturn(String)}
	 * @param expression KaRL expression to evaluate
	 * @return value of expression
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public KnowledgeRecord wait(String expression) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		return wait(expression, WaitSettings.DEFAULT_WAIT_SETTINGS);
	}
	
	
	/**
	 * Waits for an expression to be non-zero.
	 * <br/><br/>The returned KnowledgeRecord <b>must</b> be freed ({@link com.madara.KnowledgeRecord#free() KnowledgeRecord.free()}) 
	 * at some point. If it is to be ignored, consider using {@link #waitNoReturn(CompiledExpression)}
	 * @param expression KaRL expression to evaluate (result of {@link #compile(String)})
	 * @return value of expression
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public KnowledgeRecord wait(CompiledExpression expression) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		return wait(expression, WaitSettings.DEFAULT_WAIT_SETTINGS);
	}
	
	
	/**
	 * Waits for an expression to be non-zero. 
	 * <br/>Provides additional settings for fine-tuning the time to wait and atomic print statements.
	 * <br/><br/>The returned KnowledgeRecord <b>must</b> be freed ({@link com.madara.KnowledgeRecord#free() KnowledgeRecord.free()}) 
	 * at some point. If it is to be ignored, consider using {@link #waitNoReturn(String,WaitSettings)}
	 * @param expression KaRL expression to evaluate
	 * @param settings Settings for waiting
	 * @return value of expression
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public KnowledgeRecord wait(String expression, WaitSettings settings) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		return KnowledgeRecord.fromPointer(jni_wait(getCPtr(), expression, settings.getCPtr()));
	}
	
	/**
	 * Waits for an expression to be non-zero. 
	 * <br/>Provides additional settings for fine-tuning the time to wait and atomic print statements.
	 * <br/><br/>The returned KnowledgeRecord <b>must</b> be freed ({@link com.madara.KnowledgeRecord#free() KnowledgeRecord.free()}) 
	 * at some point. If it is to be ignored, consider using {@link #waitNoReturn(CompiledExpression,WaitSettings)}
	 * @param expression KaRL expression to evaluate (result of {@link #compile(String)})
	 * @param settings Settings for waiting
	 * @return value of expression
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public KnowledgeRecord wait(CompiledExpression expression, WaitSettings settings) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		return new KnowledgeRecord(jni_wait(getCPtr(), expression.getCPtr(), settings.getCPtr()));
	}
	
	
	/**
	 * Waits for an expression to be non-zero. 
	 * @param expression KaRL expression to evaluate
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public void waitNoReturn(String expression) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		waitNoReturn(expression, WaitSettings.DEFAULT_WAIT_SETTINGS);
	}
	
	
	/**
	 * Waits for an expression to be non-zero. 
	 * @param expression KaRL expression to evaluate (result of {@link #compile(String)})
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public void waitNoReturn(CompiledExpression expression) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		waitNoReturn(expression, WaitSettings.DEFAULT_WAIT_SETTINGS);
	}
	
	
	/**
	 * Waits for an expression to be non-zero. 
	 * @param expression KaRL expression to evaluate
	 * @param settings Settings for waiting
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public void waitNoReturn(String expression, WaitSettings settings) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		jni_waitNoReturn(getCPtr(), expression, settings.getCPtr());
	}
	
	
	/**
	 * Waits for an expression to be non-zero. 
	 * @param expression KaRL expression to evaluate (result of {@link #compile(String)})
	 * @param settings Settings for waiting
	 * @throws KnowledgeBaseLockedException If called from a DefinedFunction
	 */
	public void waitNoReturn(CompiledExpression expression, WaitSettings settings) throws KnowledgeBaseLockedException
	{
		checkContextLock();
		jni_waitNoReturn(getCPtr(), expression.getCPtr(), settings.getCPtr());
	}
	
	/**
	 * Checks the CONTEXT_LOCK to see if the current thread is allowed
	 * to invoke member methods. Access will be denied if the a MadaraFunction
	 * is in in stack trace.
	 * @see {@link com.madara.MadaraFunction MadaraFunction}
	 * @throws AccessNotAllowedException
	 */
	private void checkContextLock() throws KnowledgeBaseLockedException
	{
		if (Thread.holdsLock(CONTEXT_LOCK))
		{
			throw new KnowledgeBaseLockedException();
		}
	}
	
	private static long callBack(String name, long ptr, long[] args, long vars)
	{	
		KnowledgeBase knowledge = knowledgeBases.get(ptr);
		MadaraFunction mf = knowledge.callbacks.get(name);
		
		MadaraVariables _vars = MadaraVariables.fromPointer(vars);
		List<KnowledgeRecord> _args = new ArrayList<KnowledgeRecord>();
		for (long arg : args)
			_args.add(KnowledgeRecord.fromPointer(arg));
		
		KnowledgeRecord ret = null;

		synchronized (knowledge.CONTEXT_LOCK)
		{
			 ret = mf.execute(_args, _vars);
		}
		
		return ret == null ? 0 : ret.getCPtr();	
	}
	
	
	/**
	 * Compiled, optimized KaRL logic.
	 */
	public static class CompiledExpression extends MadaraJNI
	{
		
		private native void jni_freeCompiledExpression(long cptr);
		
		/**
		 * Create a Java representation of a compiled expression
		 * @param cptr pointer to a C++ instance of CompiledExpression
		 */
		public CompiledExpression(long cptr)
		{
			setCPtr(cptr);
		}
		
		/**
		 * Deletes the C++ instantiation. To prevent memory leaks, this <b>must</b> be called
		 * before an instance of KnowledgeBase gets garbage collected
		 */
		public void free()
		{
			jni_freeCompiledExpression(getCPtr());
		}
	}
	
	public class KnowledgeBaseLockedException extends Exception
	{
		private static final long serialVersionUID = 2818595961365992639L;

		public KnowledgeBaseLockedException()
		{
			super(KnowledgeBase.this + " is currently locked. Are you invoking it from inside a DefinedFunction?");
		}
	}
	
}
