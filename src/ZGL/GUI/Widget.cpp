#include "Headers/Widget.h"

#include "../Core/Core.h"
#include "../Math/ZMath.h"
#include "../Input/Input.h"

#include "Headers/Button.h"
#include "Headers/Textbox.h"
#include "Headers/Slider.h"
#include "Headers/VertScrollbar.h"

#define CloseButtonW 16

#define cResizeWidth 16 // Width of the cursor resize texture
#define cResizeHeight 25 // Height of the cursor resize texture

#define ResizeRect 8 // The width or height of the resize rects
#define ResizeOffset 20 // The offset of which we can no longer resize in a direction

#define bRectPos(pX, pY, w, h) pX, (pY + h) - ResizeRect
#define lRectPos(pX, pY, w, h) pX, pY
#define rRectPos(pX, pY, w, h) (pX + w) - ResizeRect, pY

#define bRectDimensions(w) w, ResizeRect
#define lRectDimensions(h) ResizeRect, h
#define rRectDimensions(h) ResizeRect, h

#define bRect(pX, pY, w, h) bRectPos(pX, pY, w, h), bRectDimensions(w) 
#define lRect(pX, pY, w, h) lRectPos(pX, pY, w, h), lRectDimensions(h)
#define rRect(pX, pY, w, h) rRectPos(pX, pY, w, h), rRectDimensions(h)

#define ScissorRectPos(x, y, h) x, (Core::GetCore().GetHeight() - y) - h

#define ScissorRect(x, y, w, h) ScissorRectPos(x, y, h), w, h

namespace ZGL
{
	void CloseButtonEvent(Widget* widget)
	{
		widget->SetActive(false);
	}

#pragma region Constructors
	Widget::Widget(float x, float y, unsigned int width, unsigned int height, Vec4 colour)
		: ZGUI(Vec2(x, y), Vec2ui(width, height)), m_colour(colour),
		m_rect(std::make_unique<RectangleI>(x, m_pos.y, width, height, m_colour)),
		m_dragRect(std::make_unique<RectangleI>(x, y, width, m_dragRectHeight)),
		m_mouseRect(std::make_unique<RectangleI>(0.f, 0.f, 1, 1)),
		m_bottomResizeRect(std::make_unique<RectangleI>(bRect(x, y, width, height))),
		m_leftResizeRect(std::make_unique<RectangleI>(lRect(x, y + m_dragRectHeight, width, height - m_dragRectHeight))),
		m_rightResizeRect(std::make_unique<RectangleI>(rRect(x, y + m_dragRectHeight, width, height - m_dragRectHeight))),
		m_resizeCursor(std::make_unique<Sprite>(0.f, 0.f, cResizeWidth, cResizeHeight)),
		m_scissorRect(std::make_unique<RectangleI>(ScissorRect(x, y, width, height)))
	{
	}

	Widget::Widget(float x, float y, Vec2ui dimensions, Vec4 colour)
		: ZGUI(Vec2(x, y), Vec2ui(dimensions)), m_colour(colour),
		m_rect(std::make_unique<RectangleI>(x, y, Vec2i(dimensions.x, dimensions.y), m_colour)),
		m_dragRect(std::make_unique<RectangleI>(x, y, dimensions.x, m_dragRectHeight)),
		m_mouseRect(std::make_unique<RectangleI>(0.f, 0.f, 1, 1)),
		m_bottomResizeRect(std::make_unique<RectangleI>(bRect(x, y, dimensions.x, dimensions.y))),
		m_leftResizeRect(std::make_unique<RectangleI>(lRect(x, y + m_dragRectHeight, dimensions.x, dimensions.y - m_dragRectHeight))),
		m_rightResizeRect(std::make_unique<RectangleI>(rRect(x, y + m_dragRectHeight, dimensions.x, dimensions.y - m_dragRectHeight))),
		m_resizeCursor(std::make_unique<Sprite>(0.f, 0.f, cResizeWidth, cResizeHeight)),
		m_scissorRect(std::make_unique<RectangleI>(ScissorRect(x, y, dimensions.x, dimensions.y)))
	{
	}

