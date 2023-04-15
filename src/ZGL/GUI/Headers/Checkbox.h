#pragma once

#include "ZGUI.h"
#include "Button.h"
#include "Widget.h"

namespace ZGL
{
	class Checkbox : public ZGUI
	{
	public:
		Checkbox(Widget& widget, Vec2 pos);
		Checkbox(Widget& widget, Vec2 pos, Vec2ui dimensions);

		void Update() override;
		void Draw(Renderer& renderer) override;

		void ButtonEvn();

		/* --- Getters --- */

		bool IsEnabled() const { return m_isEnabled; }

		/* --- Setters --- */

		void SetEnabled(bool val) { m_isEnabled = val; }
		
		void SetPos(Vec2 pos) override;
		void SetPos(float x, float y) override;
		void SetX(float x) override;
		void SetY(float y) override;

		void SetDimensions(Vec2ui dimensions) override;
		void SetDimensions(unsigned int width, unsigned int height) override;
		void SetWidth(unsigned int width) override;
		void SetHeight(unsigned int height) override;

		void SetButtonTexture(std::shared_ptr<Texture2D> texture);
		void SetCheckMarkTexture(std::shared_ptr<Texture2D> texture);

		void SetColour(Vec4 colour) { m_button->SetColour(colour); }
		void SetHoverColour(Vec4 colour) { m_button->SetHoverColour(colour); }
		void SetPressColour(Vec4 colour) { m_button->SetPressColour(colour); }

	private:
		std::unique_ptr<Button> m_button;
		std::unique_ptr<Sprite> m_checkmark;

		Vec2 m_widgetOffset;

		bool m_isEnabled = false;
	} typedef CheckBox;
}