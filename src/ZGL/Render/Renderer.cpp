#include "Headers/Renderer.h"

#include <array>
#include <iostream>
#include <algorithm>

#define MaxQuadCount 1000
#define MaxVertexCount MaxQuadCount * 4
#define MaxIndexCount MaxQuadCount * 6

namespace ZGL
{
	struct RenderData
	{
		uint32_t IndexCount = 0;

		Vertex* QuadBuffer = nullptr;
		Vertex* QuadBufferPtr = nullptr;

		std::array<GLuint, 32> TextureSlotsID;
		uint32_t UsedTextureSlots = 0;

	};

	static RenderData s_Data;

#pragma region Batch Renderer
	Renderer::BatchRenderer::BatchRenderer()
	{ }

	void Renderer::BatchRenderer::Init()
	{
		if (s_Data.QuadBuffer == nullptr)
			s_Data.QuadBuffer = new Vertex[MaxVertexCount];
		else
		{
			std::cout << "Error Quad Buffer already exists! Call End() before Init again!\n";
			return;
		}

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glCreateBuffers(1, &m_vb);
		glBindBuffer(GL_ARRAY_BUFFER, m_vb);
		glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (const void*)offsetof(Vertex, Position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (const void*)offsetof(Vertex, Colour));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (const void*)offsetof(Vertex, TexID));

		uint32_t indices[MaxIndexCount];
		uint32_t offset = 0;
		for (size_t i = 0; i < MaxIndexCount; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		glCreateBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		m_shader = std::make_unique<Shader>("resources/shaders/SpriteV.shader",
			"resources/shaders/SpriteF.shader");

		m_textShader = std::make_unique<Shader>("resources/shaders/TextV.shader",
			"resources/shaders/TextF.shader");

		int sampler[32]; 
		for (int i = 0; i < 32; ++i)
			sampler[i] = i;

		m_shader->Use();
		m_shader->SetUniform1iv("u_Textures", 32, sampler);

		m_textShader->Use();
		m_textShader->SetUniform1iv("u_Textures", 32, sampler);
	}

