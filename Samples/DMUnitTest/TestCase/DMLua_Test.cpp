// ----------------------------------------------------------------
// Copyright (c) tencent 
// All rights reserved.
// 
// File name:	DMLua_Test.cpp
// File mark:   
// File summary:测试LUA的测试用例

// Author:		guoyouhuang
// Edition:     1.0
// Create date:  
// ----------------------------------------------------------------
#pragma once
#include "DMUintTestAfx.h"
#include "DMLua.h"

#ifdef _DEBUG
#pragma comment(lib, "lua_d.lib")
#else
#pragma comment(lib, "lua.lib")
#endif

class DMLuaTest:public::testing::Test
{
public:
	virtual void SetUp(){}
	virtual void TearDown(){}
};


enum NumEnum 
{
	ONE = 1,
	TWO = 2,
	THREE = 3,
};

int cpp_func(int arg1, int arg2)
{ 
	return arg1 + arg2;
}

void cpp_test_str(char* str)
{
	printf("%s\n", str);
}

TEST_F(DMLuaTest, simple1)
{
	printf("DMLuaTest::simple1----------------------------------\n");
	DMLUA::lua_context   L;
	DMLUA::open_base(L);
	DMLUA::init(L);

	long task_id = 120;
	long task_id2 = -1;
	long task_id3 = -2;
	int rand_id = 123;

	DMLUA::set<long>(L, "task_id", task_id);
	DMLUA::set<long>(L, "task_id2", task_id2);
	DMLUA::set<long>(L, "task_id3", task_id3);
	DMLUA::set<long>(L, "rand_id", rand_id);

	DMLUA::set<long>(L, "ONE", ONE);

	DMLUA::def(L, "cpp_func", cpp_func);
	DMLUA::def(L, "cpp_test_str", cpp_test_str);

	wchar_t szPath[MAX_PATH] = {0};
	GetRootFullPath(L".\\UTRes\\UTTest\\lua\\sample1.lua",szPath,MAX_PATH);
	char szLuaPath[MAX_PATH] = {0};
	UnicodeToAscii(szPath,szLuaPath,MAX_PATH);

	DMLUA::dofile(L, szLuaPath);

	int result = DMLUA::call<int>(L, "lua_func", 3, 4);

	printf("lua_func(3,4) = %d\n", result);
}

static int cpp_int = 100;
TEST_F(DMLuaTest, simple2)
{
	printf("DMLuaTest::simple2----------------------------------\n");
	DMLUA::lua_context   L;
	DMLUA::open_base(L);
	DMLUA::init(L);

	wchar_t szPath[MAX_PATH] = {0};
	GetRootFullPath(L".\\UTRes\\UTTest\\lua\\sample2.lua",szPath,MAX_PATH);
	char szLuaPath[MAX_PATH] = {0};
	UnicodeToAscii(szPath,szLuaPath,MAX_PATH);

	DMLUA::set(L, "cpp_int", cpp_int);
	DMLUA::dofile(L, szLuaPath);

	int lua_int = DMLUA::get<int>(L, "lua_int");
	printf("lua_int = %d\n", lua_int);
}

struct A
{
	A(int v) : m_Value(v) {}
	int		   m_Value;
};

// 基类
struct base
{
	base() {}

	const char* is_base(){ return "this is base"; }
	int  m_base_var;
};

// 子类
class test : public base
{
public:
	test(int val) : m_test(val) {base::m_base_var = 1121;}
	~test() {} 

	A* create_a()
	{
		return new A(10);
	}

	const char* is_test(){ return "this is test"; }

	void ret_void() {}
	int ret_int()				{ return m_test;}
	int ret_mul(int m) const	{ return m_test * m;}
	test* get()					
	{ 
		return this; 
	}
	void set(A a)				
	{
		m_test = a.m_Value;
	}

	int m_test;
};

