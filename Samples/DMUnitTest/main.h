#pragma once

#pragma once

class DMEnvironment: public testing::Environment
{
public:
	virtual void SetUp();
	virtual void TearDown();

public://全局共享变量
	DMApp*						 m_pDMApp;
};

extern DMEnvironment *pGlobalPtr;          // 全局指针