#pragma once
#include <string>
#include <memory>
#include <iostream>

#include "Texture2D.h"

#include "../../Math/Vectors.h"

namespace ZGL
{
	class Sprite
	{
	public:
		Sprite();
		Sprite(float xPos, float yPos);
		Sprite(float xPos, float yPos, int width, int height);
		Sprite(Vec2 pos);
		Sprite(Vec2 pos, Vec2i dimensions);
		~Sprite();

		// Creates a new texture
		void LoadTexture(const std::string& filePath);

		/* --- Getters --- */


		inline Vec2 GetPos() const { return Vec2(m_posX, m_posY); }
		inline float GetX() const { return m_posX; }
		inline float GetY() const { return m_posY; }
		inline float GetRotation() const { return m_rotation; }
		inline float GetDepth() const { return m_depth; }

		inline Vec2i GetDimensions() const { return Vec2i(m_width, m_height); }
		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }

		inline int GetTextureWidth() const { return m_textureWidth; }
		inline int GetTextureHeight() const { return m_textureHeight; }

		inline float GetScaleX() const { return m_scale.x; }
		inline float GetScaleY() const { return m_scale.y; }

		inline Vec4 GetSubCoords() const { return m_SubCoords; };
		
		inline Vec4 GetColour() const { return m_colour; }

		void BindTexture(GLuint slot = 0) const;
		inline GLuint GetTextureID() const { return m_textureID; }
		inline bool HasTexture() const { return m_texture != nullptr; }

		/* --- Setters --- */
		
		// Doesn't create a new texture since we're using a shared pointer, so we share the texture
		void SetTexture(std::shared_ptr<Texture2D> texture);

		void SetPos(const Vec2& pos) { m_posX = pos.x; m_posY = pos.y; }
		void SetPos(float x, float y) { m_posX = x;  m_posY = y; }
		void SetWidth(int width)
		{
			if (width > 0) m_width = width;
			else std::cout << "Error in Sprite: Width must be greater than 0!\n";
		}

		void SetHeight(int height)
		{
			if (height > 0) m_height = height;
			else std::cout << "Error in Sprite: Height must be greater than 0!\n";
		}

		void SetRotation(float rotation);

		inline void SetColour(const Vec4& colour) { m_colour = colour; }
		inline void SetColour(float r, float g, float b, float a) { m_colour = { r, g, b, a }; }
		
		inline void SetScaleX(float scale) { m_scale.x = scale; }
		inline void SetScaleY(float scale) { m_scale.y = scale; }

		void SetSubCoords(float x, float y, int width, int height);

		void SetDepth(float depth)
		{
			if (depth >= 0.0f && depth <= 1.0f) m_depth = depth;
			else std::cout << "Error in Sprite: Depth must be between 0.0f and 1.0f\n";
		}
	
	private:
		std::shared_ptr<Texture2D> m_texture; // Shared pointer to easily share textures

		Vec4 m_colour;
		Vec4 m_SubCoords;

		float m_posX, m_posY;
		int m_width, m_height;
		float m_rotation = 0.f;
		float m_depth = 0.0f;
		
		Vec2 m_scale = Vec2::One();

		float m_subX, m_subY;
		int m_subWidth, m_subHeight;
		
		int m_textureWidth, m_textureHeight;
		
		GLuint m_textureID;

		void SetSubX(float x);
		void SetSubY(float y);
		void SetSubWidth(int width);
		void SetSubHeight(int height);
	};
}