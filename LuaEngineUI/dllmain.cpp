#include <Windows.h>
#include "D3D12Hook.h"
#include <utility>
#include <fstream>
#include "loader.h"
#include "util.h"
#include "lua_core.h"



static int CreateStatusAPI() {
	LuaCore::Lua_register("CheckImguiStatus", [](lua_State* pL) -> int
		{
			// 目前只返回固定值0，用于检测模块是否加载
			lua_pushinteger(pL, 0);
			return 0;
		});
}

DWORD WINAPI AttachThread(LPVOID lParam) {
	if (D3D12::Init() == D3D12::Status::Success) {
		D3D12::InstallHooks();
		// 为用户端提供一个检测状态的 API
		CreateStatusAPI();
	}
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, 0, &AttachThread, static_cast<LPVOID>(hModule), 0, nullptr);
		break;
	}
	case DLL_PROCESS_DETACH: {
		D3D12::RemoveHooks();
		break;
	}
	}
	return TRUE;
}

