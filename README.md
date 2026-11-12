# Tower Defense Game

This is a console-based tower defense game developed using C++. The game involves a tower defending against waves of enemies. The tower can automatically fire at enemies or shoot manually. The game features a grid-based representation of the game world, where bullets, enemies, explosions, and the tower are displayed.

## Tasks

Task: Implement a memory pool for enemies to avoid frequent allocations and deallocations during gameplay, improving performance and memory management. Check out the Pooler class header for more information.

Task: Implement a method to find the closes enemy to the tower. Check out the Tower class header for more information.

Task: Implement a Log method to log messages to a file. Check out the Logger class header for more information.

Check below for details on how to build and run the game.

## Requirements

- A C++ compiler (e.g., g++, clang++)
- Standard C++ library
- POSIX or Windows compatible environment for input handling

## Building the Game

1. Clone the repository:

    ```sh
    git clone https://github.com/yourusername/tower-defense-cli.git
    cd tower-defense-cli
    ```

2. Build the project using CMake

    ```sh
    cmake --preset Release-[version]
    cmake --build build/Release-[version] --config Release-[version]
    ```

   Replace [version] for the preset adequate for your operating system (Windows, macOS, Ubuntu). Adjust the command according to your file structure and platform.
   
## Running the Game

To run the game, execute the compiled binary:

```sh
build/Release-[version]/tower-defense-cli.exe
```

#### Controls
A: Turns Auto Fire on/off
Escape: Quits the game