	Widget::Widget(Vec2 pos, unsigned int width, unsigned int height, Vec4 colour)
		: ZGUI(pos, Vec2ui(width, height)), m_colour(colour),
		m_rect(std::make_unique<RectangleI>(pos, width, height, m_colour)),
		m_dragRect(std::make_unique<RectangleI>(pos, width, m_dragRectHeight)),
		m_mouseRect(std::make_unique<RectangleI>(0.f, 0.f, 1, 1)),
		m_bottomResizeRect(std::make_unique<RectangleI>(bRect(pos.x, pos.y, width, height))),
		m_leftResizeRect(std::make_unique<RectangleI>(lRect(pos.x, pos.y + m_dragRectHeight, width, height - m_dragRectHeight))),
		m_rightResizeRect(std::make_unique<RectangleI>(rRect(pos.x, pos.y + m_dragRectHeight, width, height - m_dragRectHeight))),
		m_resizeCursor(std::make_unique<Sprite>(0.f, 0.f, cResizeWidth, cResizeHeight)),
		m_scissorRect(std::make_unique<RectangleI>(ScissorRect(pos.x, pos.y, width, height)))
	{
	}

	Widget::Widget(Vec2 pos, Vec2ui dimensions, Vec4 colour)
		: ZGUI(pos, dimensions), m_colour(colour),
		m_rect(std::make_unique<RectangleI>(pos, Vec2i(dimensions.x, dimensions.y), m_colour)),
		m_dragRect(std::make_unique<RectangleI>(pos, dimensions.x, m_dragRectHeight)),
		m_mouseRect(std::make_unique<RectangleI>(0.f, 0.f, 1, 1)),
		m_bottomResizeRect(std::make_unique<RectangleI>(bRect(pos.x, pos.y, dimensions.x, dimensions.y))),
		m_leftResizeRect(std::make_unique<RectangleI>(lRect(pos.x, pos.y + m_dragRectHeight, dimensions.x, dimensions.y - m_dragRectHeight))),
		m_rightResizeRect(std::make_unique<RectangleI>(rRect(pos.x, pos.y + m_dragRectHeight, dimensions.x, dimensions.y - m_dragRectHeight))),
		m_resizeCursor(std::make_unique<Sprite>(0.f, 0.f, cResizeWidth, cResizeHeight)),
		m_scissorRect(std::make_unique<RectangleI>(ScissorRect(pos.x, pos.y, dimensions.x, dimensions.y)))
	{
	}
#pragma endregion

	Vec2 mouseDistance(0.f);
	void Widget::Update()
	{
		if (m_isActive)
		{
			Input& input = Input::GetInput();

			m_mouseRect->SetPos(input.GetMousePos());

			if (m_mouseRect->HasIntersected(m_dragRect.get()))
			{
				if (input.IsMouseButtonPressed(Mouse::LEFT_BUTTON))
				{
					m_isPressed = true;
					mouseDistance = Vec2(m_mouseRect->GetPos().x - m_pos.x, m_mouseRect->GetPos().y - m_pos.y);
				}
			}
			if (input.IsMouseButtonReleased(Mouse::LEFT_BUTTON)) m_isPressed = false;


			if (m_isPressed)
			{
				Window& window = Core::GetCore().GetWindow();

				SetPos(m_mouseRect->GetPos() - mouseDistance);

				if (m_rect->GetRight() > window.GetWidth()) SetX((float)(window.GetWidth() - m_rect->GetWidth()));
				else if (m_rect->GetLeft() < 0) SetX(0);

				if (m_rect->GetTop() < 0) SetY(0);
				else if (m_rect->GetBottom() > window.GetHeight()) SetY((float)(window.GetHeight() - m_rect->GetHeight()));
			}

			for (auto it = m_elements.begin(); it != m_elements.end(); ++it)
			{
				if (it->second->GetX() < m_rect->GetRight() && it->second->GetY() < m_rect->GetBottom())
				{
					it->second->Update();

					Textbox* tb = dynamic_cast<Textbox*>(it->second);

					// Update methods for ui that requires a mouse rect.
					if (m_mouseRect->HasIntersected(m_rect.get()) && !m_isPressed && !m_isResizing)
					{
						if (tb) { tb->Update(*m_mouseRect); m_isElementHovered = tb->IsHover(); }
						else m_isElementHovered = false;
					}
					else if (input.IsMouseButtonPressed(Mouse::LEFT_BUTTON) && !m_isResizeable)
					{
						/* --- Figure out textbox stuff --- */
						if (tb && tb->IsHeld())
						{
							tb->Update(*m_mouseRect);
							m_isElementHovered = true;
						}

						if (tb && (m_isPressed || !tb->IsHeld()))
						{
							tb->SetActive(false);
							m_isElementHovered = false;
						}
					}
				}
			}

			if (m_isUsingCloseButton && m_closeButton)
				m_closeButton->Update();

			if (m_vertScrollbar)
				m_vertScrollbar->Update();

			Resize();
		}
	}

