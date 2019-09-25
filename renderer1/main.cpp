#include "context.h"
#include "render.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"
int main()
{
	context game;
	game.init(640, 480, "Source3");

	vertex triVerts[] = 
	{
		{{-.5f,-.5f,0,1},{0,0,1,0}, {1,0,0,1}, {0.f,0.f} },
		{{.5f,-.5f,0,1}, {0,0,1,0}, {0,1,0,1}, {1.0f,0.f} },
		{{0,.5f,0,1},    {0,0,1,0}, {0,0,1,1}, {.5f,1.f} }


	};
	unsigned int triIndices[] = { 0,1,2 };

	geometry triangle = makeGeometry(triVerts, 3, triIndices, 3);
	const char * basicVert =
		"#version 430\n"
		"layout (location = 0) in vec4 position;\n"

		"layout (location = 2) in vec4 color;\n"
		"out vec4 vColor;\n"

		"layout (location = 3) in vec2 uv;\n"

		"layout (location = 1) in vec4 normal;\n"

		"layout (location = 0) uniform mat4 proj;\n"
		"layout (location = 1) uniform mat4 view;\n"
		"layout (location = 2) uniform mat4 model;\n"


		"out vec2 vUV;\n"
		"out vec3 vNormal;\n"

		"void main() { gl_Position = proj * view * model * position; vColor = color; vUV = uv; vNormal = normalize(model * normal).xyz; }";

	const char * basicFrag =
		"#version 430\n"
		"layout (location = 3) uniform sampler2D albedo;\n"
		"layout (location = 4) uniform vec3 lightDir;\n"

		"in vec2 vUV;\n"
		"in vec3 vNormal;\n"
		"in vec4 vColor;\n"

		"out vec4 vertColor;\n"

		"void main() { vec4 base = texture(albedo, vUV); float d = max(0, dot(vNormal, -lightDir)); vec4 diffuse = d * vec4(1,1,1,1); vertColor = diffuse * base;}";

	shader basicShade = makeShader(basicVert, basicFrag);

	glm::mat4 triModel = glm::identity<glm::mat4>();
	glm::mat4 camProj = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);
	glm::mat4 camView = glm::lookAt(glm::vec3(0, 0, -5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	light sun;
	sun.direction = glm::vec4{ -1,0,0,1 };

	texture triTex = loadTexture("tex.tga");

		setUniform(basicShade, 0, camProj);
		setUniform(basicShade, 1, camView);
		setUniform(basicShade, 3, triTex,0);
		setUniform(basicShade, 4, sun.direction);

	while (!game.shouldClose())
	{
		game.tick();
		game.clear();

		triModel = glm::rotate(triModel, glm::radians(5.0f), glm::vec3(0, 1, 0));
	
		setUniform(basicShade, 2, triModel);

		draw(basicShade, triangle);
		
	}

	game.term();

	   

	return 0;
}