	void Renderer::BatchRenderer::End()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vb);
		glDeleteBuffers(1, &m_ibo);

		delete[] s_Data.QuadBuffer;
		s_Data.QuadBuffer = nullptr;
	}

	void Renderer::BatchRenderer::DrawQuad(const Sprite& sprite)
	{
		if (s_Data.IndexCount >= MaxIndexCount || s_Data.UsedTextureSlots > 32)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		float textureIndex = -1.0f;

		bool isScaled = sprite.GetScaleX() != 1.f || sprite.GetScaleY() != 1.f;
		bool isRotated = sprite.GetRotation() != 0.f;

		for (uint32_t i = 0; i < s_Data.UsedTextureSlots; i++)
		{
			if (s_Data.TextureSlotsID[i] == sprite.GetTextureID())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == -1.0f)
		{
			textureIndex = (float)s_Data.UsedTextureSlots;
			s_Data.TextureSlotsID[s_Data.UsedTextureSlots] = sprite.GetTextureID();
			++s_Data.UsedTextureSlots;
		}

		Vec2 posBL(sprite.GetPos());
		Vec2 posBR(sprite.GetX() + sprite.GetWidth(), sprite.GetY());
		Vec2 posTR(sprite.GetX() + sprite.GetWidth(), sprite.GetY() + sprite.GetHeight());
		Vec2 posTL(sprite.GetX(), sprite.GetY() + sprite.GetHeight());

		if (isScaled || isRotated)
		{
			float halfW = sprite.GetWidth() / 2.f;
			float halfH = sprite.GetHeight() / 2.f;

			// Set positions to be the origin so we rotate around that
			// We have to subtract by our sprite position because of our mvp
			Vec2 tempBL((posBL.x - sprite.GetX()) - halfW, (posBL.y - sprite.GetY()) - halfH);
			Vec2 tempBR((posBR.x - sprite.GetX()) - halfW, (posBR.y - sprite.GetY()) - halfH);
			Vec2 tempTR((posTR.x - sprite.GetX()) - halfW, (posTR.y - sprite.GetY()) - halfH);
			Vec2 tempTL((posTL.x - sprite.GetX()) - halfW, (posTL.y - sprite.GetY()) - halfH);

			// Rotate the positions
			Mat4 scaleMat = Mat4::ScaleX(sprite.GetScaleX()) * Mat4::ScaleY(sprite.GetScaleY());
			Mat4 rotateMat = Mat4::RotateZ(sprite.GetRotation());
			tempBL = scaleMat * rotateMat * tempBL;
			tempBR = scaleMat * rotateMat * tempBR;
			tempTR = scaleMat * rotateMat * tempTR;
			tempTL = scaleMat * rotateMat * tempTL;

			// Set the positions back to their original position 
			// (doing it how we did will rotate around the center of the sprite)
			// We also have to add back our subtracted sprite positions because of the mvp
			posBL = Vec2((tempBL.x + sprite.GetX()) + halfW, (tempBL.y + sprite.GetY()) + halfH);
			posBR = Vec2((tempBR.x + sprite.GetX()) + halfW, (tempBR.y + sprite.GetY()) + halfH);
			posTR = Vec2((tempTR.x + sprite.GetX()) + halfW, (tempTR.y + sprite.GetY()) + halfH);
			posTL = Vec2((tempTL.x + sprite.GetX()) + halfW, (tempTL.y + sprite.GetY()) + halfH);
		}

		// Setup the first vertex | Bottom Left
		s_Data.QuadBufferPtr->Position = { posBL.x, posBL.y, sprite.GetDepth() };
		s_Data.QuadBufferPtr->Colour = { sprite.GetColour().x, sprite.GetColour().y, sprite.GetColour().z, sprite.GetColour().w };
		s_Data.QuadBufferPtr->TexCoords = { sprite.GetSubCoords().z, /*0.0f*/ sprite.GetSubCoords().w };
		s_Data.QuadBufferPtr->TexID = textureIndex;
		s_Data.QuadBufferPtr++;

		// Setup the second vertex | Bottom right
		s_Data.QuadBufferPtr->Position = { posBR.x, posBR.y, sprite.GetDepth() };
		s_Data.QuadBufferPtr->Colour = { sprite.GetColour().x, sprite.GetColour().y, sprite.GetColour().z, sprite.GetColour().w };
		s_Data.QuadBufferPtr->TexCoords = { sprite.GetSubCoords().x, /*0.0f*/ sprite.GetSubCoords().w };
		s_Data.QuadBufferPtr->TexID = textureIndex;
		s_Data.QuadBufferPtr++;

		// Setup the third vertex | Top Right
		s_Data.QuadBufferPtr->Position = { posTR.x, posTR.y, sprite.GetDepth() };
		s_Data.QuadBufferPtr->Colour = { sprite.GetColour().x, sprite.GetColour().y, sprite.GetColour().z, sprite.GetColour().w };
		s_Data.QuadBufferPtr->TexCoords = { sprite.GetSubCoords().x, sprite.GetSubCoords().y };
		s_Data.QuadBufferPtr->TexID = textureIndex;
		s_Data.QuadBufferPtr++;

		// Setup the fourth vertex | Top Left
		s_Data.QuadBufferPtr->Position = { posTL.x, posTL.y, sprite.GetDepth() };
		s_Data.QuadBufferPtr->Colour = { sprite.GetColour().x, sprite.GetColour().y, sprite.GetColour().z, sprite.GetColour().w };
		s_Data.QuadBufferPtr->TexCoords = { sprite.GetSubCoords().z, sprite.GetSubCoords().y /*0.0f, 0.0f*/ };
		s_Data.QuadBufferPtr->TexID = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.IndexCount += 6;
	}

	void Renderer::BatchRenderer::DrawTextQuad(const Character& ch, float x, float y,
		float width, float height, float depth, Vec4 colour)
	{
		if (s_Data.IndexCount >= MaxIndexCount || s_Data.UsedTextureSlots > 32)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		float textureIndex = -1.0f;

		for (uint32_t i = 0; i < s_Data.UsedTextureSlots; i++)
		{
			if (s_Data.TextureSlotsID[i] == ch.TextureID)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == -1.0f)
		{
			textureIndex = (float)s_Data.UsedTextureSlots;
			s_Data.TextureSlotsID[s_Data.UsedTextureSlots] = ch.TextureID;
			++s_Data.UsedTextureSlots;
		}

		// Setup the first vertex | Bottom Left
		s_Data.QuadBufferPtr->Position = { x, y, depth };
		s_Data.QuadBufferPtr->Colour = { colour.x, colour.y,colour.z, colour.w };
		s_Data.QuadBufferPtr->TexCoords = { 0.0f, 0.0f };
		s_Data.QuadBufferPtr->TexID = textureIndex;
		s_Data.QuadBufferPtr++;

		// Setup the second vertex | Bottom right
		s_Data.QuadBufferPtr->Position = { x + width, y, depth };
		s_Data.QuadBufferPtr->Colour = { colour.x, colour.y, colour.z, colour.w };
		s_Data.QuadBufferPtr->TexCoords = { 1.0f, 0.0f };
		s_Data.QuadBufferPtr->TexID = textureIndex;
		s_Data.QuadBufferPtr++;

		// Setup the third vertex | Top Right
		s_Data.QuadBufferPtr->Position = { x + width, y + height, depth };
		s_Data.QuadBufferPtr->Colour = { colour.x, colour.y, colour.z, colour.w };
		s_Data.QuadBufferPtr->TexCoords = { 1.0f, 1.0f };
		s_Data.QuadBufferPtr->TexID = textureIndex;
		s_Data.QuadBufferPtr++;

		// Setup the fourth vertex | Top Left
		s_Data.QuadBufferPtr->Position = { x,  y + height, depth };
		s_Data.QuadBufferPtr->Colour = { colour.x, colour.y, colour.z, colour.w };
		s_Data.QuadBufferPtr->TexCoords = { 0.0f, 1.0f };
		s_Data.QuadBufferPtr->TexID = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.IndexCount += 6; // Incease our index count
	}

	void Renderer::BatchRenderer::BeginBatch()
	{
		s_Data.QuadBufferPtr = s_Data.QuadBuffer; // Set our buffer pointer to the beginning
	}

	void Renderer::BatchRenderer::EndBatch()
	{
		GLsizeiptr size = (uint8_t*)s_Data.QuadBufferPtr - (uint8_t*)s_Data.QuadBuffer;
		glBindBuffer(GL_ARRAY_BUFFER, m_vb);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.QuadBuffer); // Send all our vertices
	}

	void Renderer::BatchRenderer::Flush()
	{
		for (uint32_t i = 0; i < s_Data.UsedTextureSlots; ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, s_Data.TextureSlotsID[i]);
		}

		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_INT, nullptr);

		s_Data.IndexCount = 0;
		s_Data.UsedTextureSlots = 0;
	}

	void Renderer::BatchRenderer::Draw(const std::vector<Sprite>& sprites, const Mat4& mvp)
	{
		BeginBatch();

		m_shader->Use();

		for (size_t i = 0; i < sprites.size(); i++)
			DrawQuad(sprites[i]);

		m_shader->SetUniformMat4f("u_MVP", mvp);

		EndBatch();
		Flush();
	}

