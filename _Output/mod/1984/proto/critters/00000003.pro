<?xml version="1.0"?>
<proto name="00000001" type="Ian">
 <text yousee="Ian" lookat="Ian" ondead="Ian!" ondeadlook="Ez te vagy"/>
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
     Char:Taunt(2,System.GetText("emberek.sve",1,8))
   else
     Char:MoveRandom(2)
   end
  </oncommon>
 </events>
</proto>