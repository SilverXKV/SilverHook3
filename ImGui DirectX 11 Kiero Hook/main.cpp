#include "includes.h"
#include "sdk.h"
#include "kiero/minhook/include/MinHook.h"
#include <vector>
#include <Windows.h>
#include <iostream>
#include <string>
#include <Psapi.h>


//HOI4 Version 1.16.0

typedef bool(__fastcall* CSessionPost)(void* pThis, CCommand* pCommand, bool ForceSend);
CSessionPost CSessionPostHook;
CSessionPost CSessionPostTramp;

typedef CAddPlayerCommand* (__fastcall* GetCAddPlayerCommand)(void* pThis, CString* User, CString* Name, DWORD* unknown, int nMachineId, bool bHotjoin, __int64 a7);
GetCAddPlayerCommand CAddPlayerCommandHook;
GetCAddPlayerCommand CAddPlayerCommandTramp;

typedef CStartGameCommand* (__fastcall* GetCStartGameCommand)(void* pThis);
GetCStartGameCommand CStartGameCommandFunc;
GetCStartGameCommand CStartGameCommandTramp;

typedef CRemovePlayerCommand* (__fastcall* GetCRemovePlayerCommand)(void* pThis, int nMachineId, int eReason, long long a4);
GetCRemovePlayerCommand CRemovePlayerCommandHook;
GetCRemovePlayerCommand CRemovePlayerCommandTramp;

typedef CCrash* (__fastcall* GetCCrash)(void* pThis, unsigned int a1);
GetCCrash CCrashFunc;
GetCCrash CCrashTramp;

typedef CPauseGameCommand* (__fastcall* GetCPauseGameCommand)(void* pthis, __int64 a2, char a3);
GetCPauseGameCommand CPauseGameFunc;
GetCPauseGameCommand CPauseGameTramp;

typedef CAiEnableCommand* (__fastcall* GetEnableAI)(void* pThis, int* tag, int toggled);
GetEnableAI AIEnableFunc;

typedef EmptyTest* (__fastcall* GetBanUser)(void* pThis, unsigned int nMachineID);
GetBanUser BanUserFunc;
GetBanUser BanUserTramp;

typedef LPVOID(__fastcall* GetCCommand)(__int64 a1);
GetCCommand GetCCommandFunc;

typedef CGameSpeed* (__fastcall* GetCGameSpeed)(void* pThis);
GetCGameSpeed IncreaseSpeedFunc;
GetCGameSpeed DecreaseSpeedFunc;

typedef EmptyTest* (__fastcall* GetCustomDiffM)(void* pThis, __int64 a2, int a3);
GetCustomDiffM GCDMF;
GetCustomDiffM GCDMH;

typedef EmptyTest* (__fastcall* GetAutoSave)(void* pThis, bool Send);
GetAutoSave AutoSaveFunc;

typedef __int64(__fastcall* CGameStateSetPlayer)(void* pThis, int* Tag);
CGameStateSetPlayer CGameStateSetPlayerHook;
CGameStateSetPlayer CGameStateSetPlayerTramp;

typedef EmptyTest* (__fastcall* GetSessionCon)(void* pThis, void* NetAddress, void* GameName, int Type, int BasicType, int Status, CString* Version, bool AllowedIdReset);
GetSessionCon SeshFunc;
GetSessionCon SeshTramp;

typedef void(__fastcall* SetDebugTooltipsEnabled)(COption* pThis, bool bSelected);
SetDebugTooltipsEnabled SetDebugTooltipsEnabledHook;
SetDebugTooltipsEnabled SetDebugTooltipsEnabledTramp;

typedef bool(__fastcall* GetNetworkDisconnect)(void* pThis);
GetNetworkDisconnect NetDisconnectHook;
GetNetworkDisconnect NetDisconnectTramp;

typedef void(__fastcall* GetSessionSetState)(void* pThis, int a2);
GetSessionSetState SetStateHook;
GetSessionSetState SetStateTramp;

typedef void(__fastcall* GetMatchmakingLeaveLobby)(void* pThis);
GetMatchmakingLeaveLobby MatchmakingLeaveHook;
GetMatchmakingLeaveLobby MatchmakingLeaveTramp;

typedef bool (__fastcall *GetSendPacket)(_ENetHost** Handle, int nConnectionIndex, CBlob* pData, const unsigned int nDataSize, uint8_t nChannel);
GetSendPacket SendPacketHook;
GetSendPacket SendPacketTramp;

typedef void(__fastcall* GetFrontEndHostWithName)(void* pThis, CString* ServerName, CString* Password, CString* Desc, CString* Tags, int a6);
GetFrontEndHostWithName HostHook;
GetFrontEndHostWithName HostTramp;

typedef void* (__fastcall* GetCGameLobbyJoinGame)(void* pThis, CString* PlayerName, void* SNetAddress, CString* Password);
GetCGameLobbyJoinGame JoinGameHook;
GetCGameLobbyJoinGame JoinGameTramp;

typedef void(__fastcall* GetOnSessionChange)(void* pThis, __int64 Type);
GetOnSessionChange SessionChangeHook;
GetOnSessionChange SessionChangeTramp;

typedef void* (__fastcall* GetMultiplayerConfig)(void* pThis, void* a2, DWORD* PlayerName, void* Config, bool isGameOwner);
GetMultiplayerConfig MultiplayerConfigHook;
GetMultiplayerConfig MultiplayerConfigTramp;


typedef void(__fastcall* GetAddHumanItem)(void* pThis, CHuman* Human);
GetAddHumanItem HumanHook;
GetAddHumanItem HumanTramp;


typedef bool(__fastcall* GetConsoleMenu)(unsigned __int64 pThis, __int64 pThat);
GetConsoleMenu CConsoleHook;
GetConsoleMenu CConsoleTramp;


typedef void(__fastcall* GetServerAddress)(void* pThis, double nAddress);
GetServerAddress SetServerAddressHook;
GetServerAddress SetServerAddressTramp;

typedef bool(__fastcall* GetProxyServer)(void* pThis, bool bReconnect, int nMinTimeout, __int64 a4, __int64 a5);
GetProxyServer CProxyServerHook;
GetProxyServer CProxyServerTramp;

typedef void*(__fastcall* GetLogStream)(void* pThis, const char* Log);
GetLogStream CLogStreamHook;
GetLogStream CLogStreamTramp;

typedef void* (__fastcall* GetOperator)(void* pThis, __int64* Log);
GetOperator COperatorHook;
GetOperator COperatorTramp;

typedef void* (__fastcall* GetCLog)(void* pThis, void* a2, const char* pFile, int LineNum, int Category);
GetCLog CLogHook;
GetCLog CLogTramp;

uintptr_t FOWAddr, DBGAddr, TAGAddr, ALWTRAddr;

Menu eMenu;


CString* cServerName;
CString* cDesc;
CString* cTags;

//music test
bool musicEnabled = false;
bool lastMusicState = false; // To detect checkbox changes
std::string tempMusicPath;


bool bGameSpeed = false;
bool bSeeCombat = false;
bool bSeeArmy = false;
bool bSeeDebug = false;

char NameBuffer[64] = "filename";
char ExtensionBuffer[16] = ".hoi4";
CSaveGameMeta* LoadSaveMeta;
CSaveFile* AddExtMeta;
CCountryTag* gTag;
int* iLoad;
CString* gOutStr;

int FakeM = 1;
int FakeM2 = 50;
bool FakeSpammer = false;

//DEBUG
bool bShowPlayers = false;
bool dSessionPost = false;
bool Debug = false;
bool bLog = false;
bool bAddress = false;
bool bFindChost = false;
bool bConsoleAlloc = false;

std::string CurrentHost = "";

bool bRefuseConnect = false;

void* pCIdleLobby = nullptr;
int iHost = 0;

void* pCSessionConfigToken = nullptr;

void* pCGameLobby = nullptr;

//ChatMessage
//__int64 pCMessage = NULL;

//SessionPost
void* pCSession = nullptr;

//AddPlayerCommand
void* pCAddPlayer = nullptr;
DWORD* dUnknown;
int dMachine;
__int64 dPdx;
CString* dUser;
CString* dHname;
bool bSpoofSteam = false;
bool bJoinAsGhost = false;
int iMyMachineID;
int iMachineIDFake = 50;
__int64 iParadoxSocialID = 0;
CString* empty = new CString;

bool bGhost2 = false;
bool bGhost3 = false;

bool bLobbyHostRemoval = false;

//RemovePlayerCommand
void* pCRemovePlayer = nullptr;
ERemovalReason dReason;
int RMID;
__int64 dRUnknown;

//GameSetState
void* pCGameState = nullptr;

//Langauge
bool lEnglish = true;
bool lRussian = false;
bool HasNotChanged = false;

std::vector<std::string> IPVector;


COption* pSetDebugTooltipsEnabled = nullptr;

bool Flip = true;

//ImGui
bool bMenuOpen = true;
bool bCE = false;
bool bCrasher = false;
bool bXP = false;
char TagBuffer[16];
char PlayerName[1024];

bool bFreeTemp = false;
bool bFreeResearch = false;
bool bFreeMIO = false;
bool bFreeUpgrade = false;

//menus
bool bLobbyMenu = false;
bool bGameMenu = false;
bool bSettingsMenu = false;
bool bEnabletdebug = false;

unsigned int iLargestPacketSize = 1;
_ENetHost** LastENetHost;
CBlob* SwapData;
bool bShouldFreeze = false;

//Strengthen
int boost = 0;
__int64 CountryTag;

//Hook
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;
uintptr_t GameBase = (uintptr_t)GetModuleHandleA("hoi4.exe");