#pragma region Shapes

	void Renderer::BatchRenderer::DrawShapeFill(const std::vector<ShapeF*>& shapes, const Mat4& mvp)
	{
		BeginBatch();

		m_shader->Use();


		for (size_t i = 0; i < shapes.size(); i++)
		{
			if (shapes[i]->IsOutline())
				shapes[i]->SetDrawType(ShapeFill);
			DrawQuad(shapes[i]->GetSprite());
		}

		m_shader->SetUniformMat4f("u_MVP", mvp);

		EndBatch();
		Flush();
	}

	void Renderer::BatchRenderer::DrawShapeFill(const std::vector<ShapeI*>& shapes, const Mat4& mvp)
	{
		BeginBatch();

		m_shader->Use();

		for (size_t i = 0; i < shapes.size(); i++)
		{
			if (shapes[i]->IsOutline())
				shapes[i]->SetDrawType(ShapeFill);
			DrawQuad(shapes[i]->GetSprite());
		}

		m_shader->SetUniformMat4f("u_MVP", mvp);

		EndBatch();
		Flush();
	}

	void Renderer::BatchRenderer::DrawShapeOutline(const std::vector<ShapeF*>& shapes, const Mat4& mvp)
	{
		BeginBatch();

		m_shader->Use();

		// Draw all our sprites
		for (size_t i = 0; i < shapes.size(); i++)
		{
			if (!shapes[i]->IsOutline())
				shapes[i]->SetDrawType(ShapeOutline);
			DrawQuad(shapes[i]->GetSprite());
		}

		m_shader->SetUniformMat4f("u_MVP", mvp);

		EndBatch();
		Flush();
	}

	void Renderer::BatchRenderer::DrawShapeOutline(const std::vector<ShapeI*>& shapes, const Mat4& mvp)
	{
		BeginBatch();

		m_shader->Use();

		for (size_t i = 0; i < shapes.size(); i++)
		{
			if (!shapes[i]->IsOutline())
				shapes[i]->SetDrawType(ShapeOutline);
			DrawQuad(shapes[i]->GetSprite());
		}

		m_shader->SetUniformMat4f("u_MVP", mvp);

		EndBatch();
		Flush();
	}

	void Renderer::BatchRenderer::DrawShape(const std::vector<ShapeF*>& shapes, const Mat4& mvp, bool isOutline)
	{
		if (isOutline)
			DrawShapeOutline(shapes, mvp);
		else
			DrawShapeFill(shapes, mvp);
	}

	void Renderer::BatchRenderer::DrawShape(const std::vector<ShapeI*>& shapes, const Mat4& mvp, bool isOutline)
	{
		if (isOutline)
			DrawShapeOutline(shapes, mvp);
		else
			DrawShapeFill(shapes, mvp);
	}
