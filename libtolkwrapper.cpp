/* References
   - https://blog.devgenius.io/how-to-add-your-c-library-in-lua-46fd246f0fa8
   - https://stackoverflow.com/questions/43980470/how-to-create-dll-of-lua-module (For correct build command in windows)
*/

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include <lua.h>
#include <lauxlib.h>
#include <windows.h>

    typedef void(__cdecl *Tolk_Load)();
    typedef bool(__cdecl *Tolk_IsLoaded)();
    typedef void(__cdecl *Tolk_Unload)();
    typedef void(__cdecl *Tolk_TrySAPI)(bool trySAPI);
    typedef void(__cdecl *Tolk_PreferSAPI)(bool preferSAPI);
    typedef const wchar_t *(__cdecl *Tolk_DetectScreenReader)();
    typedef bool(__cdecl *Tolk_HasSpeech)();
    typedef bool(__cdecl *Tolk_HasBraille)();
    typedef bool(__cdecl *Tolk_Output)(const wchar_t *str, bool interrupt);
    typedef bool(__cdecl *Tolk_Speak)(const wchar_t *str, bool interrupt);
    typedef bool(__cdecl *Tolk_Braille)(const wchar_t *str);
    typedef bool(__cdecl *Tolk_IsSpeaking)();
    typedef bool(__cdecl *Tolk_Silence)();

    HINSTANCE hGetProcIDDLL;
    Tolk_Load func_tolk_load;
    Tolk_IsLoaded func_tolk_isLoaded;
    Tolk_Unload func_tolk_unload;
    Tolk_TrySAPI func_tolk_trySAPI;
    Tolk_PreferSAPI func_tolk_preferSAPI;
    Tolk_DetectScreenReader func_tolk_detectScreenReader;
    Tolk_HasSpeech func_tolk_hasSpeech;
    Tolk_HasBraille func_tolk_hasBraille;
    Tolk_Output func_tolk_output;
    Tolk_Speak func_tolk_speak;
    Tolk_Braille func_tolk_braille;
    Tolk_IsSpeaking func_tolk_isSpeaking;
    Tolk_Silence func_tolk_silence;

    const wchar_t *charToWChatT(const char *c)
    {
        const size_t cSize = strlen(c) + 1;
        wchar_t *wc = new wchar_t[cSize];
        mbstowcs(wc, c, cSize);
        return wc;
    }

    const char *wChatTToChar(const wchar_t *c)
    {
        // get the number of characters in the string.
        int currentCharIndex = 0;
        char currentChar = c[currentCharIndex];
        while (currentChar != '\0')
        {
            currentCharIndex++;
            currentChar = c[currentCharIndex];
        }
        const int charCount = currentCharIndex + 1;
        // allocate a new block of memory size char (1 byte) instead of wide char (2 bytes)
        char *filePathC = (char *)malloc(sizeof(char) * charCount);
        for (int i = 0; i < charCount; i++)
        {
            // convert to char (1 byte)
            char character = c[i];
            *filePathC = character;
            filePathC += sizeof(char);
        }
        filePathC += '\0';
        filePathC -= (sizeof(char) * charCount);
        return filePathC;
    }

    static int l_tolk_load(lua_State *L)
    {
        if (!hGetProcIDDLL || !func_tolk_load) return 1;
        func_tolk_load();
        return 1;
    }

    static int l_tolk_isLoaded(lua_State *L)
    {
        if (!hGetProcIDDLL || !func_tolk_isLoaded) return 1;
        printf("%d", func_tolk_isLoaded());
        lua_pushboolean(L, func_tolk_isLoaded());
        return 1;
    }

    static int l_tolk_unload(lua_State *L)
    {
        if (!hGetProcIDDLL || !func_tolk_unload)
            return 1;
        func_tolk_unload();
        return 1;
    }

    static int l_tolk_trySAPI(lua_State *L)
    {
        if (!hGetProcIDDLL || !func_tolk_trySAPI) return 1;
        bool trySAPI = lua_toboolean(L, 1);
        func_tolk_trySAPI(trySAPI);
        return 1;
    }

    static int l_tolk_preferSAPI(lua_State *L)
    {
        if (!hGetProcIDDLL || !func_tolk_preferSAPI) return 1;
        bool preferSAPI = lua_toboolean(L, 1);
        func_tolk_preferSAPI(preferSAPI);
        return 1;
    }

    static int l_tolk_detectScreenReader(lua_State *L)
    {
        if (!hGetProcIDDLL || !func_tolk_detectScreenReader)
            return 1;
        lua_pushstring(L, wChatTToChar(func_tolk_detectScreenReader()));
        return 1;
    }

    static int l_tolk_hasSpeech(lua_State *L)
    {
        if (!hGetProcIDDLL || !func_tolk_hasSpeech) return 1;
        lua_pushboolean(L, func_tolk_hasSpeech());
        return 1;
    }

    static int l_tolk_hasBraille(lua_State *L)
    {
        if (!hGetProcIDDLL || !func_tolk_hasBraille) return 1;
        lua_pushboolean(L, func_tolk_hasBraille());
        return 1;
    }

    static int l_tolk_output(lua_State *L)
    {
        if (!hGetProcIDDLL || !func_tolk_output)
            return 1;

        const char *text = luaL_checkstring(L, 1);
        bool interrupt = lua_toboolean(L, 2);
        lua_pushboolean(L, func_tolk_output(charToWChatT(text), interrupt));

        return 1;
    }

    static int l_tolk_speak(lua_State *L)
    {
        if (!hGetProcIDDLL || !func_tolk_speak)
            return 1;

        const char *text = luaL_checkstring(L, 1);
        bool interrupt = lua_toboolean(L, 2);
        lua_pushboolean(L, func_tolk_speak(charToWChatT(text), interrupt));

        return 1;
    }

    static int l_tolk_braille(lua_State *L)
    {
        if (!hGetProcIDDLL || !func_tolk_braille)
            return 1;

        const char *text = luaL_checkstring(L, 1);
        lua_pushboolean(L, func_tolk_braille(charToWChatT(text)));

        return 1;
    }

    static int l_tolk_isSpeaking(lua_State *L)
    {
        if (!hGetProcIDDLL || !func_tolk_isSpeaking)
            return 1;
        lua_pushboolean(L, func_tolk_isSpeaking());
        return 1;
    }

    static int l_tolk_silence(lua_State *L)
    {
        if (!hGetProcIDDLL || !func_tolk_silence)
            return 1;
        lua_pushboolean(L, func_tolk_silence());
        return 1;
    }

    int luaopen_libtolkwrapper(lua_State *L)
    {
        hGetProcIDDLL = LoadLibrary(".\\Tolk.dll");
        if (!hGetProcIDDLL)
        {
            printf("\nCould not load Tolk.dll\n");
            return 1;
        }

        func_tolk_load = (Tolk_Load)GetProcAddress(hGetProcIDDLL, "Tolk_Load");
        if (!func_tolk_load)
        {
            printf("\nCould not load the function: Tolk_Load\n");
            return 1;
        }

        func_tolk_isLoaded = (Tolk_IsLoaded)GetProcAddress(hGetProcIDDLL, "Tolk_IsLoaded");
        if (!func_tolk_isLoaded)
        {
            printf("\nCould not load the function: Tolk_IsLoaded\n");
            return 1;
        }

        func_tolk_unload = (Tolk_Unload)GetProcAddress(hGetProcIDDLL, "Tolk_Unload");
        if (!func_tolk_unload)
        {
            printf("\nCould not load the function: Tolk_Unload\n");
            return 1;
        }

        func_tolk_trySAPI = (Tolk_TrySAPI)GetProcAddress(hGetProcIDDLL, "Tolk_TrySAPI");
        if (!func_tolk_trySAPI)
        {
            printf("\nCould not load the function: Tolk_TrySAPI\n");
            return 1;
        }

        func_tolk_preferSAPI = (Tolk_PreferSAPI)GetProcAddress(hGetProcIDDLL, "Tolk_PreferSAPI");
        if (!func_tolk_preferSAPI)
        {
            printf("\nCould not load the function: Tolk_PreferSAPI\n");
            return 1;
        }

        func_tolk_detectScreenReader = (Tolk_DetectScreenReader)GetProcAddress(hGetProcIDDLL, "Tolk_DetectScreenReader");
        if (!func_tolk_detectScreenReader)
        {
            printf("\nCould not load the function: Tolk_DetectScreenReader\n");
            return 1;
        }

        func_tolk_hasSpeech = (Tolk_HasSpeech)GetProcAddress(hGetProcIDDLL, "Tolk_HasSpeech");
        if (!func_tolk_hasSpeech)
        {
            printf("\nCould not load the function: Tolk_HasSpeech\n");
            return 1;
        }

        func_tolk_hasBraille = (Tolk_HasBraille)GetProcAddress(hGetProcIDDLL, "Tolk_HasBraille");
        if (!func_tolk_hasBraille)
        {
            printf("\nCould not load the function: Tolk_HasBraille\n");
            return 1;
        }

        func_tolk_output = (Tolk_Output)GetProcAddress(hGetProcIDDLL, "Tolk_Output");
        if (!func_tolk_output)
        {
            printf("\nCould not load the function: Tolk_Output\n");
            return 1;
        }

        func_tolk_speak = (Tolk_Speak)GetProcAddress(hGetProcIDDLL, "Tolk_Speak");
        if (!func_tolk_speak)
        {
            printf("\nCould not load the function: Tolk_Speak\n");
            return 1;
        }

        func_tolk_braille = (Tolk_Braille)GetProcAddress(hGetProcIDDLL, "Tolk_Braille");
        if (!func_tolk_braille)
        {
            printf("\nCould not load the function: Tolk_Braille\n");
            return 1;
        }

        func_tolk_isSpeaking = (Tolk_IsSpeaking)GetProcAddress(hGetProcIDDLL, "Tolk_IsSpeaking");
        if (!func_tolk_isSpeaking)
        {
            printf("\nCould not load the function: Tolk_IsSpeaking\n");
            return 1;
        }

        func_tolk_silence = (Tolk_Silence)GetProcAddress(hGetProcIDDLL, "Tolk_Silence");
        if (!func_tolk_silence)
        {
            printf("\nCould not load the function: Tolk_Silence\n");
            return 1;
        }

        static const struct luaL_Reg tolkwrapperLib[] = {
            {"Load", l_tolk_load},
            {"IsLoaded", l_tolk_isLoaded},
            {"Unload", l_tolk_unload},
            {"TrySAPI", l_tolk_trySAPI},
            {"PreferSAPI", l_tolk_preferSAPI},
            {"DetectScreenReader", l_tolk_detectScreenReader},
            {"HasSpeech", l_tolk_hasSpeech},
            {"HasBraille", l_tolk_hasBraille},
            {"Output", l_tolk_output},
            {"Speak", l_tolk_speak},
            {"Braille", l_tolk_braille},
            {"IsSpeaking", l_tolk_isSpeaking},
            {"Silence", l_tolk_silence},
            {NULL, NULL}};
        luaL_register(L, "tolkwrapperLib", tolkwrapperLib);
        return 1;
    }

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus