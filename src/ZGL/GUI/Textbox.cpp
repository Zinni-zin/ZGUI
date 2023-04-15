#include "Headers/Textbox.h"

#include <iostream>
#include <algorithm>

#include "../Core/Core.h"

#define HoverWidth 7
#define HoverHeight 23
#define TextCursorWidth 2
#define TextCursorHeight 23

#define HighlightHeight 38

#define MouseHoldDelayS 0.1f // Amount to determine we are highlighting with the mouse
#define DeleteDelayS 0.1f // Speed/delay to delete per press
#define DeleteHoldDelayS 0.025f // Speed/delay to delete after held
#define DeleteHeldDelayS 0.2f // The amount of time to determine the backspace/delete key is held
#define CursorMoveDelayS 0.1f //Speed/delay to move the cursor per press
#define CursorMoveHeldDelayS 0.05f // Speed/delay to move the cursor after held
#define CursorDelayS 0.2f // The amount of time to determine one of the move keys is held
#define PasteDelay 0.25f // Speed/delay with pasting text

#define BlinkDelay 0.60f // Speed/delay the text cursor blinks

// Initial Speed/delay the text will be highlight when the mouse cursor is out of the text box
#define HighlightOutDelay 0.60f 

#define ScissorRectY(y, h) (Core::GetCore().GetHeight() - y) - h
#define ScissorRectPos(x, y, h) x, ScissorRectY(y, h)
#define ScissorRect(x, y, w, h) ScissorRectPos(x, y, h), w, h

namespace ZGL
{
	Timer Textbox::s_timer;
	Timer Textbox::s_delTimer;
	Timer Textbox::s_delHeldTimer;
	Timer Textbox::s_moveTimer;
	Timer Textbox::s_moveHeldTimer;
	Timer Textbox::s_pasteTimer;
	Timer Textbox::s_cursorBlinkTimer;
	Timer Textbox::s_outHighlightTimer;

#pragma region Constructors and Destructor

	Textbox::Textbox(Widget& widget, Font& font,
		Vec2 pos, Vec2ui dimensions, Vec4 colour, Vec4 textColour, bool addToWidget)
		: ZGUI(pos + widget.GetPos(), dimensions), m_widgetOffset(pos), m_font(font),
		m_text(std::make_unique<Text>(font, m_pos, textColour)),
		m_widget(widget), m_colour(colour),
		m_rect(std::make_unique<RectangleI>(m_pos, Vec2i(dimensions.x, dimensions.y), colour)),
		m_hoverCursor(std::make_unique<Sprite>(0.f, 0.f, HoverWidth, HoverHeight)),
		m_textCursor(std::make_unique<Sprite>(0.f, 0.f, TextCursorWidth, TextCursorHeight)),
		m_highlightRect(std::make_unique<RectangleI>(Vec2(0.f), Vec2i(0, HighlightHeight), Vec4(0.258f, 0.749f, 0.96f, .5f))),
		m_input(Input::GetInput()),
		m_scissorRect(std::make_unique<RectangleI>(ScissorRect(m_pos.x, m_pos.y, dimensions.x, dimensions.y)))
	{
		if(addToWidget) widget.AddElement(this);

		m_onTextInputEvent = [&](Event& e)
		{ onTextInputEvent(*((TextInputEvent*)&e)); };

		Core::GetCore().GetGLFW_Dispatcher().Attach(m_onTextInputEvent, EventType::TEXT_INPUT);
	}

	Textbox::Textbox(Widget& widget, Font& font,
		Vec2 pos, Vec2ui dimensions, int drawOrder, Vec4 colour, Vec4 textColour)
		: ZGUI(pos + widget.GetPos(), dimensions), m_widgetOffset(pos), m_font(font),
		m_text(std::make_unique<Text>(font, m_pos, textColour)),
		m_widget(widget), m_colour(colour),
		m_rect(std::make_unique<RectangleI>(m_pos, Vec2i(dimensions.x, dimensions.y), colour)),
		m_hoverCursor(std::make_unique<Sprite>(0.f, 0.f, HoverWidth, HoverHeight)),
		m_textCursor(std::make_unique<Sprite>(0.f, 0.f, TextCursorWidth, TextCursorHeight)),
		m_highlightRect(std::make_unique<RectangleI>(Vec2(0.f), Vec2i(0, HighlightHeight), Vec4(0.258f, 0.749f, 0.96f, .5f))),
		m_input(Input::GetInput()),
		m_scissorRect(std::make_unique<RectangleI>(ScissorRect(m_pos.x, m_pos.y, dimensions.x, dimensions.y)))
	{
		widget.AddElement(drawOrder, this);

		m_onTextInputEvent = [&](Event& e)
		{ onTextInputEvent(*((TextInputEvent*)&e)); };

		Core::GetCore().GetGLFW_Dispatcher().Attach(m_onTextInputEvent,
			EventType::TEXT_INPUT);
	}

	Textbox::Textbox(Widget& widget, Font& font, const std::string& defaultText,
		Vec2 pos, Vec2ui dimensions, Vec4 colour, Vec4 textColour)
		: ZGUI(pos + widget.GetPos(), dimensions), m_widgetOffset(pos), m_font(font),
		m_text(std::make_unique<Text>(defaultText, font, m_pos.x, m_pos.y, textColour)),
		m_widget(widget), m_colour(colour),
		m_rect(std::make_unique<RectangleI>(m_pos, Vec2i(dimensions.x, dimensions.y), colour)),
		m_hoverCursor(std::make_unique<Sprite>(0.f, 0.f, HoverWidth, HoverHeight)),
		m_textCursor(std::make_unique<Sprite>(0.f, 0.f, TextCursorWidth, TextCursorHeight)),
		m_highlightRect(std::make_unique<RectangleI>(Vec2(0.f), Vec2i(0, HighlightHeight), Vec4(0.258f, 0.749f, 0.96f, .5f))),
		m_input(Input::GetInput()),
		m_scissorRect(std::make_unique<RectangleI>(ScissorRect(m_pos.x, m_pos.y, dimensions.x, dimensions.y)))
	{
		widget.AddElement(this);

		m_onTextInputEvent = [&](Event& e)
		{ onTextInputEvent(*((TextInputEvent*)&e)); };

		Core::GetCore().GetGLFW_Dispatcher().Attach(m_onTextInputEvent,
			EventType::TEXT_INPUT);
	}

	Textbox::Textbox(Widget& widget, Font& font, const std::string& defaultText,
		Vec2 pos, Vec2ui dimensions, int drawOrder, Vec4 colour, Vec4 textColour)
		: ZGUI(pos + widget.GetPos(), dimensions), m_widgetOffset(pos), m_font(font),
		m_text(std::make_unique<Text>(defaultText, font, m_pos.x, m_pos.y)),
		m_widget(widget), m_colour(colour),
		m_rect(std::make_unique<RectangleI>(m_pos, Vec2i(dimensions.x, dimensions.y), colour)),
		m_hoverCursor(std::make_unique<Sprite>(0.f, 0.f, HoverWidth, HoverHeight)),
		m_textCursor(std::make_unique<Sprite>(0.f, 0.f, TextCursorWidth, TextCursorHeight)),
		m_highlightRect(std::make_unique<RectangleI>(Vec2(0.f), Vec2i(0, HighlightHeight), Vec4(0.258f, 0.749f, 0.96f, .5f))),
		m_input(Input::GetInput()),
		m_scissorRect(std::make_unique<RectangleI>(ScissorRect(m_pos.x, m_pos.y, dimensions.x, dimensions.y)))
	{
		widget.AddElement(drawOrder, this);

		m_onTextInputEvent = [&](Event& e)
		{ onTextInputEvent(*((TextInputEvent*)&e)); };

		Core::GetCore().GetGLFW_Dispatcher().Attach(m_onTextInputEvent,
			EventType::TEXT_INPUT);
	}

	Textbox::~Textbox()
	{
		m_widget.RemoveElement(this);

		Core::GetCore().GetGLFW_Dispatcher().Detach(m_onTextInputEvent,
			EventType::TEXT_INPUT);
	}

#pragma endregion

	void Textbox::Update()
	{
		if (!m_isFirstClick && m_input.IsMouseButtonReleased(Mouse::LEFT_BUTTON))
			m_isFirstClick = true;
		else if (m_input.IsMouseButtonReleased(Mouse::LEFT_BUTTON))
			if (m_isHeld) 
				m_isHeld = false;

		if (m_isActive)
		{
			DeleteCharacters();
			MoveTextCursor();
			Shortcuts();
			MoveText();

			if (m_text->GetText().empty())
				m_isHighlighted = false;

			if (s_cursorBlinkTimer.AsSeconds(s_cursorBlinkTimer.GetElapsedTime()) > BlinkDelay)
			{
				Vec4 colour = m_textCursor->GetColour();

				if (m_isBlinking && colour.w > 0.f) 
					colour.w = 0.f;
				else
					colour.w = 1.f;

				m_textCursor->SetColour(colour);
				s_cursorBlinkTimer.Restart();

				m_isBlinking = true;
			}

			if (!m_isHighlighted)
				m_isHighlightedAll = false;
		}

		if (!m_isActive && m_isHeld) 
			m_isHeld = false;

		if (m_widget.IsResizing()) 
			SetupScissorRect();
		if (m_pos.x + m_dimensions.x >= m_widget.GetRect().GetRight())
		{
			m_scissorRect->SetWidth(m_widget.GetScissorRect().GetWidth() - (int)m_widgetOffset.x);
			m_rect->SetWidth(m_scissorRect->GetWidth());
		}

		if (m_onTextChanged && m_isSetOnEnter && m_input.IsKeyPressed(Key::ENTER))
			m_onTextChanged();
	}

	void Textbox::Update(const RectangleI& mouseRect)
	{
		if (mouseRect.HasIntersected(m_rect.get()))
		{
			m_isHover = true;

			if (m_input.IsMouseButtonPressed(Mouse::LEFT_BUTTON))
			{
				if (m_isFirstClick)
				{
					s_timer.Restart();
					m_isFirstClick = false;
					m_isHighlighted = false;
				}

				// Check if how long the mouse has been held for
				if (s_timer.AsSeconds(s_timer.GetElapsedTime()) > MouseHoldDelayS)
				{
					if (!m_isHeld) 
						m_iStartHighlight = m_text->GetIndex(mouseRect.GetPos().x);
					else 
						m_iEndHighlight = m_text->GetIndex(mouseRect.GetPos().x);

					if (m_isHeld)
						CalcHighlightRec();

					m_isHeld = true;
				}
			}

			// Calculate non highlgiht position 
			if (!m_isHeld && m_input.IsMouseButtonPressed(Mouse::LEFT_BUTTON))
			{
				m_isActive = true;

				float centeredY = m_pos.y + (m_dimensions.y / 2) - (m_textCursor->GetHeight() / 2);
				m_textCursor->SetPos(m_text->GetPos().x, centeredY);

				if (!m_text->GetText().empty())
				{
					m_textIndex = m_text->GetIndex(mouseRect.GetPos().x);
					Vec2 pos = Vec2(m_text->GetPos().x + m_text->GetWidth(m_textIndex), centeredY);

					m_textCursor->SetPos(pos);
					m_highlightRect->SetPos(pos);
				}
			}

			if (m_hoverCursor != nullptr)
			{
				m_hoverCursor->SetPos(mouseRect.GetPos());

				if (Core::GetCore().IsShowingCursor()) 
					Core::GetCore().ShowCursor(false);
			}
		}
		else
		{ // We are no longer within the bounds of the text box
			m_isHover = false;

			if (m_input.IsMouseButtonPressed(Mouse::LEFT_BUTTON) && !m_isHeld)
			{
				m_isActive = false;
				if (m_onTextChanged && m_isSetOnEnter)
					m_onTextChanged();
			}

			if (!Core::GetCore().IsShowingCursor()) 
				Core::GetCore().ShowCursor(true);

			if (m_isActive && m_isHeld) 
				HighlightOutsideBox(mouseRect.GetPos().x);
		}
	}