// 全局子类的实例
test g_test(11);
TEST_F(DMLuaTest, simple3)
{ 
	printf("DMLuaTest::simple3----------------------------------\n");
	DMLUA::lua_context   L;
	DMLUA::open_base(L);
	DMLUA::open_string(L);
	DMLUA::init(L);

	DMLUA::class_add<base>(L, "base");
	DMLUA::class_def<base>(L, "is_base", &base::is_base);
	DMLUA::class_mem<base>(L, "m_base_var", &base::m_base_var);

	DMLUA::class_add<test>(L, "test");
	DMLUA::class_inh<test, base>(L);
	DMLUA::class_con<test>(L, DMLUA::constructor<test,int>);
	DMLUA::class_def<test>(L, "create_a", &test::create_a);
	DMLUA::class_def<test>(L, "is_test", &test::is_test);
	DMLUA::class_def<test>(L, "ret_void", &test::ret_void);
	DMLUA::class_def<test>(L, "ret_int", &test::ret_int);
	DMLUA::class_def<test>(L, "ret_mul", &test::ret_mul);
	DMLUA::class_def<test>(L, "get", &test::get);
	DMLUA::class_def<test>(L, "set", &test::set);
	DMLUA::class_mem<test>(L, "m_test", &test::m_test);

	DMLUA::set(L, "g_test", &g_test);

	wchar_t szPath[MAX_PATH] = {0};
	GetRootFullPath(L".\\UTRes\\UTTest\\lua\\sample3.lua",szPath,MAX_PATH);
	char szLuaPath[MAX_PATH] = {0};
	UnicodeToAscii(szPath,szLuaPath,MAX_PATH);

	DMLUA::dofile(L, szLuaPath);
}

TEST_F(DMLuaTest, simple4)
{ 
	printf("DMLuaTest::simple4----------------------------------\n");
	DMLUA::lua_context   L;
	luaL_openlibs(L);

	// 创建table haha
	DMLUA::table haha(L, "haha");

	// 设置数值
	haha.set("value", 1);

	// 设置table
	haha.set("inside", DMLUA::table(L));

	// 获取inside
	DMLUA::table inside = haha.get<DMLUA::table>("inside");

	// 插入数值
	inside.set("value", 2);

	wchar_t szPath[MAX_PATH] = {0};
	GetRootFullPath(L".\\UTRes\\UTTest\\lua\\sample4.lua",szPath,MAX_PATH);
	char szLuaPath[MAX_PATH] = {0};
	UnicodeToAscii(szPath,szLuaPath,MAX_PATH);

	DMLUA::dofile(L, szLuaPath);

	// 从lua获取变量
	const char* test = haha.get<const char*>("test");
	printf("haha.test = %s\n", test);

	// 
	DMLUA::table from_lua_table(L, "lua_table");

	// 调用lua函数
	DMLUA::call<void>(L, "print_table", from_lua_table);

	// 新建一个表格
	DMLUA::table temp(L);

	// 设置值
	temp.set("name", "local table !!");

	// 调用lua函数
	DMLUA::call<void>(L, "print_table", temp);

	// 调用lua函数
	DMLUA::table ret = DMLUA::call<DMLUA::table>(L, "return_table", "give me a table !!");
	printf("ret.name =\t%s\n", ret.get<const char*>("name"));
}

void show_error(const char* error)
{
	printf("hgy _ALERT -> %s\n", error);
}

TEST_F(DMLuaTest, simple5)
{ 
	printf("DMLuaTest::simple5----------------------------------\n");
	DMLUA::lua_context   L;
	luaL_openlibs(L);

	printf("%s\n","-------------------------- current stack");
	DMLUA::enum_stack(L);

	lua_pushnumber(L, 1);

	printf("%s\n","-------------------------- stack after push '1'");
	DMLUA::enum_stack(L);

	wchar_t szPath[MAX_PATH] = {0};
	GetRootFullPath(L".\\UTRes\\UTTest\\lua\\sample5.lua",szPath,MAX_PATH);
	char szLuaPath[MAX_PATH] = {0};
	UnicodeToAscii(szPath,szLuaPath,MAX_PATH);

	DMLUA::dofile(L, szLuaPath);

	printf("%s\n","-------------------------- calling test_error()");
	DMLUA::call<void>(L, "test_error");// test_error_3不存在，自动调用show_error

	DMLUA::def(L, "_ALERT", show_error);
	DMLUA::call<void>(L, "_ALERT", "test !!!");

	printf("%s\n","-------------------------- calling test_error()");
	DMLUA::call<void>(L, "test_error");
}


