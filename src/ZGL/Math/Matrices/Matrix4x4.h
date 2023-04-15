#pragma once

#include "../Vectors/Vector2.h"
#include "../Vectors/Vector3.h"
#include "../Vectors/Vector4.h"
#include <math.h>

#include <iostream>

namespace ZGL
{
	class Matrix4x4
	{
	private:
		bool m_isRowMajor = true;

	public:
		// [row][col]
		float m_cells[4][4];

#pragma region Constructors
		Matrix4x4(bool isRowMajor = true) // Initialize with identity matrix
			: m_isRowMajor(isRowMajor)
		{
			m_cells[0][0] = 1.0f;
			m_cells[0][1] = 0.0f;
			m_cells[0][2] = 0.0f;
			m_cells[0][3] = 0.0f;

			m_cells[1][0] = 0.0f;
			m_cells[1][1] = 1.0f;
			m_cells[1][2] = 0.0f;
			m_cells[1][3] = 0.0f;

			m_cells[2][0] = 0.0f;
			m_cells[2][1] = 0.0f;
			m_cells[2][2] = 1.0f;
			m_cells[2][3] = 0.0f;

			m_cells[3][0] = 0.0f;
			m_cells[3][1] = 0.0f;
			m_cells[3][2] = 0.0f;
			m_cells[3][3] = 1.0f;
		}

		Matrix4x4(float f1, float f2, float f3, float f4, float f5, float f6,
			float f7, float f8, float f9, float f10, float f11, float f12,
			float f13, float f14, float f15, float f16, bool isRowMajor = true)
			: m_isRowMajor(isRowMajor)
		{
			m_cells[0][0] = f1;
			m_cells[0][1] = f2;
			m_cells[0][2] = f3;
			m_cells[0][3] = f4;

			m_cells[1][0] = f5;
			m_cells[1][1] = f6;
			m_cells[1][2] = f7;
			m_cells[1][3] = f8;

			m_cells[2][0] = f9;
			m_cells[2][1] = f10;
			m_cells[2][2] = f11;
			m_cells[2][3] = f12;

			m_cells[3][0] = f13;
			m_cells[3][1] = f14;
			m_cells[3][2] = f15;
			m_cells[3][3] = f16;
		}

		Matrix4x4(float cells[4][4], bool isRowMajor = true)
			: m_isRowMajor(isRowMajor)
		{
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					m_cells[i][j] = cells[i][j];
		}

		Matrix4x4(float val, bool isIdentity = true, bool isRowMajor = true)
			: m_isRowMajor(isRowMajor)
		{
			if (isIdentity)
			{
				m_cells[0][0] = val;
				m_cells[0][1] = 0.0f;
				m_cells[0][2] = 0.0f;
				m_cells[0][3] = 0.0f;

				m_cells[1][0] = 0.0f;
				m_cells[1][1] = val;
				m_cells[1][2] = 0.0f;
				m_cells[1][3] = 0.0f;

				m_cells[2][0] = 0.0f;
				m_cells[2][1] = 0.0f;
				m_cells[2][2] = val;
				m_cells[2][3] = 0.0f;

				m_cells[3][0] = 0.0f;
				m_cells[3][1] = 0.0f;
				m_cells[3][2] = 0.0f;
				m_cells[3][3] = val;
			}
			else
			{
				for (int i = 0; i < 4; ++i)
					for (int j = 0; j < 4; ++j)
						m_cells[i][j] = val;
			}
		}
#pragma endregion

