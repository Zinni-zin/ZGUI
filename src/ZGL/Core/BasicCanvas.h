#pragma once
#include <vector>
#include <utility>

#include "Canvas.h"
#include "../Input/Camera.h"
#include "../Render/Headers/Sprite.h"
#include "../Render/Headers/Renderer.h"

#include "../Shapes/Headers/Rectangle.h"
#include "../Shapes/Headers/Circle.h"

#include "../Math/Matrices.h"


namespace ZGL
{
	class BasicCanvas : public Canvas
	{
	public:
		BasicCanvas(int width, int height, bool isTopLeftOrigin = true, size_t spritesReserve = (size_t)10);
		BasicCanvas(float x, float y, int width, int height, 
			bool isTopLeftOrigin = true, size_t spritesReserve = (size_t)10);

		void Update();
		void Draw(Renderer& renderer);
		void Draw(Renderer& renderer, const Sprite& sprite);
		void DrawShapes(Renderer& renderer, bool isOutline = false);
		void DrawText(Renderer& renderer, Text& text);

		inline Vec2 GetPos() { return { m_posX, m_posY }; }
		inline Vec2i GetDimensions() { return { m_width, m_height }; }

		void SetPos(float x, float y);
		void SetDimensions(int width, int height);
		void SetClearColour(const Vec3& colour) { m_clearColour = colour; }
		void SetClearColour(float red, float green, float blue);

		void AddSprite(const Sprite& sprite);
		void RemoveSprite(const Sprite& sprite);
		void SetSprites(const std::vector<Sprite>& sprites);

		void SetShapes(const std::vector<ShapeF*>& shapes) { m_floatShapes = shapes; }
		void SetShapes(const std::vector<ShapeI*>& shapes) { m_intShapes = shapes; }

		// TL - Top Left, TR - Top Right, BL - BottomLeft, BR - bottom Right
		void SetSelectorTextures(std::shared_ptr<Texture2D> TL, std::shared_ptr<Texture2D> TR, std::shared_ptr<Texture2D> BL, std::shared_ptr<Texture2D> BR);

		void SetViewport();
	private:
		float m_posX, m_posY;
		int m_width, m_height;
		std::vector<Sprite> m_sprites;
		std::vector<ShapeF*> m_floatShapes;
		std::vector<ShapeI*> m_intShapes;

		std::vector<Sprite> m_selectSprites;

		void SetSelectSpritePos(const Vec2& mousePos, float width, float height, bool isGridMovement = true);

		Vec3 m_clearColour;
	};
}