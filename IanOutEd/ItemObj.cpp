#include "stdio.h"
#include "ItemObj.h"

//////////////////////////////////////////////////////////////////
//
// TItem

PItem TItem::Prev()
{
	PItem Item;
	Item = this;
	while ( Item->FNext != this )
		Item = Item->FNext;
	return Item;
}

//////////////////////////////////////////////////////////////////
//
// TList

TList::TList()
{
	FLast = NULL;
}

Boolean TList::Exists(PItem Item)
{
	PItem Cur;

	if ( Empty() )
		return False;

	Cur = FLast->FNext;
	while (Cur != Item && Cur!= FLast )
		 Cur = Cur->FNext;

	return Cur == Item ? True : False;
}

void TList::Insert(PItem Item)
{
	if (!Item )
		return;

	if ( Empty() )
		Item->FNext = Item;
	else
	{
		Item->FNext = FLast->FNext;
		FLast->FNext = Item;
	}
	FLast = Item;
}

Boolean TList::Disconnect(PItem Item)
{
	PItem Cur;
	if ( Empty() ) return False;

	Cur = FLast;
	while (Cur->FNext != Item && Cur->FNext != FLast )
		 Cur = Cur -> FNext;

	if ( Cur->FNext == Item)
	{
		Cur ->FNext = Item->FNext;
		if (FLast == Item)
			if ( Cur == Item)
				FLast = NULL;
			else
				FLast = Cur;
		return True;
	}
	return False;
}

void TList::Delete(PItem Item)
{
	if ( Disconnect(Item) ) delete Item;
}

void TList::DeleteAll()
{
	while (!Empty()) Delete(First());
}

void TList::Each(ItemProcedure DoToItem)
{
	PItem Item;

	Item = First();
	while ( Item )
	{
		DoToItem(Item);
		Item = Next(Item);
	}
}

PItem TList::GetItem()
{
	Current = First();

	while ( Current && !Found() )
		Current = Next(Current);

	return Current;
}

TList::~TList()
{
	DeleteAll();
}