#pragma once

#include "Vectors.h"
#include "Matrices.h"

namespace ZGL
{
	inline float ToRadians(float degrees)
	{
		return degrees * (float)(PI / 180);
	}
}