#ifndef __ULIST_H
#define __ULIST_H

#include "stdio.h"

/*#define Boolean int
#define True  1
#define False 0*/

class TItem;
class TList;

typedef TItem *PItem;
typedef TList *PList;

typedef void (*ItemProcedure)(PItem Item);

//////////////////////////////////////////////////////////////////
//
// TItem

class TItem
{
public:
	TItem() {};
	virtual ~TItem() {};

	PItem Next() { return FNext; };
	PItem Prev();

private:
	PItem FNext;

	friend class TList;
	friend class TFRMList;
	friend class TFRMChr;
};

//////////////////////////////////////////////////////////////////
//
// TList

class TList : public TItem
{
public:
	TList();
	~TList(); 
	bool Empty() { return FLast == NULL ? true : false; };
	PItem First() { return Empty () == true ? NULL : FLast->FNext; };
	PItem Last() { return FLast; };
	PItem Next(PItem Item) { return Item == FLast ? NULL : Item->FNext; };
	PItem RNext(PItem Item) { return Item->FNext; } ;
	PItem Prev(PItem Item) { return Item == First() ? NULL : Item->Prev(); };
	PItem RPrev(PItem Item) { return Item->Prev(); } ;

	bool Exists(PItem Item);
	bool Disconnect(PItem Item);
	void Delete(PItem Item);
	void DeleteAll();
	void Each(ItemProcedure DoToItem);
	PItem GetItem();

	virtual void Insert(PItem Item);
	virtual bool Found() { return true; };

public:
	PItem Current;

//private:
	PItem FLast;

	//friend class TFRMChr;
};

#endif //__ULIST_H

