#pragma once

#include "../Vectors/Vector2.h"
#include <math.h>

namespace ZGL
{
	class Matrix2x2
	{
	private:
		// [row][col]
		float m_cells[2][2];

	public:
#pragma region Constructors
		Matrix2x2() // Initialize with identity matrix
		{
			m_cells[0][0] = 1.0f;
			m_cells[0][1] = 0.0f;

			m_cells[1][0] = 0.0f;
			m_cells[1][1] = 1.0f;
		}

		Matrix2x2(float f1, float f2, float f3, float f4)
		{
			m_cells[0][0] = f1;
			m_cells[0][1] = f2;

			m_cells[1][0] = f3;
			m_cells[1][1] = f4;
		}

		Matrix2x2(float cells[2][2])
		{
			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
					m_cells[i][j] = cells[i][j];
			}
		}
#pragma endregion

		Vec2 Vec2Mul(const Vec2& v) const
		{
			Vec2 result;
			result.x = m_cells[0][0] * v.x + m_cells[0][1] * v.y;
			result.y = m_cells[1][0] * v.x + m_cells[1][1] * v.y;
			return result;
		}

		Matrix2x2 Mat2x2Mul(const Matrix2x2& mat) const
		{
			Matrix2x2 result;
			for (int rowLeft = 0; rowLeft < 2; ++rowLeft)
			{
				for (int colRight = 0; colRight < 2; ++colRight)
				{
					result.m_cells[rowLeft][colRight] = 0;
					for (int i = 0; i < 2; ++i)
					{
						result.m_cells[rowLeft][colRight] += m_cells[rowLeft][i] * mat.m_cells[i][colRight];
					}
				}
			}

			return result;
		}

		static float Determinant(const Matrix2x2 mat2)
		{
			return (mat2.m_cells[0][0] * mat2.m_cells[1][1]) - (mat2.m_cells[0][1] * mat2.m_cells[1][0]);
		}

#pragma region Transformations
		static Matrix2x2 Scale(float factor)
		{
			return { factor , 0, 0, factor };
		}

		static Matrix2x2 Identity()
		{
			return Scale(1);
		}

		static Matrix2x2 FlipY()
		{
			return
			{
			  1,  0,
			  0, -1
			};
		}

		static Matrix2x2 Rotation(float theta)
		{
			const float cosTheta = (float)cos(theta);
			const float sinTheta = (float)sin(theta);

			return 
			{ 
				cosTheta, -sinTheta, 
				sinTheta,  cosTheta 
			};
		}
#pragma endregion

#pragma region Operator Overloading
		Vec2 operator*(const Vec2& v) const
		{
			return Vec2Mul(v);
		}

		Matrix2x2 operator*(const Matrix2x2& rhs) const
		{
			return Mat2x2Mul(rhs);
		}

		Matrix2x2& operator*=(const Matrix2x2& rhs)
		{
			return *this = *this * rhs;
		}
#pragma endregion

	} typedef Mat2;
}
