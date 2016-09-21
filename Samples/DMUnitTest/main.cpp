#include "DMUintTestAfx.h"
#include "main.h"

void DMEnvironment::SetUp()
{
	OleInitialize(NULL);
	m_pDMApp = new DMApp(NULL);
}

void DMEnvironment::TearDown()
{
	DM_DELETE(m_pDMApp);
}

DMEnvironment *pGlobalPtr;          // È«¾ÖÖ¸Õë

TEST(foo,foo)
{
	EXPECT_EQ(1,TRUE);
}

int main(int argc, char* argv[])
{
	pGlobalPtr = new DMEnvironment;
	testing::AddGlobalTestEnvironment(pGlobalPtr);   //×¢²á

	char curdir[MAX_PATH] = {0};

	unsigned int seed = (unsigned int)time(NULL);
	srand(seed);

	testing::InitGoogleTest(&argc, argv);
	int err = RUN_ALL_TESTS();

	getchar();
	return err;
}