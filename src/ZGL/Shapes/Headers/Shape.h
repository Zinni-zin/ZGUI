#pragma once
#include "../../Render/Headers/Sprite.h"
#include "../../Math/Vectors.h"

#include <memory>

#define ShapeFill 0
#define ShapeOutline 1

namespace ZGL
{
	class ShapeI;

	class ShapeF
	{
	public:
#pragma region Constructors
		ShapeF(float x, float y, float width, float height, const Vec4& colour = Vec4::One())
			: m_pos(Vec2(x, y)), m_dimensions(Vec2(width, height))
		{
			m_sprite = new Sprite(x, y, (int)width, (int)height);
			m_sprite->SetColour(colour);
		}

		ShapeF(Vec2 pos, float width, float height, const Vec4& colour = Vec4::One())
			: m_pos(pos), m_dimensions(Vec2(width, height))
		{
			m_sprite = new Sprite(pos.x, pos.y, (int)width, (int)height);
			m_sprite->SetColour(colour);
		}

		ShapeF(float x, float y, Vec2 dimensions, const Vec4& colour = Vec4::One())
			: m_pos(Vec2(x, y)), m_dimensions(dimensions)
		{ 
			m_sprite = new Sprite(x, y, (int)dimensions.x, (int)dimensions.y);
			m_sprite->SetColour(colour);
		}

		ShapeF(float x, float y, float dimensions, const Vec4& colour = Vec4::One())
			: m_pos(Vec2(x, y)), m_dimensions(Vec2(dimensions, dimensions))//, m_rotation(0.f)
		{ 
			m_sprite = new Sprite(x, y, (int)dimensions, (int)dimensions);
			m_sprite->SetColour(colour);
		}

		ShapeF(Vec2 pos, Vec2 dimensions, const Vec4& colour = Vec4::One())
			: m_pos(pos), m_dimensions(dimensions)
		{ 
			m_sprite = new Sprite(pos.x, pos.y, (int)dimensions.x, (int)dimensions.y);
			m_sprite->SetColour(colour);
		}

		~ShapeF()
		{
			delete m_sprite;
			m_sprite = nullptr;
		}
#pragma endregion

		Vec2 GetPos() const { return m_pos; }
		Vec2 GetDimensions() const { return m_dimensions; }
		Vec4 GetColour() const { return m_sprite->GetColour(); }

		Sprite GetSprite() const { return *m_sprite; }

		bool IsOutline() const { return m_isOutline; }

		virtual float GetX() const { return m_pos.x; }
		virtual float GetY() const { return m_pos.y; }

		virtual float GetWidth() const { return m_dimensions.x; }
		virtual float GetHeight() const { return m_dimensions.y; }

		virtual float GetSurfaceArea() const = 0;
		virtual bool HasIntersected(ShapeF* other) const = 0;
		virtual bool HasIntersected(ShapeI* other) const = 0;

		void SetFillTexture(std::shared_ptr<Texture2D> fillTexture)
		{
			m_fill = fillTexture;
			m_sprite->SetTexture(m_fill);
			m_isFillTextureSet = true;
			m_isOutline = false;
		}

		void SetOutlineTexture(std::shared_ptr<Texture2D> outlineTexture)
		{
			m_outline = outlineTexture;
			m_sprite->SetTexture(m_outline);
			m_isOutlineTextureSet = true;
			m_isOutline = true;
		}

		void SetSpriteTexture(std::shared_ptr<Texture2D> fillTexture, std::shared_ptr<Texture2D> outlineTexture)
		{
			SetFillTexture(fillTexture);
			SetOutlineTexture(outlineTexture);
		}

		void SetPos(const Vec2& pos) { m_pos = pos; m_sprite->SetPos(pos); }
		void SetPos(float x, float y) { m_pos = Vec2(x, y); m_sprite->SetPos(x, y); }
		void SetX(float x) { m_pos.x = x; m_sprite->SetPos(x, m_sprite->GetY()); }
		void SetY(float y) { m_pos.y = y; m_sprite->SetPos(m_sprite->GetX(), y); }
		void SetColour(const Vec4& colour) { m_sprite->SetColour(colour); }
		void SetColour(float r, float g, float b, float a) { m_sprite->SetColour(r, g, b, a); }

		void SetDrawType(unsigned int drawType)
		{

			if (drawType == ShapeFill)
			{
				if (!m_isFillTextureSet)
				{
					std::cout << "Error Fill Texture is not Set(obj: " << this << ")!\n";
					return;
				}

				m_sprite->SetTexture(m_fill);
				m_isOutline = false;
			}
			else if (drawType == ShapeOutline)
			{
				if (!m_isOutlineTextureSet)
				{
					std::cout << "Error Outline Texture is not Set(obj: " << this << ")!\n";
					return;
				}

				m_sprite->SetTexture(m_outline);
				m_isOutline = true;
			}
			else
			{
				std::cout << "Error Invalid Draw Type!\n";
				return;
			}
		}

	protected:
		Vec2 m_pos;
		Vec2 m_dimensions;

		Sprite* m_sprite;
		std::shared_ptr<Texture2D> m_fill, m_outline;

