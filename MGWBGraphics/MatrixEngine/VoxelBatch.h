#pragma once
#include <GL/glew.h>
#include <vector>
#include "SpriteBatch.h"
namespace MatrixEngine 
{
	struct VoxelGlyph
	{
		GLuint texture;
		float depth;

		Vertex3D topLeft;
		Vertex3D bottomLeft;
		Vertex3D topRight;
		Vertex3D bottomRight;
	};

	class VoxelBatch
	{
	public:
		VoxelBatch();
		~VoxelBatch();

		void init();
		void begin(GlyphSortType sortType);
		void end();
		void draw(const glm::vec3 & bottomLeft, const glm::vec3 & topRight, const glm::vec4 & uvRect, GLuint texture, float depth, const Color & color);
		void renderBatch();

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();

		static bool compareFrontToBack(VoxelGlyph* a, VoxelGlyph* b);
		static bool compareBackToFront(VoxelGlyph* a, VoxelGlyph* b);
		static bool compareTexture(VoxelGlyph* a, VoxelGlyph* b);
		
		GLuint _vao;
		GLuint _vbo;
		GlyphSortType _sortType;

		std::vector<VoxelGlyph*> _voxelGlyphs;
		std::vector<RenderBatch> _renderBatches; //TODO: change RenderBatches for voxels
	};
}