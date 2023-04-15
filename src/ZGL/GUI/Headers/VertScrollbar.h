#pragma once

#include "ZGUI.h"
#include "Widget.h"

#include "../../Shapes/Headers/Rectangle.h"

#include <functional>

namespace ZGL
{
	// Note: This class is unfinished!

	class VertScrollbar : public ZGUI
	{
	public:
		VertScrollbar(Widget& widget, unsigned int width = 0, unsigned int sliderWidth = 0,
			bool attachToWidget = true);
		VertScrollbar(ZGUI* attachedElement, unsigned int width = 0, unsigned int sliderWidth = 0);

		void Update() override;
		void Draw(Renderer& renderer) override;

		/* ---- Getters ---- */

		/* ---- Setters ---- */

		void SetRectColour(float r, float g, float b, float a = 1.f) { m_rect->SetColour(r, g, b, a); }
		void SetRectColour(Vec4 colour) { m_rect->SetColour(colour); }
		void SetSliderColour(float r, float g, float b, float a = 1.f) { m_slider->SetColour(r, g, b, a); }
		void SetSliderColour(Vec4 colour) { m_slider->SetColour(colour); }

		void SetRectTexture(std::shared_ptr<Texture2D> texture);
		void SetRectOutline(std::shared_ptr<Texture2D> texture);
		void SetSliderTexture(std::shared_ptr<Texture2D> texture);

#pragma region Set Pos and Dimensions

	// Positions are automatic, so we just call CalcPos(), this is mainly used for widget moving.
		void SetPos(Vec2 pos) override;

		// Positions are automatic, so we just call CalcPos(), this is mainly used for widget moving.
		void SetPos(float x, float y) override;

		// Positions are automatic, so we just call CalcPos(), this is mainly used for widget moving.
		void SetX(float x) override;

		// Positions are automatic, so we just call CalcPos(), this is mainly used for widget moving.
		void SetY(float y) override;

		void SetDimensions(Vec2ui dimensions) override;

		void SetDimensions(unsigned int width, unsigned int height) override;

		void SetHeight(unsigned int height) override;

#pragma endregion

	private:
		Vec2 m_widgetOffset;
		Vec2ui m_sliderDimensions;

		std::unique_ptr<RectangleI> m_rect;
		std::unique_ptr<RectangleI> m_slider;

		/*
		* attachedElements will update position of the elements that  
		* the scrollbar is attached to,
		* to the offset of the scroll bar.
		*/
		void CalcPos(std::vector<ZGUI*>* attachedElements = nullptr);

#pragma region Element Stuff
	private:
		std::vector<ZGUI*> m_elements;

		ZGUI* m_attachedElement;

		void SetAttachedElement(ZGUI* element);
		void AddElement(ZGUI* element);
		void AddElements(std::vector<ZGUI*> elements);
		void OffsetElements(std::vector<ZGUI*>* elements, float offsetFromBar = 0.5f);
#pragma endregion
	};
}