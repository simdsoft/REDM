// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DMTaskT.h
// File mark:   
// File summary:支持构造最多7个输入参数，5个输出参数的函数
//              1.使用DMNewRunnableFunction构造普通函数，DMNewRunnableMethod构造成员函数:DMNewRunnableFunction(&FUN, arg0, arg1, arg2)，DMNewRunnableMethod(&CLS::FUN,CLS::THIS,arg0, arg1, arg2)
//              2.注意构造DMNewRunnableMethod时，使用的是IDMCancelableTask, 默认类支持DMRefNum,类在运行前AddRef，运行后ReleaseRef,可以通过声明DMDISABLE_RUNNABLE_METHOD_REFCOUNT(类名)取消
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2019-2-28
// ----------------------------------------------------------------
#pragma once

namespace DM
{
	class IDMTask : public DMRefNum
	{
	public:
		virtual ~IDMTask(){}
		virtual void Run() = 0;
	};

	class IDMCancelableTask : public IDMTask
	{
	public:
		virtual void Cancel() = 0;	///< 不是所有的任务都支持取消操作.
	};
	typedef IDMCancelableTask*			IDMTaskPtr;
	///---------------------------------------------------------------------------------------------------------------------------
	template <typename T>
	struct DMTupleTraits 
	{
		typedef T ValueType;
		typedef T& RefType;
		typedef const T& ParamType;
	};

	template <typename T>
	struct DMTupleTraits<T&> 
	{
		typedef T ValueType;
		typedef T& RefType;
		typedef T& ParamType;
	};

	struct DMTuple0 
	{
		typedef DMTuple0 ValueTuple;
		typedef DMTuple0 RefTuple;
	};

	template <typename A>
	struct DMTuple1 
	{
	public:
		typedef A TypeA;
		typedef DMTuple1<typename DMTupleTraits<A>::ValueType> ValueTuple;
		typedef DMTuple1<typename DMTupleTraits<A>::RefType> RefTuple;

		DMTuple1() {}
		explicit DMTuple1(typename DMTupleTraits<A>::ParamType a) 
			: a(a) 
		{
		}

		A a;
	};

	template <typename A, typename B>
	struct DMTuple2 
	{
	public:
		typedef A TypeA;
		typedef B TypeB;
		typedef DMTuple2<typename DMTupleTraits<A>::ValueType,
			typename DMTupleTraits<B>::ValueType> ValueTuple;
		typedef DMTuple2<typename DMTupleTraits<A>::RefType,
			typename DMTupleTraits<B>::RefType> RefTuple;

		DMTuple2() {}
		DMTuple2(typename DMTupleTraits<A>::ParamType a,
			typename DMTupleTraits<B>::ParamType b)
			: a(a), b(b) 
		{
		}

		A a;
		B b;
	};

	template <typename A, typename B, typename C>
	struct DMTuple3 
	{
	public:
		typedef A TypeA;
		typedef B TypeB;
		typedef C TypeC;
		typedef DMTuple3<typename DMTupleTraits<A>::ValueType,
			typename DMTupleTraits<B>::ValueType,
			typename DMTupleTraits<C>::ValueType> ValueTuple;
		typedef DMTuple3<typename DMTupleTraits<A>::RefType,
			typename DMTupleTraits<B>::RefType,
			typename DMTupleTraits<C>::RefType> RefTuple;

		DMTuple3() {}
		DMTuple3(typename DMTupleTraits<A>::ParamType a,
			typename DMTupleTraits<B>::ParamType b,
			typename DMTupleTraits<C>::ParamType c)
			: a(a), b(b), c(c)
		{
		}

		A a;
		B b;
		C c;
	};

	template <typename A, typename B, typename C, typename D>
	struct DMTuple4 {
	public:
		typedef A TypeA;
		typedef B TypeB;
		typedef C TypeC;
		typedef D TypeD;
		typedef DMTuple4<typename DMTupleTraits<A>::ValueType,
			typename DMTupleTraits<B>::ValueType,
			typename DMTupleTraits<C>::ValueType,
			typename DMTupleTraits<D>::ValueType> ValueTuple;
		typedef DMTuple4<typename DMTupleTraits<A>::RefType,
			typename DMTupleTraits<B>::RefType,
			typename DMTupleTraits<C>::RefType,
			typename DMTupleTraits<D>::RefType> RefTuple;

		DMTuple4() {}
		DMTuple4(typename DMTupleTraits<A>::ParamType a,
			typename DMTupleTraits<B>::ParamType b,
			typename DMTupleTraits<C>::ParamType c,
			typename DMTupleTraits<D>::ParamType d)
			: a(a), b(b), c(c), d(d) 
		{
		}

		A a;
		B b;
		C c;
		D d;
	};

	template <typename A, typename B, typename C, typename D, typename E>
	struct DMTuple5 {
	public:
		typedef A TypeA;
		typedef B TypeB;
		typedef C TypeC;
		typedef D TypeD;
		typedef E TypeE;
		typedef DMTuple5<typename DMTupleTraits<A>::ValueType,
			typename DMTupleTraits<B>::ValueType,
			typename DMTupleTraits<C>::ValueType,
			typename DMTupleTraits<D>::ValueType,
			typename DMTupleTraits<E>::ValueType> ValueTuple;
		typedef DMTuple5<typename DMTupleTraits<A>::RefType,
			typename DMTupleTraits<B>::RefType,
			typename DMTupleTraits<C>::RefType,
			typename DMTupleTraits<D>::RefType,
			typename DMTupleTraits<E>::RefType> RefTuple;

