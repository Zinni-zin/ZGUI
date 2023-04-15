#include "Application.h"

#include <time.h>
#include <vector>
#include <string>
#include <iostream>

#include "ZGL/Math/ZMath.h"


void TestButtonEvn(const std::string& text)
{
	std::cout << text << std::endl;
}

Application::Application() : m_core(ZGL::Core::GetCore()), m_timeStep(ZGL::Timer((int32_t)(50))) { }

int Application::Init()
{
	if (m_core.init("ZGUI") == -1)
	{
		std::cout << "Error Initializing Core!\n";
		return -1;
	}

	m_ortho = ZGL::Mat4::OrthoRH(0.f, (float)ZGL::Core::GetCore().GetWindow().GetWidth(), 
		(float)ZGL::Core::GetCore().GetWindow().GetHeight(), 0.f, -1.f, 1.f);

	ZGL::Font font(32, 32);
	font.Load("resources/fonts/Arial/arial.ttf");
	
	/* Handle test widget */

	std::shared_ptr<ZGL::Texture2D> fillTexture = std::make_shared<ZGL::Texture2D>("resources/textures/Shapes/RectangleFill.png");
	std::shared_ptr<ZGL::Texture2D> outlineTexture = std::make_shared<ZGL::Texture2D>("resources/textures/Shapes/RectangleOutline.png");

	std::shared_ptr<ZGL::Texture2D> textBoxFill = std::make_shared<ZGL::Texture2D>("resources/textures/UI/TextBoxFill.png");
	std::shared_ptr<ZGL::Texture2D> textBoxOutline = std::make_shared<ZGL::Texture2D>("resources/textures/UI/TextBoxOutline.png");

	std::shared_ptr<ZGL::Texture2D> resizeCursor = std::make_shared<ZGL::Texture2D>("resources/textures/UI/ResizeCursor.png");
	
	std::shared_ptr<ZGL::Texture2D> textHoverCursor = std::make_shared<ZGL::Texture2D>("resources/textures/UI/TextMouseCursor.png");
	std::shared_ptr<ZGL::Texture2D> textCursor = std::make_shared<ZGL::Texture2D>("resources/textures/UI/TextCursor.png");

	std::shared_ptr<ZGL::Texture2D> closeTexture = std::make_shared<ZGL::Texture2D>("resources/textures/UI/CloseButton.png");

	std::shared_ptr<ZGL::Texture2D> checkTexture = std::make_shared<ZGL::Texture2D>("resources/textures/UI/Checkmark.png");

	m_testWidget = std::make_unique<ZGL::Widget>(500.f, 0.f, 500, 500);
	m_testWidget->SetRectTexture(fillTexture, outlineTexture);
	m_testWidget->SetDragRectTexture(fillTexture, outlineTexture);
	m_testWidget->SetResizeCursorTexture(resizeCursor);
	m_testWidget->SetCloseButtonTexture(closeTexture);

	/*
	m_testWidget->SetResizeRectTextureB(fillTexture);
	m_testWidget->SetResizeRectTextureL(outlineTexture);
	m_testWidget->SetResizeRectTextureR(outlineTexture);

	m_testWidget->SetResizeRectColourB(ZGL::Vec4(0.7f, 0.2f, 0.8f, 1.f));
	m_testWidget->SetResizeRectColourL(ZGL::Vec4(0.5f, 0.7f, 0.8f, 1.f));
	m_testWidget->SetResizeRectColourR(ZGL::Vec4(0.2f, 0.7f, 0.2f, 1.f));
	*/

	/* --- Test Button --- */
	m_testButton = std::make_unique<ZGL::Button>(*m_testWidget, 
		ZGL::Vec2(15.f, 90.f), ZGL::Vec2ui(100, 25));
	m_testButton->SetTexture(fillTexture);
	m_testButton->SetEventFnc(std::bind(TestButtonEvn, "Button"));
	m_testButton->SetFontSize(ZGL::Vec2i(20, 20));
	m_testButton->CreateText("Button", font);

	/* --- Test Checkbox --- */

	m_testCheckbox = std::make_unique<ZGL::CheckBox>(*m_testWidget,
		ZGL::Vec2(125.f, 90.f), ZGL::Vec2ui(25, 25));
	m_testCheckbox->SetButtonTexture(fillTexture);
	m_testCheckbox->SetCheckMarkTexture(checkTexture);

	SliderTests(fillTexture, font);
	NumericUpDownTests(fillTexture, textBoxFill, textBoxOutline, textHoverCursor, textCursor, font);

	/* --- Test Text Box --- */
	m_testBox = std::make_unique<ZGL::Textbox>(*m_testWidget, font, 
		ZGL::Vec2(15.f, 20.f), ZGL::Vec2ui(200, 50));

	m_testBox->SetRectTexture(textBoxFill, textBoxOutline);
	m_testBox->SetMouseCursorTexture(textHoverCursor);
	m_testBox->SetTextCursorTexture(textCursor);
	m_testBox->SetHighlightRectTexture(fillTexture);

	// m_testBox->SetSpecialCharacterUse(false);
	// m_testBox->SetUsingSpace(false);
	// m_testBox->SetNumbersOnly(true);
	// m_testBox->SetLettersOnly(true);
	// m_testBox->SetMaxChars(5);

	m_testBox->SetTextOffsetX(7.f);

	m_testWidget->UseVertScrollbar(fillTexture, fillTexture);
	m_testWidget->GetVertscrollbar().SetSliderColour(0.55f, 0.55f, 0.55f);

	// m_widgetScrollbar = std::make_unique<ZGL::VertScrollbar>(*m_testWidget);
	// m_widgetScrollbar->SetRectTexture(fillTexture);
	// m_widgetScrollbar->SetSliderTexture(fillTexture);
	// m_widgetScrollbar->SetRectColour(0.2f, 0.2f, 0.2f);

	std::shared_ptr<ZGL::Texture2D> zSnow = std::make_shared<ZGL::Texture2D>("resources/textures/test.png");
	m_testSprites.emplace_back(0.f, 0.f, 100, 100);
	m_testSprites[0].SetTexture(zSnow);
	m_testSprites[0].SetScaleX(-1.f);

	return 0;
}

