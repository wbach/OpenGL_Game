#pragma once
#include <GL/glew.h>
#include <vector>


namespace Utils
{
	static const GLuint s_GLuint_zero = 0;

	static void BindFrameBuffer(const int & frameBuffer, const int & width, const int & height)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
		glViewport(0, 0, width, height);
	}

	static int CreateFrameBuffer()
	{
		GLuint frame_buffer;
		glGenFramebuffers(1, &frame_buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
		glDrawBuffer(GL_NONE);
		return frame_buffer;
	}

	static int CreateDepthBufferAttachment(const int & width, const int & height) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
		// Disable writes to the color buffer
		glDrawBuffer(GL_NONE);

		// Disable reads from the color buffer
		glReadBuffer(GL_NONE);
		return texture;
	}

	static int CreateVao()
	{
		GLuint vao_id;
		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);
		return vao_id;
	}

	static GLuint BindIndicesBuffer(const std::vector<unsigned short>& indices)
	{
		GLuint vbo_id;
		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
		return vbo_id;
	}

	static GLuint StoreDataInAttributesList(const int & attributeNumber, const int & coordinateSize, const std::vector<float>& data)
	{
		GLuint vbo_id;
		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, false, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return vbo_id;
	}

	static void UnbindVao()
	{
		glBindVertexArray(0);
	}

	static void EnableCulling()
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	static void DisableCulling()
	{
		glDisable(GL_CULL_FACE);
	}

	static void SimpleRenderVao(const GLuint & vao, int indices, int attributes = 1)
	{
		glBindVertexArray(vao);
		for (int x = 0; x < attributes; x++)
			glEnableVertexAttribArray(x);

		glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_SHORT, 0);

		for (int x = attributes; x > 0; x--)
			glDisableVertexAttribArray(x);
		glBindVertexArray(0);
	}

	static void CreateQuad(GLuint & vao, GLuint & vbo_indices, GLuint & vbo_vertex, GLuint & vbo_text_coord, int & indices_size)
	{
		//std::vector<float> vertex = { -0.5, 0.5, 0, -0.5, -0.5, 0, 0.5, -0.5, 0, 0.5, 0.5, 0 };
		std::vector<float> vertex = { -1,  1, 0,
			-1, -1, 0,
			1, -1, 0,
			1,  1, 0 };
		std::vector<float> text_coords = {
			0, 0,
			0, 1,
			1, 1,
			1, 0 };
		std::vector<unsigned short> indices = { 0, 1, 3, 3, 1, 2 };
		indices_size = indices.size();
		vao = CreateVao();
		vbo_indices = BindIndicesBuffer(indices);
		vbo_vertex = StoreDataInAttributesList(0, 3, vertex);
		vbo_text_coord = StoreDataInAttributesList(1, 2, text_coords);
		UnbindVao();
	}

	static void DeleteQuad(GLuint & vao, GLuint & vbo_indices, GLuint & vbo_vertex, GLuint & vbo_text_coord)
	{
		glDeleteBuffers(1, &vbo_indices);
		glDeleteBuffers(1, &vbo_vertex);
		glDeleteBuffers(1, &vbo_text_coord);
		glDeleteVertexArrays(1, &vao);
	}

}