	void Textbox::onTextInputEvent(TextInputEvent& e)
	{
		if (m_isActive)
		{
			if (m_isHighlighted && IsAddable((char)e.GetCodepoint()))
			{
				int startIndex = m_iStartHighlight;
				int endIndex = m_iEndHighlight;

				// Make sure to swap start and end indexes if they are at different positions
				if (m_iStartHighlight > m_iEndHighlight) 
					std::swap(startIndex, endIndex);

				std::string text = m_text->GetText();

				text.erase(startIndex, endIndex - startIndex);

				text.insert(startIndex, 1, (char)e.GetCodepoint());
				m_text->SetText(text);
				m_textIndex = startIndex + 1;
				
				if (m_onTextChanged && !m_isSetOnEnter)
					m_onTextChanged();

				m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());

				m_isHighlighted = false;
				m_isBlinking = false;
			}
			else if ((size_t)m_textIndex < m_text->GetText().size())
			{
				// Only add the text if it is addable and if we are within our size limit
				if ((m_maxChars <= 0 || m_text->GetText().size() < (size_t)m_maxChars) && IsAddable((char)e.GetCodepoint()))
				{
					std::string text = m_text->GetText();
					text.insert(m_textIndex, 1, (char)e.GetCodepoint());
					m_text->SetText(text);
					++m_textIndex;
					
					if (m_onTextChanged && !m_isSetOnEnter) 
						m_onTextChanged();

					m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());

					m_isBlinking = false;
				}
			}
			else
			{
				if ((m_maxChars <= 0 || m_text->GetText().size() < (size_t)m_maxChars) && IsAddable((char)e.GetCodepoint()))
				{
					*m_text += (char)e.GetCodepoint();
					++m_textIndex;
					if (m_onTextChanged && !m_isSetOnEnter) m_onTextChanged();

					m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());
					m_isBlinking = false;
				}
			}

			if (m_autoCalcTextY)
				SetTextOffsetY((m_dimensions.y / 2) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));
		}
	}

	void Textbox::Draw(Renderer& renderer)
	{
		glScissor((GLint)m_scissorRect->GetX(), (GLint)m_scissorRect->GetY(),
			m_scissorRect->GetWidth(), m_scissorRect->GetHeight());

		if (m_rect != nullptr && m_rect->GetFillTexture() != nullptr)
			renderer.DrawShapeFill(m_rect.get(), m_mvp);

		if (m_text != nullptr && !m_text->GetText().empty())
			renderer.DrawText(*m_text, m_mvp);

		if (m_isActive && m_isHighlighted && m_highlightRect != nullptr && m_highlightRect->GetFillTexture() != nullptr)
			renderer.DrawShapeFill(m_highlightRect.get(), m_mvp);

		if (!m_isHighlighted && m_isActive && m_textCursor != nullptr && m_textCursor->HasTexture())
			renderer.Draw(*m_textCursor, m_mvp);

		if (m_rect != nullptr && m_rect->GetOutlineTexture() != nullptr)
			renderer.DrawShapeOutline(m_rect.get(), m_mvp);

		glScissor((GLint)m_widget.GetScissorRect().GetX(), (GLint)m_widget.GetScissorRect().GetY(),
			m_widget.GetScissorRect().GetWidth(), m_widget.GetScissorRect().GetHeight());

		if (m_isHover && m_hoverCursor != nullptr && m_hoverCursor->HasTexture())
			renderer.Draw(*m_hoverCursor, m_mvp);
	}

#pragma region Key Input

	void Textbox::DeleteCharacters()
	{
		bool comboPressed = (m_input.IsKeyPressed(Key::LEFT_CONTROL) || m_input.IsKeyPressed(Key::LEFT_SHIFT));

		// Combo backspace (ctrl + backspace | shift + backspace)
		bool comboPressedBS = comboPressed && m_input.IsKeyPressed(Key::BACKSPACE);

		// Combo delete (ctrl + delete | shift + delete)
		bool comboPressedDel = comboPressed && m_input.IsKeyPressed(Key::DELETE);

#pragma region Delete Backwards (Backspace key)

		// Delete by word
		if (comboPressed && m_input.IsKeyPressed(Key::BACKSPACE))
		{
			std::string text = m_text->GetText();
			if (!text.empty() && m_textIndex > 0)
			{
				// If we are holding delete it faster otherwise delete slower
				float delay = (m_delIsHeld) ? DeleteHoldDelayS : DeleteDelayS;

				if (s_delTimer.AsSeconds(s_delTimer.GetElapsedTime()) > delay)
				{
					// Figure out where the space is
					int spaceIndex = 0;
					for (int i = m_textIndex; i > 0; --i)
					{
						if (text[i] == ' ')
						{
							spaceIndex = i;
							break;
						}
					}

					// Delete up to the space
					text.erase(spaceIndex, m_textIndex);
					m_text->SetText(text);

					if (m_onTextChanged && !m_isSetOnEnter)
						m_onTextChanged();

					// Set the cursor to where the space was
					m_textIndex = spaceIndex;
					m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());

					m_isDeleting = true;

					if (s_delHeldTimer.AsSeconds(s_delHeldTimer.GetElapsedTime()) > DeleteHeldDelayS)
						m_delIsHeld = true;

					s_delTimer.Restart();
				}
			}
		} // Backspace individual characters
		else if (!m_isHighlighted && m_input.IsKeyPressed(Key::BACKSPACE))
		{
			std::string text = m_text->GetText();
			if (!text.empty() && m_textIndex > 0)
			{
				float delay = (m_delIsHeld) ? DeleteHoldDelayS : DeleteDelayS;

				if (s_delTimer.AsSeconds(s_delTimer.GetElapsedTime()) > delay)
				{
					// Erase the character before the text cursor
					text.erase(m_textIndex - 1, 1);
					m_text->SetText(text);

					if (m_onTextChanged && !m_isSetOnEnter) 
						m_onTextChanged();

					// Move the text cursor down
					--m_textIndex;
					m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());

					m_isDeleting = true;

					if (s_delHeldTimer.AsSeconds(s_delHeldTimer.GetElapsedTime()) > DeleteHeldDelayS)
						m_delIsHeld = true;

					s_delTimer.Restart();
				}
			}
		}

#pragma endregion

