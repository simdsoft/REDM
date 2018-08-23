#include "DMUintTestAfx.h"
#include "main.h"

void DMEnvironment::SetUp()
{
	m_pDMApp = new DMApp(NULL);
#ifdef DLL_DMMAIN// lib库下不支持插件
#ifdef _DEBUG
	m_pDMApp->LoadPlugins(L"dmpluginsd.cfg");
#else
	m_pDMApp->LoadPlugins(L"dmplugins.cfg");
#endif
#endif//DLL_DMMAIN
}

void DMEnvironment::TearDown()
{
	DM_DELETE(m_pDMApp);
}

DMEnvironment *pGlobalPtr;          // 全局指针

TEST(foo,foo)
{
	EXPECT_EQ(1,TRUE);
}

int main(int argc, char* argv[])
{
	pGlobalPtr = new DMEnvironment;
	testing::AddGlobalTestEnvironment(pGlobalPtr);   //注册



	char curdir[MAX_PATH] = {0};

	unsigned int seed = (unsigned int)time(NULL);
	srand(seed);

	testing::InitGoogleTest(&argc, argv);
	int err = RUN_ALL_TESTS();

	getchar();
	return err;
}