		DMTuple5() {}
		DMTuple5(typename DMTupleTraits<A>::ParamType a,
			typename DMTupleTraits<B>::ParamType b,
			typename DMTupleTraits<C>::ParamType c,
			typename DMTupleTraits<D>::ParamType d,
			typename DMTupleTraits<E>::ParamType e)
			: a(a), b(b), c(c), d(d), e(e) 
		{
		}

		A a;
		B b;
		C c;
		D d;
		E e;
	};

	template <typename A, typename B, typename C, typename D, typename E, typename F>
	struct DMTuple6 {
	public:
		typedef A TypeA;
		typedef B TypeB;
		typedef C TypeC;
		typedef D TypeD;
		typedef E TypeE;
		typedef F TypeF;
		typedef DMTuple6<typename DMTupleTraits<A>::ValueType,
			typename DMTupleTraits<B>::ValueType,
			typename DMTupleTraits<C>::ValueType,
			typename DMTupleTraits<D>::ValueType,
			typename DMTupleTraits<E>::ValueType,
			typename DMTupleTraits<F>::ValueType> ValueTuple;
		typedef DMTuple6<typename DMTupleTraits<A>::RefType,
			typename DMTupleTraits<B>::RefType,
			typename DMTupleTraits<C>::RefType,
			typename DMTupleTraits<D>::RefType,
			typename DMTupleTraits<E>::RefType,
			typename DMTupleTraits<F>::RefType> RefTuple;

		DMTuple6() {}
		DMTuple6(typename DMTupleTraits<A>::ParamType a,
			typename DMTupleTraits<B>::ParamType b,
			typename DMTupleTraits<C>::ParamType c,
			typename DMTupleTraits<D>::ParamType d,
			typename DMTupleTraits<E>::ParamType e,
			typename DMTupleTraits<F>::ParamType f)
			: a(a), b(b), c(c), d(d), e(e), f(f) 
		{
		}

		A a;
		B b;
		C c;
		D d;
		E e;
		F f;
	};

	template <typename A, typename B, typename C, typename D, typename E, typename F, typename G>
	struct DMTuple7 {
	public:
		typedef A TypeA;
		typedef B TypeB;
		typedef C TypeC;
		typedef D TypeD;
		typedef E TypeE;
		typedef F TypeF;
		typedef G TypeG;
		typedef DMTuple7<typename DMTupleTraits<A>::ValueType,
			typename DMTupleTraits<B>::ValueType,
			typename DMTupleTraits<C>::ValueType,
			typename DMTupleTraits<D>::ValueType,
			typename DMTupleTraits<E>::ValueType,
			typename DMTupleTraits<F>::ValueType,
			typename DMTupleTraits<G>::ValueType> ValueTuple;
		typedef DMTuple7<typename DMTupleTraits<A>::RefType,
			typename DMTupleTraits<B>::RefType,
			typename DMTupleTraits<C>::RefType,
			typename DMTupleTraits<D>::RefType,
			typename DMTupleTraits<E>::RefType,
			typename DMTupleTraits<F>::RefType,
			typename DMTupleTraits<G>::RefType> RefTuple;

		DMTuple7() {}
		DMTuple7(typename DMTupleTraits<A>::ParamType a,
			typename DMTupleTraits<B>::ParamType b,
			typename DMTupleTraits<C>::ParamType c,
			typename DMTupleTraits<D>::ParamType d,
			typename DMTupleTraits<E>::ParamType e,
			typename DMTupleTraits<F>::ParamType f,
			typename DMTupleTraits<G>::ParamType g)
			: a(a), b(b), c(c), d(d), e(e), f(f), g(g) 
		{
		}

		A a;
		B b;
		C c;
		D d;
		E e;
		F f;
		G g;
	};

	///-----------------------------------------------------------------------------------------------
	// 构造Tuple
	inline DMTuple0 DMMakeTuple() 
	{
		return DMTuple0();
	}

	template <typename A>
	inline DMTuple1<A> DMMakeTuple(const A& a) 
	{
		return DMTuple1<A>(a);
	}

	template <typename A, typename B>
	inline DMTuple2<A, B> DMMakeTuple(const A& a, const B& b) 
	{
		return DMTuple2<A, B>(a, b);
	}

	template <typename A, typename B, typename C>
	inline DMTuple3<A, B, C> DMMakeTuple(const A& a, const B& b, const C& c) 
	{
		return DMTuple3<A, B, C>(a, b, c);
	}

	template <typename A, typename B, typename C, typename D>
	inline DMTuple4<A, B, C, D> DMMakeTuple(const A& a, const B& b, const C& c,
		const D& d) 
	{
		return DMTuple4<A, B, C, D>(a, b, c, d);
	}

	template <typename A, typename B, typename C, typename D, typename E>
	inline DMTuple5<A, B, C, D, E> DMMakeTuple(const A& a, const B& b, const C& c,
		const D& d, const E& e) 
	{
		return DMTuple5<A, B, C, D, E>(a, b, c, d, e);
	}

	template <typename A, typename B, typename C, typename D, typename E, typename F>
	inline DMTuple6<A, B, C, D, E, F> DMMakeTuple(const A& a, const B& b, const C& c,
		const D& d, const E& e, const F& f) 
	{
		return DMTuple6<A, B, C, D, E, F>(a, b, c, d, e, f);
	}

