#pragma once
#include "DMResZipHelper.h"
#include "DMZipHelper.h"
namespace DM
{
#define  RES_INDEXFILE_CHECK          1// 开启文件是否存在强制校验存在
	/// <summary>
	///     外置zip示例和内置打包文件夹一致
	///		内置Res打包文件夹为以下结构:如果你觉得不能满足需求，可以自己修改一份，自己注册进去
	///     Res文件夹包含两个子文件夹：1.layout文件夹 2.themes文件夹
	///		1.1.layout文件夹定义了所有布局xml和skin、style的全局xml
	///     2.1.themes文件夹下定义了多个主题包
	///     3.每个文件夹下均有一个dmindex.xml来标识资源索引
	/// </summary>
	class DMResZipImpl:public IDMRes,public DMArrayT<DMZipItemArrayPtr>
	{
		DMDECLARE_CLASS_NAME(DMResZipImpl,L"DMResZipImpl",DMREG_Res);
	public:
		DMResZipImpl();
		~DMResZipImpl();
	public:
		virtual DMCode LoadResPack(WPARAM wp, LPARAM lp);
		virtual DMCode IsItemExists(LPCWSTR lpszType, LPCWSTR lpszName,LPCWSTR lpszThemeName=NULL);
		virtual DMCode GetItemSize(LPCWSTR lpszType, LPCWSTR lpszName, unsigned long& ulSize,LPCWSTR lpszThemeName=NULL);
		virtual DMCode GetItemBuf(LPCWSTR lpszType, LPCWSTR lpszName, LPVOID lpBuf, unsigned long ulSize,LPCWSTR lpszThemeName=NULL);
		virtual DMCode LoadTheme(WPARAM wp, LPARAM lp);// 不使用这种方式
		virtual DMCode SetCurTheme(LPCWSTR lpszName, LPCWSTR lpszOldName=NULL);
		virtual DMCode SendExpandInfo(WPARAM wp, LPARAM lp);  ///< lp为0时，把长度赋值到*wp

	public:// 辅助
		DMCode ParseThemes(LPCWSTR lpszIndexPath);
		DMCode ParseIndex(LPCWSTR lpszIndexPath,DMZipItemArray** ppItem);
		LPCWSTR GetItemPath(LPCWSTR lpszType,LPCWSTR lpszName,LPCWSTR lpszThemeName);
		DMZipItemArrayPtr FindResItemObj(LPCWSTR lpszName);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_bool_ATTRIBUTE(L"bloop", m_bThemeLoop, DM_ECODE_OK)
			DM_STRING_ATTRIBUTE(L"name", m_strCurTheme, DM_ECODE_OK)
			DM_bool_ATTRIBUTE(L"boutstyle", m_bOutStyle, DM_ECODE_OK)
			DM_bool_ATTRIBUTE(L"bassert",m_bAssert, DM_ECODE_OK)
		DM_END_ATTRIBUTES()

	protected:
		virtual void PreArrayObjRemove(const DMZipItemArrayPtr &obj);

	protected:
		DM::CStringW                          m_strDir;
		DM::CStringW                          m_strCurTheme;       // 当前使用的theme Key
		DMZipItemArrayPtr					  m_pCurTheme;         // 当前使用的theme
		bool                                  m_bThemeLoop;        // 是否启用循环查找theme(当前theme中找不到资源,直接去其他theme中查找)
		bool                                  m_bOutStyle;         // 当前是外部模式（外部强制改变了skin池的数据，这时所有的theme应该都需要刷新)
		DMSmartPtrT<DMZipItemArray>			  m_pLayout;		   // Layout的列表
		CDMZipHelper						  m_zipFile;           // zip辅助

	public:
		DM::CStringW                          m_strZipKey;         // 多ZIP时，标识唯一的ZIP
		bool                                  m_bAssert;
	};
	typedef DMResZipImpl* DMResZipImplPtr;


}//namespace DM