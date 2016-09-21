function ThreadTest()
    print("ThreadTest start")
    print("TestFunc aaaaaa")
    TestFunc()
    TestFunc2(1.2)
    print("TestFunc bbbbbb")
	
    print("g_test::TestFunc() aaaaaa")
    g_test:TestFunc()
    g_test:TestFunc2(2.3)
    print("g_test::TestFunc() bbbbbb")
	
    print("ThreadTest end")
end
