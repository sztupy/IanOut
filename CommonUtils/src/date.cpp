#include "date.h"

int date::Year;
int date::Month;
int date::Day;
int date::Hour;
int date::Minute;
int date::Second;

void date::AddSecond(int i)
{
	Second+=i;
	while (Second>=60) {
		Second-=60;
		AddMinute(1);
	}
	while (Second<0) {
		Second+=60;
		AddMinute(-1);
	}
}

void date::AddMinute(int i)
{
	Minute+=i;
	while (Minute>=60) {
		Minute-=60;
		AddHour(1);
	}
	while (Minute<0) {
		Minute+=60;
		AddHour(-1);
	}
}

void date::AddHour(int i)
{
	Hour+=i;
	while (Hour>=24) {
		Hour-=24;
		AddDay(1);
	}
	while (Hour<0) {
		Hour+=24;
		AddDay(-1);
	}
}

void date::AddDay(int i)
{
	Day+=i;
	int numday=30;
		switch (Month) {
			case 0:case 2: case 4: case 6:case 7:case 9:case 11:numday=31;break;
			case 3:case 5: case 8: case 10:numday=30;break;
			case 1:if ((Year%4==0) && ((Year%100 !=0) || (Year%400 == 0))) numday=29; else numday=28;break;
		}
	while (Day>numday) {
		Day-=numday;
		AddMonth(1);
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
		AddMonth(-1);
	}
}

void date::AddMonth(int i)
{
	Month+=i;
	while (Month>=12) {
		Month-=12;
		AddYear(1);
	}
	while (Month<0) {
		Month+=12;
		AddYear(-1);
	}
}

void date::AddYear(int i)
{
	Year+=i;
}