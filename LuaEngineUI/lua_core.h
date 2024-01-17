#pragma once
#include <windows.h>
#include <sdkddkver.h>

#include <sstream>
#include <vector>

#include "imgui.h"
#include "sol_ImGui.h"
#include <map>

#define DllExport   __declspec( dllimport )

// 在 lua_caller 命名空间中定义一个函数，用于调用 LuaEngine.dll 中的 lua_core 命名空间中的 Lua_Run 函数
namespace LuaCore {
    struct LuaScriptData {
        lua_State* L;
        std::string name;
        std::string file;
        bool start;
        LuaScriptData(
            lua_State* L = nullptr,
            std::string name = "",
            std::string file = "",
            bool start = true
        ) :L(L), name(name), file(file), start(start) { };
    };
    bool initUI = false;
    DllExport extern time_t reloadTime;
    time_t reload;
    DllExport extern void run(std::string func, lua_State* runL = nullptr);
    DllExport extern std::vector<std::string> getLuaFiles();
    DllExport extern std::map<std::string, LuaScriptData> getLuas();
    void Imgui_Bindings() {
        for (std::string file_name : getLuaFiles()) {
            LuaScriptData luae = getLuas()[file_name];
            if (luae.start) {
                sol::state_view lua(luae.L);
                sol_ImGui::Init(lua);
            }
        }
        initUI = true;
    }
}
