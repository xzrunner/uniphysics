#pragma once

#include <tessellation/Painter.h>

namespace up
{
namespace rigid
{

class DebugDraw
{
public:
	virtual ~DebugDraw() {}

	auto& GetPainter() { return m_pt; }
	//void ClearPainter() { m_pt.Clear(); }

protected:
	tess::Painter m_pt;

}; // DebugDraw

}
}