	void Widget::Draw(Renderer& renderer)
	{
		if (m_isActive)
		{
			glEnable(GL_SCISSOR_TEST);

			// Draw background rect texture
			if (m_rect != nullptr && m_rect->GetFillTexture() != nullptr)
				renderer.DrawShapeFill(m_rect.get(), m_mvp);

			for (auto it = m_elements.rbegin(); it != m_elements.rend(); ++it)
				if (it->second != nullptr)
					it->second->Draw(renderer);

			// Draw outline texture
			if (m_rect != nullptr && m_rect->GetOutlineTexture() != nullptr)
				renderer.DrawShapeFill(m_rect.get(), m_mvp);
			
			if (m_dragRect != nullptr)
			{
				if (m_dragRect->GetFillTexture() != nullptr)
					renderer.DrawShapeFill(m_dragRect.get(), m_mvp);

				if (m_dragRect->GetOutlineTexture() != nullptr)
					renderer.DrawShapeOutline(m_dragRect.get(), m_mvp);
			}

			if (m_vertScrollbar)
				m_vertScrollbar->Draw(renderer);

			glScissor(0, 0,
				Core::GetCore().GetWindow().GetWidth(), Core::GetCore().GetWindow().GetHeight());

			if (m_isResizingHighlight && m_resizeCursor->HasTexture())
				renderer.Draw(*m_resizeCursor, m_mvp);

			if (m_isUsingCloseButton && m_closeButton)
				m_closeButton->Draw(renderer);

		}
	}

