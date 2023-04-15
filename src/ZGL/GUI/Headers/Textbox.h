#pragma once

#include "ZGUI.h"
#include "Widget.h"

#include "../../Core/Timer.h"
#include "../../Input/Input.h"

#include "../../Render/Headers/Text.h"
#include "../../Shapes/Headers/Rectangle.h"

#include "../../Events/Headers/TextInputEvent.h"


namespace ZGL
{
	class Textbox : public ZGUI
	{
	public:
#pragma region Constructors and Destructor

		Textbox(Widget& widget, Font& font, Vec2 pos, Vec2ui dimensions,
			Vec4 colour = Vec4(1.f), Vec4 textColour = Vec4(0.f, 0.f, 0.f, 1.f), bool addToWidget = true);

		Textbox(Widget& widget, Font& font, Vec2 pos, Vec2ui dimensions, int drawOrder,
			Vec4 colour = Vec4(1.f), Vec4 textColour = Vec4(0.f, 0.f, 0.f, 1.f));

		Textbox(Widget& widget, Font& font, const std::string& defaultText, Vec2 pos,
			Vec2ui dimensions, Vec4 colour = Vec4(1.f), Vec4 textColour = Vec4(0.f, 0.f, 0.f, 1.f));

		Textbox(Widget& widget, Font& font, const std::string& defaultText, Vec2 pos,
			Vec2ui dimensions, int drawOrder, Vec4 colour = Vec4(1.), Vec4 textColour = Vec4(0.f, 0.f, 0.f, 1.f));

		~Textbox();

#pragma endregion


		/* --- Getters --- */

		Vec2 GetTextSize(int SizeType) const { return m_text->GetSize(SizeType); }
		float GetTextWidth(int SizeType) const { return GetTextSize(SizeType).x; }
		float GetTextWidth() { return m_text->GetWidth(); }
		float GetTextHeight(int SizeType) const { return GetTextSize(SizeType).y; }

		Vec2ui GetRectDimensions() const { return Vec2ui(m_rect->GetDimensions().x, m_rect->GetDimensions().y); }
		unsigned int GetRectWidth() const { return m_rect->GetWidth(); }
		unsigned int GetRectHeight() const { return m_rect->GetHeight(); }

		Vec2ui GetScissorDimensions() const { return Vec2ui(m_scissorRect->GetDimensions().x, m_scissorRect->GetDimensions().y); }
		unsigned int GetScissorWidth() const { return m_scissorRect->GetWidth(); }
		unsigned int GetScissorHeight() const { return m_scissorRect->GetHeight(); }

		std::string GetText() const { return m_text->GetText(); }

		bool AutoCalcTextY() const { return m_autoCalcTextY; }
		bool IsHeld() const { return m_isHeld; }
		bool IsHover() const { return m_isHover; }

		// Get the max amount of characters allowed within the textbox
		int GetMaxChars() const { return m_maxChars; }

		bool IsUsingNumbersOnly() { return m_isOnlyNums;}
		bool IsUsingLettersOnly() { return m_isOnlyLetters;}
		bool IsUsingSpecialCharacter() { return m_isUsingSpecialChars; }
		bool IsUsingSpace() { return m_isUsingSpace; }

		/* --- Setters --- */

		void SetPos(Vec2 pos) override;
		void SetPos(float x, float y) override;
		void SetX(float x) override;
		void SetY(float y) override;

		void SetTextOffset(Vec2 pos);
		void SetTextOffset(float x, float y);
		void SetTextOffsetX(float x);
		void SetTextOffsetY(float y);

		void SetDimensions(Vec2ui dimensions) override;
		void SetDimensions(unsigned int width, unsigned int height) override;
		void SetWidth(unsigned int width) override;
		void SetHeight(unsigned int height) override;

		void SetColour(Vec4 colour) { m_rect->SetColour(colour); }
		void SetColour(float r, float g, float b, float a) { m_rect->SetColour(r, g, b, a); }

		void SetTextColour(Vec4 colour) { m_text->SetColour(colour); }
		void SetTextColour(float r, float g, float b, float a) { m_text->SetColour(r, g, b, a); }

		void SetText(const std::string& text) 
		{
			m_text->SetText(text); 	
			if (m_autoCalcTextY) SetTextOffsetY((m_dimensions.y / 2) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));
		}

		void SetTextSize(Vec2i size) { m_text->SetFontSize(size.x, size.y); }
		void SetTextScale(Vec2 scale) { m_text->SetScale(scale.x, scale.y); }

		void SetHighlightColour(Vec4 colour) { m_highlightRect->SetColour(colour); }
		void SetHighlightColour(float r, float g, float b, float a) { m_highlightRect->SetColour(r, g, b, a); }