		Vec4 Vec4Mul(const Vec4& v) const
		{
			Vec4 result;
			if (m_isRowMajor)
			{
				result.x = m_cells[0][0] * v.x + m_cells[1][0] * v.y + m_cells[2][0] * v.z + m_cells[3][0] * v.w;
				result.y = m_cells[0][1] * v.x + m_cells[1][1] * v.y + m_cells[2][1] * v.z + m_cells[3][1] * v.w;
				result.z = m_cells[0][2] * v.x + m_cells[1][2] * v.y + m_cells[2][2] * v.z + m_cells[3][2] * v.w;
				result.w = m_cells[0][3] * v.x + m_cells[1][3] * v.y + m_cells[2][3] * v.z + m_cells[3][3] * v.w;
			}
			else
			{
				result.x = m_cells[0][0] * v.x + m_cells[0][1] * v.y + m_cells[0][2] * v.z + m_cells[0][3] * v.w;
				result.y = m_cells[1][0] * v.x + m_cells[1][1] * v.y + m_cells[1][2] * v.z + m_cells[1][3] * v.w;
				result.z = m_cells[2][0] * v.x + m_cells[2][1] * v.y + m_cells[2][2] * v.z + m_cells[2][3] * v.w;
				result.w = m_cells[3][0] * v.x + m_cells[3][1] * v.y + m_cells[3][2] * v.z + m_cells[3][3] * v.w;
			}
			return result;
		}

		Matrix4x4 Mat4x4Mul(const Matrix4x4& mat) const
		{
			Matrix4x4 result;
			for (int rowLeft = 0; rowLeft < 4; ++rowLeft)
			{
				for (int colRight = 0; colRight < 4; ++colRight)
				{
					result.m_cells[rowLeft][colRight] = 0;
					for (int i = 0; i < 4; ++i)
						result.m_cells[rowLeft][colRight] += m_cells[rowLeft][i] * mat.m_cells[i][colRight];
				}
			}

			return result;
		}

		void Transpose()
		{
			// Copy our data for now
			float cells[4][4];
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					cells[i][j] = m_cells[i][j];

			// Set the row and columns
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					m_cells[i][j] = cells[j][i];
		}

		static Matrix4x4 Transpose(const Matrix4x4& mat4)
		{
			Matrix4x4 result;

			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					result.m_cells[i][j] = mat4.m_cells[j][i];

			return result;
		}

		static float Determinant(const Matrix4x4& mat4)
		{
			Mat3 imat3 // row 1, col i(1)
			(
				mat4.m_cells[1][1], mat4.m_cells[1][2], mat4.m_cells[1][3],
				mat4.m_cells[2][1], mat4.m_cells[2][2], mat4.m_cells[2][3],
				mat4.m_cells[3][1], mat4.m_cells[3][2], mat4.m_cells[3][3]
			);

			Mat3 jmat3 // row 1, col j(2)
			(
				mat4.m_cells[1][0], mat4.m_cells[1][2], mat4.m_cells[1][3],
				mat4.m_cells[2][0], mat4.m_cells[2][2], mat4.m_cells[2][3],
				mat4.m_cells[3][0], mat4.m_cells[3][2], mat4.m_cells[3][3]
			);

			Mat3 kmat3 // row 1, col k(3)
			(
				mat4.m_cells[1][0], mat4.m_cells[1][1], mat4.m_cells[1][3],
				mat4.m_cells[2][0], mat4.m_cells[2][1], mat4.m_cells[2][3],
				mat4.m_cells[3][0], mat4.m_cells[3][1], mat4.m_cells[3][3]
			);

			Mat3 lmat3 // row 1, col l(4)
			(
				mat4.m_cells[1][0], mat4.m_cells[1][1], mat4.m_cells[1][2],
				mat4.m_cells[2][0], mat4.m_cells[2][1], mat4.m_cells[2][2],
				mat4.m_cells[3][0], mat4.m_cells[3][1], mat4.m_cells[3][2]
			);

			// Alternating sign pattern
			return (mat4.m_cells[0][0] * Mat3::Determinant(imat3))
				- (mat4.m_cells[0][1] * Mat3::Determinant(jmat3))
				+ (mat4.m_cells[0][2] * Mat3::Determinant(kmat3))
				- (mat4.m_cells[0][3] * Mat3::Determinant(lmat3));
		}

