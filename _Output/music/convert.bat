set MUSIC=01hub 03wrldmp 05raider 07desert 08vats 10labone 12junktn 13carvrn 14necro 16follow 17arroyo 18modoc 19reno 20car 21sf 22vcity 23world 24redd akiss wind1 wind2

for %%a in (%MUSIC%) do (
  if exist %%a.acm (
    acm2wav %%a.acm
    oggenc %%a.wav
    del %%a.acm
    del %%a.wav
  )
)
