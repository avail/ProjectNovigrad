#include <Windows.h>
#include <fstream>
#include "stdlib.h"
#include <iostream>
#include "World.h"
#include "Name.h"
#include "IInputListener.h"
#include "Node.h"
#include "layer.h"
#include "System.h"
#include "../hooking/Hooking.h"
#include "includes.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;


HANDLE thread = nullptr;

bool UIOpen = true;

ProjectNovigrad::CSystem* thePSystem = nullptr;

class CMod : public IInputListener
{
public:
  ProjectNovigrad::CSystem* m_pSystem;
  ProjectNovigrad::TW3::CEntity** pEntities;
  CMod(ProjectNovigrad::CSystem* pSystem)
  {
    m_pSystem = pSystem;
    pSystem->GetInput()->RegisterInputListener(this);
  }

  ~CMod()
  {
  }

  void OnInputEvent(EInputAction inputAction, EInputKey inputKey, float tick)
  {
    if (inputAction == EInputAction::IACT_Release && inputKey == EInputKey::IK_F10)
    {
      /*auto entities = new ProjectNovigrad::TW3::TDynArray<ProjectNovigrad::TW3::CEntity*>();
      entities->base_pointer = nullptr;
      entities->begin = 0;
      entities->max = 100;
      entities->count = 0;
      pSystem->GetGame()->GetMainLayer()->GetEntities(entities);
      pEntities = entities->base_pointer;*/
    }

    if (inputAction == EInputAction::IACT_Release && inputKey == EInputKey::IK_F11)
    {
      /*auto esp = new ProjectNovigrad::TW3::EntitySpawnInfo;
      auto entityName = ProjectNovigrad::TW3::CreateTString(std::wstring(L"items\work\carry_crate\carry_crate_cage.w2ent"));
      auto entityBaseSafeHandle = new ProjectNovigrad::TW3::BaseSafeHandle();

      pSystem->GetDepot()->LoadResource(&entityBaseSafeHandle, entityName, true);

      entityBaseSafeHandle->operator=(&esp->field_24);

      pSystem->GetGame()->GetMainLayer()->CreateEntitySync(esp);

      //wprintf(L"new entity friendly name? %ls\n", str1.ptr);
      //printf("new entity pos %f.3 %f.3 %f.3\n", newent->GetWorldPosition().x, newent->GetWorldPosition().y, newent->GetWorldPosition().z);
      */
    }

    if (inputAction == EInputAction::IACT_Release && inputKey == EInputKey::IK_F9)
    {
      /*char* buffer = new char[1000];
      auto pEnt = pSystem->GetGame()->GetPlayerEntity();
      auto pos = pEnt->GetWorldPosition();
      ProjectNovigrad::TW3::EulerAngles angles;
      angles.pitch = 0.0f;
      angles.roll = 0.0f;
      angles.yaw = 0.0f;

      auto selectedEntity = pEntities[rand() % 100];
      selectedEntity->Teleport(&pos, &angles);
      auto entPos = selectedEntity->GetWorldPosition();
      ProjectNovigrad::TW3::TString<wchar_t> friendlyName;
      selectedEntity->GetFriendlyName(&friendlyName);
      wprintf(L"Entity %ls teleported to player.\n", friendlyName.ptr);

      pSystem->GetGame()->GetActiveWorld()->SetWaterVisible(true, true);
      //pSystem->GetGame()->GetActiveWorld()->Shutdown();
      wprintf(L"pPlayerEntityPos = %ls %ls %ls\n", std::to_wstring(pos.x).c_str(), std::to_wstring(pos.y).c_str(), std::to_wstring(pos.z).c_str());
      delete buffer;*/

    }
    thePSystem->GetGame()->ProcessFreeCameraInput(inputKey, inputAction, tick);
  }

  void Run()
  {
    auto pGame = m_pSystem->GetGame();
    if (!pGame) {
      printf("pGame = nullptr"); 
      return;
    }
    printf("Game gotten\n");
    while (pGame == nullptr) {
      Sleep(500);
    }
    printf("pGame = %d\n", pGame);
  }
};

CMod* pMod = nullptr;

struct HandleData
{
    DWORD pid;
    HWND hWnd;
};

HWND FindMainWindow(DWORD dwPID);
BOOL CALLBACK EnumWindowsCallback(HWND hWnd, LPARAM lParam);