		static Matrix4x4 GetCoefficient(const Matrix4x4& mat4)
		{
			Matrix4x4 coefficient;

			for (int i = 0; i < 4; ++i) // Loops through the row, i is the row we're excluding
			{
				/*
				   This determines our starting sign for the row
				   0 = +
				   1 = -
				   2 = +
				   3 = -
				*/
				float sign = (i == 1 || i == 3) ? -1.0f : 1.0f;

				short initRow = 1;
				short midRow = 2;
				short endRow = 3;


				if (i != 0) initRow = 0;
				if (i == 3) endRow = 2;
				if (initRow == 1 || i == 1) midRow = 2;
				else if (initRow == 0) midRow = 1;

				for (int j = 0; j < 4; ++j) // Loops through the column, j is the col we're excluding
				{

					short initCol = 1;
					short midCol = 2;
					short endCol = 3;

					if (j != 0) initCol = 0;
					if (j == 3) endCol = 2;

					if (initCol == 1 || j == 1) midCol = 2;
					else if (initCol == 0) midCol = 1;

					Mat3 mat3
					(
						mat4.m_cells[initRow][initCol], mat4.m_cells[initRow][midCol], mat4.m_cells[initRow][endCol],
						mat4.m_cells[midRow][initCol], mat4.m_cells[midRow][midCol], mat4.m_cells[midRow][endCol],
						mat4.m_cells[endRow][initCol], mat4.m_cells[endRow][midCol], mat4.m_cells[endRow][endCol]
					);

					coefficient.m_cells[i][j] = Mat3::Determinant(mat3) * sign;

					sign *= -1.0f;
				}
			}

			return coefficient;
		}

		static Matrix4x4 Inverse(const Matrix4x4& mat4)
		{
			return Transpose(GetCoefficient(mat4)) * (1.0f / Determinant(mat4));
		}

		static Matrix4x4 OrthoLH(float left, float right, float bottom, float top, float zNear, float zFar)
		{
			Matrix4x4 result;

			result.m_cells[0][0] = 2.0f / (right - left);
			result.m_cells[1][1] = 2.0f / (top - bottom);
			result.m_cells[3][0] = -(right + left) / (right - left);
			result.m_cells[3][1] = -(top + bottom) / (top - bottom);

#if 0
			result.m_cells[2][2] = 1.0f / (zFar - zNear);
			result.m_cells[3][2] = -zNear / (zFar - zNear);
#else
			result.m_cells[2][2] = 2.0f / (zFar - zNear);
			result.m_cells[3][2] = -(zFar + zNear) / (zFar - zNear);
#endif
		}

		static Matrix4x4 OrthoRH(float left, float right, float bottom, float top, float zNear, float zFar)
		{
			Matrix4x4 result;
			result.m_cells[0][0] = 2.0f / (right - left);
			result.m_cells[1][1] = 2.0f / (top - bottom);
			result.m_cells[3][0] = -(right + left) / (right - left);
			result.m_cells[3][1] = -(top + bottom) / (top - bottom);

#if 0
			result.m_cells[2][2] = 1.0f / (zFar - zNear);
			result.m_cells[3][2] = -zNear / (zFar - zNear);
#else
			result.m_cells[2][2] = -2.0f / (zFar - zNear);
			result.m_cells[3][2] = -(zFar + zNear) / (zFar - zNear);
#endif

			return result;
		}

#pragma region Transformations

		static Matrix4x4 Scale(float factor)
		{
			return
			{
				factor ,   0.0f,   0.0f  , 0.0f,
				 0.0f  , factor,   0.0f  , 0.0f,
				 0.0f  ,   0.0f,   factor, 0.0f,
				 0.0f  ,   0.0f,   0.0f  , 1.0f
			};
		}

		static Matrix4x4 Scale(float factorX, float factorY, float factorZ)
		{
			return
			{
				 factorX,   0.0f  ,   0.0f   , 0.0f,
				 0.0f  ,   factorY,   0.0f   , 0.0f,
				 0.0f  ,   0.0f   ,   factorX, 0.0f,
				 0.0f  ,   0.0f   ,   0.0f   , 1.0f
			};
		}

		static Matrix4x4 ScaleX(float factor)
		{
			return
			{
				factor ,   0.0f,   0.0f  , 0.0f,
				 0.0f  ,   1.0f,   0.0f  , 0.0f,
				 0.0f  ,   0.0f,   1.0f, 0.0f,
				 0.0f  ,   0.0f,   0.0f  , 1.0f
			};
		}