	void Widget::Resize()
	{
		if (m_isResizeable)
		{
			bool isInBottomRect = m_mouseRect->HasIntersected(m_bottomResizeRect.get());
			bool isInLeftRect = m_mouseRect->HasIntersected(m_leftResizeRect.get());
			bool isInRightRect = m_mouseRect->HasIntersected(m_rightResizeRect.get());

			#pragma region Figure out the angle we are resizing

			if (!m_isPressed && (isInBottomRect && (isInLeftRect || isInRightRect)))
			{
				m_resizeCursor->SetPos(m_mouseRect->GetPos());
				if (isInLeftRect)
				{
					m_resizeCursor->SetRotation(ToRadians(-45.f));
					m_resizeCursor->SetPos(m_mouseRect->GetPos().x - m_resizeCursor->GetWidth(), m_mouseRect->GetPos().y);
					m_isResizingLeft = true;
					m_isResizingRight = false;
				}
				else if (isInRightRect)
				{
					m_resizeCursor->SetRotation(ToRadians(45.f));
					m_isResizingLeft = false;
					m_isResizingRight = true;
				}

				m_isResizingBoth = true;
				m_isResizingBottom = true;
				m_isResizingHighlight = true;
			}
			else if (!m_isPressed && !m_isResizingBoth && isInBottomRect)
			{
				m_resizeCursor->SetPos(m_mouseRect->GetPos());
				m_resizeCursor->SetRotation(ToRadians(0.f));
				m_isResizingBoth = false;
				m_isResizingBottom = true;
				m_isResizingLeft = false;
				m_isResizingRight = false;
				m_isResizingHighlight = true;
			}
			else if (!m_isPressed && !m_isResizingBoth && !m_isResizingRight && isInLeftRect)
			{
				m_resizeCursor->SetPos(m_mouseRect->GetPos());
				m_resizeCursor->SetRotation(ToRadians(90.f));
				m_isResizingBoth = false;
				m_isResizingBottom = false;
				m_isResizingLeft = true;
				m_isResizingRight = false;
				m_isResizingHighlight = true;
			}
			else if (!m_isPressed && !m_isResizingBoth && !m_isResizingLeft && isInRightRect)
			{
				m_resizeCursor->SetPos(m_mouseRect->GetPos());
				m_resizeCursor->SetRotation(ToRadians(90.f));
				m_isResizingBoth = false;
				m_isResizingBottom = false;
				m_isResizingLeft = false;
				m_isResizingRight = true;
				m_isResizingHighlight = true;
			}
			else if(!m_isResizing)
			{
				m_isResizingBoth = false;
				m_isResizingBottom = false;
				m_isResizingLeft = false;
				m_isResizingRight = false;
				m_isResizingHighlight = false;
			}

#pragma endregion

			/* Figure displaying the mouse cursor*/
			if ((m_isResizingHighlight || m_isResizing) && !m_isElementHovered && Core::GetCore().IsShowingCursor())
				Core::GetCore().ShowCursor(false);
			else if (!m_isResizingHighlight && !m_isElementHovered && !Core::GetCore().IsShowingCursor())
				Core::GetCore().ShowCursor(true);
		
			// Figure out if we are currently resizing or not
			if (m_isResizingHighlight && !m_isElementHovered && Input::GetInput().IsMouseButtonPressed(Mouse::LEFT_BUTTON))
				m_isResizing = true;

			/* --- Resize The Widget */
			if (m_isResizing)
			{
				m_resizeCursor->SetPos(m_mouseRect->GetPos());

				if (m_isResizingLeft && m_mouseRect->GetPos().x < (m_rect->GetRight() - ResizeOffset))
				{
					unsigned int width = (unsigned int)(m_rect->GetRight() - m_mouseRect->GetX());

					m_rect->SetWidth(width);
					m_dragRect->SetWidth(width);
					SetX(m_mouseRect->GetX());
					m_bottomResizeRect->SetWidth(width);
					m_scissorRect->SetWidth(width);
				}
				else if (m_isResizingRight && m_mouseRect->GetPos().x > (m_rect->GetLeft() + ResizeOffset))
				{
					unsigned int width = m_rect->GetWidth();
					if (m_mouseRect->GetPos().x > m_rect->GetRight()) // Increase width
						width += (unsigned int)(m_mouseRect->GetPos().x - m_rect->GetRight());
					else if (m_mouseRect->GetPos().x < m_rect->GetRight()) // Decrease height
						width -= (unsigned int)(m_rect->GetRight() - m_mouseRect->GetPos().x);

					m_rect->SetWidth(width);
					m_rightResizeRect->SetPos((float)(m_rect->GetRight() - ResizeRect),
						m_rightResizeRect->GetY());

					m_bottomResizeRect->SetWidth(width);
					m_dragRect->SetWidth(width);
					m_scissorRect->SetWidth(width);

					m_closeButton->SetX((float)(m_dragRect->GetRight() - CloseButtonW));
				}
				if (m_isResizingBottom && m_mouseRect->GetPos().y > (m_dragRect->GetPos().y + m_dragRect->GetHeight() + ResizeOffset))
				{
					unsigned int height = m_rect->GetHeight();
					if(m_mouseRect->GetPos().y > m_rect->GetBottom()) // Decrease height
						height += (unsigned int)(m_mouseRect->GetPos().y - m_rect->GetBottom());
					else if(m_mouseRect->GetPos().y < m_rect->GetBottom()) // Increase Height
						height -= (unsigned int)(m_rect->GetBottom() - m_mouseRect->GetPos().y);

					m_rect->SetHeight(height);
					m_bottomResizeRect->SetPos(m_bottomResizeRect->GetX(),
						(float)(m_rect->GetBottom() - ResizeRect));

					m_rightResizeRect->SetHeight(height - ResizeRect);
					m_leftResizeRect->SetHeight(height - ResizeRect);

					m_vertScrollbar->SetHeight(height);

					m_scissorRect->SetPos(ScissorRectPos(m_rect->GetX(), m_rect->GetY(), height));
					m_scissorRect->SetHeight(height);
				}
			}
			
			// Determine if we stopped resizing
			if (m_isPressed || (m_isResizing && Input::GetInput().IsMouseButtonReleased(Mouse::LEFT_BUTTON)))
				m_isResizing = false;
		}
	}