	template <typename A, typename B, typename C, typename D, typename E, typename F, typename G>
	inline DMTuple7<A, B, C, D, E, F, G> DMMakeTuple(const A& a, const B& b, const C& c,
		const D& d, const E& e, const F& f,
		const G& g) 
	{
		return DMTuple7<A, B, C, D, E, F, G>(a, b, c, d, e, f, g);
	}

	///-----------------------------------------------------------------------------------------------
	// 构造引用Tuple
	template <typename A>
	inline DMTuple1<A&> DMMakeRefTuple(A& a) 
	{
		return DMTuple1<A&>(a);
	}

	template <typename A, typename B>
	inline DMTuple2<A&, B&> DMMakeRefTuple(A& a, B& b) 
	{
		return DMTuple2<A&, B&>(a, b);
	}

	template <typename A, typename B, typename C>
	inline DMTuple3<A&, B&, C&> DMMakeRefTuple(A& a, B& b, C& c) 
	{
		return DMTuple3<A&, B&, C&>(a, b, c);
	}

	template <typename A, typename B, typename C, typename D>
	inline DMTuple4<A&, B&, C&, D&> DMMakeRefTuple(A& a, B& b, C& c, D& d) 
	{
		return DMTuple4<A&, B&, C&, D&>(a, b, c, d);
	}

	template <typename A, typename B, typename C, typename D, typename E>
	inline DMTuple5<A&, B&, C&, D&, E&> DMMakeRefTuple(A& a, B& b, C& c, D& d, E& e) 
	{
		return DMTuple5<A&, B&, C&, D&, E&>(a, b, c, d, e);
	}

	template <typename A, typename B, typename C, typename D, typename E, typename F>
	inline DMTuple6<A&, B&, C&, D&, E&, F&> DMMakeRefTuple(A& a, B& b, C& c, D& d, E& e,
		F& f) 
	{
		return DMTuple6<A&, B&, C&, D&, E&, F&>(a, b, c, d, e, f);
	}

	template <typename A, typename B, typename C, typename D, typename E, typename F, typename G>
	inline DMTuple7<A&, B&, C&, D&, E&, F&, G&> DMMakeRefTuple(A& a, B& b, C& c, D& d,
		E& e, F& f, G& g) 
	{
		return DMTuple7<A&, B&, C&, D&, E&, F&, G&>(a, b, c, d, e, f, g);
	}

	///------------------------------------------------------------------------------------------------------------------------------------函数包装处
	// 成员方法派发
	template <class ObjT, class Method>
	inline void DMDispatchToMethod(ObjT* obj, Method method, const DMTuple0& arg) {
		(obj->*method)();
	}

	template <class ObjT, class Method, class A>
	inline void DMDispatchToMethod(ObjT* obj, Method method, const A& arg) {
		(obj->*method)(arg);
	}

	template <class ObjT, class Method, class A>
	inline void DMDispatchToMethod(ObjT* obj, Method method, const DMTuple1<A>& arg) {
		(obj->*method)(arg.a);
	}

	template<class ObjT, class Method, class A, class B>
	inline void DMDispatchToMethod(ObjT* obj,
		Method method,
		const DMTuple2<A, B>& arg) {
			(obj->*method)(arg.a, arg.b);
	}