void Application::Run()
{
	ZGL::Renderer& renderer = ZGL::Renderer::GetRenderer();

	m_clock.Restart();
	while (!glfwWindowShouldClose(m_core.GetWindow().GetGLFWindow()))
	{
		Update();

		m_core.GetWindow().Update();

		ZGL::Timer dt = m_clock.Restart();
		m_accumulatedDT += dt.GetMicroseconds();

		if (m_accumulatedDT >= m_timeStep)
		{
			double deltaTime = m_accumulatedDT.GetSeconds();
			m_accumulatedDT -= m_timeStep;

			FixedUpdate(deltaTime);
			m_accumulatedDT.SetMicroseconds(0);
		}

		m_core.Clear(0.05f, 0.05f, 0.05f);

		Draw(renderer);

		glfwSwapBuffers(m_core.GetWindow().GetGLFWindow());
	}
}

void Application::Update()
{
	m_testWidget->Update();
}

void Application::FixedUpdate(double deltaTime)
{
}

void Application::Draw(ZGL::Renderer& renderer)
{
	m_testWidget->Draw(renderer);
	// renderer.Draw(m_testSprites, m_ortho);
}

Application& Application::GetInstance()
{
	static Application app;
	return app;
}

void Application::SliderTests(std::shared_ptr<ZGL::Texture2D> fillTexture, ZGL::Font& font)
{
	/* --- Test Slider Int --- */
	m_testSliderI = std::make_unique<ZGL::SliderI>(*m_testWidget,
		ZGL::Vec2(50.f, 140.f), ZGL::Vec2ui(100, 6), ZGL::Vec2ui(6, 25));
	m_testSliderI->HasToBeInBorder(false);
	m_testSliderI->IsAbleToExendBorder(false);
	m_testSliderI->SetRectTexture(fillTexture);
	m_testSliderI->SetSliderTexture(fillTexture);
	m_testSliderI->SetColour(ZGL::Vec4(0.8f, 0.8f, 0.8f, 1.f));
	//m_testSliderI->IsCenteredY(true);
	m_testSliderI->SetRectCenterY();
	//m_testSliderI->SetFont(font, ZGL::Vec2i(20, 20));
	//m_testSliderI->SetTextColour(ZGL::Vec4(0.25f, 0.25f, 0.25f, 1.f));
	m_testSliderI->SetMinVal(-333);
	m_testSliderI->SetMaxVal(327);
	m_testSliderI->SetVal(100);

	/* --- Test Slider Float --- */
	m_testSliderF = std::make_unique<ZGL::SliderF>(*m_testWidget,
		ZGL::Vec2(50.f, 170.f), ZGL::Vec2ui(100, 25), ZGL::Vec2ui(6, 25));
	m_testSliderF->HasToBeInBorder(true);
	m_testSliderF->IsAbleToExendBorder(false);
	m_testSliderF->SetRectTexture(fillTexture);
	m_testSliderF->SetSliderTexture(fillTexture);
	m_testSliderF->SetColour(ZGL::Vec4(0.8f, 0.8f, 0.8f, 1.f));
	//m_testSliderF->IsCenteredY(true);
	m_testSliderF->SetRectCenterY();
	m_testSliderF->SetFont(font, ZGL::Vec2i(20, 20));
	m_testSliderF->SetTextColour(ZGL::Vec4(0.25f, 0.25f, 0.25f, 1.f));
	m_testSliderF->SetMinVal(0.0f);
	m_testSliderF->SetMaxVal(60.0f);
	m_testSliderF->SetVal(14.4f);


	/* --- Test Slider Double --- */
	m_testSliderD = std::make_unique<ZGL::SliderD>(*m_testWidget,
		ZGL::Vec2(50.f, 200.f), ZGL::Vec2ui(115, 25), ZGL::Vec2ui(6, 25));
	m_testSliderD->HasToBeInBorder(true);
	m_testSliderD->IsAbleToExendBorder(false);
	m_testSliderD->SetRectTexture(fillTexture);
	m_testSliderD->SetSliderTexture(fillTexture);
	m_testSliderD->SetColour(ZGL::Vec4(0.8f, 0.8f, 0.8f, 1.f));
	//m_testSliderD->IsCenteredY(true);
	m_testSliderD->SetRectCenterY();
	m_testSliderD->SetFont(font, ZGL::Vec2i(20, 20));
	m_testSliderD->SetTextColour(ZGL::Vec4(0.25f, 0.25f, 0.25f, 1.f));
	m_testSliderD->SetMinVal(-20.0);
	m_testSliderD->SetMaxVal(0.0);
	m_testSliderD->SetVal(-15.0);
}