	void Widget::UseCloseButton(bool val)
	{
		m_isUsingCloseButton = val;
		
		if (val && !m_closeButton)
		{
			m_closeButton = std::make_unique<Button>(
				Vec2((float)(m_dragRect->GetRight() - CloseButtonW), m_dragRect->GetY()),
				Vec2ui(CloseButtonW, m_dragRectHeight));

			m_closeButton->SetEventFnc(std::bind(CloseButtonEvent, this));
		}
		else
			m_closeButton.reset(nullptr);
	}

	void Widget::SetCloseButtonTexture(std::shared_ptr<Texture2D> texture)
	{
		if (!m_closeButton)
		{
			m_isUsingCloseButton = true;

			m_closeButton = std::make_unique<Button>(
				Vec2((float)(m_dragRect->GetRight() - CloseButtonW), m_dragRect->GetY()),
				Vec2ui(CloseButtonW, m_dragRectHeight));

			m_closeButton->SetEventFnc(std::bind(CloseButtonEvent, this));

			m_closeButton->SetHoverColour(1.f);
			m_closeButton->SetPressColour(1.f);
		}

		m_closeButton->SetTexture(texture);
	}

	void Widget::UseVertScrollbar(unsigned int width, unsigned sliderWidth)
	{
		if (m_vertScrollbar)
		{
			std::cout << "Error! Scrollbar already exists for widget: " << this << std::endl;
			return;
		}

		m_vertScrollbar = std::make_unique<VertScrollbar>(this, width, sliderWidth);
	}

	void Widget::UseVertScrollbar(std::shared_ptr<Texture2D> fillTexture,
		std::shared_ptr<Texture2D> sliderTexture, unsigned int width, unsigned int sliderWidth)
	{
		if (m_vertScrollbar)
		{
			std::cout << "Error! Scrollbar already exists for widget: " << this << std::endl;
			return;
		}

		m_vertScrollbar = std::make_unique<VertScrollbar>(this, width, sliderWidth);
		m_vertScrollbar->SetRectTexture(fillTexture);
		m_vertScrollbar->SetSliderTexture(sliderTexture);
		m_vertScrollbar->SetRectColour(0.2f, 0.2f, 0.2f);
	}

#pragma region Update Position/Dimensions
	void Widget::SetPos(Vec2 pos) 
	{
		m_pos = pos; 
		m_rect->SetPos(pos); 
		m_dragRect->SetPos(pos); 

		m_bottomResizeRect->SetPos(pos.x, (pos.y + m_rect->GetHeight()) - ResizeRect);
		m_leftResizeRect->SetPos(pos.x, pos.y + m_dragRect->GetHeight());
		m_rightResizeRect->SetPos((pos.x + m_rect->GetWidth()) - ResizeRect, pos.y + m_dragRect->GetHeight());
		
		m_scissorRect->SetPos(ScissorRectPos(pos.x, pos.y, m_rect->GetHeight()));

		if (m_closeButton)
			m_closeButton->SetPos(Vec2((float)(m_dragRect->GetRight() - CloseButtonW), m_dragRect->GetY()));

		if (m_vertScrollbar)
		{
			float yPos = (m_dragRect) ? pos.y + m_dragRect->GetHeight() : pos.y;
			m_vertScrollbar->SetPos(pos.x, yPos);
		}

		for (auto it = m_elements.begin(); it != m_elements.end(); ++it)
			it->second->SetPos(pos);
	}

