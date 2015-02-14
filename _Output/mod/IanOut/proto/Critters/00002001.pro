<?xml version="1.0"?>
<proto name="00001000" type="unique">
 <text yousee="Magdi" lookat="Megdi a pletykás vénasszony" ondead="Magdika hullája" ondeadlook="Phew... érte nem kár..."/>
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
   Char:Taunt(0,System.GetText("necrop.sve",15,16))
  </onuncommon>
  <ontalk>
   Char = Classes.GetCurrentChar()
   if Char.var1 == 0 then
     Text.SetFile("magdi.ios")
     Text.SetPosition(1)
     Mode.TalkMode()
   else
     Ian = Classes.GetIan()
     Ian:Taunt(1,System.GetText("necrop.sve",13,14))
   end
  </ontalk>
 </events>
</proto>