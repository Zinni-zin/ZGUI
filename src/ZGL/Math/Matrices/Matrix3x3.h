#pragma once

#include "../Vectors/Vector3.h"
#include <math.h>

namespace ZGL
{
	class Matrix3x3
	{
	private:
		// [row][col]
		float m_cells[3][3];

	public:
#pragma region Constructors
		Matrix3x3() // Initialize with identity matrix
		{
			m_cells[0][0] = 1.0f;
			m_cells[0][1] = 0.0f;
			m_cells[0][2] = 0.0f;

			m_cells[1][0] = 0.0f;
			m_cells[1][1] = 1.0f;
			m_cells[1][2] = 0.0f;

			m_cells[2][0] = 0.0f;
			m_cells[2][1] = 0.0f;
			m_cells[2][2] = 1.0f;
		}

		Matrix3x3(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9)
		{
			m_cells[0][0] = f1;
			m_cells[0][1] = f2;
			m_cells[0][2] = f3;

			m_cells[1][0] = f4;
			m_cells[1][1] = f5;
			m_cells[1][2] = f6;

			m_cells[2][0] = f7;
			m_cells[2][1] = f8;
			m_cells[2][2] = f9;
		}

		Matrix3x3(float cells[3][3])
		{
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
					m_cells[i][j] = cells[i][j];
			}
		}
#pragma endregion

		Vec3 Vec3Mul(const Vec3& v) const
		{
			Vec3 result;
			result.x = m_cells[0][0] * v.x + m_cells[0][1] * v.y + m_cells[0][2] * v.z;
			result.y = m_cells[1][0] * v.x + m_cells[1][1] * v.y + m_cells[1][2] * v.z;
			result.z = m_cells[2][0] * v.x + m_cells[2][1] * v.y + m_cells[2][2] * v.z;
			return result;
		}

		Matrix3x3 Mat3x3Mul(const Matrix3x3& mat) const
		{
			Matrix3x3 result;
			for (int rowLeft = 0; rowLeft < 3; ++rowLeft)
			{
				for (int colRight = 0; colRight < 3; ++colRight)
				{
					result.m_cells[rowLeft][colRight] = 0;
					for (int i = 0; i < 3; ++i)
					{
						result.m_cells[rowLeft][colRight] += m_cells[rowLeft][i] * mat.m_cells[i][colRight];
					}
				}
			}

			return result;
		}

		static float Determinant(const Matrix3x3 mat3)
		{
			/*
			1, 2, 3
			4, 5, 6
			7, 8, 9
			*/
			Matrix2x2 imat2 // row 1, col i(1)
			(
				mat3.m_cells[1][1], mat3.m_cells[1][2],
				mat3.m_cells[2][1], mat3.m_cells[2][2]
			);

			Matrix2x2 jmat2 // row 1, col j(2)
			(
				mat3.m_cells[1][0], mat3.m_cells[1][2],
				mat3.m_cells[2][0], mat3.m_cells[2][2]
			);

			Matrix2x2 kmat2 // row 1, col k(3)
			(
				mat3.m_cells[1][0], mat3.m_cells[1][1],
				mat3.m_cells[2][0], mat3.m_cells[2][1]
			);

			return (mat3.m_cells[0][0] * Mat2::Determinant(imat2)) -
				(mat3.m_cells[0][1] * Mat2::Determinant(jmat2)) +
				(mat3.m_cells[0][2] * Mat2::Determinant(kmat2));
		}

#pragma region Transformations
		static Matrix3x3 Scale(float factor)
		{
			return
			{
				factor ,   0.0f,   0.0f,
				 0.0f  , factor,   0.0f,
				 0.0f  ,   0.0f,   1.0f
			};
		}

		static Matrix3x3 Identity()
		{
			return Scale(1);
		}

		static Matrix3x3 FlipY()
		{
			return
			{
			  1.0f,  0.0f, 0.0f,
			  0.0f, -1.0f, 0.0f,
			  0.0f,  0.0f, 1.0f
			};
		}

		static Matrix3x3 Rotation(float theta)
		{
			const float cosTheta = (float)cos(theta);
			const float sinTheta = (float)sin(theta);

			return
			{
				cosTheta, -sinTheta, 0.0f,
				sinTheta,  cosTheta, 0.0f,
				0.0f    ,  0.0f    , 1.0f
			};
		}

		static Matrix3x3 Translation(float x, float y)
		{
			return
			{
				1.0f, 0.0f, x,
				0.0f, 1.0f, y,
				0.0f, 0.0f, 1.0f
			};
		}

		static Matrix3x3 Transform(const Matrix3x3& translation, const Matrix3x3& rotation, const Matrix3x3& scale)
		{
			return scale * rotation * translation;
		}

		static Matrix3x3 Transform(const Vec2 translation, float rotation, float scaleFactor)
		{
			return Scale(scaleFactor) * Rotation(rotation) * Translation(translation.x, translation.y);
		}

		static Matrix3x3 Transform(const Vec2 translation)
		{
			return Scale(1) * Rotation(0) * Translation(translation.x, translation.y);
		}
#pragma endregion

#pragma region Operator Overloading
		Vec2 operator*(const Vec2& v) const
		{
			Vec3 vec3 = *this * Vec3(v.x, v.y, 1);
			return { vec3.x, vec3.y };
		}

		Vec3 operator*(const Vec3& v) const
		{
			return Vec3Mul(v);
		}

		Matrix3x3 operator*(const Matrix3x3& rhs) const
		{
			return Mat3x3Mul(rhs);
		}

		Matrix3x3 operator*(const float rhs) const
		{
			Matrix3x3 result;

			for (int i = 0; i < 3; ++i)
				for (int j = 0; j < 3; j++)
					result.m_cells[i][j] = rhs * m_cells[i][j];

			return result;
		}

		Matrix3x3& operator*=(const Matrix3x3& rhs)
		{
			return *this = *this * rhs;
		}
#pragma endregion
	} typedef Mat3;
}