#pragma region Delete Foward (Delete Key)

		if (comboPressed && m_input.IsKeyPressed(Key::DELETE)) // Delete by word
		{
			std::string text = m_text->GetText();
			if (!text.empty() && (size_t)m_textIndex < m_text->GetText().size())
			{
				float delay = (m_delIsHeld) ? DeleteHoldDelayS : DeleteDelayS;

				if (s_delTimer.AsSeconds(s_delTimer.GetElapsedTime()) > delay)
				{
					// Figure out where the space is
					int spaceIndex = text.size() - 1;
					for (int i = m_textIndex; (size_t)i < text.size(); ++i)
					{
						if (m_text->GetText()[i] == ' ' && m_textIndex == i) continue;

						if (text[i] == ' ')
						{
							spaceIndex = i + 1;
							break;
						}
					}

					// Erase the word and space
					text.erase(m_textIndex, spaceIndex);
					m_text->SetText(text);

					if (m_onTextChanged && !m_isSetOnEnter) 
						m_onTextChanged();

					m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());

					m_isDeleting = true;

					// Check if delete key is held
					if (s_delHeldTimer.AsSeconds(s_delHeldTimer.GetElapsedTime()) > DeleteHeldDelayS)
						m_delIsHeld = true;

					s_delTimer.Restart();
				}
			}
		}
		else if (!m_isHighlighted && m_input.IsKeyPressed(Key::DELETE)) // Delete individual characters
		{
			std::string text = m_text->GetText();
			if (!text.empty() && (size_t)m_textIndex < m_text->GetText().size())
			{
				// If we are holding delete it faster otherwise delete slower
				float delay = (m_delIsHeld) ? DeleteHoldDelayS : DeleteDelayS;

				if (s_delTimer.AsSeconds(s_delTimer.GetElapsedTime()) > delay)
				{
					text.erase(m_textIndex, 1);
					m_text->SetText(text);
					
					if (m_onTextChanged && !m_isSetOnEnter)
						m_onTextChanged();
					
					m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());

					m_isDeleting = true;

					if (s_delHeldTimer.AsSeconds(s_delHeldTimer.GetElapsedTime()) > DeleteHeldDelayS)
						m_delIsHeld = true;

					s_delTimer.Restart();
				}
			}
		}
#pragma endregion

		// Delete highlighted area
		if (m_isHighlighted && (m_input.IsKeyPressed(Key::BACKSPACE) || m_input.IsKeyPressed(Key::DELETE)))
		{
			std::string text = m_text->GetText();
			if (!text.empty())
			{
				int startIndex = m_iStartHighlight;

				// Make sure to swap start and end indexes if they are at different positions
				if (m_iStartHighlight > m_iEndHighlight)
					std::swap(startIndex, m_iEndHighlight);

				// Erase
				text.erase(startIndex, m_iEndHighlight - startIndex);
				m_text->SetText(text);
				if (m_onTextChanged && !m_isSetOnEnter) 
					m_onTextChanged();

				if (text.empty())
				{
					m_textIndex = 0;
					m_textCursor->SetPos(m_text->GetPos().x, m_textCursor->GetY());
				}
				else
					m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());

				m_isHighlighted = false;
				s_delTimer.Restart();
			}
		}

		if (m_input.IsKeyReleased(Key::BACKSPACE) && m_input.IsKeyReleased(Key::DELETE))
		{
			m_delIsHeld = false;
			m_isDeleting = false;
			s_delHeldTimer.Restart();
		}
	}

	void Textbox::MoveTextCursor()
	{
		bool wordMovePressed = (m_input.IsKeyPressed(Key::LEFT_CONTROL) && m_input.IsKeyReleased(Key::LEFT_SHIFT));
		bool highlightPressed = (m_input.IsKeyReleased(Key::LEFT_CONTROL) && m_input.IsKeyPressed(Key::LEFT_SHIFT));
		bool highlightWordPressed = (m_input.IsKeyPressed(Key::LEFT_CONTROL) && m_input.IsKeyPressed(Key::LEFT_SHIFT));

#pragma region Move Right

		if (wordMovePressed && !highlightPressed && !highlightWordPressed && m_input.GetInput().IsKeyPressed(Key::RIGHT))
		{
			float delay = (m_isMoveHeld) ? CursorMoveHeldDelayS : CursorMoveDelayS;

			if (s_moveTimer.AsSeconds(s_moveTimer.GetElapsedTime()) > delay)
			{
				// If we are highlighted we want to put the text cursor at the beginning
				if (m_isHighlighted)
				{
					m_isHighlighted = false;
					int startIndex = m_iStartHighlight;
					int endIndex = m_iEndHighlight;

					// Make sure to swap start and end indexes if they are at different positions
					if (m_iStartHighlight > m_iEndHighlight) 
						std::swap(startIndex, endIndex);

					m_textIndex = endIndex;

					m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());

					if (s_moveHeldTimer.AsSeconds(s_moveHeldTimer.GetElapsedTime()) > CursorDelayS)
						m_isMoveHeld = true;

					s_moveTimer.Restart();
				} // Otherwise make sure we're in bounds and move the cursor
				else if ((size_t)m_textIndex < m_text->GetText().size())
				{
					// Figure out where the space is
					int spaceIndex = m_text->GetText().size();
					for (int i = m_textIndex; (size_t)i < m_text->GetText().size(); ++i)
					{
						if (m_text->GetText()[i] == ' ')
						{
							spaceIndex = i + 1;
							break;
						}
					}

					m_textIndex = spaceIndex;
					m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());

					if (s_moveHeldTimer.AsSeconds(s_moveHeldTimer.GetElapsedTime()) > CursorDelayS)
						m_isMoveHeld = true;

					s_moveTimer.Restart();
				}
			}
		}
		else if (!highlightPressed && !highlightWordPressed && m_input.GetInput().IsKeyPressed(Key::RIGHT))
		{
			float delay = (m_isMoveHeld) ? CursorMoveHeldDelayS : CursorMoveDelayS;

			if (s_moveTimer.AsSeconds(s_moveTimer.GetElapsedTime()) > delay)
			{
				// If we are highlighted we want to put the text cursor at the beginning
				if (m_isHighlighted)
				{
					m_isHighlighted = false;
					int startIndex = m_iStartHighlight;
					int endIndex = m_iEndHighlight;

					// Make sure to swap start and end indexes if they are at different positions
					if (m_iStartHighlight > m_iEndHighlight) 
						std::swap(startIndex, endIndex);

					m_textIndex = endIndex;

					m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());

					if (s_moveHeldTimer.AsSeconds(s_moveHeldTimer.GetElapsedTime()) > CursorDelayS)
						m_isMoveHeld = true;

					s_moveTimer.Restart();
				} // Otherwise make sure we're in bounds and move the cursor
				else if ((size_t)m_textIndex < m_text->GetText().size())
				{
					++m_textIndex;

					m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());

					if (s_moveHeldTimer.AsSeconds(s_moveHeldTimer.GetElapsedTime()) > CursorDelayS)
						m_isMoveHeld = true;

					s_moveTimer.Restart();
				}
			}
		}
