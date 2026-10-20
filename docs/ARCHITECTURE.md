# Architecture

## Current foundation

The first milestone is a native desktop editor shell:

- **C++20** for engine/editor code.
- **CMake** for configuration and builds.
- **SDL3** for the desktop window, input events, and OpenGL context.
- **Dear ImGui docking branch** for editor panels and UI.
- **OpenGL 3.3 core context** as the initial graphics API.

The current editor is intended to have an in-memory scene-object list, a selectable hierarchy, editable name/active/transform fields, and a clearly labelled viewport placeholder. The viewport is editor UI only; it does not render scene geometry.

## Intended separation

As the project grows, editor UI should not own engine runtime logic. Planned boundaries:

- `engine/core`: lifecycle, logging, and common utilities.
- `engine/scene`: scene data and entity/component model.
- `engine/render`: graphics API abstraction and rendering.
- `engine/physics`: physics integration and collision queries.
- `engine/assets`: asset references, import pipeline, and resource lifetime.
- `editor`: scene authoring UI and editor-only tools.
- `tests`: automated tests for engine systems.

These are architectural targets, not implemented modules. Introduce them when there is real behavior to test rather than adding empty layers for appearances.

## Dependency policy

Third-party dependencies are fetched at pinned tags through CMake FetchContent, not vendored into this repository. The first configure therefore needs network access. Review upstream licenses and notices before distributing builds.

## Engineering rules

1. Label placeholders honestly.
2. Keep the default build reproducible and straightforward.
3. Add tests alongside real engine behavior.
4. Do not add accounts, telemetry, cloud dependencies, subscriptions, or licensing gates.
5. Prefer small milestones that can be built and verified independently.