int TestFunc(lua_State* L)
{
	printf("# TestFunc aaaaaa\n");
	return lua_yield(L, 0);
}

int TestFunc2(lua_State* L, float a)
{
	printf("# TestFunc2(L,%f) bbbbbb\n", a);
	return lua_yield(L, 0);
}

class TestClass
{
public:

	int TestFunc(lua_State* L)
	{
		printf("# TestClass::TestFunc aaaaaa\n");
		return lua_yield(L, 0);
	}

	int TestFunc2(lua_State* L, float a)
	{
		printf("# TestClass::TestFunc2(L,%f) bbbbbb\n", a);
		return lua_yield(L, 0);
	}
};

TEST_F(DMLuaTest, simple6)
{ 
	printf("DMLuaTest::simple6----------------------------------\n");
	DMLUA::lua_context   L;
	luaL_openlibs(L);

	DMLUA::def(L, "TestFunc", &TestFunc);
	DMLUA::def(L, "TestFunc2", &TestFunc2);

	DMLUA::class_add<TestClass>(L, "TestClass");
	DMLUA::class_def<TestClass>(L, "TestFunc", &TestClass::TestFunc);
	DMLUA::class_def<TestClass>(L, "TestFunc2", &TestClass::TestFunc2);

	TestClass g_test;
	DMLUA::set(L, "g_test", &g_test);

#if 0
	wchar_t szPath[MAX_PATH] = {0};
	GetRootFullPath(L".\\UTRes\\UTTest\\lua\\sample6.lua",szPath,MAX_PATH);
	char szLuaPath[MAX_PATH] = {0};
	UnicodeToAscii(szPath,szLuaPath,MAX_PATH);

	DMLUA::dofile(L, szLuaPath);

	DMLUA::lua_context new_L(lua_newthread(L));
	DMLUA::enum_stack(L);
	DMLUA::enum_stack(new_L);
	lua_getglobal(L, "ThreadTest");

	printf("* lua_resume()\n");
	lua_resume(L, L, 0);
	lua_resume(L, L, 0);
	lua_resume(L, L, 0);
	lua_resume(L, L, 0);
	lua_resume(L, L, 0);
#endif 

}

class HgyObj
{
public:
	HgyObj(){printf("HgyObj\n");}
	~HgyObj(){printf("~HgyObj\n");}
	void PrintInfo(){printf("PrintInfo\n");}
};

class HgyObjFactory
{
public:
	HgyObjFactory()
	{
		printf("HgyObjFactory");
	}
	HgyObj* CreateObj()
	{
		return new HgyObj();
	}
	void DestroyObj(HgyObj* obj)
	{
		delete obj;
	}
};

TEST_F(DMLuaTest, simple7)
{ 
	printf("DMLuaTest::simple7----------------------------------\n");
	DMLUA::lua_context   L;
	luaL_openlibs(L);

	HgyObjFactory hits;

	DMLUA::class_add<HgyObj>(L, "HgyObj");
	DMLUA::class_def<HgyObj>(L,"PrintInfo",&HgyObj::PrintInfo);

	DMLUA::class_add<HgyObjFactory>(L,"HgyObjFactory");
	DMLUA::class_con<HgyObjFactory>(L, DMLUA::constructor<HgyObjFactory>);// 一定要初始化构造函数，不然无法使用
	DMLUA::class_def<HgyObjFactory>(L, "CreateObj", &HgyObjFactory::CreateObj);
	DMLUA::class_def<HgyObjFactory>(L, "DestroyObj", &HgyObjFactory::DestroyObj);

	DMLUA::set(L, "hits", &hits);
	wchar_t szPath[MAX_PATH] = {0};
	GetRootFullPath(L".\\UTRes\\UTTest\\lua\\sample7.lua",szPath,MAX_PATH);
	char szLuaPath[MAX_PATH] = {0};
	UnicodeToAscii(szPath,szLuaPath,MAX_PATH);

	DMLUA::dofile(L, szLuaPath);
	DMLUA::enum_stack(L);

	DMLUA::table turret = DMLUA::get<DMLUA::table>( L, "turret" );
	DMLUA::table obj1 = turret.call<DMLUA::table >("create", turret);
	turret.call<void>( "destroy", turret, obj1 );
}

