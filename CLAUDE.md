# CLAUDE.md

이 저장소에서 작업할 때 참고할 지침입니다.

## 프로젝트 개요

MapleStory2를 모방한 DirectX 11 기반 3D MMORPG 클라이언트. 자체 제작 게임 엔진(`Engine` → `Engine.dll`) 위에서 게임 로직(`Client` → `Client.exe`)이 동작합니다. C++17 / Visual Studio 2022 / Windows x64.

## 프로젝트 문서 (`Docs/`)

구조 파악이나 특정 시스템을 다룰 때 아래 문서를 **먼저 찾아 읽으세요**. 코드를 수정하면 관련 문서도 갱신합니다.

| 문서 | 언제 읽나 |
|------|-----------|
| [Docs/README.md](Docs/README.md) | 전체 개요와 문서 인덱스 |
| [Docs/01-아키텍처.md](Docs/01-아키텍처.md) | 솔루션 구성·빌드 파이프라인·실행 흐름·디자인 패턴·객체 모델 |
| [Docs/02-엔진.md](Docs/02-엔진.md) | `Engine` DLL — 매니저(CGameInstance)·렌더링·컴포넌트·상태머신·이벤트 |
| [Docs/03-클라이언트.md](Docs/03-클라이언트.md) | `Client` 게임 로직 — 레벨·캐릭터·스킬·이펙트·건축·UI·데이터베이스 |
| [Docs/04-리소스와데이터.md](Docs/04-리소스와데이터.md) | 리소스 디렉토리·JSON 데이터·에셋 변환 툴(FBX/NIF)·빌드 배치 |

## 코드베이스 탐색 팁

- 소스만 빠르게 보려면 `Client/Bin/` 경로(대용량 리소스 수천 개)를 제외하세요:
  `git ls-files "*.cpp" "*.h" | grep -v "Bin/"`
- 게임의 핵심 enum/전역 상수는 `Client/Public/Client_Defines.h`(레벨·레이어·아이템·스킬·몬스터·퀘스트 ID)와 `Engine/Public/Engine_Defines.h`에 집중되어 있습니다.
- 정적 게임 데이터는 거의 전부 `Client/Bin/Resources/Json/`의 JSON으로 주도되며, enum↔문자열 매핑은 `Client_Defines.h`의 `NLOHMANN_JSON_SERIALIZE_ENUM`에 있습니다.

## 빌드 / 작업 주의사항

- **엔진(`Engine`)을 수정한 뒤에는 루트의 `UpdateLib.bat`을 실행**해야 산출물(DLL·lib·헤더·셰이더)이 `EngineSDK/`와 `Client/Bin/`으로 복사되어 클라이언트가 최신 엔진을 사용합니다.
- `EngineSDK/`(헤더/lib)와 `Engine/Bin/`은 빌드 산출물 복사본입니다. 엔진 헤더 원본은 `Engine/Public/`에서 편집하세요.
- 실행 작업 디렉토리는 `Client/Bin/`이며, 리소스·셰이더·데이터는 이 위치 기준 상대 경로로 로드됩니다.
