#include "VoxelBatch.h"
#include <algorithm>


namespace MatrixEngine {
	VoxelBatch::VoxelBatch() : _vao(0), _vbo(0)
	{
	}


	VoxelBatch::~VoxelBatch()
	{
	}

	void VoxelBatch::init()
	{
		createVertexArray();
	}

	void VoxelBatch::begin(GlyphSortType sortType)
	{
		_sortType = sortType;
		_renderBatches.clear();
		for (auto i = 0; i < _voxelGlyphs.size(); i++)
		{
			delete _voxelGlyphs[i];
		}
		_voxelGlyphs.clear();
	}

	void VoxelBatch::end()
	{
		sortGlyphs(); 
		createRenderBatches();
	}

	void VoxelBatch::draw(const glm::vec3& bottomLeft, const glm::vec3& topRight, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color)
	{
		//TODO: set texture, depth, 4 corner vertices (color, position and UV)
		/*VoxelGlyph* newVoxelGlyph = new VoxelGlyph();
		newVoxelGlyph->texture = texture;
		newVoxelGlyph->depth = depth;

		newVoxelGlyph->topLeft.color = color;
		newVoxelGlyph->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		newVoxelGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);
		
		newVoxelGlyph->bottomLeft.color = color;
		newVoxelGlyph->bottomLeft.setPosition(bottomLeft.x, bottomLeft.y, bottomLeft.z);
		newVoxelGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);
		
		newVoxelGlyph->bottomRight.color = color;
		newVoxelGlyph->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		newVoxelGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
		
		newVoxelGlyph->topRight.color = color;
		newVoxelGlyph->topRight.setPosition(topRight.x, topRight.y, topRight.z);
		newVoxelGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		_voxelGlyphs.push_back(newVoxelGlyph);*/
	}

	void VoxelBatch::renderBatch()
	{
		glBindVertexArray(_vao);

		for (int i = 0; i < _renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
			glDrawArrays(GL_TEXTURE_2D, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}
		glBindVertexArray(0);
	}

	void VoxelBatch::createRenderBatches()
	{
		std::vector<Vertex3D> vertices;
		vertices.resize(_voxelGlyphs.size() * 6 * 6); //TODO: make sure multiplication size is correct
		if (_voxelGlyphs.empty())
		{
			return;
		}

		int offset;
		int currentVertex = 0;

		_renderBatches.emplace_back(offset, 6, _voxelGlyphs[0]->texture);
		vertices[currentVertex++] = _voxelGlyphs[0]->topLeft; //TODO: make sure order of vertices is correct, it seems it's not!
		vertices[currentVertex++] = _voxelGlyphs[0]->bottomLeft;
		vertices[currentVertex++] = _voxelGlyphs[0]->bottomRight;
		vertices[currentVertex++] = _voxelGlyphs[0]->bottomRight;
		vertices[currentVertex++] = _voxelGlyphs[0]->topRight;
		vertices[currentVertex++] = _voxelGlyphs[0]->topLeft;

		offset += 6;
		auto nVoxelGlyphs = _voxelGlyphs.size();
		for (auto currentGlyph = 1; currentGlyph < nVoxelGlyphs; currentGlyph++)
		{
			if (_voxelGlyphs[currentGlyph]->texture != _voxelGlyphs[currentGlyph-1]->texture)
			{
				_renderBatches.emplace_back(offset, 6, _voxelGlyphs[currentGlyph]->texture); //TODO: make sure the "6" is a correct number
			}
			else
			{
				_renderBatches.back().numVertices += 6; //TODO: make sure the "6" is a correct number
			}
			vertices[currentVertex++] = _voxelGlyphs[currentGlyph]->topLeft; //TODO: make sure order of vertices is correct, it seems it's not!
			vertices[currentVertex++] = _voxelGlyphs[currentGlyph]->bottomLeft;
			vertices[currentVertex++] = _voxelGlyphs[currentGlyph]->bottomRight;
			vertices[currentVertex++] = _voxelGlyphs[currentGlyph]->bottomRight;
			vertices[currentVertex++] = _voxelGlyphs[currentGlyph]->topRight;
			vertices[currentVertex++] = _voxelGlyphs[currentGlyph]->topLeft;

			offset += 6;
		}
		// Bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		// Orphan the buffer for speed
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3D), nullptr, GL_DYNAMIC_DRAW); //TODO: confirm size is correct
		// Upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex3D), vertices.data()); //TODO: confirm size is correct
		// Unbind VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VoxelBatch::createVertexArray()
	{
		if (_vao == 0)
		{
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);
		if (_vbo == 0)
		{
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glEnableVertexAttribArray(0); //TODO: set correct attributes both here and in shaders!
		glEnableVertexAttribArray(1); //TODO: set correct attributes both here and in shaders!
		glEnableVertexAttribArray(2); //TODO: set correct attributes both here and in shaders!
		// Position attribute pointer
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, position)); //TODO: set correct vertex attribute pointers for position, color and UV!
		// Color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, color));
		// UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));

		glBindVertexArray(0);
	}

	void VoxelBatch::sortGlyphs()
	{
		bool(* comparator)(VoxelGlyph* a, VoxelGlyph* b) = compareBackToFront;
		switch (_sortType)
		{
		case GlyphSortType::BACK_TO_FRONT:
			comparator = compareBackToFront;
			break;
		case GlyphSortType::FRONT_TO_BACK:
			comparator = compareFrontToBack;
			break;
		case GlyphSortType::TEXTURE:
			comparator = compareTexture;
			break;
		}
		std::stable_sort(_voxelGlyphs.begin(), _voxelGlyphs.end(), comparator);
	}

	bool VoxelBatch::compareFrontToBack(VoxelGlyph* a, VoxelGlyph* b)
	{
		return (a->depth < b->depth);
	}

	bool VoxelBatch::compareBackToFront(VoxelGlyph* a, VoxelGlyph* b)
	{
		return (a->depth > b->depth);
	}

	bool VoxelBatch::compareTexture(VoxelGlyph* a, VoxelGlyph* b)
	{
		return (a->texture < b->texture);
	}
}