	template<class ObjT, class Method, class A, class B, class C>
	inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple3<A, B, C>& arg) {
			(obj->*method)(arg.a, arg.b, arg.c);
	}

	template<class ObjT, class Method, class A, class B, class C, class D>
	inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple4<A, B, C, D>& arg) {
			(obj->*method)(arg.a, arg.b, arg.c, arg.d);
	}

	template<class ObjT, class Method, class A, class B, class C, class D, class E>
	inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple5<A, B, C, D, E>& arg) {
			(obj->*method)(arg.a, arg.b, arg.c, arg.d, arg.e);
	}

	template<class ObjT, class Method, class A, class B, class C, class D, class E,
	class F>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple6<A, B, C, D, E, F>& arg) {
			(obj->*method)(arg.a, arg.b, arg.c, arg.d, arg.e, arg.f);
	}

	template<class ObjT, class Method, class A, class B, class C, class D, class E,
	class F, class G>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple7<A, B, C, D, E, F, G>& arg) {
			(obj->*method)(arg.a, arg.b, arg.c, arg.d, arg.e, arg.f, arg.g);
	}

	///------------------------------------------------------------------------------------------------------------------------------------函数包装处
	// 普通函数
	template <class Function>
	inline void DMDispatchToFunction(Function function, const DMTuple0& arg) {
		(*function)();
	}

	template <class Function, class A>
	inline void DMDispatchToFunction(Function function, const A& arg) {
		(*function)(arg);
	}

	template <class Function, class A>
	inline void DMDispatchToFunction(Function function, const DMTuple1<A>& arg) {
		(*function)(arg.a);
	}

	template<class Function, class A, class B>
	inline void DMDispatchToFunction(Function function, const DMTuple2<A, B>& arg) {
		(*function)(arg.a, arg.b);
	}

	template<class Function, class A, class B, class C>
	inline void DMDispatchToFunction(Function function, const DMTuple3<A, B, C>& arg) {
		(*function)(arg.a, arg.b, arg.c);
	}

	template<class Function, class A, class B, class C, class D>
	inline void DMDispatchToFunction(Function function,
		const DMTuple4<A, B, C, D>& arg) {
			(*function)(arg.a, arg.b, arg.c, arg.d);
	}

	template<class Function, class A, class B, class C, class D, class E>
	inline void DMDispatchToFunction(Function function,
		const DMTuple5<A, B, C, D, E>& arg) {
			(*function)(arg.a, arg.b, arg.c, arg.d, arg.e);
	}

	template<class Function, class A, class B, class C, class D, class E, class F>
	inline void DMDispatchToFunction(Function function,
		const DMTuple6<A, B, C, D, E, F>& arg) {
			(*function)(arg.a, arg.b, arg.c, arg.d, arg.e, arg.f);
	}


	///------------------------------------------------------------------------------------------------------------------------------------函数包装处
	// Dispatchers with 0 out param (as a DMTuple0).
	template <class ObjT, class Method>
	inline void DMDispatchToMethod(ObjT* obj,
		Method method,
		const DMTuple0& arg, DMTuple0*) {
			(obj->*method)();
	}

	template <class ObjT, class Method, class A>
	inline void DMDispatchToMethod(ObjT* obj, Method method, const A& arg, DMTuple0*) {
		(obj->*method)(arg);
	}

	template <class ObjT, class Method, class A>
	inline void DMDispatchToMethod(ObjT* obj,
		Method method,
		const DMTuple1<A>& arg, DMTuple0*) {
			(obj->*method)(arg.a);
	}

	template<class ObjT, class Method, class A, class B>
	inline void DMDispatchToMethod(ObjT* obj,
		Method method,
		const DMTuple2<A, B>& arg, DMTuple0*) {
			(obj->*method)(arg.a, arg.b);
	}

	template<class ObjT, class Method, class A, class B, class C>
	inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple3<A, B, C>& arg, DMTuple0*) {
			(obj->*method)(arg.a, arg.b, arg.c);
	}

	template<class ObjT, class Method, class A, class B, class C, class D>
	inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple4<A, B, C, D>& arg, DMTuple0*) {
			(obj->*method)(arg.a, arg.b, arg.c, arg.d);
	}

	template<class ObjT, class Method, class A, class B, class C, class D, class E>
	inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple5<A, B, C, D, E>& arg, DMTuple0*) {
			(obj->*method)(arg.a, arg.b, arg.c, arg.d, arg.e);
	}

	template<class ObjT, class Method, class A, class B, class C, class D, class E,
	class F>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple6<A, B, C, D, E, F>& arg, DMTuple0*) {
			(obj->*method)(arg.a, arg.b, arg.c, arg.d, arg.e, arg.f);
	}

	///------------------------------------------------------------------------------------------------------------------------------------函数包装处
	// Dispatchers with 1 out param.
	template<class ObjT, class Method,
	class OutA>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple0& in,
		DMTuple1<OutA>* out) {
			(obj->*method)(&out->a);
	}

	template<class ObjT, class Method, class InA,
	class OutA>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const InA& in,
		DMTuple1<OutA>* out) {
			(obj->*method)(in, &out->a);
	}

	template<class ObjT, class Method, class InA,
	class OutA>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple1<InA>& in,
		DMTuple1<OutA>* out) {
			(obj->*method)(in.a, &out->a);
	}

	template<class ObjT, class Method, class InA, class InB,
	class OutA>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple2<InA, InB>& in,
		DMTuple1<OutA>* out) {
			(obj->*method)(in.a, in.b, &out->a);
	}

	template<class ObjT, class Method, class InA, class InB, class InC,
	class OutA>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple3<InA, InB, InC>& in,
		DMTuple1<OutA>* out) {
			(obj->*method)(in.a, in.b, in.c, &out->a);
	}

	template<class ObjT, class Method, class InA, class InB, class InC, class InD,
	class OutA>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple4<InA, InB, InC, InD>& in,
		DMTuple1<OutA>* out) {
			(obj->*method)(in.a, in.b, in.c, in.d, &out->a);
	}

	template<class ObjT, class Method,
	class InA, class InB, class InC, class InD, class InE,
	class OutA>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple5<InA, InB, InC, InD, InE>& in,
		DMTuple1<OutA>* out) {
			(obj->*method)(in.a, in.b, in.c, in.d, in.e, &out->a);
	}

	template<class ObjT, class Method,
	class InA, class InB, class InC, class InD, class InE, class InF,
	class OutA>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple6<InA, InB, InC, InD, InE, InF>& in,
		DMTuple1<OutA>* out) {
			(obj->*method)(in.a, in.b, in.c, in.d, in.e, in.f, &out->a);
	}

	///------------------------------------------------------------------------------------------------------------------------------------函数包装处
	// Dispatchers with 2 out param.
	template<class ObjT, class Method,
	class OutA, class OutB>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple0& in,
		DMTuple2<OutA, OutB>* out) {
			(obj->*method)(&out->a, &out->b);
	}

	template<class ObjT, class Method, class InA,
	class OutA, class OutB>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const InA& in,
		DMTuple2<OutA, OutB>* out) {
			(obj->*method)(in, &out->a, &out->b);
	}

	template<class ObjT, class Method, class InA,
	class OutA, class OutB>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple1<InA>& in,
		DMTuple2<OutA, OutB>* out) {
			(obj->*method)(in.a, &out->a, &out->b);
	}

	template<class ObjT, class Method, class InA, class InB,
	class OutA, class OutB>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple2<InA, InB>& in,
		DMTuple2<OutA, OutB>* out) {
			(obj->*method)(in.a, in.b, &out->a, &out->b);
	}

	template<class ObjT, class Method, class InA, class InB, class InC,
	class OutA, class OutB>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple3<InA, InB, InC>& in,
		DMTuple2<OutA, OutB>* out) {
			(obj->*method)(in.a, in.b, in.c, &out->a, &out->b);
	}

	template<class ObjT, class Method, class InA, class InB, class InC, class InD,
	class OutA, class OutB>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple4<InA, InB, InC, InD>& in,
		DMTuple2<OutA, OutB>* out) {
			(obj->*method)(in.a, in.b, in.c, in.d, &out->a, &out->b);
	}

	template<class ObjT, class Method,
	class InA, class InB, class InC, class InD, class InE,
	class OutA, class OutB>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple5<InA, InB, InC, InD, InE>& in,
		DMTuple2<OutA, OutB>* out) {
			(obj->*method)(in.a, in.b, in.c, in.d, in.e, &out->a, &out->b);
	}

	template<class ObjT, class Method,
	class InA, class InB, class InC, class InD, class InE, class InF,
	class OutA, class OutB>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple6<InA, InB, InC, InD, InE, InF>& in,
		DMTuple2<OutA, OutB>* out) {
			(obj->*method)(in.a, in.b, in.c, in.d, in.e, in.f, &out->a, &out->b);
	}
	///------------------------------------------------------------------------------------------------------------------------------------函数包装处
	// Dispatchers with 3 out param.
	template<class ObjT, class Method,
	class OutA, class OutB, class OutC>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple0& in,
		DMTuple3<OutA, OutB, OutC>* out) {
			(obj->*method)(&out->a, &out->b, &out->c);
	}

	template<class ObjT, class Method, class InA,
	class OutA, class OutB, class OutC>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const InA& in,
		DMTuple3<OutA, OutB, OutC>* out) {
			(obj->*method)(in, &out->a, &out->b, &out->c);
	}

	template<class ObjT, class Method, class InA,
	class OutA, class OutB, class OutC>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple1<InA>& in,
		DMTuple3<OutA, OutB, OutC>* out) {
			(obj->*method)(in.a, &out->a, &out->b, &out->c);
	}

	template<class ObjT, class Method, class InA, class InB,
	class OutA, class OutB, class OutC>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple2<InA, InB>& in,
		DMTuple3<OutA, OutB, OutC>* out) {
			(obj->*method)(in.a, in.b, &out->a, &out->b, &out->c);
	}

	template<class ObjT, class Method, class InA, class InB, class InC,
	class OutA, class OutB, class OutC>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple3<InA, InB, InC>& in,
		DMTuple3<OutA, OutB, OutC>* out) {
			(obj->*method)(in.a, in.b, in.c, &out->a, &out->b, &out->c);
	}

	template<class ObjT, class Method, class InA, class InB, class InC, class InD,
	class OutA, class OutB, class OutC>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple4<InA, InB, InC, InD>& in,
		DMTuple3<OutA, OutB, OutC>* out) {
			(obj->*method)(in.a, in.b, in.c, in.d, &out->a, &out->b, &out->c);
	}

	template<class ObjT, class Method,
	class InA, class InB, class InC, class InD, class InE,
	class OutA, class OutB, class OutC>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple5<InA, InB, InC, InD, InE>& in,
		DMTuple3<OutA, OutB, OutC>* out) {
			(obj->*method)(in.a, in.b, in.c, in.d, in.e, &out->a, &out->b, &out->c);
	}

	template<class ObjT, class Method,
	class InA, class InB, class InC, class InD, class InE, class InF,
	class OutA, class OutB, class OutC>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple6<InA, InB, InC, InD, InE, InF>& in,
		DMTuple3<OutA, OutB, OutC>* out) {
			(obj->*method)(in.a, in.b, in.c, in.d, in.e, in.f, &out->a, &out->b, &out->c);
	}

	///------------------------------------------------------------------------------------------------------------------------------------函数包装处
	// Dispatchers with 4 out param.
	template<class ObjT, class Method,
	class OutA, class OutB, class OutC, class OutD>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple0& in,
		DMTuple4<OutA, OutB, OutC, OutD>* out) {
			(obj->*method)(&out->a, &out->b, &out->c, &out->d);
	}

	template<class ObjT, class Method, class InA,
	class OutA, class OutB, class OutC, class OutD>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const InA& in,
		DMTuple4<OutA, OutB, OutC, OutD>* out) {
			(obj->*method)(in, &out->a, &out->b, &out->c, &out->d);
	}

	template<class ObjT, class Method, class InA,
	class OutA, class OutB, class OutC, class OutD>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple1<InA>& in,
		DMTuple4<OutA, OutB, OutC, OutD>* out) {
			(obj->*method)(in.a, &out->a, &out->b, &out->c, &out->d);
	}

	template<class ObjT, class Method, class InA, class InB,
	class OutA, class OutB, class OutC, class OutD>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple2<InA, InB>& in,
		DMTuple4<OutA, OutB, OutC, OutD>* out) {
			(obj->*method)(in.a, in.b, &out->a, &out->b, &out->c, &out->d);
	}

	template<class ObjT, class Method, class InA, class InB, class InC,
	class OutA, class OutB, class OutC, class OutD>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple3<InA, InB, InC>& in,
		DMTuple4<OutA, OutB, OutC, OutD>* out) {
			(obj->*method)(in.a, in.b, in.c, &out->a, &out->b, &out->c, &out->d);
	}

	template<class ObjT, class Method, class InA, class InB, class InC, class InD,
	class OutA, class OutB, class OutC, class OutD>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple4<InA, InB, InC, InD>& in,
		DMTuple4<OutA, OutB, OutC, OutD>* out) {
			(obj->*method)(in.a, in.b, in.c, in.d, &out->a, &out->b, &out->c, &out->d);
	}

	template<class ObjT, class Method,
	class InA, class InB, class InC, class InD, class InE,
	class OutA, class OutB, class OutC, class OutD>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple5<InA, InB, InC, InD, InE>& in,
		DMTuple4<OutA, OutB, OutC, OutD>* out) {
			(obj->*method)(in.a, in.b, in.c, in.d, in.e,
				&out->a, &out->b, &out->c, &out->d);
	}

	template<class ObjT, class Method,
	class InA, class InB, class InC, class InD, class InE, class InF,
	class OutA, class OutB, class OutC, class OutD>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple6<InA, InB, InC, InD, InE, InF>& in,
		DMTuple4<OutA, OutB, OutC, OutD>* out) {
			(obj->*method)(in.a, in.b, in.c, in.d, in.e, in.f,
				&out->a, &out->b, &out->c, &out->d);
	}

	///------------------------------------------------------------------------------------------------------------------------------------函数包装处
	// Dispatchers with 5 out param.
	template<class ObjT, class Method,
	class OutA, class OutB, class OutC, class OutD, class OutE>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple0& in,
		DMTuple5<OutA, OutB, OutC, OutD, OutE>* out) {
			(obj->*method)(&out->a, &out->b, &out->c, &out->d, &out->e);
	}

	template<class ObjT, class Method, class InA,
	class OutA, class OutB, class OutC, class OutD, class OutE>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const InA& in,
		DMTuple5<OutA, OutB, OutC, OutD, OutE>* out) {
			(obj->*method)(in, &out->a, &out->b, &out->c, &out->d, &out->e);
	}

	template<class ObjT, class Method, class InA,
	class OutA, class OutB, class OutC, class OutD, class OutE>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple1<InA>& in,
		DMTuple5<OutA, OutB, OutC, OutD, OutE>* out) {
			(obj->*method)(in.a, &out->a, &out->b, &out->c, &out->d, &out->e);
	}

	template<class ObjT, class Method, class InA, class InB,
	class OutA, class OutB, class OutC, class OutD, class OutE>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple2<InA, InB>& in,
		DMTuple5<OutA, OutB, OutC, OutD, OutE>* out) {
			(obj->*method)(in.a, in.b, &out->a, &out->b, &out->c, &out->d, &out->e);
	}

	template<class ObjT, class Method, class InA, class InB, class InC,
	class OutA, class OutB, class OutC, class OutD, class OutE>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple3<InA, InB, InC>& in,
		DMTuple5<OutA, OutB, OutC, OutD, OutE>* out) {
			(obj->*method)(in.a, in.b, in.c, &out->a, &out->b, &out->c, &out->d, &out->e);
	}

	template<class ObjT, class Method, class InA, class InB, class InC, class InD,
	class OutA, class OutB, class OutC, class OutD, class OutE>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple4<InA, InB, InC, InD>& in,
		DMTuple5<OutA, OutB, OutC, OutD, OutE>* out) {
			(obj->*method)(in.a, in.b, in.c, in.d, &out->a, &out->b, &out->c, &out->d,
				&out->e);
	}

	template<class ObjT, class Method,
	class InA, class InB, class InC, class InD, class InE,
	class OutA, class OutB, class OutC, class OutD, class OutE>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple5<InA, InB, InC, InD, InE>& in,
		DMTuple5<OutA, OutB, OutC, OutD, OutE>* out) {
			(obj->*method)(in.a, in.b, in.c, in.d, in.e,
				&out->a, &out->b, &out->c, &out->d, &out->e);
	}

	template<class ObjT, class Method,
	class InA, class InB, class InC, class InD, class InE, class InF,
	class OutA, class OutB, class OutC, class OutD, class OutE>
		inline void DMDispatchToMethod(ObjT* obj, Method method,
		const DMTuple6<InA, InB, InC, InD, InE, InF>& in,
		DMTuple5<OutA, OutB, OutC, OutD, OutE>* out) {
			(obj->*method)(in.a, in.b, in.c, in.d, in.e, in.f,
				&out->a, &out->b, &out->c, &out->d, &out->e);
	}

	///------------------------------------------------------------------------------------------------------------------------------------构造可运行函数类
	// 构造可运行函数类
	const size_t kDeadTask = 0xDEAD7A53;

	// 删除对象的Task.
	template<class T>
	class DMDeleteTask : public IDMCancelableTask
	{
	public:
		explicit DMDeleteTask(const T* obj) : m_obj(obj) {}
		virtual void Run()
		{
			delete m_obj;
		}
		virtual void Cancel()
		{
			m_obj = NULL;
		}

	private:
		const T* m_obj;
	};

	// 调用对象Release()方法的Task.
	template<class T>
	class DMReleaseTask : public IDMCancelableTask
	{
	public:
		explicit DMReleaseTask(const T* obj) : m_obj(obj) {}
		virtual void Run()
		{
			if(m_obj)
			{
				m_obj->Release();
			}
		}
		virtual void Cancel()
		{
			m_obj = NULL;
		}

	private:
		const T* m_obj;
	};

	// Equivalents for use by base::Bind().
	template<typename T>
	void DMDeletePointer(T* obj)
	{
		delete obj;
	}

	template<typename T>
	void DMReleasePointer(T* obj)
	{
		obj->Release();
	}

	// DMRunnableMethodTraits --------------------------------------------------------
	//
	// This traits-class is used by DMRunnableMethod to manage the lifetime of the
	// callee object.  By default, it is assumed that the callee supports AddRef
	// and Release methods.  A particular class can specialize this template to
	// define other lifetime management.  For example, if the callee is known to
	// live longer than the DMRunnableMethod object, then a DMRunnableMethodTraits
	// struct could be defined with empty DMRetainCallee and DMReleaseCallee methods.
	//
	// The DMDISABLE_RUNNABLE_METHOD_REFCOUNT macro is provided as a convenient way
	// for declaring a DMRunnableMethodTraits that disables refcounting.

	template<class T>
	struct DMRunnableMethodTraits
	{
		DMRunnableMethodTraits()
		{
		}

		~DMRunnableMethodTraits()
		{
		}


		void DMRetainCallee(T* obj)
		{
#ifndef NDEBUG
			// Catch DMNewRunnableMethod being called in an object's constructor.  This
			// isn't safe since the method can be invoked before the constructor
			// completes, causing the object to be deleted.
			//  如果引用的类对象引用计数原始为0,+1-1可能调用默认的delete
			obj->AddRef();
			obj->Release();
#endif
			obj->AddRef();
		}

		void DMReleaseCallee(T* obj)
		{
			obj->Release();
		}
	private:
	};

	// Convenience macro for declaring a DMRunnableMethodTraits that disables
	// refcounting of a class.  This is useful if you know that the callee
	// will outlive the DMRunnableMethod object and thus do not need the ref counts.
	//
	// The invocation of DMDISABLE_RUNNABLE_METHOD_REFCOUNT should be done at the
	// global namespace scope.  Example:
	//
	//   namespace foo {
	//   class Bar {
	//     ...
	//   };
	//   }  // namespace foo
	//
	//   DMDISABLE_RUNNABLE_METHOD_REFCOUNT(foo::Bar)
	//
	// This is different from DISALLOW_COPY_AND_ASSIGN which is declared inside the
	// class.
