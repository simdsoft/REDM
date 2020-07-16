#pragma once
#include "DMResZipImpl.h"

namespace DM
{
	class DMResMultZipImpl: public IDMRes,public DMArrayT<DMResZipImplPtr>
	{
		DMDECLARE_CLASS_NAME(DMResMultZipImpl,"DMResMultZipImpl",DMREG_Res);
	public:
		DMResMultZipImpl();
		~DMResMultZipImpl();
	public:
		virtual DMCode LoadResPack(WPARAM wp, LPARAM lp);
		virtual DMCode IsItemExists(LPCSTR lpszType, LPCSTR lpszName, LPCSTR lpszThemeName = NULL);
		virtual DMCode GetItemSize(LPCSTR lpszType, LPCSTR lpszName, unsigned long& ulSize, LPCSTR lpszThemeName = NULL);
		virtual DMCode GetItemBuf(LPCSTR lpszType, LPCSTR lpszName, DMBufT<byte>&, PULONG lpULSize, LPCSTR lpszThemeName = NULL);
		virtual DMCode LoadTheme(WPARAM wp, LPARAM lp);
		virtual DMCode SetCurTheme(LPCSTR lpszName, LPCSTR lpszOldName=NULL);
		virtual DMCode SendExpandInfo(WPARAM wp, LPARAM lp);


	protected:
		virtual void PreArrayObjRemove(const DMResZipImplPtr &obj);
		virtual bool EqualArrayObj(const DMResZipImplPtr &objsrc, const DMResZipImplPtr &objdest);
	};
}//namespace DM