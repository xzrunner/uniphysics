cmake_minimum_required(VERSION 3.16)

project(uniphysics)

add_definitions(
    -DNO_BOOST
)

################################################################################
# Source groups
################################################################################
set(0.rigid__base
    "include/uniphysics/rigid/Body.h"
    "include/uniphysics/rigid/Constraint.h"
    "include/uniphysics/rigid/DebugDraw.h"
    "include/uniphysics/rigid/Shape.h"
    "include/uniphysics/rigid/World.h"
)
source_group("0.rigid\\base" FILES ${0.rigid__base})

set(0.rigid__box2d
    "include/uniphysics/rigid/box2d/Body.h"
    "include/uniphysics/rigid/box2d/config.h"
    "include/uniphysics/rigid/box2d/DebugDraw.h"
    "include/uniphysics/rigid/box2d/Fixture.h"
    "include/uniphysics/rigid/box2d/Joint.h"
    "include/uniphysics/rigid/box2d/Shape.h"
    "include/uniphysics/rigid/box2d/World.h"
    "source/rigid/box2d/Body.cpp"
    "source/rigid/box2d/DebugDraw.cpp"
    "source/rigid/box2d/Fixture.cpp"
    "source/rigid/box2d/Joint.cpp"
    "source/rigid/box2d/Shape.cpp"
    "source/rigid/box2d/World.cpp"
)
source_group("0.rigid\\box2d" FILES ${0.rigid__box2d})

set(0.rigid__bullet
    "include/uniphysics/rigid/bullet/Body.h"
    "include/uniphysics/rigid/bullet/Constraint.h"
    "include/uniphysics/rigid/bullet/Shape.h"
    "include/uniphysics/rigid/bullet/Utility.h"
    "include/uniphysics/rigid/bullet/World.h"
    "source/rigid/bullet/Body.cpp"
    "source/rigid/bullet/Constraint.cpp"
    "source/rigid/bullet/Shape.cpp"
    "source/rigid/bullet/World.cpp"
)
source_group("0.rigid\\bullet" FILES ${0.rigid__bullet})

set(0.rigid__tools
    "include/uniphysics/rigid/Factory.h"
    "source/rigid/Factory.cpp"
)
source_group("0.rigid\\tools" FILES ${0.rigid__tools})

set(3.cloth
    "include/uniphysics/cloth/Factory.h"
    "include/uniphysics/cloth/SceneController.h"
)
source_group("3.cloth" FILES ${3.cloth})

set(3.cloth__base
    "include/uniphysics/cloth/Cloth.h"
    "include/uniphysics/cloth/Fabric.h"
    "include/uniphysics/cloth/Solver.h"
)
source_group("3.cloth\\base" FILES ${3.cloth__base})

set(3.cloth__mesh
    "include/uniphysics/cloth/ClothMeshData.h"
    "source/cloth/ClothMeshData.cpp"
)
source_group("3.cloth\\mesh" FILES ${3.cloth__mesh})

set(3.cloth__utility
    "include/uniphysics/cloth/JobManager.h"
    "source/cloth/JobManager.cpp"
)
source_group("3.cloth\\utility" FILES ${3.cloth__utility})

set(external__task
    "external/task/PxCpuDispatcher.h"
    "external/task/PxGpuDispatcher.h"
    "external/task/PxGpuTask.h"
    "external/task/PxTask.h"
    "external/task/PxTaskDefine.h"
    "external/task/PxTaskManager.h"
)
source_group("external\\task" FILES ${external__task})

set(utility
    "include/uniphysics/noncopyable.h"
)
source_group("utility" FILES ${utility})

set(ALL_FILES
    ${0.rigid__base}
    ${0.rigid__box2d}
    ${0.rigid__bullet}
    ${0.rigid__tools}
    ${3.cloth}
    ${3.cloth__base}
    ${3.cloth__mesh}
    ${3.cloth__utility}
    ${external__task}
    ${utility}
)

################################################################################
# Target
################################################################################

add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE include external/sm external/box2d/include external/tessellation/include external/bullet3/src external/NvCloth/NvCloth/extensions/include external/NvCloth/PxShared/include external/NvCloth/NvCloth/samples/SampleBase external/NvCloth/NvCloth/include)
