# Project Novigrad

**Write native mods for The Witcher 3**

## Building
 1. Clone the project:
 ```bash
 git clone https://github.com/Traderain/ProjectNovigrad --recursive -j 8
 ```
 2. Open ProjectNovigrad.sln in Visual Studio 2019
 3. Set TW3_DIR to your game's root directory eg.: "D:\SteamLibrary\steamapps\common\TW3"
 4. Compile the dll in Release/x64 mode
 5. Run the game
 6. The ImGUI overlay can be toggled with the insert key

## Project structure
```
  ├───hooking -> Hooking library from CitizenFX used in the project
  ├───jitasm -> Hooking library from CitizenFX used in the project
  ├───src -> The actual source files for ProjectNovigrad
  │   ├───imgui -> UI drawing library used by the project
  │   └───kiero -> DirectX11 hooking library
  │       └───minhook -> Hooking library used by kiero
  └───vtable -> vtable hooking utils
```
