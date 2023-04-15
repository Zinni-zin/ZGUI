#include "Headers/VertScrollbar.h"

#define RectWidth 10

namespace ZGL
{
	// Note: This class is unfinished!

	VertScrollbar::VertScrollbar(Widget& widget, unsigned int width, unsigned int sliderWidth,
		bool attachToWidget)
		: ZGUI(Vector2f(0.f, 0.f), Vector2ui(width, 0)), m_attachedElement(nullptr),
		m_rect(std::make_unique<RectangleI>(Vec2f(0.f, 0.f), Vec2i(width, 0))),
		m_slider(std::make_unique<RectangleI>(Vec2f(0.f, 0.f), Vec2i(sliderWidth, 0)))
	{
		if (width == 0) m_rect->SetWidth(RectWidth);
		if (sliderWidth == 0) m_slider->SetWidth(RectWidth);

		if (attachToWidget)
		{
			AddElements(widget.GetElements());
			widget.AddElement(this);
			m_attachedElement = &widget;
		}

		CalcPos();

	//	std::cout << "VertScrollbar(" << this << ")" << "\n\n";

		std::cout << "Elements Scrollbar (" << this << ") Will Edit:\n";

		for (int i = 0; (size_t)i < m_elements.size(); ++i)
			std::cout << (i + 1) << ": " << m_elements[i] << std::endl;
	}

	VertScrollbar::VertScrollbar(ZGUI* attachedElement, unsigned int width, unsigned int sliderWidth)
		: ZGUI(Vector2f(0.f, 0.f), Vector2ui(width, 0)), m_attachedElement(nullptr),
		m_rect(std::make_unique<RectangleI>(Vec2f(0.f, 0.f), Vec2i(width, 0))),
		m_slider(std::make_unique<RectangleI>(Vec2f(0.f, 0.f), Vec2i(sliderWidth, 0)))
	{
		if (width == 0) m_rect->SetWidth(RectWidth);
		if (sliderWidth == 0) m_slider->SetWidth(RectWidth);

		SetAttachedElement(attachedElement);

		if (Widget* widget = dynamic_cast<Widget*>(attachedElement))
			AddElements(widget->GetElements());

		//std::cout << "VertScrollbar(" << this << ")" << "\n\n";

		std::cout << "Elements Scrollbar (" << this << ") Will Edit:\n";

		for (int i = 0; (size_t)i < m_elements.size(); ++i)
			std::cout << (i + 1) << ": " << m_elements[i] << std::endl;
	}

	void VertScrollbar::Update()
	{
	}

	void VertScrollbar::Draw(Renderer& renderer)
	{
		if (m_rect && m_rect->GetFillTexture())
			renderer.DrawShapeFill(m_rect.get(), m_mvp);

		if (m_slider && m_slider->GetFillTexture())
			renderer.DrawShapeFill(m_slider.get(), m_mvp);

		if (m_rect && m_rect->GetOutlineTexture())
			renderer.DrawShapeOutline(m_rect.get(), m_mvp);
	}

	void VertScrollbar::CalcPos(std::vector<ZGUI*>* attachedElements)
	{
		Vec2 elementPos = m_attachedElement->GetPos();

		if (Widget* widget = dynamic_cast<Widget*>(m_attachedElement))
		{
			if (widget->GetDragRect().GetFillTexture() || widget->GetDragRect().GetOutlineTexture())
			{
				elementPos.y += widget->GetDragRectHeight();
				m_rect->SetHeight(widget->GetHeight() - widget->GetDragRectHeight());
			}
		}

		m_pos = elementPos;
		m_rect->SetPos(elementPos);
		m_slider->SetPos(elementPos);

		if (attachedElements)
			OffsetElements(attachedElements);
	}

#pragma region Setup Elements

	void VertScrollbar::SetAttachedElement(ZGUI* element)
	{
		if (!m_attachedElement) 
		{
			m_attachedElement = element; 
			CalcPos();
		}
		else 
			std::cout << "Error VertScrollbar(" << this << ") Element already attached!\n";
	}

	void VertScrollbar::AddElement(ZGUI* element)
	{
		m_elements.push_back(element);
	}

	void VertScrollbar::AddElements(std::vector<ZGUI*> elements)
	{
		if(m_elements.empty()) m_elements.reserve(elements.size());
		for (ZGUI* element : elements) m_elements.push_back(element);
	}

	void VertScrollbar::OffsetElements(std::vector<ZGUI*>* elements, float offsetFromBar)
	{
		for (ZGUI* element : *elements)
		{
			element->SetX(m_pos.x + m_dimensions.x + offsetFromBar);
		}
	}
#pragma endregion

#pragma region Set Textures

	void VertScrollbar::SetRectTexture(std::shared_ptr<Texture2D> texture)
	{
		m_rect->SetFillTexture(texture);

		if (m_slider->GetWidth() <= 0)
			m_slider->SetWidth(texture->GetWidth());

		CalcPos();
	}

	void VertScrollbar::SetRectOutline(std::shared_ptr<Texture2D> texture)
	{
		m_rect->SetOutlineTexture(texture);

		if (m_slider->GetWidth() <= 0)
			m_slider->SetWidth(texture->GetWidth());

		CalcPos();
	}

	void VertScrollbar::SetSliderTexture(std::shared_ptr<Texture2D> texture)
	{
		m_slider->SetFillTexture(texture);

		if (m_slider->GetHeight() <= 0)
			m_slider->SetHeight(texture->GetHeight());

		if (m_slider->GetWidth() <= 0)
		{
			if (m_rect->GetWidth() > 0) m_slider->SetWidth(m_rect->GetWidth());
			else m_slider->SetWidth(texture->GetWidth());
		}

		CalcPos();
	}

#pragma endregion

#pragma region Update Pos/Dimensions
	void VertScrollbar::SetPos(Vec2 pos)
	{
		m_pos = pos;
		m_rect->SetPos(pos);

		// Todo: Make sure the slider stays at the same position it was
		m_slider->SetPos(pos);
	}

	void VertScrollbar::SetPos(float x, float y)
	{
		m_pos.x = x;
		m_pos.y = y;
		m_rect->SetPos(x, y);

		// Todo: Make sure the slider stays at the same position it was
		m_slider->SetPos(x, y);
	}

	void VertScrollbar::SetX(float x)
	{
		m_pos.x = x;
		m_rect->SetX(x);
		m_slider->SetX(x);
	}

	void VertScrollbar::SetY(float y)
	{
		m_pos.y = y;
		m_rect->SetY(y);

		// Todo: Make sure the slider stays at the same position it was
		m_slider->SetY(y);
	}

	void VertScrollbar::SetDimensions(Vec2ui dimensions) 
	{
		m_dimensions.y = dimensions.y;
		m_rect->SetHeight(dimensions.y);
	}

	void VertScrollbar::SetDimensions(unsigned int width, unsigned int height) 
	{
		m_dimensions.y = width;
		m_rect->SetHeight(height);
	}

	void VertScrollbar::SetHeight(unsigned int height) 
	{
		m_dimensions.y = height;
		m_rect->SetHeight(height);
	}
#pragma endregion
}