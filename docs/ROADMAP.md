# Roadmap

A checkbox means implemented and verified, not merely planned.

## Milestone 0 — Repository and editor foundation

- [x] Establish C++20 and CMake project configuration.
- [x] Add an SDL3 desktop window and Dear ImGui docking shell source.
- [x] Plan an in-memory hierarchy and editable transform fields in the editor shell.
- [x] Mark the scene viewport as a placeholder.
- [ ] Build and run on Windows, Linux, and macOS.
- [ ] Add automated build checks for supported platforms.

## Milestone 1 — Scene model

- [ ] Introduce stable entity identifiers.
- [ ] Separate scene data from editor UI.
- [ ] Implement create, rename, duplicate, and delete operations.
- [ ] Add scene serialization and safe load/save.
- [ ] Add unit tests for scene operations and serialization.

## Milestone 2 — Real 3D rendering

- [ ] Add a renderer module and explicit resource lifetime rules.
- [ ] Render a triangle, then a lit mesh, then a textured mesh.
- [ ] Add a perspective camera and editor camera navigation.
- [ ] Render scene objects in the viewport.
- [ ] Add grid, selection outline, and transform gizmos backed by scene data.

## Milestone 3 — Assets and workflow

- [ ] Configure a project root and content browser.
- [ ] Import common mesh and texture formats.
- [ ] Track asset metadata and report import errors clearly.
- [ ] Add drag-and-drop asset placement.

## Milestone 4 — Physics and gameplay

- [ ] Choose and integrate a suitable open-source physics library.
- [ ] Add rigid bodies, colliders, layers, and physics debugging.
- [ ] Define a game/runtime loop separate from editor UI.
- [ ] Establish a scripting direction and document its trade-offs.

## Milestone 5 — Production foundations

- [ ] Add automated tests and CI build checks.
- [ ] Add profiling and useful diagnostic logs.
- [ ] Add animation, audio, packaging, and export workflows.
- [ ] Document supported platforms, dependency licenses, and limitations.

## Release principle

Do not call a milestone complete until its behavior is implemented, tested, and documented. The project is early-stage software and is not yet suitable as a replacement for mature commercial engines.
