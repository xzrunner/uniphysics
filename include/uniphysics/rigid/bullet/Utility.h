#pragma once

#include <SM_Vector.h>

#include <LinearMath/btVector3.h>

namespace up
{
namespace rigid
{
namespace bullet
{

static btVector3 vec3_sm2bt(const sm::vec3& p) {
    return btVector3(p.x, p.y, p.z);
}

static sm::vec3 vec3_bt2sm(const btVector3& p) {
    return sm::vec3(p.x(), p.y(), p.z());
}

}
}
}