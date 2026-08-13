# CopyMaple2 프로젝트 문서

MapleStory2를 모방한 3D MMORPG 클라이언트입니다. **DirectX 11** 기반의 자체 제작 게임 엔진(`Engine`) 위에서 게임 로직(`Client`)이 동작하는 구조입니다.

> 이 문서들은 코드베이스 구조 파악을 돕기 위한 참고용입니다. 코드가 바뀌면 함께 갱신하세요.

## 문서 목록

| 문서 | 내용 |
|------|------|
| [01-아키텍처.md](01-아키텍처.md) | 솔루션/프로젝트 구성, 빌드 파이프라인, 실행 흐름, 핵심 디자인 패턴 |
| [02-엔진.md](02-엔진.md) | Engine DLL의 매니저·컴포넌트·렌더링 구조 |
| [03-클라이언트.md](03-클라이언트.md) | 게임플레이 시스템(레벨, 캐릭터, 스킬, UI, 데이터베이스 등) |
| [04-리소스와데이터.md](04-리소스와데이터.md) | 리소스 디렉토리, JSON 데이터 파일, 에셋 변환 툴 |

## 한눈에 보기

```
CopyMaple2.sln
├── Engine        (ENGINE_DLL)  - 자체 제작 게임 엔진 → Engine.dll
├── Client        (실행 파일)    - 게임 로직 → Client.exe  (Engine.dll 링크)
├── EngineSDK     (정적 라이브러리/헤더 모음) - Engine 빌드 산출물이 복사됨
├── FBXConverter  (콘솔 툴)      - FBX → 자체 바이너리(.model) 변환
└── NIFToBinary   (콘솔 툴)      - NIF(이펙트) → 자체 바이너리 변환
```

- **언어/환경**: C++17, Visual Studio 2022(v17.11), Windows x64
- **그래픽스**: DirectX 11, Deferred Rendering, DirectXTK, Effects11(`.fx`/HLSL)
- **외부 라이브러리**: assimp(모델 로딩), niflib(NIF 파싱), nlohmann/json(데이터 직렬화), DirectSound(사운드)
- **네임스페이스**: 엔진은 `Engine`, 게임은 `Client` (`BEGIN(Engine)` / `END` 매크로 사용)
