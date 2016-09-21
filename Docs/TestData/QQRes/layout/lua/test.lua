print("lua 定时换肤随机示例.........")
print("修改lua,点击运行脚本测试效果")

--单独换肤函数
function  themefun(count)
	local themeprex = "主题"
	local themename = string.format("%s%d",themeprex,count)
	local w_themename = DMCA2W(themename)
	local w_themeoldname = DMCA2W("")
	g_pDMRes:SetCurTheme(w_themename:GetBuffer(),w_themeoldname:GetBuffer())	
	w_themename:ReleaseBuffer()--GetBuffer后一定ReleaseBuffer，不然内存无法释放,未知风险
	w_themeoldname:ReleaseBuffer()
	g_pDMApp:RedrawAll()
end

widget_wnd = nil
-- WidgetWnd起始调用DMEVT_SCRIPT_START函数
function DMEVT_SCRIPT_START(args)
	print("开始运行......")
	local duiwnd = args.m_pSender
	widget_wnd = duiwnd
	duiwnd:DM_SetTimer(123,11) --设置1234定时器
	return 1
end

function DMEVT_SCRIPT_TIMER(args)
	-- 每次运行随机换肤
	t = os.time()
	themefun(t%17)
	return 1
end

function DMEVT_SCRIPT_END(args)
	-- 停止定时器
	widget_wnd:DM_KillTimer(123)
	return 1
end