		static Matrix4x4 ScaleY(float factor)
		{
			return
			{
				 1.0f  ,   0.0f,   0.0f  , 0.0f,
				 0.0f  , factor,   0.0f  , 0.0f,
				 0.0f  ,   0.0f,   1.0f  , 0.0f,
				 0.0f  ,   0.0f,   0.0f  , 1.0f
			};
		}

		static Matrix4x4 ScaleZ(float factor)
		{
			return
			{
				 1.0f  ,   0.0f,   0.0f  , 0.0f,
				 0.0f  ,   1.0f,   0.0f  , 0.0f,
				 0.0f  ,   0.0f,   factor, 0.0f,
				 0.0f  ,   0.0f,   0.0f  , 1.0f
			};
		}

		static Matrix4x4 Identity()
		{
			return Scale(1);
		}

		static Matrix4x4 FlipX()
		{
			return
			{
			  -1.0f,  0.0f, 0.0f, 0.0f,
			   0.0f, 1.0f, 0.0f, 0.0f,
			   0.0f,  0.0f, 1.0f, 0.0f,
			   0.0f,  0.0f, 0.0f, 1.0f
			};
		}

		static Matrix4x4 FlipY()
		{
			return
			{
			  1.0f,  0.0f, 0.0f, 0.0f,
			  0.0f, -1.0f, 0.0f, 0.0f,
			  0.0f,  0.0f, 1.0f, 0.0f,
			  0.0f,  0.0f, 0.0f, 1.0f
			};
		}

		static Matrix4x4 FlipZ()
		{
			return
			{
			  1.0f, 0.0f,  0.0f, 0.0f,
			  0.0f, 1.0f,  0.0f, 0.0f,
			  0.0f, 0.0f, -1.0f, 0.0f,
			  0.0f, 0.0f,  0.0f, 1.0f
			};
		}

		static Matrix4x4 RotateX(float theta)
		{
			const float cosTheta = (float)cos(theta);
			const float sinTheta = (float)sin(theta);

			return
			{
				1.0f,  0.0f    , 0.0f    , 0.0f,
				0.0f,  cosTheta, sinTheta, 0.0f,
				0.0f, -sinTheta, cosTheta, 0.0f,
				0.0f,  0.0f    , 0.0f    , 1.0f
			};
		}

		static Matrix4x4 RotateY(float theta)
		{
			const float cosTheta = (float)cos(theta);
			const float sinTheta = (float)sin(theta);

			return
			{
				cosTheta,  0.0f  , -sinTheta,  0.0f,
				0.0f    ,  1.0f  ,  0.0f    ,  0.0f,
				sinTheta,  0.0f  ,  cosTheta,  0.0f,
				0.0f    ,  0.0f  ,  0.0f    ,  1.0f
			};
		}

		static Matrix4x4 RotateZ(float theta)
		{
			const float cosTheta = (float)cos(theta);
			const float sinTheta = (float)sin(theta);

			return
			{
				cosTheta, -sinTheta, 0.0f, 0.0f,
				sinTheta,  cosTheta, 0.0f, 0.0f,
				0.0f    ,  0.0f    , 1.0f, 0.0f,
				0.0f    ,  0.0f    , 0.0f, 1.0f,
			};
		}

