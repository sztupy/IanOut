#include "stdio.h"

#ifndef __ULIST_H
#define __ULIST_H

#define Boolean int
#define True  1
#define False 0

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
	Boolean Empty() { return FLast == NULL ? True : False; };
	PItem First() { return Empty () == True ? NULL : FLast->FNext; };
	PItem Last() { return FLast; };
	PItem Next(PItem Item) { return Item == FLast ? NULL : Item->FNext; };
	PItem RNext(PItem Item) { return Item->FNext; } ;
	PItem Prev(PItem Item) { return Item == First() ? NULL : Item->Prev(); };
	PItem RPrev(PItem Item) { return Item->Prev(); } ;

	Boolean Exists(PItem Item);
	Boolean Disconnect(PItem Item);
	void Delete(PItem Item);
	void DeleteAll();
	void Each(ItemProcedure DoToItem);
	PItem GetItem();

	virtual void Insert(PItem Item);
	virtual Boolean Found() { return True; };

public:
	PItem Current;

//private:
	PItem FLast;

	friend class TFRMChr;
};

#endif //__ULIST_H