	private:
		bool m_isOutline = false;
		bool m_isFillTextureSet = false;
		bool m_isOutlineTextureSet = false;
	} typedef FloatShape;

	class ShapeI
	{
	public:
#pragma region Constructors
		ShapeI(float x, float y, int width, int height, const Vec4& colour = Vec4::One())
			: m_pos(Vec2(x, y)), m_dimensions(Vec2i(width, height))
		{
			m_sprite = new Sprite(x, y, width, height);
			m_sprite->SetColour(colour);
		}

		ShapeI(Vec2 pos, int width, int height, const Vec4& colour = Vec4::One())
			: m_pos(pos), m_dimensions(Vec2i(width, height))
		{ 
			m_sprite = new Sprite(pos.x, pos.y, width, height);
			m_sprite->SetColour(colour);
		}

		ShapeI(float x, float y, Vec2i dimensions, const Vec4& colour = Vec4::One())
			: m_pos(Vec2(x, y)), m_dimensions(dimensions)
		{ 
			m_sprite = new Sprite(x, y, dimensions.x, dimensions.y);
			m_sprite->SetColour(colour);
		}

		ShapeI(float x, float y, int dimensions, const Vec4& colour = Vec4::One())
			: m_pos(Vec2(x, y)), m_dimensions(Vec2i(dimensions, dimensions))
		{ 
			m_sprite = new Sprite(x, y, dimensions, dimensions);
			m_sprite->SetColour(colour);
		}

		ShapeI(Vec2 pos, Vec2i dimensions, const Vec4& colour = Vec4::One())
			: m_pos(pos), m_dimensions(dimensions)
		{
			m_sprite = new Sprite(pos.x, pos.y, dimensions.x, dimensions.y);
			m_sprite->SetColour(colour);
		}
#pragma endregion

		~ShapeI()
		{ 
			delete m_sprite;
			m_sprite = nullptr;
		}

		Vec2 GetPos() const { return m_pos; }
		Vec2i GetDimensions() const{ return m_dimensions; }
		Vec4 GetColour() const { return m_sprite->GetColour(); }

		Sprite GetSprite() const { return *m_sprite; }

		std::shared_ptr<Texture2D> GetFillTexture() const { return m_fill; }
		std::shared_ptr<Texture2D> GetOutlineTexture() const { return m_outline; }

		bool IsOutline() const { return m_isOutline; }

		float GetX() const { return m_pos.x; }
		float GetY() const { return m_pos.y; }

		virtual int GetWidth() const { return m_dimensions.x; }
		virtual int GetHeight() const { return m_dimensions.y; }

		virtual int GetSurfaceArea() const = 0;
		virtual bool HasIntersected(ShapeF* other) const = 0;
		virtual bool HasIntersected(ShapeI* other) const = 0;

		void SetFillTexture(std::shared_ptr<Texture2D> fillTexture)
		{
			m_fill = fillTexture;
			m_sprite->SetTexture(m_fill);
			m_isFillTextureSet = true;
			m_isOutline = false;
		}

		void SetOutlineTexture(std::shared_ptr<Texture2D> outlineTexture)
		{
			m_outline = outlineTexture;
			m_sprite->SetTexture(m_outline);
			m_isOutlineTextureSet = true;
			m_isOutline = true;
		}

		void SetSpriteTexture(std::shared_ptr<Texture2D> fillTexture, std::shared_ptr<Texture2D> outlineTexture)
		{
			SetFillTexture(fillTexture);
			SetOutlineTexture(outlineTexture);
		}

		void SetPos(const Vec2& pos) { m_pos = pos; m_sprite->SetPos(pos); }
		void SetPos(float x, float y) { m_pos = Vec2(x, y); m_sprite->SetPos(x, y); }
		void SetX(float x) { m_pos.x = x; m_sprite->SetPos(x, m_sprite->GetY()); }
		void SetY(float y) { m_pos.y = y; m_sprite->SetPos(m_sprite->GetX(), y); }
		void SetColour(const Vec4& colour) { m_sprite->SetColour(colour); }
		void SetColour(float r, float g, float b, float a) { m_sprite->SetColour(r, g, b, a); }

		void SetDrawType(unsigned int drawType)
		{

			if (drawType == ShapeFill)
			{
				if (!m_isFillTextureSet)
				{
					std::cout << "Error Fill Texture is not Set(obj: " << this << ")!\n";
					return;
				}

				m_sprite->SetTexture(m_fill);
				m_isOutline = false;
			}
			else if (drawType == ShapeOutline)
			{
				if (!m_isOutlineTextureSet)
				{
					std::cout << "Error Outline Texture is not Set(obj: " << this << ")!\n";
					return;
				}

				m_sprite->SetTexture(m_outline);
				m_isOutline = true;
			}
			else
			{
				std::cout << "Error Invalid Draw Type!\n";
				return;
			}
		}

	protected:
		Vec2 m_pos;
		Vec2i m_dimensions;

		Sprite* m_sprite;
		std::shared_ptr<Texture2D> m_fill, m_outline;

	private:
		bool m_isOutline = false;
		bool m_isFillTextureSet = false;
		bool m_isOutlineTextureSet = false;
	} typedef IntShape;
}