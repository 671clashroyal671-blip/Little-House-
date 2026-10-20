# Little Hous Engine

**A free, open-source game engine project by Little Hous Group.**

Little Hous Engine is being built from the ground up with a focus on capable 3D graphics, physics, flexible gameplay systems, and a professional desktop development experience. The goal is to give creators a free engine without subscriptions, account requirements, or cloud lock-in.

> **Status: early development.** The repository currently contains the initial native editor shell. It is not yet a functioning 3D game engine and is not a replacement for mature engines.

## What exists now

- C++20 and CMake project foundation.
- SDL3 desktop window and OpenGL context.
- Dear ImGui docking editor layout.
- In-memory hierarchy with selectable scene objects.
- Inspector fields for object name, active state, position, rotation, and scale.
- A viewport placeholder that is deliberately labelled; it does not render 3D scene geometry yet.

## Planned

- Independent scene/entity model with reliable save/load.
- Real-time 3D rendering and editor camera.
- Asset import and project management.
- Physics simulation and collision tools.
- Gameplay runtime, scripting, animation, audio, profiling, and packaging.

Track progress in [the roadmap](docs/ROADMAP.md). Read [the architecture notes](docs/ARCHITECTURE.md) for current technical boundaries.

## Build

### Prerequisites

- CMake 3.25 or newer
- Ninja
- A C++20 compiler
- Git
- Platform graphics/window development dependencies (Linux may need SDL3's X11/Wayland and audio development packages)

The first configure downloads SDL3 and Dear ImGui at pinned tags using CMake FetchContent, so it needs network access.

### Configure and compile

```sh
cmake --preset default
cmake --build --preset default
```

The editor executable is produced in the CMake build directory. Run it from your platform's build output location.

For an optimized build:

```sh
cmake --preset release
cmake --build --preset release
```

## Project principles

- Free and open source.
- No login, subscription, telemetry, or cloud service required by the engine.
- Honest status labels: placeholders are not advertised as finished features.
- Small, testable milestones over feature checklists that do not work.
- Third-party libraries keep their own licenses.

## Contributing

Issues and pull requests are welcome. Please include reproduction steps for bugs and keep changes focused. Do not mark roadmap items complete until they have been implemented and verified.

## License

Original project code is released under the MIT License. Third-party dependencies remain under their respective licenses.
