<?xml version="1.0"?>
<proto name="00001000" type="unique">
 <text yousee="Zsuzsa" lookat="Zsuzsa, Necropolis vezetõje" ondead="Zsuzsa hullája" ondeadlook="Valaha õ volt Necropolis vezetõje"/>
 <unarmed hand1="100000" hand2="100000"/>
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
   Char:MoveRandom(2)
  </oncommon>
  <onuncommon>
   Char = Classes.GetCurrentChar()
   Char:Taunt(0,System.GetText("necrop.sve",11,12))
  </onuncommon>
  <ontalk>
   Char = Classes.GetCurrentChar()
   Char:Taunt(0,System.GetText("necrop.sve",18,18))
  </ontalk>
 </events>
</proto>