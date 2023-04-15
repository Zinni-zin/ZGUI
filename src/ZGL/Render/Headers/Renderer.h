#pragma once
#include <vector>
#include <array>
#include <unordered_map>
#include <memory>

#include "GL/glew.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Text.h"
#include "Sprite.h"
#include "Shader.h"
#include "Texture2D.h"
#include "../../Shapes/Headers/Shape.h"

#include "../../Math/Matrices.h"
#include "../../Math/Vectors.h"

namespace ZGL
{
	struct Vertex
	{
		Vec3 Position;
		Vec4 Colour;
		Vec2 TexCoords;
		float TexID;
	};

	class Renderer
	{
#pragma region Extra Structs/Classes
		class BatchRenderer
		{
		public:
			BatchRenderer();

			void Init();

			void End();
			
			void Draw(const std::vector<Sprite>& sprites, const Mat4& mvp);

			void DrawShapeFill(const std::vector<ShapeF*>& shapes, const Mat4& mvp);
			void DrawShapeFill(const std::vector<ShapeI*>& shapes, const Mat4& mvp);

			void DrawShapeOutline(const std::vector<ShapeF*>& shapes, const Mat4& mvp);
			void DrawShapeOutline(const std::vector<ShapeI*>& shapes, const Mat4& mvp);

			void DrawShape(const std::vector<ShapeF*>& shapes, const Mat4& mvp, bool isOutline = false);
			void DrawShape(const std::vector<ShapeI*>& shapes, const Mat4& mvp, bool isOutline = false);

			void DrawText(Text& text, const Mat4& mvp);

		private:
			std::unique_ptr<Shader> m_shader;
			std::unique_ptr<Shader> m_textShader;
			std::unordered_map<GLuint, short> m_textures;

			GLuint m_vb;
			GLuint m_vao;
			GLuint m_ibo;

			void DrawQuad(const Sprite& sprite);
		
			void DrawTextQuad(const Character& ch, float x, float y,
				float width, float height, float depth = 0.0f, Vec4 colour = Vec4(1.0f));

			void BeginBatch();

			void EndBatch();

			void Flush();
		};
#pragma endregion

	public:
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		~Renderer();

		void Draw(const Sprite& sprite, const Mat4& mvp);

		void DrawShapeFill(ShapeF* shape, const Mat4& mvp);
		void DrawShapeFill(ShapeI* shape, const Mat4& mvp);

		void DrawShapeOutline(ShapeF* shape, const Mat4& mvp);
		void DrawShapeOutline(ShapeI* shape, const Mat4& mvp);

		void DrawShape(ShapeF* shapes, const Mat4& mvp, bool isOutline = false);
		void DrawShape(ShapeI* shapes, const Mat4& mvp, bool isOutline = false);

#pragma region Batch Draw Calls
		
		void Draw(const std::vector<Sprite>& sprites, const Mat4& mvp);

		void DrawShapeFill(const std::vector<ShapeF*>& shapes, const Mat4& mvp);
		void DrawShapeFill(const std::vector<ShapeI*>& shapes, const Mat4& mvp);

		void DrawShapeOutline(const std::vector<ShapeF*>& shapes, const Mat4& mvp);
		void DrawShapeOutline(const std::vector<ShapeI*>& shapes, const Mat4& mvp);

		void DrawShape(const std::vector<ShapeF*>& shapes, const Mat4& mvp, bool isOutline = false);
		void DrawShape(const std::vector<ShapeI*>& shapes, const Mat4& mvp, bool isOutline = false);

		void DrawText(Text& text, const Mat4& mvp);
#pragma endregion

		static Renderer& GetRenderer();
	private:
		Renderer();

		std::unique_ptr<Shader> m_shader; 
		BatchRenderer m_batchRenderer;

		GLuint m_vb;
		GLuint m_vao;
		GLuint m_ibo;

		Vertex* DrawQuad(Vertex* target, const Sprite& sprite);

		void Flush();
	};
}