	void Widget::SetPos(float x, float y) 
	{ 
		m_pos = Vec2(x, y); 
		m_rect->SetPos(x, y);
		m_dragRect->SetPos(x, y); 

		m_bottomResizeRect->SetPos(x, (y + m_rect->GetHeight()) - ResizeRect);
		m_leftResizeRect->SetPos(x, y + m_dragRect->GetHeight());
		m_rightResizeRect->SetPos((x + m_rect->GetWidth()) - ResizeRect, y + m_dragRect->GetHeight());
		
		m_scissorRect->SetPos(ScissorRectPos(x, y, m_rect->GetHeight()));

		if (m_closeButton)
			m_closeButton->SetPos((float)(m_dragRect->GetRight() - CloseButtonW), y);

		if (m_vertScrollbar)
		{
			float yPos = (m_dragRect) ? y + m_dragRect->GetHeight() : y;
			m_vertScrollbar->SetPos(x, yPos);
		}

		for (auto it = m_elements.begin(); it != m_elements.end(); ++it)
			it->second->SetPos(x, y);
	}

	void Widget::SetX(float x) 
	{ 
		m_pos.x = x; 
		m_rect->SetPos(x, m_pos.y); 
		m_dragRect->SetPos(x, m_pos.y); 

		m_bottomResizeRect->SetPos(x, m_bottomResizeRect->GetY());
		m_leftResizeRect->SetPos(x, m_leftResizeRect->GetY());
		m_rightResizeRect->SetPos((x + m_rect->GetWidth()) - ResizeRect, m_rightResizeRect->GetY());

		m_scissorRect->SetPos(ScissorRectPos(x, m_rect->GetY(), m_rect->GetHeight()));

		if (m_closeButton)
			m_closeButton->SetX((float)(m_dragRect->GetRight() - CloseButtonW));

		if (m_vertScrollbar)
			m_vertScrollbar->SetX(x);

		for (auto it = m_elements.begin(); it != m_elements.end(); ++it)
			it->second->SetX(x);
	}

	void Widget::SetY(float y) 
	{ 
		m_pos.y = y; 
		m_rect->SetPos(m_pos.x, y); 
		m_dragRect->SetPos(m_pos.x, y); 

		m_bottomResizeRect->SetPos(m_pos.x, (y + m_rect->GetHeight()) - ResizeRect);
		m_leftResizeRect->SetPos(m_pos.x, y + m_dragRect->GetHeight());
		m_rightResizeRect->SetPos((m_pos.x + m_rect->GetWidth()) - ResizeRect, y + m_dragRect->GetHeight());

		m_scissorRect->SetPos(ScissorRectPos(m_rect->GetX(), y, m_rect->GetHeight()));

		if (m_closeButton)
			m_closeButton->SetY(y);

		if (m_vertScrollbar)
		{
			m_vertScrollbar->SetY((m_dragRect) ? y + m_dragRect->GetHeight() : y);
		}

		for (auto it = m_elements.begin(); it != m_elements.end(); ++it)
			it->second->SetY(y);
	}
#pragma endregion

#pragma region GUI Elements
	void Widget::AddElement(int order, ZGUI* element)
	{
		if (m_elements.find(order) != m_elements.end()) return;

		m_elements[order] = element;
	}

	void Widget::AddElement(ZGUI* element)
	{
		m_elements[m_elements.size()] = element;
	}

	void Widget::RemoveElement(ZGUI* element)
	{
		for (auto it = m_elements.begin(); it != m_elements.end(); ++it)
		{
			if (element == it->second)
			{
				it = m_elements.erase(it);
				return;
			}
		}
	}

	void Widget::RemoveElement(int pos)
	{
		auto it = m_elements.erase(pos);
	}

	void Widget::ClearElements()
	{
		m_elements.clear();
	}

	std::vector<ZGUI*> Widget::GetElements()
	{
		std::vector<ZGUI*> elements;
		elements.reserve(m_elements.size());

		for (auto it = m_elements.begin(); it != m_elements.end(); ++it)
			elements.emplace_back(it->second);

		return elements;
	}
#pragma endregion
}
