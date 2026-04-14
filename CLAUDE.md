# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

CopyMaple2 is a MapleStory-inspired 3D RPG built on a custom DirectX 11 game engine. The codebase is split into an Engine DLL and a Client executable, with an SDK layer bridging them. Comments and commits are primarily in Korean.

## Build

- **IDE:** Visual Studio 2022 (v143 toolset, C++17, Unicode)
- **Solution:** `CopyMaple2.sln` — contains Engine, Client, FBXConverter, NIFToBinary projects
- **Build from CLI:** `msbuild CopyMaple2.sln /p:Configuration=Release /p:Platform=x64`
- **Build order:** Engine (DLL) → Client (EXE). Client depends on Engine.
- **Post-build:** `UpdateLib.bat` copies Engine DLL/LIB/headers/shaders into `EngineSDK/` so Client can link against them. Run this manually if Engine public headers change and the post-build step doesn't trigger.
- **Output:** Engine DLL → `Engine/Bin/`, Client EXE → `Client/Bin/`
- **No test framework** — no unit tests exist.

## Architecture

### Three-Layer Structure

```
Engine (DLL)  →  EngineSDK (headers + libs)  →  Client (EXE)
```

- **Engine/Public/** — headers exposed to SDK. Changes here require running `UpdateLib.bat`.
- **Engine/Private/** — engine implementation.
- **EngineSDK/Inc/** — mirror of Engine/Public/ headers, consumed by Client.
- **Client/Public/** — game-specific headers (characters, UI, levels, skills).
- **Client/Private/** — game logic implementation (~100+ source files).

### Core Patterns

**Reference-counted base class (`CBase`):** All engine objects inherit from `CBase` which provides `AddRef()`/`Release()`. Use `Safe_AddRef()` and `Safe_Release()` macros.

**Component-based GameObjects:** `CGameObject` holds a `map<wstring, CComponent*>` of components (`CTransform`, `CRenderer`, `CCollider*`, `CStateMachine`, etc.). Components and GameObjects both follow a `Clone()` prototype pattern.

**Singleton managers via `CGameInstance`:** Central facade providing access to all subsystems — rendering, input, timing, level management, object management, collision, UI, physics, audio, events, lighting, fonts.

**Prototype pattern:** Objects/components are registered as prototypes in `CPrototype_Manager`, then cloned when instantiated.

**Level/Layer system:** Levels (`CLevel`) contain Layers (`CLayer`), which hold GameObjects. Layer IDs: `LAYER_TERRAIN`, `LAYER_PLAYER`, `LAYER_BULLET`, `LAYER_MONSTER`, `LAYER_INTERACTION`, `LAYER_UI`, `LAYER_CAMERA`, `LAYER_NONCOLLISION`, `LAYER_WORLD_ITEM`.

### Update Lifecycle (per frame, 60 FPS locked)

```
Priority_Update → Update → Compute_Matrix → Late_Update → Final_Update → Render
```

### Entry Point

`Client/Default/Client.cpp` → `wWinMain()` → creates `CMainApp` → initializes `CGameInstance` → loads `LEVEL_LOGO` → enters main loop.

## Key Dependencies

- DirectX 11, DirectXTK, Effects11 (in `Engine/ThirdPartyLib/`)
- DirectInput (keyboard/mouse), DirectSound (audio)
- Assimp (FBX model loading)
- nlohmann/json (`json.hpp` — item/skill/quest data serialization)
- Niflib (NIF model loading, used by NIFToBinary converter)

## Conventions

- Wide strings (`wstring`, `_wstring`) used throughout for Unicode/Korean text support.
- Custom typedefs: `_float`, `_uint`, `_int`, `_bool`, `_float3`, `_float4x4` etc. wrapping DirectXMath types.
- HRESULT return values for initialization/setup functions; check with `FAILED()` macro.
- Shader files (HLSL 5.0) live in `EngineSDK/hlsl/`, compiled at runtime.
- Window size: 1280×720 (defined in `Client_Defines.h`).
