#include "DmMainAfx.h"
#include "DMDateTime.h"
#include <time.h>

namespace DM
{
	static const int PQUARTERSDAYS[]={0, 12, 12, 31, 3, 3, 30, 6, 6, 30, 9, 9, 31};
	static const int NQUARTERSDAYS[]={0, 3, 3, 31, 6, 6, 30, 9, 9, 30, 12, 12, 31};

	static const int PSEMIYEARDAYS[]={0, 12, 12, 12, 12, 12, 30, 6, 6, 6, 6, 6, 31};
	static const int NSEMIYEARDAYS[]={0, 6, 6, 6, 6, 6, 30, 12, 12, 12, 12, 12, 31};

	DMDateTime::DMDateTime()
	{
		ResetToday();
	}

	DMDateTime::DMDateTime(const DMDateTime & dt)
	{
		m_iYear = dt.m_iYear;
		m_iMonth = dt.m_iMonth;
		m_iDay = dt.m_iDay;
		m_iHour = dt.m_iHour;
		m_iMin = dt.m_iMin;
		m_iSec = dt.m_iSec;
	}

	DMDateTime::DMDateTime(int dt)
	{
		SetDate(dt);
	}

	DMDateTime::DMDateTime(int iYear, int iMonth, int iDay)
		: m_iYear(1900)
		, m_iMonth(1)
		, m_iDay(1)
	{
		SetDate(iYear, iMonth, iDay);
	}

	DMDateTime::DMDateTime(const CStringW & strDate)
	{
		SetDate(strDate);
	}

	bool DMDateTime::IsValid() const
	{
		if (m_iYear >= 1900 && m_iMonth >= 1 
			&& m_iMonth <= 12)
		{
			if (m_iDay <= GetMonthEndDay()
				&& m_iHour >= 0 && m_iHour <= 23
				&& m_iMin >= 0 && m_iMin <= 59
				&& m_iSec >= 0 && m_iSec <= 59)
			{
				return true;
			}
		}
		return false;
	}

	DMDateTime DMDateTime::Today()
	{
		return DMDateTime();
	}

	void DMDateTime::Reset()
	{

	}

	void DMDateTime::ResetToday()
	{
		struct tm Tm = {0};
		time_t tCount = time(NULL);
		localtime_s(&Tm, &tCount);

		m_iYear = Tm.tm_year + 1900;
		m_iMonth = Tm.tm_mon + 1;
		m_iDay = Tm.tm_mday;
		m_iHour = Tm.tm_hour;
		m_iMin = Tm.tm_min;
		m_iSec = Tm.tm_sec;
	}

	void DMDateTime::SetYear(unsigned short val)
	{
		if (val >= 1900)
		{
			m_iYear = val;
		}
	}

	void DMDateTime::SetMonth(BYTE val)
	{
		if (val >= 1 && val <= 12)
		{
			m_iMonth = val;
		}
	}

	void DMDateTime::SetDayOfMonth(BYTE val)
	{
		int iMonEnd = DMDateHelper::MonthEndDay(GetYear(), GetMonth());
		if (val >= 1 && val <= iMonEnd)
		{
			m_iDay = val;
		}
	}

	BYTE DMDateTime::GetWeekDay() const
	{
		struct tm Tm = {0};

		Tm.tm_year = m_iYear - 1900;
		Tm.tm_mon = m_iMonth - 1;
		Tm.tm_mday = m_iDay;

		time_t tCount = mktime(&Tm);
		localtime_s(&Tm, &tCount);

		return Tm.tm_wday;
	}

	unsigned short DMDateTime::GetYearDay() const
	{
		struct tm Tm = {0};

		Tm.tm_year = m_iYear - 1900;
		Tm.tm_mon = m_iMonth - 1;
		Tm.tm_mday = m_iDay;

		time_t tCount = mktime(&Tm);
		localtime_s(&Tm, &tCount);

		int iDays = DMDateHelper::YMDToDays(m_iYear, m_iMonth, m_iDay);
		int iBegDays = DMDateHelper::YMDToDays(m_iYear, 1, 1);

		return (iDays - iBegDays);
	}

	int DMDateTime::GetDayCount() const
	{
		return DMDateHelper::YMDToDays(m_iYear, m_iMonth, m_iDay);
	}

	int DMDateTime::GetMonthEndDay() const
	{
		return DMDateHelper::MonthEndDay(m_iYear, m_iMonth);
	}