#define DMDISABLE_RUNNABLE_METHOD_REFCOUNT(TypeName) \
	template<> \
	struct DMRunnableMethodTraits<TypeName> { \
	void DMRetainCallee(TypeName* manager) {} \
	void DMReleaseCallee(TypeName* manager) {} \
	}

	// DMRunnableMethod and DMRunnableFunction -----------------------------------------
	//
	// Runnable methods are a type of task that call a function on an object when
	// they are run. We implement both an object and a set of DMNewRunnableMethod and
	// DMNewRunnableFunction functions for convenience. These functions are
	// overloaded and will infer the template types, simplifying calling code.
	//
	// The template definitions all use the following names:
	// T                - the class type of the object you're supplying
	//                    this is not needed for the Static version of the call
	// Method/Function  - the signature of a pointer to the method or function you
	//                    want to call
	// Param            - the parameter(s) to the method, possibly packed as a Tuple
	// A                - the first parameter (if any) to the method
	// B                - the second parameter (if any) to the method
	//
	// Put these all together and you get an object that can call a method whose
	// signature is:
	//   R T::MyFunction([A[, B]])
	//
	// Usage:
	// PostTask(FROM_HERE, DMNewRunnableMethod(object, &Object::method[, a[, b]])
	// PostTask(FROM_HERE, DMNewRunnableFunction(&function[, a[, b]])

	// DMRunnableMethod and DMNewRunnableMethod implementation -------------------------


	template<class T, class Method, class Params>
	class DMRunnableMethod : public IDMCancelableTask
	{
	public:
		DMRunnableMethod(Method meth, T* obj, const Params& params)
			: m_obj(obj), m_meth(meth), m_params(params)
		{
			m_traits.DMRetainCallee(m_obj);
		}

		~DMRunnableMethod()
		{
			DMReleaseCallee();
			m_obj = reinterpret_cast<T*>(kDeadTask);
		}

		virtual void Run()
		{
			if(m_obj)
			{
				DMDispatchToMethod(m_obj, m_meth, m_params);
			}
		}

		virtual void Cancel()
		{
			DMReleaseCallee();
		}

	private:
		void DMReleaseCallee()
		{
			T* obj = m_obj;
			m_obj = NULL;
			if(obj)
			{
				m_traits.DMReleaseCallee(obj);
			}
		}

		T*						 m_obj;
		Method					 m_meth;
		Params					 m_params;
		DMRunnableMethodTraits<T>  m_traits;
	};

	template<class T, class Method>
	inline IDMCancelableTask* DMNewRunnableMethod(Method method, T* object)
	{
		return new DMRunnableMethod<T, Method, DMTuple0>(method, object, DMMakeTuple());
	}

	template<class T, class Method, class A>
	inline IDMCancelableTask* DMNewRunnableMethod(Method method, T* object, const A& a)
	{
		return new DMRunnableMethod<T, Method, DMTuple1<A> >(method, object,
			DMMakeTuple(a));
	}

	template<class T, class Method, class A, class B>
	inline IDMCancelableTask* DMNewRunnableMethod(Method method, T* object, 
		const A& a, const B& b)
	{
		return new DMRunnableMethod<T, Method, DMTuple2<A, B> >(method, object,
			DMMakeTuple(a, b));
	}

	template<class T, class Method, class A, class B, class C>
	inline IDMCancelableTask* DMNewRunnableMethod(Method method, T* object,
		const A& a, const B& b, const C& c)
	{
		return new DMRunnableMethod<T, Method, DMTuple3<A, B, C> >(method, object,
			DMMakeTuple(a, b, c));
	}

	template<class T, class Method, class A, class B, class C, class D>
	inline IDMCancelableTask* DMNewRunnableMethod(Method method, T* object,
		const A& a, const B& b,
		const C& c, const D& d)
	{
		return new DMRunnableMethod<T, Method, DMTuple4<A, B, C, D> >(method, object,
			DMMakeTuple(a, b, c, d));
	}

	template<class T, class Method, class A, class B, class C, class D, class E>
	inline IDMCancelableTask* DMNewRunnableMethod(Method method, T* object,
		const A& a, const B& b,
		const C& c, const D& d, const E& e)
	{
		return new DMRunnableMethod<T,
			Method,
			DMTuple5<A, B, C, D, E> >(method, object,
			DMMakeTuple(a, b, c, d, e));
	}

	template<class T, class Method, class A, class B, class C, class D, class E,
	class F>
		inline IDMCancelableTask* DMNewRunnableMethod(Method method, T* object,
		const A& a, const B& b,
		const C& c, const D& d, const E& e,
		const F& f)
	{
		return new DMRunnableMethod<T,
			Method,
			DMTuple6<A, B, C, D, E, F> >(method, object,
			DMMakeTuple(a, b, c, d, e, f));
	}

	template<class T, class Method, class A, class B, class C, class D, class E,
	class F, class G>
		inline IDMCancelableTask* DMNewRunnableMethod(Method method, T* object,
		const A& a, const B& b,
		const C& c, const D& d, const E& e,
		const F& f, const G& g)
	{
		return new DMRunnableMethod<T,
			Method,
			DMTuple7<A, B, C, D, E, F, G> >(method, object,
			DMMakeTuple(a, b, c, d, e, f, g));
	}


	// DMRunnableFunction and DMNewRunnableFunction implementation ---------------------
	template<class Function, class Params>
	class DMRunnableFunction : public IDMTask
	{
	public:
		DMRunnableFunction(Function function, const Params& params)
			: function_(function), params_(params)
		{
		}

		~DMRunnableFunction()
		{
			function_ = reinterpret_cast<Function>(kDeadTask);
		}

		virtual void Run()
		{
			if(function_)
			{
				DMDispatchToFunction(function_, params_);
			}
		}

	private:
		Function function_;
		Params params_;
	};

	template<class Function>
	inline IDMTask* DMNewRunnableFunction(Function function)
	{
		return new DMRunnableFunction<Function, DMTuple0>(function, DMMakeTuple());
	}

	template<class Function, class A>
	inline IDMTask* DMNewRunnableFunction(Function function, const A& a)
	{
		return new DMRunnableFunction<Function, DMTuple1<A> >(function, DMMakeTuple(a));
	}

	template<class Function, class A, class B>
	inline IDMTask* DMNewRunnableFunction(Function function, const A& a, const B& b)
	{
		return new DMRunnableFunction<Function, DMTuple2<A, B> >(function,
			DMMakeTuple(a, b));
	}

	template<class Function, class A, class B, class C>
	inline IDMTask* DMNewRunnableFunction(Function function, const A& a, const B& b,
		const C& c)
	{
		return new DMRunnableFunction<Function, DMTuple3<A, B, C> >(function,
			DMMakeTuple(a, b, c));
	}

	template<class Function, class A, class B, class C, class D>
	inline IDMTask* DMNewRunnableFunction(Function function, const A& a, const B& b,
		const C& c, const D& d)
	{
		return new DMRunnableFunction<Function, DMTuple4<A, B, C, D> >(function,
			DMMakeTuple(a, b, c, d));
	}

	template<class Function, class A, class B, class C, class D, class E>
	inline IDMTask* DMNewRunnableFunction(Function function, const A& a, const B& b,
		const C& c, const D& d, const E& e)
	{
		return new DMRunnableFunction<Function, DMTuple5<A, B, C, D, E> >(function,
			DMMakeTuple(a, b, c, d, e));
	}

	template<class Function, class A, class B, class C, class D, class E,
	class F>
		inline IDMTask* DMNewRunnableFunction(Function function, const A& a, const B& b,
		const C& c, const D& d, const E& e,
		const F& f)
	{
		return new DMRunnableFunction<Function, DMTuple6<A, B, C, D, E, F> >(function,
			DMMakeTuple(a, b, c, d, e, f));
	}

	template<class Function, class A, class B, class C, class D, class E,
	class F, class G>
		inline IDMTask* DMNewRunnableFunction(Function function, const A& a, const B& b,
		const C& c, const D& d, const E& e, const F& f,
		const G& g)
	{
		return new DMRunnableFunction<Function, DMTuple7<A, B, C, D, E, F, G> >(function,
			DMMakeTuple(a, b, c, d, e, f, g));
	}
}//namespace DM