		static Matrix4x4 Translate(float x, float y, bool isRowMajor = true)
		{
			if (isRowMajor)
			{
				return
				{
					1.0f, 0.0f, 0.0f, 1.0f,
					0.0f, 1.0f, 0.0f, 1.0f,
					0.0f, 0.0f, 1.0f, 1.0f,
					 x  ,  y  , 0.0f, 1.0f
				};
			}

			return
			{
				1.0f, 0.0f, 0.0f, x,
				0.0f, 1.0f, 0.0f, y,
				0.0f, 0.0f, 1.0f, 1.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}

		static Matrix4x4 Translate(float x, float y, float z, bool isRowMajor = true)
		{
			if (isRowMajor)
			{
				return
				{
					1.0f, 0.0f, 0.0f, 1.0f,
					0.0f, 1.0f, 0.0f, 1.0f,
					0.0f, 0.0f, 1.0f, 1.0f,
					 x  ,  y  ,  z  , 1.0f
				};
			}

			return
			{
				1.0f, 0.0f, 0.0f, x,
				0.0f, 1.0f, 0.0f, y,
				0.0f, 0.0f, 1.0f, z,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}

		static Matrix4x4 Translate(Vec3 vec, bool isRowMajor = true)
		{
			if (isRowMajor)
			{
				return
				{
					1.0f,  0.0f,  0.0f,  0.0f,
					0.0f,  1.0f,  0.0f,  0.0f,
					0.0f,  0.0f,  1.0f,  0.0f,
					vec.x, vec.y, vec.z, 1.0f
				};
			}

			return
			{
				1.0f, 0.0f, 0.0f, vec.x,
				0.0f, 1.0f, 0.0f, vec.y,
				0.0f, 0.0f, 1.0f, vec.z,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}

		static Matrix4x4 Transform(const Matrix4x4& translation, const Matrix4x4& rotation, const Matrix4x4& scale)
		{
			return scale * rotation * translation;
		}

		// Transform vec2 with no rotation
		static Matrix4x4 Transform(const Vec2& translation, float scaleFactor = 1.0f, bool isRowMajor = true)
		{
			return Scale(scaleFactor) * RotateY(0) * Translate(translation.x, translation.y, isRowMajor);
		}

		// Transform vec2 with rotation in the y-axis
		static Matrix4x4 TransformR(const Vec2& translation, float rotation, float scaleFactor = 1.0f, bool isRowMajor = true)
		{
			return Scale(scaleFactor) * RotateY(rotation) * Translate(translation.x, translation.y, isRowMajor);
		}

		// Transform vec3 with no rotation
		static Matrix4x4 Transform(const Vec3& translation, float scaleFactor = 1.0f, bool isRowMajor = true)
		{
			return Scale(scaleFactor) * Translate(translation.x, translation.y, isRowMajor);
		}

		// Transform vec3 with rotation in the x-axis
		static Matrix4x4 TransformRX(const Vec3& translation, float rotationX, float scaleFactor = 1.0f, bool isRowMajor = true)
		{
			return Scale(scaleFactor) * RotateX(rotationX) * Translate(translation, isRowMajor);
		}

		// Transform vec3 with rotation in the y-axis
		static Matrix4x4 TransformRY(const Vec3& translation, float rotationY, float scaleFactor = 1.0f, bool isRowMajor = true)
		{
			return Scale(scaleFactor) * RotateX(rotationY) * Translate(translation, isRowMajor);
		}

		// Transform vec3 with rotation in the z-axis
		static Matrix4x4 TransformRZ(const Vec3& translation, float rotationZ, float scaleFactor = 1.0f, bool isRowMajor = true)
		{
			return Scale(scaleFactor) * RotateX(rotationZ) * Translate(translation, isRowMajor);
		}
#pragma endregion

#pragma region Operator Overloading
		Vec2 operator*(const Vec2& v) const
		{
			Vec4 vec4 = *this * Vec4(v.x, v.y, 1, 1);
			return { vec4.x, vec4.y };
		}

		Vec3 operator*(const Vec3& v) const
		{
			Vec4 vec4 = *this * Vec4(v.x, v.y, v.z, 1);
			return { vec4.x, vec4.y, vec4.z };
		}

		Vec4 operator*(const Vec4& v) const
		{
			return Vec4Mul(v);
		}

		Matrix4x4 operator*(const Matrix4x4& rhs) const
		{
			return Mat4x4Mul(rhs);
		}

		Matrix4x4 operator*(const float rhs) const
		{
			Matrix4x4 result;

			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; j++)
					result.m_cells[i][j] = rhs * m_cells[i][j];

			return result;
		}

		Matrix4x4& operator*=(const Matrix4x4& rhs)
		{
			return *this = *this * rhs;
		}
#pragma endregion

		void Print()
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					std::cout << m_cells[i][j] << " ";
				}

				std::cout << std::endl;
			}
		}
	} typedef Mat4;
}