		void SetFillTexture(std::shared_ptr<Texture2D> texture) { m_rect->SetFillTexture(texture); }
		void SetOutlineTexture(std::shared_ptr<Texture2D> texture) { m_rect->SetOutlineTexture(texture); }
		void SetRectTexture(std::shared_ptr<Texture2D> fill, std::shared_ptr<Texture2D> outline) { m_rect->SetSpriteTexture(fill, outline); }

		void SetMouseCursorTexture(std::shared_ptr<Texture2D> texture) { m_hoverCursor->SetTexture(texture); }
		void SetTextCursorTexture(std::shared_ptr<Texture2D> texture) { m_textCursor->SetTexture(texture); }

		void SetHighlightRectTexture(std::shared_ptr<Texture2D> texture) { m_highlightRect->SetFillTexture(texture); }

		void AutoCalcTextY(bool val) { m_autoCalcTextY = val; }
		void SetActive(bool active) 
		{ 
			m_isActive = active;
			if (m_onTextChanged && m_isSetOnEnter) m_onTextChanged();
		}

		void SetNumbersOnly(bool val);
		void SetNumCharacters(bool val); // Allows for '-' and '.'
		void SetLettersOnly(bool val);
		void SetSpecialCharacterUse(bool val) { m_isUsingSpecialChars = val; }
		void SetUsingSpace(bool val) { m_isUsingSpace = val; }
		void SetUsingInt(bool val) { m_isUsingInt = val; }
		void SetUsingUnsigned(bool val) { m_isUnsigned = val; }

		// If true when enter is pressed or clicked off the onTextChange function is called
		void SetOnChangeOnEnter(bool val) { m_isSetOnEnter = val; }

		void SetOnTextChangedEventFnc(const std::function<void()>& eventFnc) { m_onTextChanged = eventFnc; }

		// Sets the max amount of characters allowed in this textbox (-1 is infinite)
		void SetMaxChars(int val) { m_maxChars = val; }

		void Update() override;
		void Update(const RectangleI& mouseRect);
		void Draw(Renderer& renderer) override;
	
	private:
		Widget& m_widget;
		Font m_font;

		Vec2 m_widgetOffset;
		Vec2 m_textOffset;
		Vec4 m_colour;

		std::unique_ptr<RectangleI> m_rect;
		std::unique_ptr<RectangleI> m_highlightRect;
		std::unique_ptr<RectangleI> m_scissorRect;
		std::unique_ptr<Text> m_text;
		std::unique_ptr<Sprite> m_textCursor;
		std::unique_ptr<Sprite> m_hoverCursor;

		std::function<void()> m_onTextChanged;

		bool m_isActive = false;
		bool m_isHover = false;
		bool m_autoCalcTextY = true;
		bool m_isHighlighted = false;
		bool m_delIsHeld = false;
		bool m_isDeleting = false;
		bool m_isBlinking = true;

		// When enter is pressed or clicked off the onTextChange function is called
		bool m_isSetOnEnter = true; 

		bool m_isOnlyNums = false;
		bool m_isUsingNumCharacters = false;
		bool m_isOnlyLetters = false;
		bool m_isUsingSpecialChars = true;
		bool m_isUsingInt = false;
		bool m_isUnsigned = false;
		bool m_isUsingSpace = true;

		// Used to reset timer if it is the first click
		bool m_isFirstClick = true;
		bool m_isHeld = false;

		bool m_isMoveHeld = false;

		bool m_isHighlightedAll = false;

		int m_textIndex = 0;
		int m_iStartHighlight = 0; // i = index
		int m_iEndHighlight = 0; // i = index

		int m_maxChars = -1;

		float m_outHighlightDelay = 0.6f;

		void onTextInputEvent(TextInputEvent& e);

		std::function<void(Event&)> m_onTextInputEvent;

	private:
		void DeleteCharacters();
		void MoveTextCursor();
		void Shortcuts();
		void CalcHighlightRec();
		void MoveText();

		void HighlightOutsideBox(float mouseX);

		bool IsNumber(char c);
		bool IsLetter(char c);
		bool IsSpecialCharacter(char c);
		bool IsAddable(char c);

		void SetupScissorRect();
	private:
		static Timer s_timer;
		static Timer s_delTimer;
		static Timer s_delHeldTimer;
		static Timer s_moveTimer;
		static Timer s_moveHeldTimer;
		static Timer s_pasteTimer;
		static Timer s_cursorBlinkTimer;
		static Timer s_outHighlightTimer;

		Input& m_input;
	};
}