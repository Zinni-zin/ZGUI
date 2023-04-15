#pragma once

#include "ZGUI.h"
#include "../../Render/Headers/Renderer.h"
#include "../../Shapes/Headers/Rectangle.h"

namespace ZGL
{
	class Button;
	class VertScrollbar;

	class Widget : public ZGUI
	{
	public:
		Widget(float x, float y, unsigned int width, unsigned int height, Vec4 colour = Vec4(1.0f, 1.0f, 1.0f, 0.4f));
		Widget(float x, float y, Vec2ui dimensions, Vec4 colour = Vec4(1.0f, 1.0f, 1.0f, 0.4f));
		Widget(Vec2 pos, unsigned int width, unsigned int height, Vec4 colour = Vec4(1.0f, 1.0f, 1.0f, 0.4f));
		Widget(Vec2 pos, Vec2ui dimensions, Vec4 colour = Vec4(1.0f, 1.0f, 1.0f, 0.4f));

		void Update() override;
		void Draw(Renderer& renderer) override;

		/* Getters */

		bool GetActive() const { return m_isActive; }

		bool IsResizing() const { return m_isResizing; }

		RectangleI& GetRect() const { return *m_rect; }
		RectangleI& GetDragRect() const { return *m_dragRect; }
		RectangleI& GetScissorRect() const { return *m_scissorRect; }
		RectangleI& GetMouseRect() const { return *m_mouseRect; }

		VertScrollbar& GetVertscrollbar() { return *m_vertScrollbar; }

		int GetDragRectHeight() const { return m_dragRectHeight; }

		/* Setters */

		void SetActive(bool active) { m_isActive = active; }

		void SetPos(Vec2 pos) override;
		void SetPos(float x, float y) override; 
		void SetX(float x) override;
		void SetY(float y) override;

		/* Set Rectangle Information */

		void SetRectFill(std::shared_ptr<Texture2D> texture) { m_rect->SetFillTexture(texture); }
		void SetRectOutline(std::shared_ptr<Texture2D> texture) { m_rect->SetOutlineTexture(texture); }
		void SetRectTexture(std::shared_ptr<Texture2D> fill, std::shared_ptr<Texture2D> outline) { m_rect->SetSpriteTexture(fill, outline); }
		void SetRectColour(Vec4 colour) { m_colour = colour; m_rect->SetColour(colour); }
		void SetRectColour(float r, float g, float b, float a = 1.0f) { m_colour = Vec4(r, g, b, a); m_rect->SetColour(m_colour); }

		void SetDragRectFill(std::shared_ptr<Texture2D> texture) { m_dragRect->SetFillTexture(texture); }
		void SetDragRectOutline(std::shared_ptr<Texture2D> texture) { m_dragRect->SetOutlineTexture(texture); }
		void SetDragRectTexture(std::shared_ptr<Texture2D> fill, std::shared_ptr<Texture2D> outline) { m_dragRect->SetSpriteTexture(fill, outline); }
		void SetDragRectColour(Vec4 colour) { m_rect->SetColour(colour); }
		void SetDragRectColour(float r, float g, float b, float a = 1.0f) { m_rect->SetColour(r, g, b, a); }

		void SetDragRectHeight(int height) { m_dragRectHeight = height; m_dragRect->SetHeight(height); }
		
		void SetResizeCursorTexture(std::shared_ptr<Texture2D> texture) { m_resizeCursor->SetTexture(texture); }

		void UseCloseButton(bool val);

		void UseVertScrollbar(unsigned int width = 0, unsigned sliderWidth = 0);
		void UseVertScrollbar(std::shared_ptr<Texture2D> filltexture, 
			std::shared_ptr<Texture2D> sliderTexture, 
			unsigned int width = 0, unsigned int sliderWidth = 0);

		// Will automatically enable the close button
		void SetCloseButtonTexture(std::shared_ptr<Texture2D> texture); 

		/* Deal with adding/removing elements */

		void AddElement(int order, ZGUI* element);
		void AddElement(ZGUI* element);
		void RemoveElement(ZGUI* element);
		void RemoveElement(int pos);
		void ClearElements();
		std::vector<ZGUI*> GetElements();

	private:
		Vec4 m_colour;

		bool m_isActive = true;
		bool m_isElementHovered = false;
		bool m_isPressed = false;
		bool m_isUsingCloseButton = false;
		int m_dragRectHeight = 15;

		std::unique_ptr<RectangleI> m_rect;
		std::unique_ptr<RectangleI> m_dragRect; // Rectangle used to drag the widget around with the mouse
		std::unique_ptr<RectangleI> m_scissorRect;

		std::unique_ptr<RectangleI> m_mouseRect;

		std::unique_ptr<Button> m_closeButton;
		std::unique_ptr<VertScrollbar> m_vertScrollbar;

		std::map<int, ZGUI*> m_elements;

	private:
		std::unique_ptr<RectangleI> m_bottomResizeRect; 
		std::unique_ptr<RectangleI> m_leftResizeRect; 
		std::unique_ptr<RectangleI> m_rightResizeRect; 
		std::unique_ptr<Sprite> m_resizeCursor;

		bool m_isResizeable = true;
		bool m_isResizing = false;
		bool m_isResizingHighlight = false; // Determine if we are hovering over any resize rectangle

		bool m_isResizingBoth = false;
		bool m_isResizingBottom = false;
		bool m_isResizingLeft = false;
		bool m_isResizingRight = false;

		void Resize();
	};
}