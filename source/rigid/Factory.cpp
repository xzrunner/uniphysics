#include "uniphysics/rigid/Factory.h"
#include "uniphysics/rigid/box2d/DebugDraw.h"

namespace up
{
namespace rigid
{

std::shared_ptr<DebugDraw> CreateDebugDraw(Type type)
{
	std::shared_ptr<DebugDraw> ret = nullptr;
	switch (type)
	{
	case Type::Box2D:
		ret = std::make_shared<box2d::DebugDraw>();
		break;
	}
	return ret;
}

}
}