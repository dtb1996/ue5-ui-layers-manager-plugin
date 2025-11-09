# UE5 UI Layers Manager Plugin

A lightweight Unreal Engine plugin for managing layered UI systems with async widget loading.

## Features

- Centralized UI management via `UUILayersManagerSubsystem`
- Support for multiple UI layers (game, menu, modals, etc.)
- Async widget loading and soft references
- Easily integrated with `AUIHUD`

## Installation

1. Clone or download this repo.
2. Copy the `UILayersManager` folder into your project's `Plugins/` directory.
3. Rebuild your project in Unreal.

## Example Setup

- Add a `PrimaryLayout` widget with layer definitions.
- In your HUD, spawn and initialize the layout.
- Use the subsystem to `PushToLayer()` and `PopFromLayer()` as needed.
