# SlideEditor CLI

SlideEditor is a command-line interface (CLI) application for creating, managing, and serializing presentation slides. It allows users to create slides, add text and shapes, remove elements, and save/load presentations in JSON format. The project is written in C++ with a modular design, using design patterns such as Factory, Command, Repository, and Strategy to ensure extensibility and maintainability.

## Features

- **Slide Management**:
  - Create slides with a unique ID, title, content, and theme.
  - Display all slides with their details (ID, title, content, theme, texts, shapes).
- **Text Management**:
  - Add text to slides with customizable properties: size (required), font, color, and line width (optional).
  - Remove text from slides by index.
- **Shape Management**:
  - Add shapes (Triangle, Circle, Rectangle, Ellipse) to slides with a specified scale.
  - Remove shapes from slides by index.
- **Serialization**:
  - Save presentations to JSON files.
  - Load presentations from JSON files, preserving slide and element data.
- **Command Processing**:
  - Parse commands with a lexer and parser, supporting flags for text properties.
  - Validate command arguments and provide error messages for invalid inputs.
- **Help System**:
  - Display all available commands when run without arguments (`./SlideEditor`) or with `help`.
  - Show detailed command usage and flags with `help <command>`.

## Project Structure

The project is organized into modular directories for clarity and maintainability:

```
SlideEditor/
├── include/
│   ├── interfaces/
│   │   ├── IShape.hpp
│   │   ├── ISlideRepository.hpp
│   │   ├── ISerializer.hpp
│   │   └── ICommand.hpp
│   ├── core/
│   │   ├── Shape.hpp
│   │   ├── Slide.hpp
│   │   ├── SlideFactory.hpp
│   │   └── SlideRepository.hpp
│   ├── commands/
│   │   └── Commands.hpp
│   ├── serialization/
│   │   └── JsonSerializer.hpp
│   ├── parser/
│   │   ├── Lexer.hpp
│   │   ├── Parser.hpp
│   │   └── Tokenizer.hpp
│   └── CommandFactory.hpp
├── src/
│   ├── core/
│   │   ├── Shape.cpp
│   │   ├── Slide.cpp
│   │   ├── SlideFactory.cpp
│   │   └── SlideRepository.cpp
│   ├── commands/
│   │   └── Commands.cpp
│   ├── serialization/
│   │   └── JsonSerializer.cpp
│   ├── parser/
│   │   ├── Lexer.cpp
│   │   ├── Parser.cpp
│   │   └── Tokenizer.cpp
│   ├── CommandFactory.cpp
│   └── main.cpp
├── external/
│   └── nlohmann/
│       └── json.hpp
├── README.md
├── Makefile
└── SlideEditor
```

