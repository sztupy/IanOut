<?xml version="1.0"?>
<proto name="00001000" type="unique">
 <text yousee="Aradesh" lookat="Aradesh, Shady Sands vezetõje" ondead="Aradesh hullája" ondeadlook="Valaha õ volt Shady Sands vezetõje"/>
 <unarmed hand1="100000" hand2="100001"/>
 <desc ability="1001001101"> 
  <onuse type="0">
   <direction x="0" y="0"/>
   <script2>
    Mode.LootBox()
   </script2>
  </onuse>
 </desc>
 <events>
  <oncommon>
   Char = Classes.GetCurrentChar()
   if math.random(2) == 1 then
     Char:Taunt(0,System.GetText("aradesh.sve",1,8))
   else
     Char:MoveRandom(2)
   end
  </oncommon>
  <ontalk>
   System.RunFile("aradesh.ios")
  </ontalk>
 </events>
</proto>