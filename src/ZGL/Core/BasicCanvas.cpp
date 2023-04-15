#include "BasicCanvas.h"
#include <iostream>

#define ShapeMoveSpeed 2.5f

namespace ZGL
{
	Vec2 ShapeInput(const Vec2& shapePos)
	{
		Vec2 result = shapePos;

		Input& input = Input::GetInput();
		if (input.IsKeyPressed(Key::UP))
			result.y -= ShapeMoveSpeed;
		else if (input.IsKeyPressed(Key::DOWN))
			result.y += ShapeMoveSpeed;

		if (input.IsKeyPressed(Key::RIGHT))
			result.x += ShapeMoveSpeed;
		if (input.IsKeyPressed(Key::LEFT))
			result.x -= ShapeMoveSpeed;

		return result;
	}

	BasicCanvas::BasicCanvas(int width, int height, bool isTopLeftOrigin, size_t spritesReserve)
		: Canvas(width, height, isTopLeftOrigin), m_posX(0), m_posY(0), m_clearColour(0.05f, 0.05f, 0.05f)
	{
		SetDimensions(width, height);
		m_sprites.reserve(spritesReserve);
		m_selectSprites.reserve(4);
		for (int i = 0; i < 4; ++i)
			m_selectSprites.push_back(Sprite(0, 0));
	}

	BasicCanvas::BasicCanvas(float x, float y, int width, int height, bool isTopLeftOrigin, size_t spritesReserve)
		: Canvas(x, y, width, height, isTopLeftOrigin), m_clearColour(0.05f, 0.05f, 0.05f)
	{
		SetDimensions(width, height);
		m_sprites.reserve(spritesReserve);
		m_selectSprites.reserve(4);
		for (int i = 0; i < 4; ++i)
			m_selectSprites.push_back(Sprite(0, 0));
	}

	bool canMoveCircle = false;
	void BasicCanvas::Update()
	{
		Input& input = Input::GetInput();
		if (input.IsKeyPressed(Key::Z))
			canMoveCircle = false;
		else if (input.IsKeyPressed(Key::X))
			canMoveCircle = true;

		if (canMoveCircle)
			m_floatShapes[0]->SetPos(ShapeInput(m_floatShapes[0]->GetPos()));
		else
			m_intShapes[0]->SetPos(ShapeInput(m_intShapes[0]->GetPos()));

		if (m_intShapes[0]->HasIntersected(m_intShapes[1]))
			m_intShapes[0]->SetColour(1.0f, 0.5f, 1.0f, 1.0f);
		else if (m_intShapes[0]->HasIntersected(m_floatShapes[1]))
			m_intShapes[0]->SetColour(0.0f, 1.0f, 0.5f, 1.0f);
		else
			m_intShapes[0]->SetColour(Vec4::One());

		if (m_floatShapes[0]->HasIntersected(m_floatShapes[1]))
			m_floatShapes[0]->SetColour(1.0f, 0.5f, 1.0f, 1.0f);
		else if (m_floatShapes[0]->HasIntersected(m_intShapes[1]))
			m_floatShapes[0]->SetColour(0.0f, 1.0f, 0.5f, 1.0f);
		else
			m_floatShapes[0]->SetColour(Vec4::One());


		SetSelectSpritePos(m_cam->GetMousePos(), 50.0f, 50.0f);

		m_cam->Update();
	}

	void BasicCanvas::Draw(Renderer& renderer)
	{
		renderer.Draw(m_sprites, m_cam->GetPV());
		renderer.Draw(m_selectSprites, m_cam->GetPV());
	}

	void BasicCanvas::Draw(Renderer& renderer, const Sprite& sprite)
	{
		renderer.Draw(sprite, m_cam->GetPV());
	}

	void BasicCanvas::DrawShapes(Renderer& renderer, bool isOutline)
	{
		renderer.DrawShape(m_intShapes, m_cam->GetPV(), isOutline);
		renderer.DrawShape(m_floatShapes, m_cam->GetPV(), isOutline);
	}

	void BasicCanvas::DrawText(Renderer& renderer, Text& text)
	{
		renderer.DrawText(text, m_cam->GetPV());
	}

	void BasicCanvas::SetViewport()
	{
		glScissor((GLint)m_posX, (GLint)m_posY, m_width, m_height);
		glEnable(GL_SCISSOR_TEST);

		glClearColor(m_clearColour.x, m_clearColour.y, m_clearColour.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glViewport((GLint)m_posX, (GLint)m_posY, m_width, m_height);
	}

	void BasicCanvas::SetPos(float x, float y)
	{
		m_posX = x;
		m_posY = y;
	}

	void BasicCanvas::SetDimensions(int width, int height)
	{
		if (width <= 0 && height <= 0)
		{
			std::cout << "Error width and height have to be greater than 0!\n";
			return;
		}

		m_width = width;
		m_height = height;
	}

	void BasicCanvas::SetClearColour(float red, float green, float blue)
	{
		m_clearColour = Vec3(red, green, blue);
	}

	void BasicCanvas::AddSprite(const Sprite& sprite)
	{
		m_sprites.emplace_back(sprite);
	}

	void BasicCanvas::RemoveSprite(const Sprite& sprite)
	{
		auto it = m_sprites.begin();
		for (Sprite& s : m_sprites)
		{
			if (&s == &sprite)
			{
				m_sprites.erase(it);
				break;
			}

			++it;
		}
	}

	void BasicCanvas::SetSprites(const std::vector<Sprite>& sprites)
	{
		m_sprites = sprites;
	}

	void BasicCanvas::SetSelectorTextures(std::shared_ptr<Texture2D> TL, std::shared_ptr<Texture2D> TR, std::shared_ptr<Texture2D> BL, std::shared_ptr<Texture2D> BR)
	{
		m_selectSprites[0].SetTexture(TL);
		m_selectSprites[1].SetTexture(TR);
		m_selectSprites[2].SetTexture(BL);
		m_selectSprites[3].SetTexture(BR);
	}

	void BasicCanvas::SetSelectSpritePos(const Vec2& mousePos, float width, float height, bool isGridMovement)
	{
		if (isGridMovement)
		{
			Vec2 pos;
			Vec2i multiFactor((int)mousePos.x / (int)width, (int)mousePos.y / (int)height);

			pos.x = multiFactor.x * width;
			pos.y = multiFactor.y * height;

			if (mousePos.x < 0) pos.x -= width;
			if (mousePos.y < 0) pos.y -= height;

			m_selectSprites[0].SetPos(pos.x, pos.y);
			m_selectSprites[1].SetPos(pos.x + width - m_selectSprites[1].GetTextureWidth(), pos.y);
			m_selectSprites[2].SetPos(pos.x, pos.y + height - m_selectSprites[2].GetTextureHeight());
			m_selectSprites[3].SetPos(pos.x + width - m_selectSprites[3].GetTextureWidth(), pos.y + height - m_selectSprites[3].GetTextureHeight());
		}
		else
		{
			m_selectSprites[0].SetPos(mousePos.x, mousePos.y);
			m_selectSprites[1].SetPos((mousePos.x + width - m_selectSprites[1].GetTextureWidth()), mousePos.y);
			m_selectSprites[2].SetPos(mousePos.x, (mousePos.y + height - m_selectSprites[2].GetTextureHeight()));
			m_selectSprites[3].SetPos((mousePos.x + width - m_selectSprites[3].GetTextureWidth()), (mousePos.y + height - m_selectSprites[3].GetTextureHeight()));
		}
	}
}