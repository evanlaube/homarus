project(homarus)

add_library(homarus homarus.h util/vec2.h util/blockallocator.cpp util/gridpartitioner.cpp physics/world.cpp physics/body.cpp shapes/shape.cpp shapes/polygon.cpp shapes/circle.cpp physics/fixture.cpp)
#add_library(homarus homarus.h util/vec2.h physics/world.cpp physics/body.cpp shapes/shape.cpp physics/fixture.cpp)