#pragma endregion

	void Renderer::BatchRenderer::DrawText(Text& text, const Mat4& mvp)
	{
		// Begin drawing
		BeginBatch();

		// Setup our shader
		m_textShader->Use();
		m_textShader->SetUniformMat4f("u_MVP", mvp);

		// Set our pen pos to our initial position
		text.SetPenPos(text.GetPos().x, text.GetPos().y);

		// Draw all our characters
		int i = 0;
		for (char c : text.GetText())
		{
			// Setup our positions to draw
			float xPos = text.GetPenPos().x +
				((i == 0) ? 0.0f : (float)text.GetCharacters()[c].Bearing.x) * text.GetScale().x;

			float yPos = text.GetPenPos().y +
				(text.GetVertOffset() - (float)text.GetCharacters()[c].Bearing.y) * text.GetScale().y;

			// Setup the vertices 
			DrawTextQuad(text.GetCharacters()[c],
				xPos, yPos,
				(float)text.GetCharacters()[c].Size.x * text.GetScale().x,
				(float)text.GetCharacters()[c].Size.y * text.GetScale().y,
				text.GetDepth(), text.GetColour());

			// Increment the pen position for the next character
			text.SetPenX(text.GetPenPos().x + (text.GetCharacters()[c].Advance >> 6) * text.GetScale().x);
			++i;
		}

		// Draw
		EndBatch();
		Flush();
	}
