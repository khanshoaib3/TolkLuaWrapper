require "libtolkwrapper"

tolkwrapperLib.TrySAPI(true);
tolkwrapperLib.Load()
tolkwrapperLib.Output("Hello There!", true)
print(tolkwrapperLib.HasBraille())
print(tolkwrapperLib.DetectScreenReader())
tolkwrapperLib.Output("Hiii!", false)
tolkwrapperLib.Unload()