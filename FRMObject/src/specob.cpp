#include "specob.h"
#include "../commonutils/date.h"

TTimerObj::TTimerObj(int IYear, int IMonth, int IDay, int IHour, int IMinute, int ISecond, std::string IToDo)
{
	ToDo = IToDo;
	if (IYear<=1000) {
		Year = date::Year+IYear;
		Month = date::Month+IMonth;
		Day = date::Day+IDay;
		Hour = date::Hour+IHour;
		Minute = date::Minute+IMinute;
		Second = date::Second+ISecond;
		ValSecond();
	} else {
		Year = IYear;
		Month = IMonth;
		Day = IDay;
		Hour = IHour;
		Minute = IMinute;
		Second = ISecond;
		ValSecond();
	}
};

void TTimerObj::ValSecond()
{
	while (Second>=60) {
		Second-=60;
		ValMinute(1);
	}
	while (Second<0) {
		Second+=60;
		ValMinute(-1);
	}
}

void TTimerObj::ValMinute(int i)
{
	Minute+=i;
	while (Minute>=60) {
		Minute-=60;
		ValHour(1);
	}
	while (Minute<0) {
		Minute+=60;
		ValHour(-1);
	}
}

void TTimerObj::ValHour(int i)
{
	Hour+=i;
	while (Hour>=24) {
		Hour-=24;
		ValDay(1);
	}
	while (Hour<0) {
		Hour+=24;
		ValDay(-1);
	}
}

void TTimerObj::ValDay(int i)
{
	Day+=i;
	int numday = 30;
		switch (Month) {
			case 0:case 2: case 4: case 6:case 7:case 9:case 11:numday=31;break;
			case 3:case 5: case 8: case 10:numday=30;break;
			case 1:if ((Year%4==0) && ((Year%100 !=0) || (Year%400 == 0))) numday=29; else numday=28;break;
		}
	while (Day>numday) {
		Day-=numday;
		ValMonth(1);
		switch (Month) {
			case 0:case 2: case 4: case 6:case 7:case 9:case 11:numday=31;break;
			case 3:case 5: case 8: case 10:numday=30;break;
			case 1:if ((Year%4==0) && ((Year%100 !=0) || (Year%400 == 0))) numday=29; else numday=28;break;
		}
	}
	while (Day<=0) {
		switch (Month>0 ? Month-1 : 12) {
			case 0:case 2: case 4: case 6:case 7:case 9:case 11:numday=31;break;
			case 3:case 5: case 8: case 10:numday=30;break;
			case 1:if ((Year%4==0) && ((Year%100 !=0) || (Year%400 == 0))) numday=29; else numday=28;break;
		}
		Day+=numday;
		ValMonth(-1);
	}
}

void TTimerObj::ValMonth(int i)
{
	Month+=i;
	while (Month>=12) {
		Month-=12;
		ValYear(1);
	}
	while (Month<0) {
		Month+=12;
		ValYear(-1);
	}
}

void TTimerObj::ValYear(int i)
{
	Year+=i;
}

bool TTimerObj::Elapsed()
{
	if (Year > date::Year) return false;
	if (Year < date::Year) return true;

	if (Month > date::Month) return false;
	if (Month < date::Month) return true;

	if (Day > date::Day) return false;
	if (Day < date::Day) return true;

	if (Hour > date::Hour) return false;
	if (Hour < date::Hour) return true;
		
	if (Minute > date::Minute) return false;
	if (Minute < date::Minute) return true;

	if (Second > date::Second) return false;
	return true;
}