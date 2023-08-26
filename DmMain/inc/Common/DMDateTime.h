// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DMDataTime.h
// File mark:   
// File summary:
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2018-12-4
// ----------------------------------------------------------------
#pragma once

namespace DM
{
	class DM_EXPORT DMDateTime
	{
	public:
		DMDateTime();
		DMDateTime(const DMDateTime& dt);
		DMDateTime(int dt);
		DMDateTime(int iYear, int iMonth, int iDay);
		DMDateTime(const CStringW& strDate);

		static DMDateTime Today();

		bool IsValid() const;

		void Reset();
		void ResetToday();

		BYTE GetWeekDay() const;
		unsigned short GetYearDay() const;

		unsigned short GetYear() const;
		BYTE GetMonth() const;
		BYTE GetDay() const;

		BYTE GetHour() const;
		BYTE GetMinute() const;
		BYTE GetSecond() const;

		void SetYear(unsigned short val);
		void SetMonth(BYTE val);
		void SetDayOfMonth(BYTE val);

		bool IsQuarterEnd();
		bool IsMonthEnd();
		bool IsLeapYear() const;

		int GetMonthEndDay() const;
		int GetDayCount() const;

		void SetDate(int iYear, int iMonth, int iDay);
		void SetDate(int iDayCount);
		void SetDate(const DMDateTime& dt);
		void SetDate(const CStringW& strDate);// 2018-11-30 12:01:02 或 2018-11-30 

		void SetTime(BYTE h, BYTE m, BYTE s);

		CStringW ToString();
		CStringW ToTime();
		CStringW ToLongDate();

		void SetMonthBegin();
		void SetMonthEnd();

		void SetQuarterBegin();
		void SetQuarterEnd();
		void SetPrevQuarter();
		void SetNextQuarter();

		void SetPrevSemiYear();
		void SetNextSemiYear();

		void SetYearBegin();
		void SetYearEnd();

		void AddDays(int iCount);
		void AddMonths(int iCount);
		void AddQuarters(int iCount);

		DMDateTime& operator+=(int iDayCount);
		DMDateTime& operator-=(int iDayCount);
		DMDateTime& operator=(const DMDateTime& dt);
		DMDateTime& operator=(int iDayCount);

		int Equal(const DMDateTime& dt);

	public:

		unsigned short m_iYear;
		BYTE m_iMonth;
		BYTE m_iDay;

		BYTE m_iHour;
		BYTE m_iMin;
		BYTE m_iSec;
	};

	class DMDateHelper
	{
	public:

		static void DaysToYMD(int iDate, unsigned short& iYear, BYTE& iMonth, BYTE& iDay);
		static int YMDToDays(int iYear, int iMonth, int iDay);
		static int MonthEndDay(int iYear, int iMonth);
		static bool IsLeapYear(int iYear);
	};

}//namespace DM