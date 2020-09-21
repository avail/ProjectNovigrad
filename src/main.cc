#include <windows.h>
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


HANDLE thread = nullptr;

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

DWORD WINAPI InitializeHook(void* arguments) {
  AllocConsole();
  FILE* file = nullptr;
  freopen_s(&file, "CONIN$", "r", stdin);
  freopen_s(&file, "CONOUT$", "w", stdout);
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
  //thePSystem = new ProjectNovigrad::CSystem;
  //thePSystem->Init();
  //std::cout << "System initialized" << std::endl;
  //pMod = new CMod(thePSystem);
  //pMod->Run();
  return 1;
}


int WINAPI DllMain(HINSTANCE instance, DWORD reason, PVOID reserved) {
  if (reason == DLL_PROCESS_ATTACH) {

    thread = CreateThread(nullptr, 0, InitializeHook, 0, 0, nullptr);
  } else if (reason == DLL_PROCESS_DETACH) {
    FreeConsole();
    delete pMod;
    delete thePSystem;
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
  }
  return 1;
}