	void DMDateTime::SetDate(const CStringW& strDate)
	{
		ResetToday();
		CStringWList vecms;
		CStringWList vec;
		SplitStringT(strDate,L' ',vecms);
		if ((int)vecms.GetCount() == 1)
		{
			SplitStringT(strDate,L'-',vec);
		}
		else if ((int)vecms.GetCount() >= 2)
		{
			SplitStringT(vecms[0],L'-',vec);
			CStringWList vecs;
			SplitStringT(vecms[1],L':',vecs);

			if ((int)vecs.GetCount() == 3)
			{
				m_iHour = _wtoi(vecs[0]);
				m_iMin = _wtoi(vecs[1]);
				m_iSec = _wtoi(vecs[2]);
			}
		}

		if ((int)vec.GetCount() == 3)
		{
			SetDate(_wtoi(vec[0]), _wtoi(vec[1]), _wtoi(vec[2]));
		}
	}

	void DMDateTime::SetDate(const DMDateTime& dt)
	{
		m_iYear = dt.m_iYear;
		m_iMonth = dt.m_iMonth;
		m_iDay = dt.m_iDay;
	}

	void DMDateTime::SetDate(int iYear, int iMonth, int iDay)
	{
		if (iYear >= 1900 && iMonth >= 1 
			&& iMonth <= 12 
			&& iDay <= DMDateHelper::MonthEndDay(iYear, iMonth))
		{
			m_iYear = iYear;
			m_iMonth = iMonth;
			m_iDay = iDay;
		}
	}

	void DMDateTime::SetDate(int iDayCount)
	{
		DMDateHelper::DaysToYMD(iDayCount, m_iYear, m_iMonth, m_iDay);
	}

	void DMDateTime::SetTime(BYTE h, BYTE m, BYTE s)
	{
		m_iHour = h;
		m_iMin = m;
		m_iSec = s;
	}

	CStringW DMDateTime::ToString()
	{
		CStringW strFmt;
		strFmt.Format(_T("%04d-%02d-%02d"), m_iYear, m_iMonth, m_iDay);
		return strFmt;
	}

	CStringW DMDateTime::ToTime()
	{
		CStringW strFmt;
		strFmt.Format(_T("%02d:%02d:%02d"), m_iHour, m_iMin, m_iSec);
		return strFmt;
	}

