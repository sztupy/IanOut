#ifndef __DATE_H
#define __DATE_H

namespace date {
	extern int Year;
	extern int Month;
	extern int Day;
	extern int Hour;
	extern int Minute;
	extern int Second;

	void AddSecond(int i);
	void AddMinute(int i);
	void AddHour(int i);
	void AddDay(int i);
	void AddMonth(int i);
	void AddYear(int i);
}


#endif // __DATE_H