require "libtolkluawrapper"

Tolk.TrySAPI(true);
Tolk.Load()
Tolk.Output("Hello There!", true)
print(Tolk.HasBraille())
print(Tolk.DetectScreenReader())
Tolk.Output("Hiii!", false)
Tolk.Unload()