uintptr_t FindPattern(char* pattern, char* mask)
{
	uintptr_t base = GameBase;
	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), (HMODULE)GameBase, &modInfo, sizeof(MODULEINFO));
	uintptr_t size = modInfo.SizeOfImage;

	uintptr_t patternLength = (uintptr_t)strlen(mask);

	for (uintptr_t i = 0; i < size - patternLength; i++)
{
	bool found = true;
	for (uintptr_t j = 0; j < patternLength; j++)
	{
		if (mask[j] != '?' && pattern[j] != *(char*)(base + i + j))
		{
			found = false;
			break; // <- skip the rest of this iteration
		}
	}
	if (found)
		return base + i;
}

	return 0xDEADBEEF;
}




//Console

void printm(const std::string& str) {
	try {
		// Attempt to redirect stdout to console
		FILE* fDummy;
		if (freopen_s(&fDummy, "CONOUT$", "w", stdout) != 0) {
			throw std::runtime_error("Failed to redirect stdout to CONOUT$.");
		}

		// Check if str is valid (e.g., not null or too large)
		if (str.empty()) {
			std::cerr << "[SilverHook] Error: Empty string provided for printing." << std::endl;
			return;  // Don't attempt to print an empty string
		}

		// Print the message to the console
		std::cout << "[SilverHook] " << str << std::endl;
	}
	catch (const std::exception& e) {
		// Catch standard exceptions and print the error message
		std::cerr << "[SilverHook] Error: " << e.what() << std::endl;
	}
	catch (...) {
		// Catch any non-standard exceptions
		std::cerr << "[SilverHook] Unknown error occurred while printing." << std::endl;
	}
}


void printLog(const std::string& str) {
	try {
		// Attempt to redirect stdout to console
		FILE* fDummy;
		if (freopen_s(&fDummy, "CONOUT$", "w", stdout) != 0) {
			throw std::runtime_error("Failed to redirect stdout to CONOUT$.");
		}

		// Check if str is valid (e.g., not null or too large)
		if (str.empty()) {
			std::cerr << "[SilverLog] Error: Empty string provided for printing." << std::endl;
			return;  // Don't attempt to print an empty string
		}

		// Print the message to the console
		std::cout << "[SilverLog] " << str << std::endl;
	}
	catch (const std::exception& e) {
		// Catch standard exceptions and print the error message
		std::cerr << "[SilverLog] Error: " << e.what() << std::endl;
	}
	catch (...) {
		// Catch any non-standard exceptions
		std::cerr << "[SilverLog] Unknown error occurred while printing." << std::endl;
	}
}

void printRawData(void* ptr, size_t size) {
	unsigned char* data = reinterpret_cast<unsigned char*>(ptr);
	std::string output = "[Raw Data]: ";
	for (size_t i = 0; i < size; ++i) {
		output += std::to_string(data[i]) + " ";
	}
	printm(output);
}

/*void printm(std::string str)
{
	FILE* fDummy;
	freopen_s(&fDummy, "CONOUT$", "w", stdout);

	std::cout << "[SilverHook] " << str << std::endl;
}*/

uintptr_t FindPatternManual(char* pattern, char* mask)
{
	uintptr_t base = GameBase;
	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), (HMODULE)GameBase, &modInfo, sizeof(MODULEINFO));
	uintptr_t size = modInfo.SizeOfImage;

	uintptr_t patternLength = (uintptr_t)strlen(mask);
	
	for (uintptr_t i = 0; i < size - patternLength; i++)
	{
		bool found = true;

		for (uintptr_t j = 0; j < patternLength; j++)
		{
			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
		}

		if (found) {
			printm(std::to_string(i));
			return base + i;
		}
	}

	return 0xDEADBEEF;
}


void printxd(std::string str)
{
	FILE* fDummy;
	freopen_s(&fDummy, "CONOUT$", "w", stdout);

	std::cout << "test" << std::endl;
}

//Cheats
void PatchMemory(uintptr_t address, unsigned char* patch, DWORD size)
{
	DWORD oldProtect;
	VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((LPVOID)address, patch, size);
	VirtualProtect((LPVOID)address, size, oldProtect, &oldProtect);
}


uintptr_t address = FindPattern(const_cast <char*>("\x74\x00\x83\xFA\x00\x75\x00\x48\x8B\xCB"), const_cast <char*>("x?xx?x?xxx"));
uintptr_t ReplaceAddress = FindPattern(const_cast <char*>("\x48\x8B\xCB\xE8\x00\x00\x00\x00\xEB\x00\x48\x8B\xCB\xE8\x00\x00\x00\x00\x48\x8B\x93"), const_cast <char*>("xxxx????x?xxxx????xxx"));

