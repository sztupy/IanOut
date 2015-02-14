<?xml version="1.0"?>
<proto name="00000100" type="global">
 <text yousee="Mantis" lookat="Az atomtámadás utáni legerõsebb ellenfél... Ian számára" ondead="Egy halott mantis" ondeadlook="Halott mantis... meg tudtad ölni... Juhé..."/>
 <unarmed hand1="200000" hand2="200000"/>
 <desc ability="1001001101"> 
  <onuse type="0">
   <direction x="0" y="0"/>
   <script2>
    if GVar.Get("NeedMantis") == 1 then
     if GVar.Get("GivenMantis") == 0 then
      GVar.Set("GivenMantis",1)
      Item = Classes.GetCurrentItem()
      Inventory = Item.inventory
      Inventory:AddItem(10,1)
     end
     Mode.LootBox()
    end
   </script2>
  </onuse>
 </desc>
 <events>
  <oncommon>
   Char = Classes.GetCurrentChar()
   Char:MoveRandom(2);
  </oncommon>
  <onkilled>
   GVar.Set("artalk",0)
  </onkilled>
 </events>
</proto>