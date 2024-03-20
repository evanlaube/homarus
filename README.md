# Homarus

Homarus is a lightweight physics simulation library written in C++. It is designed to provide a simple yet powerful framework for simulating rigid body dynamics, collisions, and interactions.

<p align="center">
  <img src="https://github.com/evanlaube/homarus/blob/main/assets/readme/pressureSim.gif" alt="Example air pressure simuation made with Homarus"/>
</p>

## Current Features

- Rigid body simulation
- Static and dynamic bodies
- Collision detection and handling
- Overlap and collision testing of circular and polygonal shapes 

## Planned Features

- Springs and joints for connecting bodies
- Friction and other material properties
- Improved and faster collision handling (Implement rotation)
- Newtonian gravity simulation

## Getting Started

To include Homarus in your project, simply clone the repository and link it with your C++ project using CMake. The library is intended to be easy to integrate into existing projects, no matter the size. 

```bash
git clone https://github.com/evanlaube/homarus.git
cd homarus
mkdir build && cd build
cmake ..
make 
```

## Usage

Homarus provides a simple API for creating and managing bodies, shapes, and fixtures. Here's a basic example of how to create a rigid body with a circle shape:

```cpp
#include "homarus.h"

int main() {
    // Create a world and set gravity vector
    World world;
    world.setGravity(0, -9.81); 

    // Create a circle shape with radius 50
    Circle circle(50);

    // Create a fixture using the created shape
    Fixture fixture(&circle);

    // Create a body using the created fixture
    Body body = world.createBody(&fixture);

    // Set position, velocity, mass, etc.
    body.setPos(Vec2d(540, 360));
    body.setVel(Vec2d(0, 0));
    body.setMass(25 * 25);
    // Add more configurations as needed...

    // Run simulation loop
    while (true) {
        // Update simulation
        world.update(0.01); // Update world by 0.01 time units

        // Insert code to render bodies, take user input, etc
    }

    return 0;
}
```

## Required Libraries for Testing

- OpenGL
- GLEW
- GLFW

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE.txt) file for details.

## Acknowledgments

Special thanks to any and all who are willing to spend their time improving this project!

## Project Details

- Written in C++
- Managed using CMake

## Contributing

Contributions are welcome! Please feel free to fork the repository and submit pull requests. Be sure to review the [CONTRIBUTING](CONTRIBUTING.md) guidelines for more information on how to contribute to this project.

## Get Involved

If you have any questions, suggestions, or feedback, please don't hesitate to [open an issue](https://github.com/evanlaube/homarus/issues) or [contact me](mailto:laubeevan@gmail.com).