void MultiplayerLobbyHack()
{

	
	unsigned char patch[] = { 0x74, 0x0F, 0x83, 0xFA, 0x01, 0x00 }; 

	void* MLHmem = VirtualAlloc(nullptr, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (MLHmem != nullptr)
	{
		DWORD relativeOffset = ReplaceAddress - (address + sizeof(patch)); //1997D40
		memcpy(MLHmem, patch, sizeof(patch));
		*(BYTE*)((uintptr_t)MLHmem) = 0x0F;
		*(BYTE*)((uintptr_t)MLHmem + 1) = 0x83;
		*(DWORD*)((uintptr_t)MLHmem + 2) = relativeOffset;

		PatchMemory(address, (unsigned char*)MLHmem, sizeof(patch));
		VirtualFree(MLHmem, 0, MEM_RELEASE);
	}

}

uintptr_t BrigadeChangeCost = FindPattern(const_cast <char*>("\x48\x63\x05\x00\x00\x00\x00\x48\x0F\xAF\xC8\x49\xB8\x00\x00\x00\x00\x00\x00\x00\x00\x49\x8B\xC0\x48\xF7\xE9\x48\xC1\xFA\x00\x48\x8B\xC2\x48\xC1\xE8\x00\x48\x03\xD0\x48\x63\xCA\x48\x63\xC3\x48\x0F\xAF\xC8\x49\x8B\xC0\x48\xF7\xE9\x48\xC1\xFA\x00\x48\x8B\xC2\x48\xC1\xE8\x00\x48\x03\xD0\x03\xFA"), const_cast <char*>("xxx????xxxxxx????????xxxxxxxxx?xxxxxx?xxxxxxxxxxxxxxxxxxxxxx?xxxxxx?xxxxx"));
uintptr_t BrigadeGroupCost = FindPattern(const_cast <char*>("\x48\x63\x15\x00\x00\x00\x00\x48\x63\x4D\x00\x48\x0F\xAF\xD1\x48\xB8\x00\x00\x00\x00\x00\x00\x00\x00\x48\xF7\xEA\x48\xC1\xFA\x00\x48\x8B\xC2\x48\xC1\xE8\x00\x48\x03\xD0\x41\x01\x17"), const_cast <char*>("xxx????xxx?xxxxxx????????xxxxxx?xxxxxx?xxxxxx"));
uintptr_t SupportCost = FindPattern(const_cast <char*>("\x48\x63\x05\x00\x00\x00\x00\x48\x0F\xAF\xC8\x48\xB8\x00\x00\x00\x00\x00\x00\x00\x00\x48\xF7\xE9\x48\x8B\x4C\x24\x00\x48\xC1\xFA\x00\x48\x8B\xC2\x48\xC1\xE8\x00\x48\x03\xD0\x48\x8B\x44\x24\x00\x03\xFA"), const_cast <char*>("xxx????xxxxxx????????xxxxxxx?xxx?xxxxxx?xxxxxxx?xx"));

void FreeTemplate()
{
	unsigned char patch[] = {
		0x48, 0x31, 0xC0,  
		0x90, 0x90, 0x90, 0x90  
	};

	PatchMemory(BrigadeChangeCost, patch, sizeof(patch));
	PatchMemory(BrigadeGroupCost, patch, sizeof(patch));
	PatchMemory(SupportCost, patch, sizeof(patch));
}

uintptr_t RampCost = FindPattern(const_cast <char*>("\x0F\xAF\x15\x00\x00\x00\x00\x03\x15\x00\x00\x00\x00\x48\x8D\x8C\x24"), const_cast <char*>("xxx????xx????xxxx"));
uintptr_t BaseCost = FindPattern(const_cast <char*>("\x03\x15\x00\x00\x00\x00\x48\x8D\x8C\x24"), const_cast <char*>("xx????xxxx"));

void FreeXP() {
	unsigned char RampPatch[]{
		0x6B, 0xD2, 0x00,
		0x90, 0x90, 0x90, 0x90
	};
	unsigned char BasePatch[]{
		0x83, 0xC2, 0x01,
		0x90, 0x90, 0x90
	};

	PatchMemory(RampCost, RampPatch, sizeof(RampPatch));
	PatchMemory(BaseCost, BasePatch, sizeof(BasePatch));

}

uintptr_t iSeeCombat = FindPattern(const_cast <char*>("\x48\x8B\x41\x00\x80\xB8\x00\x00\x00\x00\x00\x75\x00\x48\x8B\x41"), const_cast <char*>("xxx?xx?????x?xxx"));

void SeeCombat() {
	unsigned char Patch[]{
		0xB0, 0x01, 0xC3  //See Combat
	};

	PatchMemory(iSeeCombat, Patch, sizeof(Patch));
}

void UnSeeCombat() {
	unsigned char Patch[]{
		0x48, 0x8B, 0x41  //Original Combat
	};
	PatchMemory(iSeeCombat, Patch, sizeof(Patch));
}

uintptr_t iGameSpeed = FindPattern(const_cast <char*>("\x74\x00\xE8\x00\x00\x00\x00\x44\x3B\xF8\x75"), const_cast <char*>(" x?x????xxxx"));

void GameSpeed() {
	unsigned char Patch[]{
		0x75, 0x50
	};


	PatchMemory(iGameSpeed, Patch, sizeof(Patch));
}

uintptr_t iSeeArmy = FindPattern(const_cast <char*>("\x80\x3D\x00\x00\x00\x00\x00\x48\x8B\xD9\x0F\x84\x00\x00\x00\x00\x80\x3D\x00\x00\x00\x00\x00\x48\x8D\x35"), const_cast <char*>("xx?????xxxxx????xx?????xxx"));
uintptr_t iSeeAir = FindPattern(const_cast <char*>("\x80\x3D\x00\x00\x00\x00\x00\x74\x00\x49\x63\x8F"), const_cast <char*>("xx?????x?xxx"));


void SeeArmy() {
	unsigned char ArmyPatch[]{
	0x80, 0x3D, 0xE3, 0x5C, 0xAC, 0x01, 0x01
	};
	unsigned char AirPatch[]{
	0x80, 0x3D, 0xFC, 0x79, 0xDE, 0x01, 0x01
	};


	PatchMemory(iSeeArmy, ArmyPatch, sizeof(ArmyPatch));
	PatchMemory(iSeeAir, AirPatch, sizeof(AirPatch));
}

void UnSeeArmy() {
	unsigned char ArmyPatch[]{
	0x80, 0x3D, 0xE3, 0x5C, 0xAC, 0x01, 0x00
	};
	unsigned char AirPatch[]{
	0x80, 0x3D, 0xFC, 0x79, 0xDE, 0x01, 0x00
	};


	PatchMemory(iSeeArmy, ArmyPatch, sizeof(ArmyPatch));
	PatchMemory(iSeeAir, AirPatch, sizeof(AirPatch));
}




uintptr_t iSeeBugs = FindPattern(const_cast <char*>("\x80\x3D\x00\x00\x00\x00\x00\x74\x00\x49\x8B\x8E\x00\x00\x00\x00\x48\x8B\x01"), const_cast <char*>("xx?????x?xxx????xxx"));

void EnableDebug() {
	unsigned char Patch[]{
	0x80, 0x3D, 0x1A, 0xB1, 0x20, 0x02, 0x01
	};

	PatchMemory(iSeeBugs, Patch, sizeof(Patch));
}

void DisableDebug() {
	unsigned char Patch[]{
	0x80, 0x3D, 0x1A, 0xB1, 0x20, 0x02, 0x00
	};

	PatchMemory(iSeeBugs, Patch, sizeof(Patch));
}

uintptr_t ConsoleMPFunc = FindPattern(const_cast <char*>("\x75\x00\x49\x8B\x8E\x00\x00\x00\x00\x48\x85\xC9\x0F\x84\x00\x00\x00\x00\x48\x8B\x01"), const_cast <char*>("x?xxx????xxxxx????xxx"));
//uintptr_t ConsoleMPFunc2 = FindPattern(const_cast <char*>("\x74\x00\xC6\x45\x00\x00\x41\xB8\x00\x00\x00\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x4D\x00\xE8\x00\x00\x00\x00\x0F\xB6\x45\x00\x88\x03"), const_cast <char*>("x?xx??xx????xxx????xxx?x????xxx?xx"));
uintptr_t ConsoleMPFunc3 = FindPattern(const_cast <char*>("\x88\x91\x00\x00\x00\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x8D\x05"), const_cast <char*>("xx????xxxxxxxxxxxxx"));
void ConsoleMP() {
	unsigned char ConsolePatch1[]{
		0x90, 0x90 //replace jne with nops, never jumps
	};
	//unsigned char ConsolePatch2[]{
	//	0xEB, 0x5E //replace je with jmp
	//};
	unsigned char ConsolePatch3[]{
		0x90,0x90,0x90,0x90,0x90  //stops crashing
	};

	PatchMemory(ConsoleMPFunc, ConsolePatch1, sizeof(ConsolePatch1));
	//PatchMemory(ConsoleMPFunc2, ConsolePatch2, sizeof(ConsolePatch2));
	PatchMemory(ConsoleMPFunc3, ConsolePatch3, sizeof(ConsolePatch3));
}


uintptr_t UpgradeLimit2 = FindPattern(const_cast <char*>("\x41\x3A\x48\x00\x73"), const_cast <char*>("xxx?x"));
void UpgradeLimit() {
	unsigned char Level1[]{
		0xBB, 0xE7, 0x00, 0x00, 0x00 //
	};
	unsigned char Level2[]{
		0x90, 0x90, 0x90, 0x90, 0x90 //
	};
	unsigned char XPCost[]{
		0x41, 0xBF, 0x01, 0x00, 0x00, 0x00, 0x90, //replace jne with nops, never jumps
	};

	PatchMemory(UpgradeLimit2, Level2, sizeof(Level2));
}




void* __fastcall SizeF(size_t Size) {
	size_t i;
	void* result;
	

	for (i = Size; ; Size = i) {
		result = _malloc_base(Size);
		if (result) {
			break;
		}
		if (!_callnewh(i)) {
			if (i != -1i64) {
				printm("Bad Allocation");
			}
			printm("Bad Array Length");
		}
	}
	return result;
}



template <typename T>
T ReadMemory(uintptr_t address)
{
	return *reinterpret_cast<T*>(address);
}

uintptr_t OffsetCalculator(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets)
{
	uintptr_t address = baseAddress;
	for (uintptr_t offset : offsets)
	{
		address = ReadMemory<uintptr_t>(address);
		address += offset;
	}
	return address;
}



void ChangeIntAddressValue(uintptr_t bAddr, uintptr_t bOff, int Tag)
{

	uintptr_t baseAddress = bAddr; 
	
	std::vector<uintptr_t> offsets = { bOff };
	
	uintptr_t finalAddress = OffsetCalculator(GameBase + baseAddress, offsets);

	DWORD* pValue = reinterpret_cast<DWORD*>(finalAddress);
	DWORD oldProtect;

	VirtualProtect(pValue, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &oldProtect);

	*pValue = Tag;

	VirtualProtect(pValue, sizeof(DWORD), oldProtect, &oldProtect);

}

void ChangeByteAddressValue(uintptr_t addr)
{
	uintptr_t address = GameBase + addr; 
	BYTE* pValue = (BYTE*)address;
	DWORD oldProtect;

	VirtualProtect(pValue, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &oldProtect);
	
	if (*pValue == 1)
	{
		*pValue = 0;
	}
	else
	{
		*pValue = 1;
	}
	
	VirtualProtect(pValue, sizeof(BYTE), oldProtect, &oldProtect);
}

void* x = operator new(38); //test
//IngameFunctions
void Crasher(int a1) {
	CCrash* x = (CCrash*)GetCCommandFunc(48);
	x = CCrashFunc(x, a1);
	CSessionPostTramp(pCSession, x, true);
}


void StartGameFunc() {
	CStartGameCommand* StartGame = (CStartGameCommand*)GetCCommandFunc(41);
	StartGame = CStartGameCommandFunc((StartGame));
	CSessionPostTramp(pCSession, StartGame, true);
}

void RemovalReason(ERemovalReason e) {
	
	CRemovePlayerCommand* RemovePlayer = (CRemovePlayerCommand*)GetCCommandFunc(41);
	RemovePlayer = CRemovePlayerCommandTramp(RemovePlayer, RMID, e, dRUnknown);
	CSessionPostTramp(pCSession, RemovePlayer, true);
}



void DefaultImGui() {
	ImVec4 NormalColor = ImVec4(0.047f, 0.047f, 0.047f, 1.0f);
	ImVec4 HoverColor = ImVec4(0.066f, 0.066f, 0.066f, 1.0f);
	ImVec4 ActiveColor = ImVec4(0.076f, 0.076f, 0.076f, 1.0f);
	ImVec4 BackgroundColor = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
	ImVec4 PureBlackColor = ImVec4(0.01f, 0.01f, 0.01f, 1.0f);
	ImVec4 WhiteColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = NormalColor;  // Normal Colors
	style.Colors[ImGuiCol_FrameBg] = NormalColor;
	style.Colors[ImGuiCol_ResizeGrip] = NormalColor;
	style.Colors[ImGuiCol_Separator] = NormalColor;

	style.Colors[ImGuiCol_ButtonHovered] = HoverColor; //Hovered Colors
	style.Colors[ImGuiCol_ResizeGripHovered] = HoverColor;
	style.Colors[ImGuiCol_FrameBgHovered] = HoverColor;
	style.Colors[ImGuiCol_SeparatorHovered] = HoverColor;

	style.Colors[ImGuiCol_ButtonActive] = ActiveColor; // Active Colors
	style.Colors[ImGuiCol_ResizeGripActive] = ActiveColor;
	style.Colors[ImGuiCol_SeparatorActive] = ActiveColor;
	style.Colors[ImGuiCol_FrameBgActive] = ActiveColor;

	style.Colors[ImGuiCol_WindowBg] = BackgroundColor; // Background

	style.Colors[ImGuiCol_TitleBg] = PureBlackColor; // Title
	style.Colors[ImGuiCol_TitleBgCollapsed] = PureBlackColor;
	style.Colors[ImGuiCol_TitleBgActive] = PureBlackColor;

	style.Colors[ImGuiCol_CheckMark] = WhiteColor; // Checkmark
}

	


void InitImGui()
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/arial.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesCyrillic()); // For Russian
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
	
	DefaultImGui();

}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}


bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

			InitImGui();
			ImGui::SetNextWindowSize(ImVec2(350, 1050));
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	if (GetAsyncKeyState(VK_INSERT) & 1)
		bMenuOpen = !bMenuOpen;

	if (GetAsyncKeyState(VK_NUMPAD0) & 1)
		StartGameFunc();

	if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		bCrasher = !bCrasher;

		

	if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
		boost = 10000;
		EmptyTest* Strength = (EmptyTest*)GetCCommandFunc(88);
		Strength = GCDMF(Strength, CountryTag, boost);
		CSessionPostTramp(pCSession, Strength, true);
		memset(TagBuffer, 0, sizeof(TagBuffer));
	}
		

	if (GetAsyncKeyState(VK_NUMPAD3) & 1) {
		boost = 0;
		EmptyTest* Strength = (EmptyTest*)GetCCommandFunc(88);
		Strength = GCDMF(Strength, CountryTag, boost);
		CSessionPostTramp(pCSession, Strength, true);
		memset(TagBuffer, 0, sizeof(TagBuffer));
	}



	if (bMenuOpen)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.WantCaptureMouse = true;
		io.WantCaptureKeyboard = true;

		if (GetAsyncKeyState(VK_LBUTTON))
		{
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
		}
		else
		{
			io.MouseReleased[0] = true;
			io.MouseDown[0] = false;
			io.MouseClicked[0] = false;
		}





		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("SilverHook 3 DEBUG", &bMenuOpen);

		ImGui::Text("Menus");
		ImGui::Columns(2);
		if (ImGui::Button(u8"Lobby Menu", ImVec2(140, 28)))
		{
			eMenu = eLobbyMenu;
		}; 
		ImGui::NextColumn();
		if (ImGui::Button(u8"Ingame Menu", ImVec2(140, 28)))
		{
			eMenu = eIngameMenu;
		};
		ImGui::NextColumn();
		if (ImGui::Button(u8"Boost Menu", ImVec2(140, 28)))
		{
			eMenu = eBoostMenu;
		}; 
		
		ImGui::NextColumn();
		if (ImGui::Button(u8"Debug Menu", ImVec2(140, 28)))
		{
			eMenu = eDebugMenu;
		};
		


		ImGui::Columns(1);
		if (eMenu == eLobbyMenu)
		{
			ImGui::Text("Host IP:");
			ImGui::SameLine();
			ImGui::Text(CurrentHost.c_str());
			ImGui::Text("Cheats");
			ImGui::Checkbox("KILL Lobby INSTANTLY", &bLobbyHostRemoval);
			ImGui::Checkbox("Spoof Steam Name", &bSpoofSteam);
			ImGui::Checkbox("AntiBan", &bRefuseConnect);
			//ImGui::Checkbox("Freeze Host", &bShouldFreeze);
			ImGui::Checkbox("Remove Name", &bJoinAsGhost);
			ImGui::Checkbox("Multiplayer Lobby Hack", &bCE);	
			ImGui::Checkbox("FakeSpammer", &FakeSpammer);
			ImGui::Checkbox("Join as Ghost", &bGhost2);
			ImGui::Text("Function Calls");
			ImGui::Text("");
			ImGui::Columns(2);
			if (ImGui::Button("Toggle tdebug", ImVec2(140, 28)) && pSetDebugTooltipsEnabled != nullptr)
			{
				bEnabletdebug = !bEnabletdebug;

				SetDebugTooltipsEnabledTramp(pSetDebugTooltipsEnabled, bEnabletdebug);
			}
			ImGui::NextColumn();
			if (ImGui::Button("Start Game", ImVec2(140, 28)) && pCSession != nullptr)
			{

				StartGameFunc();

			}
			ImGui::NextColumn();
			if (ImGui::Button("Reset ID", ImVec2(140, 28)) && pCSession != nullptr)
			{

				FakeM = 1;

			}
			ImGui::NextColumn();
			if (ImGui::Button("Add Fake Player", ImVec2(140, 28)) && pCSession != nullptr)
			{
				DWORD* tt = dUnknown;
				__int64 tP = dPdx;
				CString* tN = dHname;
				CString* tNN = dUser;


				const char* HOIName = PlayerName;
				
				tN->assign(HOIName, strlen(HOIName));

				const char* SteamName = PlayerName;
				
				tNN->assign(SteamName, strlen(SteamName));

				CAddPlayerCommand* AddFake = (CAddPlayerCommand*)GetCCommandFunc(200);
				
				AddFake = CAddPlayerCommandTramp(AddFake, tN, tNN, tt, FakeM2, false, tP);
				FakeM2++;

				CSessionPostTramp(pCSession, AddFake, true);
			}
			ImGui::NextColumn();
			if (ImGui::Button("Fake Leave", ImVec2(140, 28)) && pCSession != nullptr) {
				CRemovePlayerCommand* RemovePlayer = (CRemovePlayerCommand*)GetCCommandFunc(200);
				RemovePlayer = CRemovePlayerCommandTramp(RemovePlayer, dMachine, 0, dRUnknown);
				CSessionPostTramp(pCSession, RemovePlayer, true);
				
			}
			ImGui::NextColumn();
			ImGui::Columns(1);
			ImGui::Text("Fake Name");
			ImGui::InputText("", PlayerName, IM_ARRAYSIZE(PlayerName));
			ImGui::SameLine();


			ImGui::Text("");
			ImGui::Text("Credits: Silver");
		}
		if (eMenu == eIngameMenu) {
			ImGui::Text("Cheats");
			
			ImGui::Checkbox("Free Templates", &bFreeTemp);
			ImGui::Checkbox("Free Upgrades", &bFreeUpgrade);
			ImGui::Text("Free Upgrades for Mech!");

			ImGui::Checkbox("See Combat", &bSeeCombat);

			ImGui::Checkbox("Crasher", &bCrasher);
			ImGui::Text("Only use crasher when\ngame has finished loading.");

			if (ImGui::Button("Enable Debug", ImVec2(140, 28)) && pCSession != nullptr)
			{
					EnableDebug();
			}
			ImGui::Text("Hold CTRL+ALT after enabling to see tags");
			ImGui::Text("Function Calls");
			ImGui::Text("");
			ImGui::Columns(2);
			if (ImGui::Button("Enable AI on all", ImVec2(140, 28)) && pCSession != nullptr)
			{
				int i = 0;
				do {
					int* TagPtr = &i;
					CAiEnableCommand* EnableAI = (CAiEnableCommand*)GetCCommandFunc(56);
					EnableAI = AIEnableFunc(EnableAI, TagPtr, 2);
					CSessionPostTramp(pCSession, EnableAI, 1);
					i++;
				} while (i <= 100);
			}
			ImGui::NextColumn();
			if (ImGui::Button("Disable AI on all", ImVec2(140, 28)) && pCSession != nullptr)
			{
				int i = 0;
				do {
					int* TagPtr = &i;
					CAiEnableCommand* EnableAI = (CAiEnableCommand*)GetCCommandFunc(56);
					EnableAI = AIEnableFunc(EnableAI, TagPtr, 0);
					CSessionPostTramp(pCSession, EnableAI, 1);
					i++;
				} while (i <= 100);
			}
			ImGui::NextColumn();
			if (ImGui::Button("Infinite Pause", ImVec2(140, 28)) && pCSession != nullptr) {
				CString* empty = new CString;
				CPauseGameCommand* UnpauseGame = (CPauseGameCommand*)GetCCommandFunc(88);
				UnpauseGame = CPauseGameFunc(UnpauseGame, (__int64)empty, 1);
				CSessionPostTramp(pCSession, UnpauseGame, true);
			}
			ImGui::NextColumn();
			if (ImGui::Button("Ghost Pause", ImVec2(140, 28)) && pCSession != nullptr) {
				CString* empty = new CString;
				CPauseGameCommand* UnpauseGame = (CPauseGameCommand*)GetCCommandFunc(88);
				UnpauseGame = CPauseGameFunc(UnpauseGame, (__int64)empty, 0);
				CSessionPostTramp(pCSession, UnpauseGame, true);
			}
			ImGui::NextColumn();
			if (ImGui::Button("Decrease Speed", ImVec2(140, 28)) && pCSession != nullptr) {
				CGameSpeed* DecreaseSpeed = (CGameSpeed*)GetCCommandFunc(80);
				DecreaseSpeed = DecreaseSpeedFunc(DecreaseSpeed);
				CSessionPostTramp(pCSession, DecreaseSpeed, true);
			}
			ImGui::NextColumn();
			if (ImGui::Button("Increase Speed", ImVec2(140, 28)) && pCSession != nullptr) {
				CGameSpeed* IncreaseSpeed = (CGameSpeed*)GetCCommandFunc(48);
				IncreaseSpeed = IncreaseSpeedFunc(IncreaseSpeed);
				CSessionPostTramp(pCSession, IncreaseSpeed, true);
			}
			ImGui::NextColumn();
			if (ImGui::Button("AutoSave", ImVec2(140, 28)) && pCSession != nullptr) {
				
				EmptyTest* AutoSave = (EmptyTest*)GetCCommandFunc(56);
				AutoSave = AutoSaveFunc(AutoSave, 1);

				CSessionPostTramp(pCSession, AutoSave, true);
			}
			ImGui::Columns(1);
			ImGui::Columns(1);
			ImGui::Text("");
			ImGui::Text("Country Tag:");
			ImGui::SetNextItemWidth(70.f);
			ImGui::InputText("", TagBuffer, IM_ARRAYSIZE(TagBuffer));
			ImGui::SameLine();

			if (ImGui::Button("Enable AI") && pCSession != nullptr)
			{
				std::string sTagBuffer = TagBuffer;
				char* endptr;
				int a1 = std::strtol(TagBuffer, &endptr, 10);

				if (sTagBuffer.length() > 0)
				{
					int* TagPtr = &a1;
					CAiEnableCommand* EnableAI = (CAiEnableCommand*)GetCCommandFunc(56);
					EnableAI = AIEnableFunc(EnableAI, TagPtr, 2);
					CSessionPostTramp(pCSession, EnableAI, 1);
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Disable AI") && pCSession != nullptr)
			{
				std::string sTagBuffer = TagBuffer;
				char* endptr;
				int a1 = std::strtol(TagBuffer, &endptr, 10);
				if (sTagBuffer.length() > 0)
				{

					int* TagPtr = &a1;
					CAiEnableCommand* DisableAI = (CAiEnableCommand*)GetCCommandFunc(56);
					DisableAI = AIEnableFunc(DisableAI, TagPtr, 0);
					CSessionPostTramp(pCSession, DisableAI, 1);
				}
			}
			if (ImGui::Button("Tagswitch") && pCSession != nullptr)
			{
				std::string sTagBuffer = TagBuffer;

				if (sTagBuffer.length() > 0)
				{
					int Tag = std::stoi(sTagBuffer);
					int* pTag = &Tag;
					if (pCGameState != nullptr)
						CGameStateSetPlayerTramp(pCGameState, pTag);

					//ChangeIntAddressValue(TAGAddr, 0x588, Tag);
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
			{
				memset(TagBuffer, 0, sizeof(TagBuffer));
			}
			ImGui::Text("");
			ImGui::Text("Credits: Silver");
		}
		if (eMenu == eBoostMenu) {
			ImGui::Text("");
			ImGui::Text("Boost Amount:");
			ImGui::SetNextItemWidth(70.f);
			ImGui::InputText("x", TagBuffer, IM_ARRAYSIZE(TagBuffer));
			ImGui::SameLine();

			if (ImGui::Button("Boost") && pCSession != nullptr)
			{
				std::string sTagBuffer = TagBuffer;
				boost = std::stoi(TagBuffer);

				if (sTagBuffer.length() > 0)
				{
					EmptyTest* Strength = (EmptyTest*)GetCCommandFunc(88);
					Strength = GCDMF(Strength, CountryTag, boost);
					CSessionPostTramp(pCSession, Strength, true);
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("200k"))
			{
				boost = 200000;
				
				EmptyTest* Strength = (EmptyTest*)GetCCommandFunc(88);
				
				Strength = GCDMF(Strength, CountryTag, boost);
				
				CSessionPostTramp(pCSession, Strength, true);

				memset(TagBuffer, 0, sizeof(TagBuffer));
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
			{
				boost = 0;
				EmptyTest* Strength = (EmptyTest*)GetCCommandFunc(88);
				Strength = GCDMF(Strength, CountryTag, boost);
				CSessionPostTramp(pCSession, Strength, true);
				memset(TagBuffer, 0, sizeof(TagBuffer));
			}
			ImGui::Text("CountryTag: ");
			ImGui::SameLine();
			std::string tag = std::to_string(CountryTag);
			ImGui::Text(tag.c_str());
			ImGui::SameLine();
			ImGui::Text("");
			ImGui::Text("Credits: Silver");
		}

		if (eMenu == eDebugMenu) 
		{
			ImGui::Text("Press Numpad 9 to display all IPs");
			ImGui::Checkbox("Debug", &Debug);
			ImGui::Checkbox("SessionPost", &dSessionPost);
			ImGui::Checkbox("Players", &bShowPlayers);
			ImGui::Checkbox("Hoi4 Log", &bLog);
			ImGui::Checkbox("Show Host IP", &bFindChost);
			
		}

		
		
		
	


		
		
		if (bCrasher) {
			Crasher(0);
			EmptyTest* AutoSave = (EmptyTest*)GetCCommandFunc(56);
			AutoSave = AutoSaveFunc(AutoSave, 1);
			CSessionPostTramp(pCSession, AutoSave, true);
		}
		if (bCE) {
			MultiplayerLobbyHack();
			
		}
		if (Debug) {
			if (!bConsoleAlloc) {
				AllocConsole();
				bConsoleAlloc = true;
			}
		}

		if (bGameSpeed)
			GameSpeed();
		if (bSeeCombat) {
			SeeCombat();
		}
		else {
			UnSeeCombat();
		}


		if (bFreeTemp) {
			FreeTemplate();
		}
		if (bFreeResearch) {
			//FreeResearch();
		}
		if (bFreeMIO) {
			//FreeMIOs();
		}
		if (bFreeUpgrade) {
			FreeXP();
			//UpgradeLimit();
		}
		//else if (!bFreeMIO) {
			//UnFreeMIOs();
		//}
		if (bShouldFreeze)
		{
			bool reply = SendPacketTramp(LastENetHost, 0, SwapData, 1, 0);

			
		}


		if (FakeSpammer) {
			DWORD* a3 = dUnknown;
			__int64 a4 = dPdx;
			CString* a1 = dUser; // = dUser;
			CString* a2 = dHname; // = dHname;
			CString* empty = new CString;
			CAddPlayerCommand* AddFake = (CAddPlayerCommand*)GetCCommandFunc(200);
			FakeM++;
			const char* SteamName = "Null";
			const char* HoiName = "Null";
			
			
			HoiName = PlayerName;
			if (Flip) {
				
				Flip = false;
			}
			else {
				Flip = true;
			}
			


			a1->assign(SteamName, strlen(SteamName));
			a2->assign(HoiName, strlen(HoiName));
			
			AddFake = CAddPlayerCommandTramp(AddFake, a1, a2, a3, FakeM, false, a4);
			CSessionPostTramp(pCSession, AddFake, true);
		}


		ImGui::End();
		ImGui::Render();

		pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	return oPresent(pSwapChain, SyncInterval, Flags);
}





bool __fastcall hkCSessionPost(void* pThis, CCommand* pCommand, bool ForceSend)
{
	
	pCSession = pThis;
	if (Debug) {
		if(dSessionPost)
		printm("CSP Called");
		//printm(std::to_string(FreeAddress));
	}
	return CSessionPostTramp(pThis, pCommand, ForceSend);
}

CAddPlayerCommand* __fastcall hkCAddPlayerCommand(CAddPlayerCommand* pThis, CString* User, CString* Name, DWORD* unknown, int nMachineId, bool bHotjoin, __int64 a7)
{
	//CAddPlayerCommand* cmd = static_cast<CAddPlayerCommand*>(pThis);

	//CString* tttt = new CString;
	

	//CString* Test = (CString*)(pThis->_User);


	//memcpy(Test, "Test", 4);

	
	iParadoxSocialID = a7;
	
	if (bLobbyHostRemoval)
		nMachineId = 1;

	iMachineIDFake = 50;
	iMyMachineID = nMachineId;

	if (bSpoofSteam)
	{
		const char* Name = PlayerName;
		User->assign(Name, strlen(Name));
	}

	if (bJoinAsGhost)
	{
		const char* Long = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
		//User = empty;
		//Name = empty;

		User->assign(Long, strlen(Long));
		Name->assign(Long, strlen(Long));
		//User->_str = "Test";

		//CNameChangeFunc(&pThis->_User, User);
		//if (Debug)
			//printm("Name Called");
	}
	
	pCAddPlayer = pThis;
	dUser = (CString*)User;
	dHname = (CString*)Name;
	dUnknown = unknown;
	dMachine = nMachineId;
	dPdx = a7;
	//if (bGhost2)
		//bGhost3 = true;
	int i = 0;
	int* TagPtr = &i;
	CAiEnableCommand* EnableAI = (CAiEnableCommand*)GetCCommandFunc(56);
	 
	
	
	if (Debug) {
		printm("APC Called");
		printm("Machine ID:");
		std::string x = std::to_string(nMachineId);
		printm(x);
	}

	if(!bGhost2)
		return CAddPlayerCommandTramp(pThis, (CString*)User, (CString*)Name, unknown, nMachineId, bHotjoin, a7);

	if(bGhost2)
		return (CAddPlayerCommand * )AIEnableFunc(EnableAI, TagPtr, 0);

}

CRemovePlayerCommand* __fastcall hkCRemovePlayerCommand(void* pThis, int _machineID, ERemovalReason eReason, __int64 a4)
{

	pCRemovePlayer = pThis; RMID = _machineID; dReason = eReason; dRUnknown = a4;
	bRefuseConnect = false;
	if (Debug) {
		printm("RPC Called");
	}
	
	return CRemovePlayerCommandTramp(pThis, _machineID, eReason, a4);

}

__int64 __fastcall hkCGameStateSetPlayer(void* pThis, int* Tag)
{

	pCGameState = pThis;
	if (Debug) {
		printm("CGSSP Called");
	}
	return CGameStateSetPlayerTramp(pThis, Tag);
	//return 0;
}




EmptyTest* __fastcall hkCustomDiffM(void* pThis, __int64 Tag, int Boost) {
	if (Debug) {
		printm("DiffMultipley");
		std::string x;
		//x = std::to_string(a2);
		printm((char*)Tag);
		//a3 = boost;
		x = std::to_string(Boost);
		printm(x);
	}
	CountryTag = Tag;

	return GCDMH(pThis, Tag, Boost);
}



CCrash* __fastcall hkCrash(void* pThis, unsigned int a1) {

	if (Debug) {
		printm("Crash Called");
		std::string x = std::to_string(a1);
		printm(x);
	}


	return CCrashTramp(pThis, a1);
}

CPauseGameCommand* __fastcall hkPauseGame(void* pThis, __int64 a2, char a3, char a4) {
	if (Debug) {
		printm("hkPauseGame Called");
		std::string x = std::to_string(a2);
		printm(x);
		x = std::to_string(a3);
		printm(x);
		x = std::to_string(a4);
		printm(x);
	}
	return CPauseGameTramp(pThis, a2, a3);
}

EmptyTest* __fastcall hkSeshCon(void* pThis, void* NetAddress, void* GameName, int Type, int BasicType, int Status, CString* Version, bool AllowedIdReset) {


	if (Debug) {
		printm("hkSeshCon Called");
		std::string x = std::to_string((char)GameName);
		printm((char*)GameName);
		x = std::to_string((char)Version);
		printm(x);
		x = std::to_string((char)NetAddress);
		printm(x);
	}

	if (Type == 2) {
		pCSessionConfigToken = pThis;
	}
	//const char* Name = "Test";

	//Version->assign(Name, strlen(Name));

	return SeshTramp(pThis, NetAddress, GameName, Type, BasicType, Status, Version, AllowedIdReset);

}

void __fastcall hkSetDebugTooltipsEnabled(COption* pThis, bool bSelected)
{
	if (Debug)
		printm("CMapModeManager::SetDebugTooltipsEnabled called.");

	pSetDebugTooltipsEnabled = pThis;

	return SetDebugTooltipsEnabledTramp(pThis, bEnabletdebug);
}

bool __fastcall hkNetDisconnect(void* pThis) {

	if (Debug)
		printm("hkNetDisconnect Called");

	if (!bRefuseConnect)
		return NetDisconnectTramp(pThis);
	
	return 0;
	

}

void __fastcall hkSetState(void* pThis, int a2) {
	if (Debug)
		printm("hkSetState Called");

	if (!bRefuseConnect)
		return SetStateTramp(pThis, a2);

	return (void)0;
}

void __fastcall hkMatchMakingLeave(void* pThis) {

	if (Debug)
		printm("hkMatchMaking Called");
	if (!bRefuseConnect)
		return MatchmakingLeaveTramp(pThis);
}

bool __fastcall hkSendPacket(_ENetHost** Handle, int nConnectionIndex, CBlob* pData, const unsigned int nDataSize, uint8_t nChannel) {

	LastENetHost = Handle;



	if (nDataSize > iLargestPacketSize)
	{
		iLargestPacketSize = nDataSize;
		SwapData = pData;
	}

	return SendPacketTramp(Handle, nConnectionIndex, pData, nDataSize, nChannel);
}

EmptyTest* __fastcall hkBanUser(void* pThis, unsigned int nMachineId) {

	if (Debug)
		printm("Ban User Called");

	int i = 0;
	int* TagPtr = &i;
	CAiEnableCommand* EnableAI = (CAiEnableCommand*)GetCCommandFunc(56);


	if(!bRefuseConnect)
		return BanUserTramp(pThis, nMachineId);
	if(bRefuseConnect)
		return (EmptyTest*)AIEnableFunc(EnableAI, TagPtr, 0);
}


void __fastcall hkHost(void* pThis, CString* ServerName, CString* Password, CString* Desc, CString* Tags, int a6) {

	if (Debug) {
		printm("Host Called");
		
		printm((char*)ServerName);
		printm((char*)Password);
		printm((char*)Desc);
		printm((char*)Tags);
		printm(std::to_string(a6));

	}

	cServerName = ServerName;
	cDesc = Desc;
	cTags = Tags;

	iHost = a6;
	pCIdleLobby = pThis;

	return HostTramp(pThis, ServerName, Password, Desc, Tags, a6);
}

void* __fastcall hkJoinGame(void* pThis, CString* PlayerName, void* SNetAddress, CString* Password) {

	if (Debug) {
		printm("Join Called");
		printm((char*)PlayerName);
		printm((char*)Password);
	}

	return JoinGameTramp(pThis, PlayerName, SNetAddress, Password);
}

void __fastcall hkSessionChange(void* pThis, __int64 Type) {

	if (Debug) {
		printm("Session Change");
		printm(std::to_string(Type));
		if (Type >= 16) {
			Type = 0;
		}
	}

	return SessionChangeTramp(pThis, Type);
}

void* __fastcall hkMultiplayerConfig(void* pThis, void* a2, DWORD* PlayerName, void* Config, bool isGameOwner) {

	if (Debug) {
		printm("MultiplayerConfig");
		//isGameOwner = true;
		//if (pCSessionConfigToken != nullptr)
			//Config = pCSessionConfigToken;
	}

	return MultiplayerConfigTramp(pThis, a2, PlayerName, Config, isGameOwner);
}



void __fastcall hkHuman(void* pThis, CHuman* Human) {
	//40 steam
	//72 User
	//176 MachineID
	if (Debug) {
		printm("AddPlayerItem Called");
		
		//printm("MachineID");
		//printm(std::to_string(Human->_nMachineId));
	}


	if (bShowPlayers) {
		if (!bConsoleAlloc) {
			AllocConsole();
			bConsoleAlloc = true;
		}
		CString* nSteamName = (CString*)((uintptr_t)Human + 40);
		std::string sName = (char*)nSteamName;

		CString* nHOI4Name = (CString*)((uintptr_t)Human + 72);
		std::string hName = (char*)nHOI4Name;

		unsigned int nMachine = *(unsigned int*)((uintptr_t)Human + 176);

		std::string sFName = "Steam Name: " + sName;
		std::string hFName = "HOI4 Name: " + hName;
		std::string fMachineID = "Machine ID: " + std::to_string(nMachine);

		printm(sFName);
		printm(hFName);
		printm(fMachineID);
		printm(" ");

	}

	return HumanTramp(pThis, Human);
}

void __fastcall hkServerAddress(void* pThis, double nAddress) {

	if (Debug) {
		printm("SetServerAddress Called");
		//printm(std::to_string(nAddress));
		//for (int i = 0; i < 128; i++) {
		//	printm(std::to_string(i));
		//	void* ServerAddress = (void*)((uintptr_t)pThis + i);
		//	std::string sServerAddr = (char*)ServerAddress;
		//	printm(sServerAddr);
		//}
		//ServerAddress = (void*)((uintptr_t)pThis + 64);
		//std::string sServerAddr = (char*)ServerAddress;
		//printm(sServerAddr);
	}

	return SetServerAddressTramp(pThis, nAddress);
}

bool __fastcall hkCProxyServer(void* pThis, bool bReconnect, int nMinTimeout, __int64 a4, __int64 a5 ) {

	if (Debug) {
		printm("ProxyServer Called");

		//for (int i = 0; i < 128; i++) {
		//	printm(std::to_string(i));
		//	void* ServerAddress = (void*)((uintptr_t)pThis + i);
		//	std::string sServerAddr = (char*)ServerAddress;
		//	printm(sServerAddr);
		//}
	}


	return CProxyServerTramp(pThis, bReconnect, nMinTimeout, a4, a5);
}

void* __fastcall hkLogStream(void* pThis, const char* Log) {
	//if (Debug)
		//printm("Log Stream Called");
	std::string m = Log;
	if (bLog) {
		if (!bConsoleAlloc) {
			AllocConsole();
			bConsoleAlloc = true;
		}
		std::string s = "LogStream: " + m;
		printLog(s);
	}
	if (m.find("CProxyServer::PackageCallback") != std::string::npos) {
		bAddress = true;
	}
	return CLogStreamTramp(pThis, Log);
}

void* __fastcall hkLog(void* pThis, void* a2, const char* pFile, int lineNo, int Category) {

	//if (Debug)
		//printm("Log Called");

	if (bLog) {
		if (!bConsoleAlloc) {
			AllocConsole();
			bConsoleAlloc = true;
		}
		std::string m = pFile;
		std::string s = "File: " + m + " " + std::to_string(lineNo) + " " + std::to_string(Category);
		printLog(s);
	}

	return CLogTramp(pThis, a2, pFile, lineNo, Category);
}

void* __fastcall hkOperator(void* pThis, __int64* Log) {

	//if (Debug)
		//printm("Operator Called");
	 
		if (bAddress) {
			__int64* rawPtr = Log;
			if (Log[3] >= 16) {
				rawPtr = (__int64*)*Log;
			}

			std::string m = (char*)rawPtr;
			CurrentHost = m;
			std::string s = "IP Address: " + m;
			if (bFindChost) {
				if (!bConsoleAlloc) {
					AllocConsole();
					bConsoleAlloc = true;
				}
				if (!(std::find(IPVector.begin(), IPVector.end(), m) != IPVector.end())) {
					IPVector.push_back(m);
					printLog(s);
				}
				if (GetAsyncKeyState(VK_NUMPAD9) & 1) {
					for (const std::string& str : IPVector) {
						printLog(str);
					}
					Sleep(500);
				}
			}
		}
	
	
	if (bLog) {
		__int64* rawPtr = Log;
		if (Log[3] >= 16) {
			rawPtr = (__int64*)*Log;

		}

		std::string m = (char*)rawPtr;
		std::string s = "Operator Value: " + m;
		printLog(s);
		
	}
	bAddress = false;
	return COperatorTramp(pThis, Log);
}

bool __fastcall hkConsole(unsigned __int64* pThis, __int64* pThat) {

	//if (Debug)
		//printm("ConsoleCheats Called");
	//ConsoleMP();
	return true;
}

void HookFunctions() {
	
	IPVector.push_back("0.0.0.0:0");
	//When Naming Functions in IDA
	//Please Use this naming Standard
	// ManFunc<ShortFunctionName>
	// so, for example
	// Sessionpost = ManFuncSessionPost
	//FOWAddr = 0x2E97319;
	//DBGAddr = 0x2E969E9;
	//TAGAddr = 0x2E96F88;
	//ALWTRAddr = 0x2E972F8;
	//BanUserFunc = GetBanUser(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x48\x83\xEC\x00\x48\x8B\xD9\x8B\xEA"), const_cast <char*>("xxxx?xxxx?xxxx?xxxxx")));
	//MH_CreateHook(BanUserFunc, &hkBanUser, (LPVOID*)&BanUserTramp);
	//MH_EnableHook(BanUserFunc);

	CSessionPostHook = CSessionPost(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\xFA\x48\x8B\xD9\x45\x84\xC0"), const_cast <char*>("xxxx?xxxx????xxxxxxxxx")));
	MH_CreateHook(CSessionPostHook, &hkCSessionPost, (LPVOID*)&CSessionPostTramp);
	MH_EnableHook(CSessionPostHook);

	CAddPlayerCommandHook = GetCAddPlayerCommand(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x48\x89\x4C\x24\x00\x41\x56\x48\x83\xEC\x00\x49\x8B\xF1\x49\x8B\xF8\x48\x8B\xDA\x4C\x8B\xF1\xE8\x00\x00\x00\x00\x66\x41\xC7\x46\x00\x00\x00\x41\xC7\x46\x00\x00\x00\x00\x00\x33\xC9\x41\x89\x4E\x00\x41\xC7\x46\x00\x00\x00\x00\x00\x66\x41\x89\x4E\x00\x49\x89\x4E\x00\x48\x8D\x05\x00\x00\x00\x00\x49\x89\x06\x49\x8D\x4E\x00\x48\x8B\xD3\xE8\x00\x00\x00\x00\x90"), const_cast <char*>("xxxx?xxxx?xxxx?xxxx?xxxxx?xxxxxxxxxxxxx????xxxx???xxx?????xxxxx?xxx?????xxxx?xxx?xxx????xxxxxx?xxxx????x")));
	MH_CreateHook(CAddPlayerCommandHook, &hkCAddPlayerCommand, (LPVOID*)&CAddPlayerCommandTramp);
	MH_EnableHook(CAddPlayerCommandHook);
	
	AutoSaveFunc = GetAutoSave(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x0F\xB6\xDA\x48\x8B\xF9\xE8\x00\x00\x00\x00\x33\xC9"), const_cast <char*>("xxxx?xxxx?xxxxxxx????xx")));

	CGameStateSetPlayerHook = CGameStateSetPlayer(FindPattern(const_cast <char*>("\x40\x56\x57\x41\x57\x48\x83\xEC\x00\x8B\x02"), const_cast <char*>("xxxxxxxx?xx")));

	MH_CreateHook(CGameStateSetPlayerHook, &hkCGameStateSetPlayer, (LPVOID*)&CGameStateSetPlayerTramp);
	MH_EnableHook(CGameStateSetPlayerHook);

	CRemovePlayerCommandHook = GetCRemovePlayerCommand(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x48\x89\x4C\x24\x00\x41\x56\x48\x83\xEC\x00\x49\x8B\xF1\x41\x8B\xD8\x8B\xFA"), const_cast <char*>("xxxx?xxxx?xxxx?xxxx?xxxxx?xxxxxxxx")));

	MH_CreateHook(CRemovePlayerCommandHook, &hkCRemovePlayerCommand, (LPVOID*)&CRemovePlayerCommandTramp);
	MH_EnableHook(CRemovePlayerCommandHook);

	
	CCrashFunc = GetCCrash(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x8B\xDA\x48\x8B\xF9\xE8\x00\x00\x00\x00\x33\xC9\x89\x5F\x00\x48\x8B\x5C\x24\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x07\x48\x8B\xC7\x66\xC7\x47\x00\x00\x00\xC7\x47\x00\x00\x00\x00\x00\x89\x4F\x00\xC7\x47\x00\x00\x00\x00\x00\x66\x89\x4F\x00\x48\x89\x4F\x00\x48\x83\xC4\x00\x5F\xC3\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x48\x8D\x05"), const_cast <char*>("xxxx?xxxx?xxxxxx????xxxx?xxxx?xxx????xxxxxxxxx???xx?????xx?xx?????xxx?xxx?xxx?xxxxxx?xxxx?xxx")));
	MH_CreateHook(CCrashFunc, &hkCrash, (LPVOID*)&CCrashTramp);
	MH_EnableHook(CCrashFunc);



	CStartGameCommandFunc = GetCStartGameCommand(FindPattern(const_cast <char*>("\x40\x53\x48\x83\xEC\x00\x48\x8B\xD9\xE8\x00\x00\x00\x00\x33\xC9\x66\xC7\x43\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\xC7\x43\x00\x00\x00\x00\x00\x48\x89\x03\x48\x8B\xC3\x89\x4B\x00\xC7\x43\x00\x00\x00\x00\x00\x66\x89\x4B\x00\x48\x89\x4B\x00\x48\x83\xC4\x00\x5B\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x48\x8D\x05"), const_cast <char*>("xxxxx?xxxx????xxxxx???xxx????xx?????xxxxxxxx?xx?????xxx?xxx?xxx?xxxxxxxxxxxxxxxxxxxx?xxxx?xxx")));
	
	GetCCommandFunc = GetCCommand(FindPattern(const_cast <char*>("\xE9\x00\x00\x00\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xE9\x00\x00\x00\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x40\x53\x48\x83\xEC\x00\x33\xC0"), const_cast <char*>("x????xxxxxxxxxxxx????xxxxxxxxxxxxxxxx?xx")));
	
	AIEnableFunc = GetEnableAI(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x41\x8B\xF0\x48\x8B\xDA\x48\x8B\xF9\xE8\x00\x00\x00\x00\x33\xC9\x66\xC7\x47\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x89\x4F\x00\x48\x89\x07\x66\x89\x4F\x00\x48\x89\x4F\x00\xC7\x47\x00\x00\x00\x00\x00\xC7\x47\x00\x00\x00\x00\x00\x8B\x03\x48\x8B\x5C\x24\x00\x89\x47\x00\x48\x8B\xC7\x89\x77\x00\x48\x8B\x74\x24\x00\x48\x83\xC4\x00\x5F\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xF9"), const_cast <char*>("xxxx?xxxx?xxxx?xxxxxxxxxx????xxxxx???xxx????xx?xxxxxx?xxx?xx?????xx?????xxxxxx?xx?xxxxx?xxxx?xxx?xxxxxxxxxxxxxxxxxxx?xxxx?xxx")));
	
	CPauseGameFunc = GetCPauseGameCommand(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x4C\x24\x00\x57\x48\x83\xEC\x00\x41\x0F\xB6\xE9\x41\x0F\xB6\xF8\x48\x8B\xDA\x48\x8B\xF1\xE8\x00\x00\x00\x00\x66\xC7\x46\x00\x00\x00\xC7\x46\x00\x00\x00\x00\x00\x33\xC9\x89\x4E\x00\xC7\x46\x00\x00\x00\x00\x00\x66\x89\x4E\x00\x48\x89\x4E\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x06\x48\x8D\x4E\x00\x48\x8B\xD3\xE8\x00\x00\x00\x00\x40\x88\x7E\x00\x40\x88\x6E\x00\x48\x8B\xC6\x48\x8B\x5C\x24\x00\x48\x8B\x6C\x24\x00\x48\x8B\x74\x24\x00\x48\x83\xC4\x00\x5F\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x40\x53"), const_cast <char*>("xxxx?xxxx?xxxx?xxxx?xxxx?xxxxxxxxxxxxxxx????xxx???xx?????xxxx?xx?????xxx?xxx?xxx????xxxxxx?xxxx????xxx?xxx?xxxxxxx?xxxx?xxxx?xxx?xxxxxxxxxxxxxxxxx")));
	MH_CreateHook(CPauseGameFunc, &hkPauseGame, (LPVOID*)&CPauseGameTramp);
	MH_EnableHook(CPauseGameFunc);

	//Dont be mistaken these are different addresses!
	IncreaseSpeedFunc = GetCGameSpeed(FindPattern(const_cast <char*>("\x40\x53\x48\x83\xEC\x00\x48\x8B\xD9\xE8\x00\x00\x00\x00\x33\xC9\x66\xC7\x43\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\xC7\x43\x00\x00\x00\x00\x00\x48\x89\x03\x48\x8B\xC3\x89\x4B\x00\xC7\x43\x00\x00\x00\x00\x00\x66\x89\x4B\x00\x48\x89\x4B\x00\x48\x83\xC4\x00\x5B\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x8B\xDA"), const_cast <char*>("xxxxx?xxxx????xxxxx???xxx????xx?????xxxxxxxx?xx?????xxx?xxx?xxx?xxxxxxxxxxxxxxxxxxxx?xxxx?xx")));
	DecreaseSpeedFunc = GetCGameSpeed(FindPattern(const_cast <char*>("\x40\x53\x48\x83\xEC\x00\x48\x8B\xD9\xE8\x00\x00\x00\x00\x66\xC7\x43"), const_cast <char*>("xxxxx?xxxx????xxx")));

	GCDMF = GetCustomDiffM(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x4C\x24\x00\x57\x48\x83\xEC\x00\x41\x8B\xD8\x48\x8B\xFA\x48\x8B\xF1\xE8\x00\x00\x00\x00\x66\xC7\x46\x00\x00\x00\xC7\x46\x00\x00\x00\x00\x00\x33\xC9\x89\x4E\x00\xC7\x46\x00\x00\x00\x00\x00\x66\x89\x4E\x00\x48\x89\x4E\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x06\x48\x8D\x4E\x00\x48\x8B\xD7\xE8\x00\x00\x00\x00\x89\x5E\x00\x48\x8B\xC6\x48\x8B\x5C\x24\x00\x48\x8B\x74\x24\x00\x48\x83\xC4\x00\x5F\xC3\xCC\x48\x89\x5C\x24\x00\x57"), const_cast <char*>("xxxx?xxxx?xxxx?xxxx?xxxxxxxxxx????xxx???xx?????xxxx?xx?????xxx?xxx?xxx????xxxxxx?xxxx????xx?xxxxxxx?xxxx?xxx?xxxxxxx?x")));
	MH_CreateHook(GCDMF, &hkCustomDiffM, (LPVOID*)&GCDMH);
	MH_EnableHook(GCDMF);

	HostHook = GetFrontEndHostWithName(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x49\x8B\xF9\x49\x8B\xF0\x48\x8B\xEA\x48\x8B\xD9\x48\x8D\x54\x24"), const_cast <char*>("xxxx?xxxx?xxxx?xxxx?xxxxxxxxxxxxxxxx")));
	MH_CreateHook(HostHook, &hkHost, (LPVOID*)&HostTramp);
	MH_EnableHook(HostHook);



	SetDebugTooltipsEnabledHook = SetDebugTooltipsEnabled(FindPattern(const_cast <char*>("\x88\x51\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x40\x53"), const_cast <char*>("xx?xxxxxxxxxxxxxxx"))); //0x4D7B10 1.7.1 //0x8B69F0 modern

	MH_CreateHook(SetDebugTooltipsEnabledHook, &hkSetDebugTooltipsEnabled, (LPVOID*)&SetDebugTooltipsEnabledTramp);
	MH_EnableHook(SetDebugTooltipsEnabledHook);

	NetDisconnectHook = GetNetworkDisconnect(FindPattern(const_cast <char*>("\x48\x85\xC9\x74\x00\x48\x8B\x01\x48\xFF\x60\x00\x32\xC0\xC3\xCC\x48\x83\xEC"), const_cast <char*>("xxxx?xxxxxx?xxxxxxx")));
	MH_CreateHook(NetDisconnectHook, &hkNetDisconnect, (LPVOID*)&NetDisconnectTramp);
	MH_EnableHook(NetDisconnectHook);

	SetStateHook = GetSessionSetState(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x63\xFA"), const_cast <char*>("xxxx?xxxx?xxxx????xxx")));
	MH_CreateHook(SetStateHook, &hkSetState, (LPVOID*)&SetStateTramp);
	MH_EnableHook(SetStateHook);

	MatchmakingLeaveHook = GetMatchmakingLeaveLobby(FindPattern(const_cast <char*>("\x48\x83\xEC\x00\x80\x3D\x00\x00\x00\x00\x00\x74\x00\x80\x3D\x00\x00\x00\x00\x00\x75\x00\x48\x85\xC9\x75\x00\x48\x8D\x05\x00\x00\x00\x00\x88\x4C\x24\x00\x48\x89\x44\x24\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x44\x24\x00\x45\x33\xC9\x41\xB8\x00\x00\x00\x00\x48\x89\x44\x24\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x80\x7C\x24\x00\x00\x88\x05\x00\x00\x00\x00\x74\x00\xCC\x48\x83\xC4\x00\xC3\x48\x85\xC9\x74\x00\x48\x8B\x01\x48"), const_cast <char*>("xxx?xx?????x?xx?????x?xxxx?xxx????xxx?xxxx?xxx????xxxx?xxxxx????xxxx?xxx????x????xxx??xx????x?xxxx?xxxxx?xxxxxx?xxx????xxx?xxxxxxxx?xx?????x?xx?????x?xxxx?xxx????xxx?xxxx?xxx????xxxx?xxxxx????xxxx?xxx????x????xxx??xx????x?xxxx?xxxxx?xxxxxx?xxx????xxx?xxxxxxxx")));
	MH_CreateHook(MatchmakingLeaveHook, &hkMatchMakingLeave, (LPVOID*)&MatchmakingLeaveTramp);
	MH_EnableHook(MatchmakingLeaveHook);

	SendPacketHook = GetSendPacket(FindPattern(const_cast <char*>("\x48\x85\xC9\x74\x00\x48\x8B\x01\x4C\x8B\x50\x00\x0F\xB6\x44\x24\x00\x88\x44\x24\x00\x49\xFF\xE2\x32\xC0\xC3\xCC\xCC\xCC\xCC\xCC\x48\x85\xC9\x0F\x84"), const_cast <char*>("xxxx?xxxxxx?xxxx?xxx?xxxxxxxxxxxxxxxx")));
	MH_CreateHook(SendPacketHook, &hkSendPacket, (LPVOID*)&SendPacketTramp);
	MH_EnableHook(SendPacketHook);


	SeshFunc = GetSessionCon(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x4C\x24\x00\x57\x48\x83\xEC\x00\x41\x8B\xF9\x48\x8B\xDA"), const_cast <char*>("xxxx?xxxx?xxxx?xxxx?xxxxxx")));
	MH_CreateHook(SeshFunc, &hkSeshCon, (LPVOID*)&SeshTramp);
	MH_EnableHook(SeshFunc);

	JoinGameHook = GetCGameLobbyJoinGame(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x49\x8B\xF9\x49\x8B\xD8"), const_cast <char*>("xxxx?xxxx?xxxx?xxxxxxxxx????xxx????xxxxxx")));
	MH_CreateHook(JoinGameHook, &hkJoinGame, (LPVOID*)&JoinGameTramp);
	MH_EnableHook(JoinGameHook);

	SessionChangeHook = GetOnSessionChange(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x4C\x8B\xF9\x83\xFA"), const_cast <char*>("xxxx?xxxxxxxxxxxxxxx?xxx????xxxxx")));
	MH_CreateHook(SessionChangeHook, &hkSessionChange, (LPVOID*)&SessionChangeTramp);
	MH_EnableHook(SessionChangeHook);

	MultiplayerConfigHook = GetMultiplayerConfig(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x4C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x49\x8B\xF1\x49\x8B\xF8\x48\x8B\xDA\x48\x8B\xE9"), const_cast <char*>("xxxx?xxxx?xxxx?xxxxxxx?xxxxxxxxxxxx")));
	MH_CreateHook(MultiplayerConfigHook, &hkMultiplayerConfig, (LPVOID*)&MultiplayerConfigTramp);
	MH_EnableHook(MultiplayerConfigHook);



	HumanHook = GetAddHumanItem(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8B\xEC\x48\x83\xEC\x00\x4C\x8B\xEA\x4C\x8B\xF9"), const_cast <char*>("xxxx?xxxxxxxxxxxxxxxxx?xxxxxx")));
	MH_CreateHook(HumanHook, &hkHuman, (LPVOID*)&HumanTramp);
	MH_EnableHook(HumanHook);


	CConsoleHook = GetConsoleMenu(FindPattern(const_cast <char*>("\x40\x53\x48\x83\xEC\x00\x48\x8B\xD9\x48\x8B\x49\x00\x48\x85\xC9\x74\x00\x48\x8B\x01\xFF\x50\x00\x84\xC0\x74\x00\x48\x8B\x8B"), const_cast <char*>("xxxxx?xxxxxx?xxxx?xxxxx?xxx?xxx")));
	MH_CreateHook(CConsoleHook, &hkConsole, (LPVOID*)&CConsoleTramp);
	MH_EnableHook(CConsoleHook);

	//SetServerAddressHook = GetServerAddress(FindPattern(const_cast <char*>("\x89\x91\x00\x00\x00\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x49\x8B\xF0"), const_cast <char*>("xx????xxxxxxxxxxxxxx?xxxx?xxxx?xxx")));
	SetServerAddressHook = GetServerAddress(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x74\x24"), const_cast <char*>("xxxx?xxxx?xxxx????xxxx")));
	MH_CreateHook(SetServerAddressHook, &hkServerAddress, (LPVOID*)&SetServerAddressTramp);
	MH_EnableHook(SetServerAddressHook);

	CProxyServerHook = GetProxyServer(FindPattern(const_cast <char*>("\x48\x8B\xC4\x48\x89\x58\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x4D\x8B\xF1"), const_cast <char*>("xxxxxx?xxxxxxxxxxxxxx????xxx????xxx?xxx?xxx")));
	MH_CreateHook(CProxyServerHook, &hkCProxyServer, (LPVOID*)&CProxyServerTramp);
	MH_EnableHook(CProxyServerHook);


	CLogStreamHook = GetLogStream(FindPattern(const_cast <char*>("\x40\x53\x48\x83\xEC\x00\x48\x8B\xD9\x49\xC7\xC0\x00\x00\x00\x00\x49\xFF\xC0\x42\x80\x3C\x02\x00\x75\x00\xE8\x00\x00\x00\x00\x48\x8B\xC3\x48\x83\xC4\x00\x5B\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x89\x54\x24"), const_cast <char*>("xxxxx?xxxxxx????xxxxxxx?x?x????xxxxxx?xxxxxxxxxxxxxx")));
	MH_CreateHook(CLogStreamHook, &hkLogStream, (LPVOID*)&CLogStreamTramp);
	MH_EnableHook(CLogStreamHook);


	CLogHook = GetCLog(FindPattern(const_cast <char*>("\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x48\x89\x54\x24\x00\x41\x56\x48\x83\xEC\x00\x41\x8B\xF1"), const_cast <char*>("xxxx?xxxx?xxxx?xxxx?xxxxx?xxx")));
	MH_CreateHook(CLogHook, &hkLog, (LPVOID*)&CLogTramp);
	MH_EnableHook(CLogHook);

	COperatorHook = GetOperator(FindPattern(const_cast <char*>("\x48\x83\x7A\x00\x00\x72\x00\x48\x8B\x12\xE9\x00\x00\x00\x00\xCC\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x80\x3D"), const_cast <char*>("xxx??x?xxxx????xxxxx?xxxx?xx")));
	MH_CreateHook(COperatorHook, &hkOperator, (LPVOID*)&COperatorTramp);
	MH_EnableHook(COperatorHook);
}






	

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)& oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);

	HookFunctions();
	return TRUE;
}



BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);

		break;
	}
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}




/* MODERN = 1.10.8

void HookFunctions()
{
	void* d3d9Device[119];

	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
	{
		EndSceneHook = EndScene((uintptr_t)d3d9Device[42]);

		MH_CreateHook(EndSceneHook, &hkEndScene, (LPVOID*)&EndSceneTramp);
		MH_EnableHook(EndSceneHook);
	}

	CSessionPostHook = CSessionPost(GameBase + 0x13A48D0); //0xC1A4C0 1.7.1 //0x13A48D0 modern

	MH_CreateHook(CSessionPostHook, &hkCSessionPost, (LPVOID*)&CSessionPostTramp);
	MH_EnableHook(CSessionPostHook);

	CAddPlayerCommandHook = GetCAddPlayerCommand(GameBase + 0xF85030); //0x946F50 1.7.1 //0xF85030 modern

	MH_CreateHook(CAddPlayerCommandHook, &hkCAddPlayerCommand, (LPVOID*)&CAddPlayerCommandTramp);
	MH_EnableHook(CAddPlayerCommandHook);

	CGameStateSetPlayerHook = CGameStateSetPlayer(GameBase + 0x142090); //0xB8D50 1.7.1 //0x142090 modern

	MH_CreateHook(CGameStateSetPlayerHook, &hkCGameStateSetPlayer, (LPVOID*)&CGameStateSetPlayerTramp);
	MH_EnableHook(CGameStateSetPlayerHook);

	SetDebugTooltipsEnabledHook = SetDebugTooltipsEnabled(GameBase + 0x8B69F0); //0x4D7B10 1.7.1 //0x8B69F0 modern

	MH_CreateHook(SetDebugTooltipsEnabledHook, &hkSetDebugTooltipsEnabled, (LPVOID*)&SetDebugTooltipsEnabledTramp);
	MH_EnableHook(SetDebugTooltipsEnabledHook);

	CSetDLCsCommandFunc = GetCSetDLCsCommand(GameBase + 0xF851D0); //0x947110 1.7.1 //0xF851D0 modern
	CStartGameCommandFunc = GetCStartGameCommand(GameBase + 0xEBD230); //0x831910 1.7.1 //0xEBD230 modern
	GetCCommandFunc = GetCCommand(GameBase + 0x16EC008); //0x16EC008 modern
}

*/