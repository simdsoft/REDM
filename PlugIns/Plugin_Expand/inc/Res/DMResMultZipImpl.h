#pragma once
#include "DMResZipImpl.h"

namespace DM
{
	class DMResMultZipImpl: public IDMRes,public DMArrayT<DMResZipImplPtr>
	{
		DMDECLARE_CLASS_NAME(DMResMultZipImpl,L"DMResMultZipImpl",DMREG_Res);
	public:
		DMResMultZipImpl();
		~DMResMultZipImpl();
	public:
		virtual DMCode LoadResPack(WPARAM wp, LPARAM lp);
		virtual DMCode IsItemExists(LPCWSTR lpszType, LPCWSTR lpszName,LPCWSTR lpszThemeName=NULL);
		virtual DMCode GetItemSize(LPCWSTR lpszType, LPCWSTR lpszName, unsigned long& ulSize,LPCWSTR lpszThemeName=NULL);
		virtual DMCode GetItemBuf(LPCWSTR lpszType, LPCWSTR lpszName, LPVOID lpBuf, unsigned long ulSize,LPCWSTR lpszThemeName=NULL);
		virtual DMCode LoadTheme(WPARAM wp, LPARAM lp);
		virtual DMCode SetCurTheme(LPCWSTR lpszName, LPCWSTR lpszOldName=NULL);
		virtual DMCode SendExpandInfo(WPARAM wp, LPARAM lp);


	protected:
		virtual void PreArrayObjRemove(const DMResZipImplPtr &obj);
		virtual bool EqualArrayObj(const DMResZipImplPtr &objsrc, const DMResZipImplPtr &objdest);
	};
}//namespace DM