#pragma endregion

	Renderer::Renderer()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glCreateBuffers(1, &m_vb);
		glBindBuffer(GL_ARRAY_BUFFER, m_vb);
		glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (const void*)offsetof(Vertex, Position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (const void*)offsetof(Vertex, Colour));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (const void*)offsetof(Vertex, TexID));

		uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

		glCreateBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		m_shader = std::make_unique<Shader>("resources/shaders/SpriteV.shader",
			"resources/shaders/SpriteF.shader");

		int sampler[32];
		for (int i = 0; i < 32; ++i)
			sampler[i] = i;

		m_shader->Use();
		m_shader->SetUniform1iv("u_Textures", 32, sampler);

		m_batchRenderer.Init();
	}

	Renderer::~Renderer()
	{
		m_batchRenderer.End();
	}

	void Renderer::Flush()
	{
		for (uint32_t i = 0; i < s_Data.UsedTextureSlots; ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, s_Data.TextureSlotsID[i]);
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_vb);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		s_Data.IndexCount = 0;
		s_Data.UsedTextureSlots = 0;
	}

	Vertex* Renderer::DrawQuad(Vertex* target, const Sprite& sprite)
	{
		if (s_Data.IndexCount >= MaxIndexCount || s_Data.UsedTextureSlots > 32)
			Flush();

		float textureIndex = -1.0f;

		for (uint32_t i = 0; i < s_Data.UsedTextureSlots; i++)
		{
			if (s_Data.TextureSlotsID[i] == sprite.GetTextureID())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == -1.0f)
		{
			textureIndex = (float)s_Data.UsedTextureSlots;
			s_Data.TextureSlotsID[s_Data.UsedTextureSlots] = sprite.GetTextureID();
			++s_Data.UsedTextureSlots;
		}

		Vec2 posBL(sprite.GetPos());
		Vec2 posBR(sprite.GetX() + sprite.GetWidth(), sprite.GetY());
		Vec2 posTR(sprite.GetX() + sprite.GetWidth(), sprite.GetY() + sprite.GetHeight());
		Vec2 posTL(sprite.GetX(), sprite.GetY() + sprite.GetHeight());

		if (sprite.GetRotation() != 0.f)
		{
			float halfW = sprite.GetWidth() / 2.f;
			float halfH = sprite.GetHeight() / 2.f;

			// Set positions to be the origin so we rotate around that
			// We have to subtract by our sprite position because of our mvp
			Vec2 tempBL((posBL.x - sprite.GetX()) - halfW, (posBL.y - sprite.GetY()) - halfH);
			Vec2 tempBR((posBR.x - sprite.GetX()) - halfW, (posBR.y - sprite.GetY()) - halfH);
			Vec2 tempTR((posTR.x - sprite.GetX()) - halfW, (posTR.y - sprite.GetY()) - halfH);
			Vec2 tempTL((posTL.x - sprite.GetX()) - halfW, (posTL.y - sprite.GetY()) - halfH);

			// Rotate the positions
			Mat4 rotateMat = Mat4::RotateZ(sprite.GetRotation());
			tempBL = rotateMat * tempBL;
			tempBR = rotateMat * tempBR;
			tempTR = rotateMat * tempTR;
			tempTL = rotateMat * tempTL;

			// Set the positions back to their original position 
			// (doing it how we did will rotate around the center of the sprite)
			// We also have to add back our subtracted sprite positions because of the mvp
			posBL = Vec2((tempBL.x + sprite.GetX()) + halfW, (tempBL.y + sprite.GetY()) + halfH);
			posBR = Vec2((tempBR.x + sprite.GetX()) + halfW, (tempBR.y + sprite.GetY()) + halfH);
			posTR = Vec2((tempTR.x + sprite.GetX()) + halfW, (tempTR.y + sprite.GetY()) + halfH);
			posTL = Vec2((tempTL.x + sprite.GetX()) + halfW, (tempTL.y + sprite.GetY()) + halfH);
		}

		// Setup the first vertex | Bottom Left
		target->Position = { posBL.x, posBL.y, sprite.GetDepth() };
		target->Colour = { sprite.GetColour().x, sprite.GetColour().y, sprite.GetColour().z, sprite.GetColour().w };
		target->TexCoords = { sprite.GetSubCoords().z, /*0.0f*/ sprite.GetSubCoords().w };
		target->TexID = textureIndex;
		target++;

		// Setup the second vertex | Bottom Right
		target->Position = { posBR.x, posBR.y, sprite.GetDepth() };
		target->Colour = { sprite.GetColour().x, sprite.GetColour().y, sprite.GetColour().z, sprite.GetColour().w };
		target->TexCoords = { sprite.GetSubCoords().x, sprite.GetSubCoords().w };
		target->TexID = textureIndex;
		target++;

		// Setup the third vertex | Top Right
		target->Position = { posTR.x, posTR.y, sprite.GetDepth() };
		target->Colour = { sprite.GetColour().x, sprite.GetColour().y, sprite.GetColour().z, sprite.GetColour().w };
		target->TexCoords = { sprite.GetSubCoords().x, sprite.GetSubCoords().y };
		target->TexID = textureIndex;
		target++;

		// Setup the fourth vertex | Top Left
		target->Position = { posTL.x, posTL.y, sprite.GetDepth() };
		target->Colour = { sprite.GetColour().x, sprite.GetColour().y, sprite.GetColour().z, sprite.GetColour().w };
		target->TexCoords = { sprite.GetSubCoords().z, sprite.GetSubCoords().y };
		target->TexID = textureIndex;
		target++;

		s_Data.IndexCount += 6;

		return target;
	}

	void Renderer::Draw(const Sprite& sprite, const Mat4& mvp)
	{
		std::array<Vertex, 4> vertices;
		Vertex* buffer = vertices.data();

		buffer = DrawQuad(buffer, sprite);

		glBindBuffer(GL_ARRAY_BUFFER, m_vb);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		m_shader->Use();

		m_shader->SetUniformMat4f("u_MVP", mvp);
		Flush();
	}