#pragma endregion

#pragma region Move Left

		if (wordMovePressed && !highlightPressed && !highlightWordPressed && m_input.GetInput().IsKeyPressed(Key::LEFT))
		{
			float delay = (m_isMoveHeld) ? CursorMoveHeldDelayS : CursorMoveDelayS;

			if (s_moveTimer.AsSeconds(s_moveTimer.GetElapsedTime()) > delay)
			{
				// If we are highlighted we want to put the text cursor at the beginning
				if (m_isHighlighted)
				{
					m_isHighlighted = false;

					int startIndex = m_iStartHighlight;
					int endIndex = m_iEndHighlight;

					// Make sure to swap start and end indexes if they are at different positions
					if (m_iStartHighlight > m_iEndHighlight)
						std::swap(startIndex, endIndex);

					m_textIndex = startIndex;

					m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());

					if (s_moveHeldTimer.AsSeconds(s_moveHeldTimer.GetElapsedTime()) > CursorDelayS)
						m_isMoveHeld = true;

					s_moveTimer.Restart();
				}
				else if (m_textIndex > 0)
				{
					// Figure out where the space is
					int spaceIndex = 0;
					for (int i = m_textIndex; i > 0; --i)
					{
						if (m_text->GetText()[i] == ' ' && m_textIndex == i) continue;

						if (m_text->GetText()[i] == ' ')
						{
							spaceIndex = i;
							break;
						}
					}

					m_textIndex = spaceIndex;
					m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());

					if (s_moveHeldTimer.AsSeconds(s_moveHeldTimer.GetElapsedTime()) > CursorDelayS)
						m_isMoveHeld = true;

					s_moveTimer.Restart();
				}
			}
		}
		else if (!highlightPressed && !highlightWordPressed && m_input.GetInput().IsKeyPressed(Key::LEFT))
		{
			float delay = (m_isMoveHeld) ? CursorMoveHeldDelayS : CursorMoveDelayS;

			if (s_moveTimer.AsSeconds(s_moveTimer.GetElapsedTime()) > delay)
			{
				// If we are highlighted we want to put the text cursor at the beginning
				if (m_isHighlighted)
				{
					m_isHighlighted = false;

					int startIndex = m_iStartHighlight;
					int endIndex = m_iEndHighlight;

					// Make sure to swap start and end indexes if they are at different positions
					if (m_iStartHighlight > m_iEndHighlight)
						std::swap(startIndex, endIndex);

					m_textIndex = startIndex;

					m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());

					if (s_moveHeldTimer.AsSeconds(s_moveHeldTimer.GetElapsedTime()) > CursorDelayS)
						m_isMoveHeld = true;

					s_moveTimer.Restart();
				} // Otherwise make sure we're in bounds and move the cursor
				else if (m_textIndex > 0)
				{
					--m_textIndex;

					m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());

					// Check if left key is held
					if (s_moveHeldTimer.AsSeconds(s_moveHeldTimer.GetElapsedTime()) > CursorDelayS)
						m_isMoveHeld = true;

					s_moveTimer.Restart();
				}
			}
		}
#pragma endregion

#pragma region Highlight Right
		if (!wordMovePressed && !highlightPressed && highlightWordPressed && m_input.GetInput().IsKeyPressed(Key::RIGHT))
		{
			float delay = (m_isMoveHeld) ? CursorMoveHeldDelayS : CursorMoveDelayS;

			if (s_moveTimer.AsSeconds(s_moveTimer.GetElapsedTime()) > delay)
			{
				if ((size_t)m_textIndex < m_text->GetText().size() && !m_text->GetText().empty())
				{
					// Figure out where the space is
					int spaceIndex = m_text->GetText().size();
					for (int i = m_textIndex; (size_t)i < m_text->GetText().size(); ++i)
					{
						if (m_text->GetText()[i] == ' ')
						{
							spaceIndex = i + 1;
							break;
						}
					}

					if (!m_isHighlighted)
					{
						m_isHighlighted = true;
						m_iStartHighlight = m_textIndex;
					}

					m_iEndHighlight = spaceIndex;
					m_textIndex = spaceIndex;

					CalcHighlightRec();

					if (s_moveHeldTimer.AsSeconds(s_moveHeldTimer.GetElapsedTime()) > CursorDelayS)
						m_isMoveHeld = true;

					s_moveTimer.Restart();
				}
			}
		}
		else if (!wordMovePressed && highlightPressed && !highlightWordPressed && m_input.GetInput().IsKeyPressed(Key::RIGHT))
		{
			float delay = (m_isMoveHeld) ? CursorMoveHeldDelayS : CursorMoveDelayS;

			if (s_moveTimer.AsSeconds(s_moveTimer.GetElapsedTime()) > delay)
			{
				if ((size_t)m_textIndex < m_text->GetText().size() && !m_text->GetText().empty())
				{
					if (!m_isHighlighted)
					{
						m_isHighlighted = true;
						m_iStartHighlight = m_textIndex;
						m_iEndHighlight = m_textIndex;
					}

					++m_iEndHighlight;
					m_textIndex = m_iEndHighlight;

					CalcHighlightRec();

					if (s_moveHeldTimer.AsSeconds(s_moveHeldTimer.GetElapsedTime()) > CursorDelayS)
						m_isMoveHeld = true;

					s_moveTimer.Restart();
				}
			}
		}
#pragma endregion

