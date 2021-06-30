#pragma once

#include <memory>

namespace up
{
namespace rigid
{

class DebugDraw;

enum class Type
{
	Box2D,
	Bullet3D,
};

std::shared_ptr<DebugDraw> CreateDebugDraw(Type type);

}
}