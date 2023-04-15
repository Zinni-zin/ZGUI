#pragma once

#include <string>
#include <functional>

#include "ZGUI.h"
#include "Widget.h"

#include "../../Core/Timer.h"
#include "../../Shapes/Headers/Rectangle.h"
#include "../../Render/Headers/Sprite.h"

namespace ZGL
{
	class Button : public ZGUI
	{
	public:
		Button(Vec2 pos);
		Button(Vec2 pos, Vec2ui dimensions);
		Button(Widget& widget, Vec2 pos);
		Button(Widget& widget, Vec2 pos, Vec2ui dimensions);
		~Button();

		void Update() override;
		void Draw(Renderer& renderer) override;

		Vec2i GetFontSize() const
		{
			if (m_fontSize.x > 0 || m_fontSize.y > 0) return m_fontSize;
			else if (m_font) return m_font->GetSize();
			else return Vec2i::Zero();
		}

		Text* GetText() { return m_text.get(); }

		RectangleI* GetMouseRect() { return s_mouseRect; }

		/* --- Setters --- */

		void SetEventFnc(const std::function<void()>& eventFnc) { m_eventFnc = eventFnc; }

		void SetPos(Vec2 pos) override;
		void SetPos(float x, float y) override;
		void SetX(float x) override;
		void SetY(float y) override;

		void SetDimensions(Vec2ui dimensions) override;
		void SetDimensions(unsigned int width, unsigned int height) override;
		void SetWidth(unsigned int width) override;
		void SetHeight(unsigned int height) override;

		// Sets outline and fill texture of button
		void SetTextures(std::shared_ptr<Texture2D> fill, std::shared_ptr<Texture2D> outline);
		void SetTexture(std::shared_ptr<Texture2D> texture);
		void SetOutlineTexture(std::shared_ptr<Texture2D> texture);

		void CreateText(const std::string& text, Font& font);
		void SetText(const std::string& text);
		void SetFontSize(Vec2i size);

		void SetColour(Vec4 colour) { m_colour = colour; }
		void SetHoverColour(Vec4 colour) { m_hoverColour = colour; }
		void SetPressColour(Vec4 colour) { m_pressColour = colour; }

		void SetTextColour(Vec4 colour);
		void SetTextHoverColour(Vec4 colour) { m_textHoverColour = colour; }
		void SetTextPressColour(Vec4 colour) { m_textPressColour = colour; }
		
		void FlipSpriteVertically() { m_rect->FlipVertically(); }
	private:
		std::function<void()> m_eventFnc;

		std::unique_ptr<RectangleI> m_rect;

		std::unique_ptr<Text> m_text;
		Font* m_font;

		Vec2 m_widgetOffset;

		Vec2i m_fontSize;

		Vec4 m_colour;
		Vec4 m_hoverColour;
		Vec4 m_pressColour;

		Vec4 m_textColour;
		Vec4 m_textHoverColour;
		Vec4 m_textPressColour;

		bool m_isPressed = false;

		static RectangleI* s_mouseRect;
		static int s_refCount;
	};
}