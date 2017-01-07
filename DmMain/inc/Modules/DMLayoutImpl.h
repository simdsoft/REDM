//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMLayoutImpl.h 
// File Des: 锚点布局
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-3	1.0			
//--------------------------------------------------------
#pragma once

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DMLayoutImpl"/>的xml属性定义
	/// </summary>
	class DMLayoutImplAttr
	{
	public:
		/// -------------------------------------------------
		///  @brief 指定位置,最常用的为指定四个坐标,示例：
		///  @remark 示例1:pos="10,11,101,102",四个点分别代表了左:10,上:11,右:101,下:102
		///  @remark 示例2:pos="10,11,-12,-13",四个点分别代表了左:10,上:11,右:父窗口右边左移12,下:父窗口下边上移13--->'-'：定义窗口的计算以相反坐标系
		///  @remark 示例3:pos="10,11,@100,@101",四个点分别代表了左:10,上:11,宽:100,高:101--->'@'：定义窗口的size，只在后两个值中有效
		///  @remark 示例4:pos="10,11,|-1,|2",四个点分别代表了左:10,上:11,右:父窗口中心点左移1(-1),下：父窗口中心点下移2-->'|':参考父窗口中心
		///  @remark 示例5:pos="[10,[11,@100,@101",四个点分别代表了左:距前一个兄弟窗口自己近的边右移(10)，上:距前一个兄弟窗口自己近的边下移(10)，宽:100,高:101--->'['参考前一个兄弟窗口与自己近的边（没有兄弟窗口就参考父窗口的可用布局空间）
		///  @remark 示例6:pos="0,1,]-10,@100",四个点分别代表了左:0，上:1，右:距下一个兄弟窗口左移10(-10),高:100--->']'参考下一个兄弟窗口与自己近的边（没有兄弟窗口就参考父窗口的可用布局空间）
		///  @remark -->'{'参考下一个兄弟窗口与自己近的边，与示例5类型，-->'}'参考下一个兄弟窗口与自己远的边,与示例6类似
		///  @remark 示例7:pos="%10,%20,%30,%40",四个点分别代表了左:父窗口布局空间的%10，上：父窗口布局空间的%20，右：父窗口布局空间的%30，下：父窗口布局空间的%40--->'%'采用在父窗口的百分比定义坐标
		static wchar_t* POS_pos;	

		/// -------------------------------------------------
		///  @brief 指定位置,在pos坐标小于四时，结合pos使用,示例:
		///  @remark 示例1:pos="%10,%11" possize="100,101",pos记录了左上角坐标,而possize记录了宽100，高101
		///  @remark 示例2:pos="%10,%20" possize="-1,-1"，pos不为空,同时possize大小为-1，-1,（-1)表示根据内容布局，使用DV_GetDesiredSize，possize前一值为横向，后一值为竖直
		///  @remark 示例3:pos="%10,%20" possize="-1,100"，pos不为空,同时possize大小为-1，100,（-1)表示根据内容布局,高度100,使用DV_GetDesiredSize，possize前一值为横向，后一值为竖直
		///  @remark 示例4:pos="" possize="-1,-1",pos为空,possize大小为-1，-1，填满父窗口
		///  @remark 示例4:pos="" possize="x,x",pos为空,x不全为-1，-1，使用自动排序,前-1表示填满横向父窗口，后-1表示填满坚向父窗口
		static wchar_t* SIZE_possize;                                                        
	};
	DMAttrValueInit(DMLayoutImplAttr,POS_pos)DMAttrValueInit(DMLayoutImplAttr,SIZE_possize)
}

namespace DM
{
#define POSFLAG_REFCENTER      '|'          // 参考父窗口中心
#define POSFLAG_REFPREV_NEAR   '['          // 参考前一个兄弟窗口与自己近的边
#define POSFLAG_REFNEXT_NEAR   ']'          // 参考下一个兄弟窗口与自己近的边
#define POSFLAG_REFPREV_FAR    '{'          // 参考前一个兄弟窗口与自己远的边
#define POSFLAG_REFNEXT_FAR    '}'          // 参考下一个兄弟窗口与自己远的边
#define POSFLAG_PERCENT        '%'          // 采用在父窗口的百分比定义坐标
#define POSFLAG_DEFSIZE        '@'          // 在pos属性中定义窗口的size，只在在定义x2,y2时有效