void Application::NumericUpDownTests(std::shared_ptr<ZGL::Texture2D> fillTexture,
	std::shared_ptr<ZGL::Texture2D> textboxTexture, std::shared_ptr<ZGL::Texture2D> textboxOutline,
	std::shared_ptr<ZGL::Texture2D> textboxMouseCursor, std::shared_ptr<ZGL::Texture2D> textboxCursor,
	ZGL::Font& font)
{
	std::shared_ptr<ZGL::Texture2D> arrowButton = std::make_shared<ZGL::Texture2D>("resources/textures/UI/ArrowButton.png");

	m_numericUpDownI = std::make_unique<ZGL::NumericUpDownI>(*m_testWidget, font,
		ZGL::Vec2(15.f, 260.f), ZGL::Vec2ui(75, 50), 
		ZGL::ButtonPosition::RIGHT, ZGL::Vec2ui(50, 50));
	m_numericUpDownI->SetButtonTexture(arrowButton);
	m_numericUpDownI->SetTextboxTexture(textboxTexture);
	m_numericUpDownI->SetTextboxOutline(textboxOutline);
	m_numericUpDownI->SetTextboxMouseCursor(textboxMouseCursor);
	m_numericUpDownI->SetTextboxCursor(textboxCursor);
	m_numericUpDownI->SetTextboxHighlightTexture(fillTexture);
	m_numericUpDownI->SetMaxVal(100);
	m_numericUpDownI->SetMinVal(-100);
	//m_numericUpDownI->SetButtonHeight(30);

	m_numericUpDownF = std::make_unique<ZGL::NumericUpDownF>(*m_testWidget, font,
		ZGL::Vec2(150.f, 260.f), ZGL::Vec2ui(75, 50), 
		ZGL::ButtonPosition::LEFT, ZGL::Vec2ui(50, 50));
	m_numericUpDownF->SetButtonTexture(arrowButton);
	m_numericUpDownF->SetTextboxTexture(textboxTexture);
	m_numericUpDownF->SetTextboxOutline(textboxOutline);
	m_numericUpDownF->SetTextboxMouseCursor(textboxMouseCursor);
	m_numericUpDownF->SetTextboxCursor(textboxCursor);
	m_numericUpDownF->SetTextboxHighlightTexture(fillTexture);
	m_numericUpDownF->SetMaxVal(100.f);
	m_numericUpDownF->SetMinVal(-100.f);
	m_numericUpDownF->SetIncrement(.5f);

	m_numericUpDownD = std::make_unique<ZGL::NumericUpDownD>(*m_testWidget, font,
		ZGL::Vec2(15.f, 315.f), ZGL::Vec2ui(75, 50), 
		ZGL::ButtonPosition::UP_DOWN, ZGL::Vec2ui(50, 50));
	m_numericUpDownD->SetButtonTexture(arrowButton);
	m_numericUpDownD->SetTextboxTexture(textboxTexture);
	m_numericUpDownD->SetTextboxOutline(textboxOutline);
	m_numericUpDownD->SetTextboxMouseCursor(textboxMouseCursor);
	m_numericUpDownD->SetTextboxCursor(textboxCursor);
	m_numericUpDownD->SetTextboxHighlightTexture(fillTexture);
	m_numericUpDownD->SetMaxVal(100.);
	m_numericUpDownD->SetMinVal(-100.);
	m_numericUpDownD->SetIncrement(5);
	m_numericUpDownD->SetForcedIncrement(true);

	m_NumericUpDownUI = std::make_unique<ZGL::NumericUpDownUI>(*m_testWidget, font,
		ZGL::Vec2(285.f, 260.f), ZGL::Vec2ui(75, 50), 
		ZGL::ButtonPosition::RIGHT, ZGL::Vec2ui(50, 50));
	m_NumericUpDownUI->SetButtonTexture(arrowButton);
	m_NumericUpDownUI->SetTextboxTexture(textboxTexture);
	m_NumericUpDownUI->SetTextboxOutline(textboxOutline);
	m_NumericUpDownUI->SetTextboxMouseCursor(textboxMouseCursor);
	m_NumericUpDownUI->SetTextboxCursor(textboxCursor);
	m_NumericUpDownUI->SetTextboxHighlightTexture(fillTexture);
	m_NumericUpDownUI->SetMaxVal(100);
	m_NumericUpDownUI->SetMinVal(0);
	m_NumericUpDownUI->SetIncrement(5);
	m_NumericUpDownUI->SetForcedIncrement(true);
}