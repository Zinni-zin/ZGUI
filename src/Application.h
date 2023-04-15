#pragma once

#include <cstdint>
#include "ZGL/Core/Core.h"
#include "ZGL/Core/Timer.h"
#include "ZGL/GUI/Headers/Widget.h"
#include "ZGL/GUI/Headers/Slider.h"
#include "ZGL/GUI/Headers/Button.h"
#include "ZGL/GUI/Headers/Textbox.h"
#include "ZGL/GUI/Headers/Checkbox.h"
#include "ZGL/GUI/Headers/NumericUpDown.h"
#include "ZGL/GUI/Headers/VertScrollbar.h"

class Application
{
public:
	Application(const Application& other) = delete;
	Application& operator=(const Application&) = delete;

	int Init();
	void Run();

	static Application& GetInstance();
private:
	Application();

	void Update();
	void FixedUpdate(double deltaTime);
	void Draw(ZGL::Renderer& renderer);

private:
	ZGL::Core& m_core;
	ZGL::Timer m_timeStep;
	ZGL::Timer m_clock;
	ZGL::Timer m_accumulatedDT;

	ZGL::Mat4 m_ortho;
	ZGL::Mat4 m_view;

	std::vector<ZGL::Sprite> m_testSprites;

	std::unique_ptr<ZGL::Widget> m_testWidget;
	std::unique_ptr<ZGL::Button> m_testButton;
	std::unique_ptr<ZGL::Checkbox> m_testCheckbox;
	std::unique_ptr<ZGL::SliderI> m_testSliderI;
	std::unique_ptr<ZGL::SliderF> m_testSliderF;
	std::unique_ptr<ZGL::SliderD> m_testSliderD;
	std::unique_ptr<ZGL::NumericUpDownI> m_numericUpDownI;
	std::unique_ptr<ZGL::NumericUpDownF> m_numericUpDownF;
	std::unique_ptr<ZGL::NumericUpDownD> m_numericUpDownD;
	std::unique_ptr<ZGL::NumericUpDownUI> m_NumericUpDownUI;
	std::unique_ptr<ZGL::Textbox> m_testBox;
	std::unique_ptr<ZGL::VertScrollbar> m_widgetScrollbar;

	void SliderTests(std::shared_ptr<ZGL::Texture2D> fillTexture, ZGL::Font& font);

	void NumericUpDownTests(std::shared_ptr<ZGL::Texture2D> fillTexture, 
		std::shared_ptr<ZGL::Texture2D> textboxTexture, std::shared_ptr<ZGL::Texture2D> textboxOutline,
		std::shared_ptr<ZGL::Texture2D> textboxMouseCursor, std::shared_ptr<ZGL::Texture2D> textboxCursor,
		ZGL::Font& font);
};