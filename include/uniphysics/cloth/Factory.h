#pragma once

#include "uniphysics/noncopyable.h"

#include <SM_Vector.h>

#include <memory>
#include <vector>

namespace up
{
namespace cloth
{

class Solver;
class Fabric;
class Cloth;
class ClothMeshData;

class Factory : noncopyable
{
public:
    virtual ~Factory() {}

    virtual std::shared_ptr<Solver>
        CreateSolver() = 0;

    virtual std::shared_ptr<Fabric>
        CreateFabric(const ClothMeshData& mesh_data, const sm::vec3& gravity) = 0;

    virtual std::unique_ptr<Cloth>
        CreateCloth(Fabric& fabric, const std::vector<sm::vec4>& particles) = 0;

}; // Factory

}
}