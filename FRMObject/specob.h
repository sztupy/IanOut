#ifndef __SPECOB_H
#define __SPECOB_H

#include <string>
#include "../commonutils/itemobj.h"

class TTimerObj;
class TTauntObj;

typedef TTimerObj *PTimerObj;
typedef TTauntObj *PTauntObj;

class TTimerObj : public TItem
{
public:
	TTimerObj(int IYear, int IMonth, int IDay, int IHour, int IMinute, int ISecond, std::string IToDo);
	~TTimerObj() {};
	int Year, Month, Day, Hour, Minute, Second;
	std::string ToDo;

	void ValSecond();
	void ValMinute(int i);
	void ValHour(int i);
	void ValDay(int i);
	void ValMonth(int i);
	void ValYear(int i);

	bool Elapsed();
};

class TTauntObj : public TItem
{
public:
	TTauntObj(int ix, int iy, int col, std::string ITaunt)
	  {
		  Taunt = ITaunt;
		  x = ix;
		  y = iy;
		  TimeOut = 50;
		  co = col;
	  };
	~TTauntObj() {};
	int x,y;
	int co;
	int TimeOut;
	std::string Taunt;
};

#endif // __SPECOB_H