#pragma region Highlight Left
		if (!wordMovePressed && !highlightPressed && highlightWordPressed && m_input.GetInput().IsKeyPressed(Key::LEFT))
		{
			float delay = (m_isMoveHeld) ? CursorMoveHeldDelayS : CursorMoveDelayS;

			if (s_moveTimer.AsSeconds(s_moveTimer.GetElapsedTime()) > delay)
			{
				if (m_textIndex > 0 && !m_text->GetText().empty())
				{
					// Figure out where the space is
					int spaceIndex = 0;
					for (int i = m_textIndex; i > 0; --i)
					{
						if (m_text->GetText()[i] == ' ' && m_textIndex == i) continue;

						if (m_text->GetText()[i] == ' ')
						{
							spaceIndex = i;
							break;
						}
					}

					if (!m_isHighlighted)
					{
						m_isHighlighted = true;
						m_iStartHighlight = m_textIndex;
					}

					m_iEndHighlight = spaceIndex;
					m_textIndex = spaceIndex;

					CalcHighlightRec();

					if (s_moveHeldTimer.AsSeconds(s_moveHeldTimer.GetElapsedTime()) > CursorDelayS)
						m_isMoveHeld = true;

					s_moveTimer.Restart();
				}
			}
		}
		else if (!wordMovePressed && highlightPressed && !highlightWordPressed && m_input.GetInput().IsKeyPressed(Key::LEFT))
		{
			float delay = (m_isMoveHeld) ? CursorMoveHeldDelayS : CursorMoveDelayS;

			if (s_moveTimer.AsSeconds(s_moveTimer.GetElapsedTime()) > delay)
			{
				if (m_textIndex > 0 && !m_text->GetText().empty())
				{
					if (!m_isHighlighted)
					{
						m_isHighlighted = true;
						m_iStartHighlight = m_textIndex;
						m_iEndHighlight = m_textIndex;
					}

					--m_iEndHighlight;
					m_textIndex = m_iEndHighlight;

					CalcHighlightRec();

					if (s_moveHeldTimer.AsSeconds(s_moveHeldTimer.GetElapsedTime()) > CursorDelayS)
						m_isMoveHeld = true;

					s_moveTimer.Restart();
				}
			}
		}
#pragma endregion

		if (m_input.IsKeyReleased(Key::RIGHT) && m_input.IsKeyReleased(Key::LEFT))
		{
			m_isMoveHeld = false;
			s_moveHeldTimer.Restart();
		}
	}

	void Textbox::Shortcuts()
	{
		bool ctrlPressed = m_input.IsKeyPressed(Key::LEFT_CONTROL);

		/* --- Copy --- */
		if (m_isHighlighted && ctrlPressed && m_input.IsKeyPressed(Key::C))
		{
			int startIndex = m_iStartHighlight;

			// Make sure to swap start and end indexes if they are at different positions
			if (m_iStartHighlight > m_iEndHighlight)
				std::swap(startIndex, m_iEndHighlight);

			glfwSetClipboardString(Core::GetCore().GetWindow().GetGLFWindow(),
				m_text->GetText().substr(startIndex, m_iEndHighlight - startIndex).c_str());
		}

		/* --- Paste --- */
		if (m_isHighlighted && ctrlPressed && m_input.IsKeyPressed(Key::V))
		{
			int startIndex = m_iStartHighlight;
			int endIndex = m_iEndHighlight;

			// Make sure to swap start and end indexes if they are at different positions
			if (m_iStartHighlight > m_iEndHighlight)
				std::swap(startIndex, endIndex);

			std::string clipStr = glfwGetClipboardString(Core::GetCore().GetWindow().GetGLFWindow());
			std::string text = m_text->GetText();

			std::string erased = text;
			erased.erase(startIndex, endIndex - startIndex);

			// Figure out if the text is addable and only paste into the range of our size limit
			std::string insertStr;
			if ((m_maxChars > 0 && ((text.size() + erased.size()) >= (size_t)m_maxChars)) ||
				(m_isOnlyLetters || m_isOnlyNums || !m_isUsingSpace || !m_isUsingSpecialChars))
			{
				for (char c : clipStr)
				{
					if (IsAddable(c))
					{
						insertStr += c;
						if (m_maxChars > 0 && (insertStr.size() + erased.size() >= (size_t)m_maxChars))
							break;
					}
				}
			}
			else 
				insertStr = clipStr;

			// Replace the text
			text.erase(startIndex, endIndex - startIndex);
			m_textIndex = startIndex;
			text.insert(m_textIndex, insertStr);
			m_textIndex = endIndex;

			m_text->SetText(text);

			if (m_onTextChanged && !m_isSetOnEnter)
				m_onTextChanged();

			m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());
			m_isHighlighted = false;
			s_pasteTimer.Restart();
		}
		else if (!m_isHighlighted && ctrlPressed && m_input.IsKeyPressed(Key::V))
		{
			if (s_pasteTimer.AsSeconds(s_pasteTimer.GetElapsedTime()) > PasteDelay)
			{
				std::string clipStr = glfwGetClipboardString(Core::GetCore().GetWindow().GetGLFWindow());
				std::string text = m_text->GetText();

				// Figure out if the text is addable and only paste into the range of our size limit
				std::string insertStr;
				if ((m_maxChars > 0 && ((text.size() + clipStr.size()) >= (size_t)m_maxChars)) ||
					(m_isOnlyLetters || m_isOnlyNums || !m_isUsingSpace || !m_isUsingSpecialChars))
				{
					for (char c : clipStr)
					{
						if (IsAddable(c))
						{
							insertStr += c;
							if (m_maxChars > 0 && (insertStr.size() + text.size() >= (size_t)m_maxChars))
								break;
						}
					}
				}
				else 
					insertStr = clipStr;


				if (!text.empty())
				{ // Make sure to only add the text if it is within our range
					if (m_maxChars <= 0 || (text.size() < (size_t)m_maxChars))
					{
						text.insert(m_textIndex, insertStr);
						if ((size_t)m_textIndex >= text.size()) *m_text += insertStr;
						m_textIndex = text.size();
					}
				}
				else
				{ // If string is empty set the string to the clipboard string
					text = insertStr;
					m_textIndex = insertStr.size();
				}

				m_text->SetText(text);

				if(m_onTextChanged && !m_isSetOnEnter) 
					m_onTextChanged();

				m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());
				m_isHighlighted = false;

				// If we are auto calculating text do that (mainly used when string is empty)
				if (m_autoCalcTextY)
					SetTextOffsetY((m_dimensions.y / 2) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));

				s_pasteTimer.Restart();
			}
		}

		/* --- Cut --- */
		if (m_isHighlighted && ctrlPressed && m_input.IsKeyPressed(Key::X))
		{
			int startIndex = m_iStartHighlight;
			int endIndex = m_iEndHighlight;

			// Make sure to swap start and end indexes if they are at different positions
			if (m_iStartHighlight > m_iEndHighlight) std::swap(startIndex, endIndex);

			std::string text = m_text->GetText();

			// Put the highlighted area into the clipboard
			glfwSetClipboardString(Core::GetCore().GetWindow().GetGLFWindow(),
				text.substr(startIndex, endIndex - startIndex).c_str());

			text.erase(startIndex, endIndex - startIndex);
			m_text->SetText(text);
			if (m_onTextChanged && !m_isSetOnEnter)
				m_onTextChanged();

			// If our text is going to be empty reset the position
			if (text.empty())
			{
				m_textIndex = 0;
				m_textCursor->SetPos(m_text->GetPos().x, m_textCursor->GetY());
			}
			else
				m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());


			m_isHighlighted = false;
			s_pasteTimer.Restart();
		}

		/* --- Highlight All --- */
		if (!m_isHighlighted && !m_text->GetText().empty() && ctrlPressed && m_input.IsKeyPressed(Key::A))
		{
			m_iStartHighlight = 0;
			m_iEndHighlight = m_text->GetText().size();
			m_isHighlighted = true;
			m_isHighlightedAll = true;
			CalcHighlightRec();
		}
	}