#pragma region Shapes
	void Renderer::DrawShapeFill(ShapeF* shape, const Mat4& mvp)
	{
		std::array<Vertex, 4> vertices;
		Vertex* buffer = vertices.data();

		if (shape->IsOutline())
			shape->SetDrawType(ShapeFill);
		buffer = DrawQuad(buffer, shape->GetSprite());

		glBindBuffer(GL_ARRAY_BUFFER, m_vb);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		m_shader->Use();

		m_shader->SetUniformMat4f("u_MVP", mvp);
		Flush();
	}

	void Renderer::DrawShapeFill(ShapeI* shape, const Mat4& mvp)
	{
		std::array<Vertex, 4> vertices;
		Vertex* buffer = vertices.data();

		if (shape->IsOutline())
			shape->SetDrawType(ShapeFill);
		buffer = DrawQuad(buffer, shape->GetSprite());

		glBindBuffer(GL_ARRAY_BUFFER, m_vb);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		m_shader->Use();

		m_shader->SetUniformMat4f("u_MVP", mvp);
		Flush();
	}

	void Renderer::DrawShapeOutline(ShapeF* shape, const Mat4& mvp)
	{
		std::array<Vertex, 4> vertices;
		Vertex* buffer = vertices.data();

		if (!shape->IsOutline())
			shape->SetDrawType(ShapeOutline);
		buffer = DrawQuad(buffer, shape->GetSprite());

		glBindBuffer(GL_ARRAY_BUFFER, m_vb);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		m_shader->Use();

		m_shader->SetUniformMat4f("u_MVP", mvp);
		Flush();
	}

	void Renderer::DrawShapeOutline(ShapeI* shape, const Mat4& mvp)
	{
		std::array<Vertex, 4> vertices;
		Vertex* buffer = vertices.data();

		if (!shape->IsOutline())
			shape->SetDrawType(ShapeOutline);
		buffer = DrawQuad(buffer, shape->GetSprite());

		glBindBuffer(GL_ARRAY_BUFFER, m_vb);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		m_shader->Use();

		m_shader->SetUniformMat4f("u_MVP", mvp);
		Flush();
	}

	void Renderer::DrawShape(ShapeF* shapes, const Mat4& mvp, bool isOutline)
	{
		if (isOutline)
			DrawShapeOutline(shapes, mvp);
		else
			DrawShapeFill(shapes, mvp);
	}

	void Renderer::DrawShape(ShapeI* shapes, const Mat4& mvp, bool isOutline)
	{
		if (isOutline)
			DrawShapeOutline(shapes, mvp);
		else
			DrawShapeFill(shapes, mvp);
	}
#pragma endregion

#pragma region Batch Drawing
	void Renderer::Draw(const std::vector<Sprite>& sprites, const Mat4& mvp)
	{
		m_batchRenderer.Draw(sprites, mvp);
	}

	void Renderer::DrawShapeFill(const std::vector<ShapeF*>& shapes, const Mat4& mvp) 
	{
		m_batchRenderer.DrawShapeFill(shapes, mvp);
	}

	void Renderer::DrawShapeFill(const std::vector<ShapeI*>& shapes, const Mat4& mvp) 
	{
		m_batchRenderer.DrawShapeFill(shapes, mvp);
	}

	void Renderer::DrawShapeOutline(const std::vector<ShapeF*>& shapes, const Mat4& mvp) 
	{
		m_batchRenderer.DrawShapeOutline(shapes, mvp);
	}

	void Renderer::DrawShapeOutline(const std::vector<ShapeI*>& shapes, const Mat4& mvp)
	{
		m_batchRenderer.DrawShapeOutline(shapes, mvp);
	}

	void Renderer::DrawShape(const std::vector<ShapeF*>& shapes, const Mat4& mvp, bool isOutline)
	{
		m_batchRenderer.DrawShape(shapes, mvp, isOutline);
	}

	void Renderer::DrawShape(const std::vector<ShapeI*>& shapes, const Mat4& mvp, bool isOutline)
	{
		m_batchRenderer.DrawShape(shapes, mvp, isOutline);
	}

	void Renderer::DrawText(Text& text, const Mat4& mvp)
	{   
		m_batchRenderer.DrawText(text, mvp);
	}
#pragma endregion

	Renderer& Renderer::GetRenderer()
	{
		static Renderer instance;
		return instance;
	}
}
