#include "render.h"

#include "glm/gtc/type_ptr.hpp"

#include "TOL/tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
geometry makeGeometry(vertex * verts, size_t vertCount, unsigned * indices, size_t indxCount)
{
	// create an instance of geometry
	geometry newGeo = {};
	newGeo.size = indxCount;

	// generate buffers
	glGenVertexArrays(1, &newGeo.vao); // Vertex Array Object
	glGenBuffers(1, &newGeo.vbo); // Vertex Buffer Object
	glGenBuffers(1, &newGeo.ibo); // index Buffer Object

	// bind buffers
	glBindVertexArray(newGeo.vao);
	glBindBuffer(GL_ARRAY_BUFFER, newGeo.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newGeo.ibo);

	// populate buffers
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(vertex), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indxCount * sizeof(unsigned), indices, GL_STATIC_DRAW);

	// describe vertex data
	glEnableVertexAttribArray(0); // position
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);

	glEnableVertexAttribArray(1); // normals
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex),(void*)16);

	glEnableVertexAttribArray(2); // color
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)32);

	glEnableVertexAttribArray(3); // UVS
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)48);

	// unbind buffers (in a specific order)
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// return the geometry
	return newGeo;
}

void freeGeometry(geometry &geo)
{
	glDeleteBuffers(1, &geo.vbo);
	glDeleteBuffers(1, &geo.ibo);
	glDeleteVertexArrays(1, &geo.vao);
	
	geo = {};
}
void freeShader(shader &shad)
{
	glDeleteProgram(shad.program);
	shad = {};
}

shader makeShader(const char * vertSource, const char * fragSource)
{
										
	// make shader obj
	shader newShad = {};

	newShad.program = glCreateProgram();

	//create shaders
	GLuint vert = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);

	//Compile shaders
	glShaderSource(vert, 1, &vertSource, 0);
	glShaderSource(frag, 1, &fragSource, 0);
	glCompileShader(vert);
	glCompileShader(frag);

	//attach the shaders
	glAttachShader(newShad.program, vert);
	glAttachShader(newShad.program, frag);

	//link the shaders
	glLinkProgram(newShad.program);

	glDeleteShader(vert);
	glDeleteShader(frag);

	return newShad;
}

void draw(const shader &shad, const geometry &geo)
{
	//bind the shader program.
	glUseProgram(shad.program);

	glBindVertexArray(geo.vao);

	//draw
	glDrawElements(GL_TRIANGLES, geo.size, GL_UNSIGNED_INT, 0);
}

void setUniform(const shader & shad, GLuint location, const glm::vec3 & value)
{
	glProgramUniform3fv(shad.program, location, 1, glm::value_ptr(value));

}

void setUniform(const shader &shad, GLuint location, const glm::mat4 &value)
{
	glProgramUniformMatrix4fv(shad.program, location, 1, GL_FALSE, glm::value_ptr(value));
}

void setUniform(const shader & shad, GLuint location, const texture & value, int textureSlot)
{
	//specify the texture slot we're working with
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	//bind the texture to that slot
	glBindTexture(GL_TEXTURE_2D, value.handle);
	//assign the uniform to the shader
	glProgramUniform1i(shad.program, location, textureSlot);
}

texture makeTexture(unsigned width, unsigned height, unsigned channels, const unsigned char *pixels)
{
	GLenum oglFormat = GL_RGBA;
	switch (channels)
	{
	case 1: oglFormat = GL_RED;
		break;
	case 2: oglFormat = GL_RG;
		break;
	case 3: oglFormat = GL_RGB;
		break;
	case 4: oglFormat = GL_RGBA;
		break;
	default:
		//TODO: fatal error, ha the program
		break;
	}

	//generating and binding the texture
	texture tex = { 0, width, height, channels };
	glGenTextures(1, &tex.handle);
	glBindTexture(GL_TEXTURE_2D, tex.handle);


	// buffer/send the actual data
	glTexImage2D(GL_TEXTURE_2D, 0, oglFormat, width, height, 0, oglFormat, GL_UNSIGNED_BYTE, pixels);

	//describe how the texture will be used
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	return tex;
}

void freeTexture(texture & tex)
{
	glDeleteTextures(1, &tex.handle);
	tex = {};
}

texture loadTexture(const char * imagePath)
{
	int imageWidth, imageHeight, imageFormat;
	unsigned char *rawPixelData = nullptr;

	// tell stb image to load image
	stbi_set_flip_vertically_on_load(true);
	rawPixelData = stbi_load(imagePath,
							&imageWidth,
							&imageHeight,
							&imageFormat,
							STBI_default); 
										
	// pass the data to make the image the texture?
	texture tex = makeTexture(imageWidth, imageHeight, imageFormat, rawPixelData);
	//free the image
	return tex;
}