#pragma endregion	

	void Textbox::CalcHighlightRec()
	{
		if (m_iStartHighlight != m_iEndHighlight)
		{
			int startIndex = m_iStartHighlight;
			int endIndex = m_iEndHighlight;

			if (m_iStartHighlight > m_iEndHighlight) std::swap(startIndex, endIndex);

			// Setup the rectangle position and width

			float centeredY = m_pos.y + (m_dimensions.y / 2) - (m_highlightRect->GetHeight() / 2);
			Vec2 pos = Vec2(m_text->GetPos().x + m_text->GetWidth(startIndex), centeredY);

			m_highlightRect->SetPos(pos);
			m_highlightRect->SetWidth((int)(m_text->GetWidth(endIndex) - m_text->GetWidth(startIndex)));

			m_isHighlighted = true;
		}
		else 
			m_isHighlighted = false;
	}

	void Textbox::MoveText()
	{
		bool isHighLightRight = m_isHighlighted && !m_isHighlightedAll && m_iStartHighlight < m_iEndHighlight && (m_highlightRect->GetRight() > m_rect->GetRight());
		bool isHighlightLeft = m_isHighlighted && !m_isHighlightedAll && m_iStartHighlight > m_iEndHighlight && (m_highlightRect->GetPos().x < m_rect->GetPos().x);

		/* Scroll Right */
		if (isHighLightRight || m_textCursor->GetX() > m_rect->GetRight())
		{

			// Update the position to show the new charcters
			float width = m_text->GetWidth(m_textIndex) - m_text->GetWidth(m_textIndex - 1);
			float x = m_text->GetPos().x - width;
			m_text->SetPosX(x);

			if (m_isHighlighted)
				CalcHighlightRec();

			m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());
		}

		/* Scroll Left */
		if (isHighlightLeft || m_textCursor->GetX() < m_rect->GetPos().x)
		{
			float width = m_text->GetWidth(m_textIndex + 1) - m_text->GetWidth(m_textIndex);
			float x = (m_textIndex == 0) ? m_pos.x + m_textOffset.x : m_text->GetPos().x + width;
			m_text->SetPosX(x);

			if (m_isHighlighted) 
				CalcHighlightRec();

			m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());
		}

		/* Scroll when deleting */

		if (m_isDeleting && (m_text->GetPos().x + m_text->GetWidth(m_textIndex)) < m_rect->GetPos().x)
		{  // If we are going to be at 0 set the position to be the default
			if (m_textIndex - 1 <= 0)
			{
				float x = m_pos.x + m_textOffset.x;
				m_text->SetPosX(x);
				m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());
			}
			// If the text can fit in the box, change the position to be default
			else if (m_pos.x + m_textOffset.x + m_text->GetWidth(m_text->GetText().size()) <= m_rect->GetRight())
			{
				float x = m_pos.x + m_textOffset.x;
				m_text->SetPosX(x);
				m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());
			}
			// Otherwise update the position so the text is about 3.5 times the difference between two indexes
			else
			{
				float width = m_text->GetWidth(m_textIndex) - m_text->GetWidth(m_textIndex - 1);
				float x = m_text->GetPos().x + (width * 3.5f) + m_textOffset.x;
				m_text->SetPosX(x);
				m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());
			}
		}

		// If our text is empty make sure to reset it
		if (m_text->GetText().empty())
		{
			m_text->SetPosX(m_pos.x + m_textOffset.x);
			m_textCursor->SetPos(m_text->GetPos().x + m_text->GetWidth(m_textIndex), m_textCursor->GetY());
		}
	}

	void Textbox::HighlightOutsideBox(float mouseX)
	{
		// Figure out if we are to the right or left of the text box
		bool increase = (mouseX > m_rect->GetRight() && (size_t)m_iEndHighlight < m_text->GetText().size()) ? true : false;
		bool decrease = (mouseX < m_rect->GetLeft() && m_iEndHighlight > 0) ? true : false;

		// Figure out the distance between the mouseX and edge of the text box
		float distance = 1.f;
		if (increase) distance = mouseX - m_rect->GetRight();
		else if (decrease) distance = m_rect->GetLeft() - mouseX;

		// Figure out the delay based off of the distance
		m_outHighlightDelay = 100.f / (distance * 2.f);
		m_outHighlightDelay = std::clamp(m_outHighlightDelay, 0.05f, HighlightOutDelay);

		if (s_outHighlightTimer.AsSeconds(s_outHighlightTimer.GetElapsedTime()) > m_outHighlightDelay)
		{
			if (increase) 
				++m_iEndHighlight;
			else if (decrease) 
				--m_iEndHighlight;

			CalcHighlightRec();
			m_textIndex = m_iEndHighlight;

			s_outHighlightTimer.Restart();
		}
	}

