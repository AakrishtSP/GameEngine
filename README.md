# Game Engine

Welcome to **Game Engine** — a customizable, lightweight engine for developing 2D  games with efficient collision detection, physics, and rendering capabilities.

![Engine Screenshot](path/to/your/screenshot.png)  
*(Add a screenshot or GIF here if available)*

## Table of Contents
1. [Overview](#overview)
2. [Features](#features)
3. [Getting Started](#getting-started)
4. [Dependencies](#dependencies)
5. [Installation](#installation)
6. [Usage](#usage)
7. [Code Structure](#code-structure)
8. [Contributing](#contributing)
9. [License](#license)

---

## Overview

The **Game Engine** project provides a foundational framework for game developers, offering essential components such as collision detection, physics, rendering, and scene management. Designed with performance and modularity in mind, it utilizes **C++** and leverages efficient algorithms for real-time applications. Key modules include the **GJK** (Gilbert-Johnson-Keerthi) algorithm for collision detection, an AABB (Axis-Aligned Bounding Box) system, and a BVH (Bounding Volume Hierarchy) for efficient collision management.

## Features

- **Collision Detection**: Uses the GJK algorithm to detect collisions with an AABB and BVH structure for optimized spatial queries.
- **Rendering**: Supports rendering of 2D objects with customizable graphics settings.
- **Physics Engine**: Implements basic physics, including rigid body dynamics and collision response.
- **Modular Design**: Easily extendable and adaptable to different game types and mechanics.
- **Cross-Platform Support**: Developed with cross-platform compatibility in mind (specific dependencies may vary by OS).

## Getting Started

### Prerequisites

Ensure that you have the following installed:

- **C++ Compiler** (e.g., GCC, Clang)
- **CMake** (version 3.10 or higher)
- **Git**

### Dependencies

The engine uses several libraries, which may vary by platform:

- **Raylib**: For rendering graphics.
- **RayImGui**: For rendering immediate mode GUI.
- **nlohmann_json**: For serializing objects to JSON.

## Installation

1. Clone the repository:

    ```bash
    git clone https://github.com/AakrishtSP/GameEngine.git
    cd GameEngine
    ```

2. Build the project:

    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

3. Run the executable (optional if set up):

    ```bash
    ./GameEngine
    ```

## Usage

### Running a Sample Scene

- Describe how to initialize and load a scene.
- Explain any command-line arguments or configuration files needed for setup.

### Customizing the Engine

1. **Adding New Game Objects**: Provide an example of adding a game object and attaching components.
2. **Configuring Physics**: Explain basic configurations for physics properties, such as mass and collision properties.
3. **Rendering Settings**: Describe options for adjusting graphics settings, if available.

## Code Structure

Here's a breakdown of the main modules in the repository:

- **src/**: Source files for the core engine.
  - **CollisionManager.cpp**: Implements collision detection and resolution, including GJK, AABB, and BVH-based detection.
  - **PhysicsManager.cpp**: Manages physics simulation and applies physical properties to game objects.
  - **Renderer.cpp**: Handles the rendering pipeline and draws scenes to the screen.
- **include/**: Header files defining interfaces and data structures.
  - **CollisionManager.hpp**: Contains definitions for collision-related functionality.
  - **Scene.hpp**: Defines the structure and hierarchy of the game scene.
- **assets/**: Any assets (models, textures, etc.) used by sample scenes.
- **tests/**: Unit tests for various components (if available).

*(Customize this structure according to your actual file organization)*

## Contributing

We welcome contributions! Here’s how you can help:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Commit your changes (`git commit -am 'Add new feature'`).
4. Push to the branch (`git push origin feature-branch`).
5. Open a pull request.

Please follow coding standards and include comments/documentation for any major changes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

