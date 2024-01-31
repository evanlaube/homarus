project(homarus)

add_library(homarus homarus.h util/vec2.h util/blockallocator.cpp physics/world.cpp physics/body.cpp shapes/shapes.cpp physics/fixture.cpp)
#add_library(homarus homarus.h util/vec2.h physics/world.cpp physics/body.cpp shapes/shape.cpp physics/fixture.cpp)