HWND FindMainWindow(DWORD dwPID)
{
    HandleData handleData{ 0 };
    handleData.pid = dwPID;
    EnumWindows(EnumWindowsCallback, (LPARAM)&handleData);
    return handleData.hWnd;
}

BOOL EnumWindowsCallback(HWND hWnd, LPARAM lParam)
{
    HandleData& data = *(HandleData*)lParam;
    DWORD pid = 0;
    GetWindowThreadProcessId(hWnd, &pid);
    if (pid == data.pid && GetWindow(hWnd, GW_OWNER) == HWND(0) && IsWindowVisible(hWnd))
    {
        data.hWnd = hWnd;
        return FALSE;
    }

    return TRUE;
}

bool init = false;
char execinputbuf[128] = "";

void InitImGui(HWND window)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    io.MouseDrawCursor = true;
    RECT rect;
    if (GetClientRect(window, &rect))
    {
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        io.DisplaySize = ImVec2(width, height);
    }
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void Exec(char* command)
{

}

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    if (!init)
    {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
        {
            pDevice->GetImmediateContext(&pContext);
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            window = FindMainWindow(GetCurrentProcessId());
            ID3D11Texture2D* pBackBuffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
            pBackBuffer->Release();
            oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
            InitImGui(window);
            init = true;
        }

        else
            return oPresent(pSwapChain, SyncInterval, Flags);
    }

    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Insert)))
    {
        std::cout << "INFO: Menu opened/closed!" << std::endl;
        UIOpen = !UIOpen;
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDrawCursor = UIOpen;
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    if (UIOpen)
    {
        ImGui::Begin("ProjectNovigrad - Debug UI", &UIOpen, ImGuiWindowFlags_MenuBar);
        ImGui::Text("Input any exec command here:");
        ImGui::InputText("string", execinputbuf, IM_ARRAYSIZE(execinputbuf));
        if (ImGui::Button("Execute"))
        {
            std::ostringstream os;
            os << "Executed \"" << std::string(execinputbuf) << "\" in the CScriptSystem of the game!";
            MessageBoxA(NULL, std::string(os.str()).c_str(), "Info", MB_OK | MB_ICONINFORMATION);
        }
    }
    ImGui::End();

    ImGui::Render();

    pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI InitializeHook(void* arguments) {
  AllocConsole();
  FILE* file = nullptr;
  freopen_s(&file, "CONIN$", "r", stdin);
  freopen_s(&file, "CONOUT$", "w", stdout);
  system("title ProjectNovigrad - DEBUG CONSOLE [Open/Close UI with INSERT key]");
  std::cout << "Initializing" << std::endl;
  hook::set_base();
  auto version_sig = hook::pattern("48 FF 42 30 48 8D 05 ? ? ? ?");
  auto gamever = version_sig.count(1).get(0).extract<char*>(0x7);
  auto ver = std::string(gamever);
  if(ver.compare(std::string("v 1.32")) != 0)
  {
      MessageBoxA(NULL, "Unsupported game version! Only v1.32 is supported!", "Error", MB_OK | MB_ICONERROR);
      exit(0x0);
  }
  else
  {
      std::cout << "Found game version: witcher3.exe [" << ver << "]" << std::endl;
  }
  std::cout << "Initializing kiero dx11 hook..." << std::endl;
  bool init_hook = false;
  do
  {
      if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
      {
          kiero::bind(8, (void**)&oPresent, hkPresent);
          init_hook = true;
      }
  } while (!init_hook);
  std::cout << "DirectX 11 hook for imgui initialized!" << std::endl;
  //thePSystem = new ProjectNovigrad::CSystem;
  //thePSystem->Init();
  //std::cout << "System initialized" << std::endl;
  //pMod = new CMod(thePSystem);
  //pMod->Run();
  return 1;
}


int WINAPI DllMain(HINSTANCE instance, DWORD reason, PVOID reserved) {
  if (reason == DLL_PROCESS_ATTACH) {
    DisableThreadLibraryCalls(instance);
    thread = CreateThread(nullptr, 0, InitializeHook, 0, 0, nullptr);
  } else if (reason == DLL_PROCESS_DETACH) {
    kiero::shutdown();
    FreeConsole();
    delete pMod;
    delete thePSystem;
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    delete execinputbuf;
  }
  return 1;
}