#pragma region Character Limiting
	void Textbox::SetNumbersOnly(bool val)
	{
		if (!m_isOnlyLetters) 
			m_isOnlyNums = val;
		else 
			std::cout << "Error Setting Numbers Only In textbox: Can't have both only numbers or letters!\n";
	}

	void Textbox::SetNumCharacters(bool val) // Allows for '-' and '.'
	{
		if (!m_isOnlyLetters) 
			m_isUsingNumCharacters = val;
		else 
			std::cout << "Error Setting Number Characters In Textbox(" << this << "): Can't assign number chars if it is only letters!\n";
	}

	void Textbox::SetLettersOnly(bool val)
	{
		if (!m_isOnlyNums) 
			m_isOnlyLetters = val;
		else 
			std::cout << "Error Setting Letters Only In textbox: Can't have both only numbers or letters!\n";
	}

	bool Textbox::IsNumber(char c)
	{
		bool val = c >= 48 && c <= 57;

		if (m_isUsingNumCharacters)
		{
			if (c == '-' && !m_isUnsigned && m_textIndex == 0 && m_text->GetText()[0] != '-')
 				val = true;
			if (c == '.' &&  !m_isUsingInt && m_text->GetText().find('.') == std::string::npos)
				val = true;
		}

		return val;
	}

	bool Textbox::IsLetter(char c)
	{
		return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
	}

	bool Textbox::IsSpecialCharacter(char c)
	{
		return !(IsNumber(c) || IsLetter(c));
	}

	bool Textbox::IsAddable(char c)
	{
		if (m_isUsingSpace && c == 32) 
			return true;
		else if (!m_isUsingSpace && c == 32) 
			return false;

		if (m_isUsingSpecialChars && IsSpecialCharacter(c) && !m_isOnlyLetters && !m_isOnlyNums) 
			return true;
		else if (!m_isUsingSpecialChars && IsSpecialCharacter(c)) 
			return false;

		if (!m_isOnlyLetters && !m_isOnlyNums) 
			return true;

		if (m_isOnlyLetters && IsLetter(c)) 
			return true;
		else if (m_isOnlyNums && IsNumber(c)) 
			return true;
		else 
			return false;
	}
#pragma endregion

#pragma region Update Position/Dimensions
	void Textbox::SetPos(Vec2 pos)
	{
		m_pos = pos + m_widgetOffset;
		m_rect->SetPos(pos + m_widgetOffset);
		m_text->SetPos(pos + m_widgetOffset + m_textOffset);
		m_scissorRect->SetX(pos.x + m_widgetOffset.x);
		SetupScissorRect();
	}

	void Textbox::SetPos(float x, float y)
	{
		m_pos = Vec2(x + m_widgetOffset.x, y + m_widgetOffset.y);
		m_rect->SetPos(x + m_widgetOffset.x, y + m_widgetOffset.y);
		m_text->SetPos(x + m_widgetOffset.x + m_textOffset.x, y + m_widgetOffset.y + m_textOffset.y);
		m_scissorRect->SetX(x + m_widgetOffset.x);
		SetupScissorRect();
	}

	void Textbox::SetX(float x)
	{
		m_pos.x = x + m_widgetOffset.x;
		m_rect->SetPos(x + m_widgetOffset.x, m_pos.y);
		m_text->SetPosX(x + m_widgetOffset.x + m_textOffset.x);
		m_scissorRect->SetX(x + m_widgetOffset.x);
		SetupScissorRect();
	}

	void Textbox::SetY(float y)
	{
		m_pos.y = y + m_widgetOffset.y;
		m_rect->SetPos(m_pos.x, y + m_widgetOffset.y);
		m_text->SetPosY(y + m_widgetOffset.y + m_textOffset.y);
		SetupScissorRect();
	}

	void Textbox::SetTextOffset(Vec2 pos)
	{
		m_textOffset = pos;
		m_text->SetPos(m_pos + pos);
	}

	void Textbox::SetTextOffset(float x, float y)
	{
		m_textOffset = Vec2(x, y);
		m_text->SetPos(m_pos.x + x, m_pos.y + y);
	}

	void Textbox::SetTextOffsetX(float x)
	{
		m_textOffset.x = x;
		m_text->SetPosX(x + m_pos.x);
	}

	void Textbox::SetTextOffsetY(float y)
	{
		m_textOffset.y = y;
		m_text->SetPosY(y + m_pos.y);
	}

	void Textbox::SetDimensions(Vec2ui dimensions)
	{
		m_dimensions = dimensions;
		m_rect->SetDimensions(Vec2i(dimensions.x, dimensions.y));
		SetupScissorRect();
	}

	void Textbox::SetDimensions(unsigned int width, unsigned int height)
	{
		m_dimensions = Vec2ui(width, height);
		m_rect->SetDimensions(width, height);
		SetupScissorRect();
	}

	void Textbox::SetWidth(unsigned int width)
	{
		m_dimensions.x = width;
		m_rect->SetWidth(width);
		SetupScissorRect();
	}

	void Textbox::SetHeight(unsigned int height)
	{
		m_dimensions.y = height;
		m_rect->SetHeight(height);
		SetupScissorRect();
	}

	void Textbox::SetupScissorRect()
	{
		if (m_rect->GetBottom() > m_widget.GetRect().GetBottom() || m_rect->GetY() < m_widget.GetRect().GetY() || (m_rect->GetY() < m_widget.GetRect().GetY() + m_widget.GetDragRect().GetHeight() && m_widget.GetDragRect().GetFillTexture()))
		{
			m_scissorRect->SetY(m_widget.GetScissorRect().GetY());
			m_scissorRect->SetHeight(m_widget.GetScissorRect().GetHeight());
		}
		else
		{
			m_scissorRect->SetY(ScissorRectY(m_pos.y, m_dimensions.y));
			m_scissorRect->SetHeight(m_dimensions.y);
		}

		if ((m_pos.x + m_dimensions.x) >= m_widget.GetRect().GetRight())
		{
			m_scissorRect->SetWidth(m_widget.GetScissorRect().GetWidth() - (int)m_widgetOffset.x);
			m_rect->SetWidth(m_scissorRect->GetWidth());
		}
		else
		{
			m_scissorRect->SetWidth(m_dimensions.x);
			m_rect->SetWidth(m_dimensions.x);
		}
	}
#pragma endregion
}