	CStringW DMDateTime::ToLongDate()
	{
		CStringW strFmt;

		strFmt.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), 
			m_iYear, m_iMonth, m_iDay, m_iHour, m_iMin, m_iSec);
		return strFmt;
	}

	unsigned short DMDateTime::GetYear() const
	{
		return m_iYear;
	}

	BYTE DMDateTime::GetMonth() const
	{
		return m_iMonth;
	}

	BYTE DMDateTime::GetDay() const
	{
		return m_iDay;
	}

	BYTE DMDateTime::GetHour() const
	{
		return m_iHour;
	}

	BYTE DMDateTime::GetMinute() const
	{
		return m_iMin;
	}

	BYTE DMDateTime::GetSecond() const
	{
		return m_iSec;
	}

	bool DMDateTime::IsQuarterEnd()
	{
		if ((m_iMonth % 3) ==0 && IsMonthEnd())
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	bool DMDateTime::IsMonthEnd()
	{
		return (m_iDay == GetMonthEndDay());
	}

	void DMDateTime::SetMonthBegin()
	{
		m_iDay = 1;
	}

	void DMDateTime::SetMonthEnd()
	{
		m_iDay = GetMonthEndDay();
	}

	void DMDateTime::SetQuarterBegin()
	{
		m_iDay = 31;
		m_iMonth = 3;
	}

	void DMDateTime::SetQuarterEnd()
	{
		m_iDay = 31;
		m_iMonth = 12;
	}

	void DMDateTime::SetPrevQuarter()
	{
		int iPrev = PQUARTERSDAYS[m_iMonth];

		if (iPrev < 28)
		{
			m_iMonth = iPrev;
			m_iDay = PQUARTERSDAYS[iPrev];
			m_iYear -= (iPrev / 12);
		}
		else
		{
			if (m_iDay != iPrev)
			{
				m_iMonth -= 1;
				SetPrevQuarter();
			}
			else
			{
				m_iDay = iPrev;
			}
		}
	}

	void DMDateTime::SetNextQuarter()
	{
		int iNext = NQUARTERSDAYS[m_iMonth];

		if (iNext < 28)
		{
			m_iMonth = iNext;
			m_iDay = NQUARTERSDAYS[iNext];
		}
		else
		{
			m_iDay = iNext;
		}
	}

	void DMDateTime::SetPrevSemiYear()
	{
		int iPrev = PSEMIYEARDAYS[m_iMonth];

		if (iPrev < 28)
		{
			m_iMonth = iPrev;
			m_iDay = PSEMIYEARDAYS[iPrev];
			m_iYear -= (iPrev / 12);
		}
		else
		{
			if (m_iDay != iPrev)
			{
				m_iMonth -= 1;
				SetPrevSemiYear();
			}
			else
			{
				m_iDay = iPrev;
			}
		}
	}

	void DMDateTime::SetNextSemiYear()
	{
		int iNext = NSEMIYEARDAYS[m_iMonth];

		if (iNext < 28)
		{
			m_iMonth = iNext;
			m_iDay = NSEMIYEARDAYS[iNext];
		}
		else
		{
			m_iDay = iNext;
		}
	}

	void DMDateTime::SetYearBegin()
	{
		m_iMonth = 1;
		m_iDay = 1;
	}

	void DMDateTime::SetYearEnd()
	{
		m_iMonth = 12;
		m_iDay = 31;
	}

	void DMDateTime::AddDays(int iCount)
	{
		SetDate(GetDayCount() + iCount);
	}

	void DMDateTime::AddMonths(int iCount)
	{
		iCount += m_iMonth;

		if (iCount <= 0)
		{
			m_iYear += iCount / 12 - 1;
			m_iMonth = (iCount % 12) + 12;
		}
		else
		{
			iCount -= 1;
			m_iMonth = (iCount % 12) + 1;
			m_iYear += iCount / 12;
		}

		if (GetMonthEndDay() < m_iDay)
		{
			m_iDay = GetMonthEndDay();
		}
	}

	void DMDateTime::AddQuarters(int iCount)
	{
		AddMonths(3 * iCount);
	}

	DMDateTime& DMDateTime::operator+=(int iDayCount)
	{
		iDayCount += DMDateHelper::YMDToDays(m_iYear, m_iMonth, m_iDay);
		SetDate(iDayCount);

		return (*this);
	}

	DMDateTime& DMDateTime::operator-=(int iDayCount)
	{
		iDayCount = DMDateHelper::YMDToDays(m_iYear, m_iMonth, m_iDay) - iDayCount;
		SetDate(iDayCount);

		return (*this);
	}

	DMDateTime& DMDateTime::operator=(const DMDateTime & dt)
	{
		if (&dt != this)
		{
			m_iYear = dt.m_iYear;
			m_iMonth = dt.m_iMonth;
			m_iDay = dt.m_iDay;
		}

		return (*this);
	}

	DMDateTime& DMDateTime::operator=(int iDayCount)
	{
		SetDate(iDayCount);
		return (*this);
	}

	int DMDateTime::Equal(const DMDateTime& dt)
	{
#define COMPAREVAL(L,R) if(L<(R)){return -1;}else if(L>R){return 1;}

		COMPAREVAL(m_iYear,dt.m_iYear)
		COMPAREVAL(m_iMonth,dt.m_iMonth)
		COMPAREVAL(m_iDay,dt.m_iDay)

		return 0;
	}



	/// DateHelp------------------------------------------
	static const int MONTHDAYS[]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	void DMDateHelper::DaysToYMD(int iDate, unsigned short& iYear, BYTE& iMonth, BYTE& iDay)
	{
		int a = iDate + 32044 + 2415019;
		int b = (4 * a + 3) / 146097;
		int c = a-((146097 * b) / 4);
		int d = (4 * c + 3) / 1461;
		int e = c - (1461 * d) / 4;
		int m = (5 * e + 2) / 153;

		iDay = static_cast<int>(e - ((153 * m + 2) / 5) + 1);
		iMonth = static_cast<int>(m + 3 - 12 * (m / 10));
		iYear = static_cast<int>(100 * b + d - 4800 + (m / 10));
	}

	int DMDateHelper::YMDToDays(int iYear, int iMonth, int iDay)
	{
		unsigned short a = static_cast<unsigned short>((14 - iMonth) / 12);
		unsigned short y = static_cast<unsigned short>(iYear + 4800 - a);
		unsigned short m = static_cast<unsigned short>(iMonth + 12 * a - 3);
		int iDate = iDay + ((153 * m + 2) / 5) + 365 * y 
			+ (y / 4) - (y / 100) + (y / 400) - 32045 - 2415019;

		return iDate;
	}

	int DMDateHelper::MonthEndDay(int iYear, int iMonth)
	{
		int nDay = 0;

		nDay = MONTHDAYS[iMonth - 1];

		if (iMonth == 2 && DMDateHelper::IsLeapYear(iYear))
		{
			nDay++;
		}

		return nDay;
	}

	bool DMDateHelper::IsLeapYear(int iYear)
	{
		if ((iYear % 400) == 0 
			|| ((iYear % 4) == 0 && (iYear % 100) != 0))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}//namespace DM