	enum POS_TYPE// 坐标属性
	{
		SizeX_Mask          = 0x0007UL,
		SizeX_Specify       = 0x0001UL,		// width >= 0,强制指定了大小
		SizeX_FitContent    = 0x0002UL,		// 按内容指定
		SizeX_FitParent     = 0x0004UL,		// width = -1 default
		SizeY_Mask          = 0x0070UL,
		SizeY_Specify       = 0x0010UL,		// height >= 0
		SizeY_FitContent    = 0x0020UL,		// 按内容指定
		SizeY_FitParent     = 0x0040UL,		// height = -1 default
	};

	enum PIT// 坐标类型
	{
		PIT_NORMAL = 0,						// 一般坐标
		PIT_CENTER,							// 参考父窗口中心点,以"|"开始
		PIT_PERCENT,						// 指定在父窗口坐标的中的百分比,以"%"开头
		PIT_PREV_NEAR,						// 参考前一个兄弟窗口与自己近的边
		PIT_NEXT_NEAR,						// 参考下一个兄弟窗口与自己近的边
		PIT_PREV_FAR,						// 参考前一个兄弟窗口与自己远的边
		PIT_NEXT_FAR,						// 参考下一个兄弟窗口与自己远的边
		PIT_OFFSET,							// 相对于前面x1,y1的偏移,只能在x2,y2中使用，以@开头
	};

	typedef struct stPOS_ITEM
	{
		stPOS_ITEM(){pit=PIT_NORMAL;bMinus=false;nPos=0.0;}
		PIT		pit;						// 坐标类型
		bool	bMinus;						// true表示这是一个负值
		float   nPos;
	}POS_ITEM;

	/// <summary>
	///		锚点布局内置实现，属性：<see cref="DMAttr::DMLayoutImplAttr"/>
	/// </summary>
	class DMLayoutImpl:public IDMLayout
	{
		DMDECLARE_CLASS_NAME(DMLayoutImpl,L"DMLayoutImpl",DMREG_Layout);
	public:
		DMLayoutImpl();

	public:
		DMCode SetOwner(LPVOID lpWnd);
		DMCode UpdateLayout(OUT LPRECT prcContainer,OUT CRect &rcWindow);
		DMCode UpdateChildLayout();

	public:// 辅助
		bool ParsePostion();
		bool ParsePostionType();
		bool ParseItem(CStringW &strPos, POS_ITEM &item);
		bool ParseChildPosition(DM::CList<DUIWindow*> *pList);
		int ParseItemValue(const POS_ITEM &item,int nMin, int nMax,bool bX);//返回POS_WAIT表示布局依赖的前或后窗口未完成布局，bX表示横坐标
		bool Update4(LPRECT lpRcContainer,OUT CRect &rcWindow);
		bool Update2(LPRECT lpRcContainer,OUT CRect &rcWindow);
		bool UpdateFull(LPRECT lpRcContainer,OUT CRect &rcWindow);
		bool UpdateAuto(LPRECT lpRcContainer,OUT CRect &rcWindow);
		bool IsUnInitPos(int iPos);
		CSize CalcSize(LPRECT pRcContainer);//  计算窗口大小


	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMLayoutImplAttr::POS_pos, OnAttributePos)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMLayoutImplAttr::SIZE_possize, OnAttributePosSize)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttributePos(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttributePosSize(LPCWSTR pszValue, bool bLoadXml);
	public:
		CStringW				 m_strPosValue;			
			
		DUIWindow*				 m_pOwner;			   // layout的宿主 
		int					     m_nCount;			   // 定义的坐标个数
		
		POS_ITEM m_Left;POS_ITEM m_Top;POS_ITEM m_Right;POS_ITEM m_Bottom;

		UINT					m_uPositionType;      //  坐标属性
		CSize                   m_size;			      // SizeX_Mask,SizeY_Mask		
		int						m_nSepSpace;          //  窗口自动排版的竖直间隔
	};
}//namespace DM