- **include/**: Header files (`.hpp`) with declarations.
  - **interfaces/**: Abstract interfaces (`IShape`, `ISlideRepository`, `ISerializer`, `ICommand`).
  - **core/**: Domain logic (`Slide`, `Text`, shape classes, factories, repository).
  - **commands/**: Command implementations for CLI operations.
  - **serialization/**: JSON serialization logic.
  - **parser/**: Command parsing (lexer, parser, tokens).
- **src/**: Source files (`.cpp`) with implementations.
- **external/**: Third-party dependency (`nlohmann/json.hpp`).
- **main.cpp**: CLI entry point, handling command-line and interactive input.

## Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/buniatyann/Slide-Editor
   cd SlideEditor
   ```

2. **Download Dependency**:
   - Download `nlohmann/json.hpp` from [nlohmann/json](https://github.com/nlohmann/json) and place it in `external/nlohmann/`.

3. **Compile**:
   - Use a C++ compiler (e.g., `g++`) with C++11 or later.
   ```bash
   g++ -std=c++20 -Iinclude -Iexternal src/core/*.cpp src/commands/*.cpp src/serialization/*.cpp src/parser/*.cpp src/CommandFactory.cpp src/main.cpp -o SlideEditor
   ```

4. **Run**:
   ```bash
   ./SlideEditor
   ```

## Usage

SlideEditor can be used in two modes:
- **Command-Line Mode**: Pass a command as arguments (e.g., `./SlideEditor addtext 1 Hello --size 1.5`).
- **Interactive Mode**: Run `./SlideEditor` and enter commands at the prompt.

### Commands

Run `./SlideEditor` or `help` to list all commands. Use `help <command>` for details on a specific command.

| Command | Syntax | Description | Flags |
|---------|--------|-------------|-------|
| `create` | `create <title> <content> <theme>` | Creates a slide with title, content, and theme. | None |
| `addtext` | `addtext <id> <text> --size <float> [--font <font>] [--color <color>] [--line-width <float>]` | Adds text to slide `<id>` with specified properties. | `--size <float>` (required), `--font <font>` (default: Arial), `--color <color>` (default: Black), `--line-width <float>` (default: 1.0) |
| `addshape` | `addshape <id> <type> <scale>` | Adds a shape (Triangle, Circle, Rectangle, Ellipse) to slide `<id>` with scale. | None |
| `removetext` | `removetext <id> <index>` | Removes text at `<index>` from slide `<id>`. | None |
| `removeshape` | `removeshape <id> <index>` | Removes shape at `<index>` from slide `<id>`. | None |
| `save` | `save <file>` | Saves presentation to `<file>` in JSON format. | None |
| `load` | `load <file>` | Loads presentation from `<file>`. | None |
| `display` | `display` | Displays all slides with details. | None |
| `help` | `help [command]` | Shows all commands or details for `<command>`. | None |
| `exit` | `exit` | Exits interactive mode. | None |

### Example Usage

1. **Create a slide**:
   ```bash
   create Intro Welcome Modern
   ```
   Output: `Created slide with ID: 1`

2. **Add text with flags**:
   ```bash
   addtext 1 Hello --size 1.5 --font Times --color Red --line-width 2.0
   ```
   Output: `Added text to slide 1`

3. **Add a shape**:
   ```bash
   addshape 1 Triangle 1.5
   ```
   Output: `Added shape Triangle to slide 1`

4. **Display slides**:
   ```bash
   display
   ```
   Output:
   ```
   Slide ID: 1
   Title: Intro
   Content: Welcome
   Theme: Modern
   Texts:
     [0] Hello (size: 1.5, font: Times, color: Red, lineWidth: 2.0)
   Shapes:
     [0] Triangle (scale: 1.5)
   -------------------
   ```

5. **Save presentation**:
   ```bash
   save presentation.json
   ```
   Output: `Saved presentation to presentation.json`

6. **Show help**:
   ```bash
   ./SlideEditor
   ```
   or
   ```bash
   help
   ```
   Output: Lists all commands.

7. **Show command details**:
   ```bash
   help addtext
   ```
   Output: Details `addtext` syntax and flags.

### JSON Format

Presentations are saved/loaded in JSON format:

```json
{
    "language": "C++",
    "slides": [
        {
            "id": 1,
            "title": "Intro",
            "content": "Welcome",
            "theme": "Modern",
            "texts": [
                {
                    "content": "Hello",
                    "size": 1.5,
                    "font": "Times",
                    "color": "Red",
                    "lineWidth": 2.0
                }
            ],
            "shapes": [
                {
                    "type": "Triangle",
                    "scale": 1.5
                }
            ]
        }
    ]
}
```

## Design Patterns

- **Factory Pattern**: `ShapeFactory` and `SlideFactory` create shapes and slides.
- **Command Pattern**: `ICommand` implementations (`CreateSlideCommand`, `AddTextCommand`, etc.) encapsulate CLI actions.
- **Repository Pattern**: `ISlideRepository` and `SlideRepository` manage slide storage.
- **Strategy Pattern**: `ISerializer` supports extensible serialization (currently JSON).
- **Interpreter Pattern**: `Lexer` and `Parser` process command input.

## Limitations

- **Argument Parsing**: Assumes no spaces in arguments (e.g., `Hello` instead of `Hello World`). Enhance with quoted strings for production use.
- **Text Properties**: Font, color, and line width are stored but not fully processed (marked with `// TODO: Implement functionality` in code).
- **Error Handling**: Uses `std::invalid_argument` and `std::out_of_range` for invalid inputs/indices.

## Future Improvements

- Add support for quoted strings in command arguments.
- Implement font, color, and line width processing for rendering (currently TODO).
- Extend with additional shape properties or new shape types.
- Add undo/redo functionality using the Command pattern.
- Support additional serialization formats (e.g., XML) via `ISerializer`.

## Build Requirements

- C++11 or later.
- `nlohmann/json.hpp` (single-header JSON library).
- Compiler: `g++`, `clang++`, or equivalent.
