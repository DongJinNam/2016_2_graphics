// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/controls.hpp>
#include <common/texture.hpp>

//circle�� texture uv ����
GLfloat *makeCircleUVBuffer(GLfloat *arr, GLint sideNums);
//object�� vertex �� * 3��ŭ Color ���� �����ϴ� �Լ�
GLfloat *makeColorBuffer(GLfloat *arr, GLint vertexNum, GLfloat red, GLfloat green, GLfloat blue);
//�� Vertex ���� �����ϴ� �Լ�
GLfloat *makeCircleVertexData(GLfloat *arr, GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides);
//����� ���̵� ���� �����ϴ� �Լ�
GLfloat *makeCylinderSide(GLfloat *arr, GLfloat radius, GLint numberOfSides);
//����� ���̵� �÷� �����ϴ� �Լ�
GLfloat *makeCylinderColor(GLfloat *arr, GLint numberOfSides, GLfloat r, GLfloat g, GLfloat b);
//����� ���̵� UV �����ϴ� �Լ�
GLfloat *makeCylinderUVBuffer(GLfloat *arr, GLint numberOfSides);
//��� ����� ���� �����ϴ� �Լ�
GLfloat *makeUmbrella(GLfloat *arr, GLfloat y, GLfloat radius, GLint numberOfSides);
//��� ����� �÷� �����ϴ� �Լ�
GLfloat *makeUmbrellaColor(GLfloat *arr, GLint vertexNum, GLfloat red, GLfloat green, GLfloat blue);
//��� ����� texture uv �����ϴ� �Լ�
GLfloat *makeUmbrellaUV(GLfloat *arr, GLint vertexNum);
//�ѷ��ڽ�Ʈ Rail ���� ����� �Լ�.
GLfloat *makeRailVertexBuffer(GLfloat *arr, GLint vertexNum);
//�ѷ��ڽ�Ʈ Rail Color ���� ����� �Լ�.
GLfloat *makeRailColorBuffer(GLfloat *arr, GLint vertexNum, GLfloat red, GLfloat green, GLfloat blue);

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Amusement Park", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Dark Red background(������ ��ο� ���������� �����Ѵ�)
	glClearColor(0.5f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Get a handle for our "myTextureSampler" uniform
	// Texture bmp �̹������� bmp ����
	static GLuint TextureFloor = loadBMP_custom("uvtemplate.bmp"); // floor texture
	static GLuint TextureWood = loadBMP_custom("wood.bmp"); // wood texture
	static GLuint TextureYellow = loadBMP_custom("yellow.bmp"); // background yellow texture
	static GLuint TextureStrip = loadBMP_custom("bluestrip.bmp"); // background strip texture
	static GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	GLuint isTexture = glGetUniformLocation(programID, "isTexture");

	// 1. cube ��� vertex data
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	// 2. Rectangle Vertex Data
	static const GLfloat g_rect_vertex_data[] = {
		-1.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, -1.0f
	};

	// 3. Rectangle UV Data
	static const GLfloat g_rect_uv_data[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	//4. cube�� ���� texture uv ��ǥ ���
	static const GLfloat g_uv_buffer_data[] = {
		0.000059f, 1.0f - 0.000004f,
		0.000103f, 1.0f - 0.336048f,
		0.335973f, 1.0f - 0.335903f,
		1.000023f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.336024f, 1.0f - 0.671877f,
		0.667969f, 1.0f - 0.671889f,
		1.000023f, 1.0f - 0.000013f,
		0.668104f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.000059f, 1.0f - 0.000004f,
		0.335973f, 1.0f - 0.335903f,
		0.336098f, 1.0f - 0.000071f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.336024f, 1.0f - 0.671877f,
		1.000004f, 1.0f - 0.671847f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.668104f, 1.0f - 0.000013f,
		0.335973f, 1.0f - 0.335903f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.668104f, 1.0f - 0.000013f,
		0.336098f, 1.0f - 0.000071f,
		0.000103f, 1.0f - 0.336048f,
		0.000004f, 1.0f - 0.671870f,
		0.336024f, 1.0f - 0.671877f,
		0.000103f, 1.0f - 0.336048f,
		0.336024f, 1.0f - 0.671877f,
		0.335973f, 1.0f - 0.335903f,
		0.667969f, 1.0f - 0.671889f,
		1.000004f, 1.0f - 0.671847f,
		0.667979f, 1.0f - 0.335851f
	};

	//******************************************
	//Merry go round(ȸ����) setting start
	//******************************************
	// �� ���̵� ����, ��� ���̵� ����, ������ ���� �������
	const GLint numOfSides = 36;
	const GLint umbrellaOfSides = 8;
	const GLint radius = 1;

	static GLfloat g_circle_buffer_data[38 * 3] = { 0.0f }; // circle�� �����ϴ� vertex buffer set
	makeCircleVertexData(g_circle_buffer_data, 0, 0, 0, radius, 36);

	static GLfloat g_circle_color_data[38 * 3] = { 0.0f }; // circle�� �����ϴ� color buffer set
	makeColorBuffer(g_circle_color_data, 38, 1.0f, 1.0f, 0.0f);

	static GLfloat g_circle_uv_data[38 * 2] = { 0.0f }; // circle�� �����ϴ� texture buffer set
	makeCircleUVBuffer(g_circle_uv_data, 36);

	static GLfloat g_side_buffer_data[36 * 18] = { 0.0f }; // ����� side�� �����ϴ� vertex buffer set
	makeCylinderSide(g_side_buffer_data, radius, numOfSides); // ������ ����(radius)�� ��� ����(numOfSides) �Ű������� �ִ´�.

	static GLfloat g_side_color_data[36 * 18] = { 0.0f }; // ����� side�� �����ϴ� color buffer set
	makeCylinderColor(g_side_color_data, numOfSides, 1.0f, 1.0f, 0.0f); // ��� ����(numOfSides), rgb ���� �Ű������� �ִ´�.

	static GLfloat g_side_uv_data[36 * 12] = { 0.0f }; // ����� side�� �����ϴ� texture buffer set
	makeCylinderUVBuffer(g_side_uv_data, numOfSides);

	static GLfloat umbrella_vertex_data[8 * 9] = { 0.0f }; // ȸ���� �����(�����) vertex buffer set
	makeUmbrella(umbrella_vertex_data, 1.0f, radius, umbrellaOfSides); //�������� ��� ����(umbrellaOfSides)�� �Է¹޴´�.

	static GLfloat umbrella_color_data[8 * 9] = { 0.0f }; // ȸ���� �����(�����) color buffer set
	makeUmbrellaColor(umbrella_color_data, umbrellaOfSides, 0.0f, 1.0f, 0.0f); //��� ����(umbrellaOfSides), rgb ���� �Է¹޴´�.

	static GLfloat umbrella_uv_data[8 * 6] = { 0.0f }; // ȸ���� �����(�����) texture buffer set
	makeUmbrellaUV(umbrella_uv_data, umbrellaOfSides);

	//******************************************
	//Merry go round(ȸ����) setting end
	//******************************************

	//******************************************
	//Roller Coaster setting start
	//******************************************

	static GLfloat rail_vertex_buffer[18 * 37] = { 0.0f };
	makeRailVertexBuffer(rail_vertex_buffer, 37);

	static GLfloat rail_color_buffer[18 * 37] = { 0.0f };
	makeRailColorBuffer(rail_color_buffer, 37, 1.0f, 1.0f, 0.0f);

	//******************************************
	//Roller Coaster setting end
	//******************************************

	//******************************************
	//GL ���α׷����� ����� buffer setting start
	//******************************************

	// cube vertex ������ ��� ����
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// texture color ������ ��� ����
	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	// �ٴ� vertex ������ ��� ����
	GLuint rectVertexBuffer;
	glGenBuffers(1, &rectVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, rectVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_rect_vertex_data), g_rect_vertex_data, GL_STATIC_DRAW);

	// �ٴ� texture ������ ��� ����
	GLuint rectUVBuffer;
	glGenBuffers(1, &rectUVBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, rectUVBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_rect_uv_data), g_rect_uv_data, GL_STATIC_DRAW);

	//******************************************
	//Merry go round buffer setting start
	//******************************************
	GLuint circlebuffer;
	glGenBuffers(1, &circlebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, circlebuffer);
	glBufferData(GL_ARRAY_BUFFER, 38 * 3 * sizeof(GLfloat), g_circle_buffer_data, GL_STATIC_DRAW);

	GLuint circleColor;
	glGenBuffers(1, &circleColor);
	glBindBuffer(GL_ARRAY_BUFFER, circleColor);
	glBufferData(GL_ARRAY_BUFFER, 38 * 3 * sizeof(GLfloat), g_circle_color_data, GL_STATIC_DRAW);

	GLuint circleUV;
	glGenBuffers(1, &circleUV);
	glBindBuffer(GL_ARRAY_BUFFER, circleUV);
	glBufferData(GL_ARRAY_BUFFER, 38 * 2 * sizeof(GLfloat), g_circle_uv_data, GL_STATIC_DRAW);

	// ����� ���̵�
	GLuint sidebuffer;
	glGenBuffers(1, &sidebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, sidebuffer);
	glBufferData(GL_ARRAY_BUFFER, numOfSides * 18 * sizeof(GLfloat), g_side_buffer_data, GL_STATIC_DRAW);

	GLuint side_color_buffer;
	glGenBuffers(1, &side_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, side_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, numOfSides * 18 * sizeof(GLfloat), g_side_color_data, GL_STATIC_DRAW);

	GLuint side_uv_buffer;
	glGenBuffers(1, &side_uv_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, side_uv_buffer);
	glBufferData(GL_ARRAY_BUFFER, numOfSides * 12 * sizeof(GLfloat), g_side_uv_data, GL_STATIC_DRAW);

	// ��� vertex and color ���� ����
	GLuint umbrellaBuffer;
	glGenBuffers(1, &umbrellaBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, umbrellaBuffer);
	glBufferData(GL_ARRAY_BUFFER, 8 * 9 * sizeof(GLfloat), umbrella_vertex_data, GL_STATIC_DRAW);

	GLuint umbrellaColor;
	glGenBuffers(1, &umbrellaColor);
	glBindBuffer(GL_ARRAY_BUFFER, umbrellaColor);
	glBufferData(GL_ARRAY_BUFFER, 8 * 9 * sizeof(GLfloat), umbrella_color_data, GL_STATIC_DRAW);

	GLuint umbrellaUV;
	glGenBuffers(1, &umbrellaUV);
	glBindBuffer(GL_ARRAY_BUFFER, umbrellaUV);
	glBufferData(GL_ARRAY_BUFFER, 8 * 6 * sizeof(GLfloat), umbrella_uv_data, GL_STATIC_DRAW);

	//******************************************
	//Merry go round buffer setting end
	//******************************************

	//******************************************
	//Roller Coaster setting start
	//******************************************

	GLuint railBuffer;
	glGenBuffers(1, &railBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, railBuffer);
	glBufferData(GL_ARRAY_BUFFER, 37 * 18 * sizeof(GLfloat), rail_vertex_buffer, GL_STATIC_DRAW);

	GLuint railColor;
	glGenBuffers(1, &railColor);
	glBindBuffer(GL_ARRAY_BUFFER, railColor);
	glBufferData(GL_ARRAY_BUFFER, 37 * 18 * sizeof(GLfloat), rail_color_buffer, GL_STATIC_DRAW);

	//******************************************
	//Roaller Coaster setting end
	//******************************************

	// For speed computation
	double lastTime = glfwGetTime();
	double lastFrameTime = lastTime;
	vec3 gOrientation1, gOrientation2; // gOrientation1 - ����ŷ�� �����.
	int flag = 0;
	// Merry go round �� ���� ���� ��� Setting Start
	vec3 gPosForMGR7(0.0f, 2.0f, -1.0f);
	vec3 gPosForMGR8(0.0f, -2.0f, 1.0f);
	vec3 gPosForMGR9(2.0f, 0.0f, -0.5f);
	vec3 gPosForMGR10(-2.0f, 0.0f, 0.5f);
	vec3 gOrientForMGRSub;
	int flagForSub1 = 0;
	int flagForSub2 = 0;
	int flagForSub3 = 0;
	int flagForSub4 = 0;
	// Merry go round �� ���� ���� ��� Setting end

	// Roller Coaster �� ���� Cube ��ġ, ȸ�� vec3
	float doublePi = 2.0 * 3.141592f; // 2 * PI

	// Roller Coaster Rail ���� �ִ� 4�� ť�꿡 ������ �ʱ� ����
	float angle = 2.0 * 3.141592f; 
	float angle2 = 2.0 * 3.141592f - (3.141592f / 2.0f);
	float angle3 = 3.141592f;
	float angle4 = 3.141592f / 2.0f;

	float height = 0.0f, height2 = 0.0f, height3 = 0.0f, height4 = 0.0f; // Roller Coaster Rail ���� �ִ� 4�� ť�꿡 ������ ���̰�(y)
	float result = 0.0f, result2 = 0.0f, result3 = 0.0f, result4 = 0.0f; // Roller Coaster Rail ���� �ִ� 4�� ť�꿡 ������ ����
	float yDiff = 0.0f, yDiff2 = 0.0f, yDiff3 = 0.0f, yDiff4 = 0.0f; // Roller Coaster Rail ���� �ִ� 4�� ť�꿡 ���� y�� ����
	float tempY = 0.0f, tempY2 = 0.0f, tempY3 = 0.0f, tempY4 = 0.0f; // Roller Coaster Rail ���� �ִ� 4�� ť�꿡 ���� y���� ���� �ӽð�
	float angVel = 0.0f, angVel2 = 0.0f, angVel3 = 0.0f, angVel4 = 0.0f; // Roller Coaster Rail ���� �ִ� 4�� ť�꿡 ����� �ӽ� ���ӵ�
	// Roller Coaster Rail ���� �ִ� 4�� ť�꿡 ���� �̵� ����
	vec3 gPosForRC(10.0f, 2.0f * (2.0f * cos(doublePi)) + 2.0f, 0.0f);
	vec3 gPosForRC2(10.0f, 2.0f * (2.0f * cos(doublePi)) + 2.0f, 0.0f);
	vec3 gPosForRC3(10.0f, 2.0f * (2.0f * cos(doublePi)) + 2.0f, 0.0f);
	vec3 gPosForRC4(10.0f, 2.0f * (2.0f * cos(doublePi)) + 2.0f, 0.0f);
	// Roller Coaster Rail ���� �ִ� 4�� ť�꿡 ���� ȸ�� ����
	vec3 gOrientForRC(0.0f, 0.0f, 0.0f);
	vec3 gOrientForRC2(0.0f, 0.0f, 0.0f);
	vec3 gOrientForRC3(0.0f, 0.0f, 0.0f);
	vec3 gOrientForRC4(0.0f, 0.0f, 0.0f);

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);
		// Use our shader
		glUseProgram(programID);

		//���������� ȸ���ϱ����� deltaTime���� ���Ѵ�.
		double currentTime = glfwGetTime();
		float deltaTime = (float)(currentTime - lastFrameTime);
		lastFrameTime = currentTime;

		//***************************
		//View ���� ����
		//***************************
		// start Keyboard�� Mouse ���� View ī�޶� ����		
		computeMatricesFromInputs();
		glm::mat4 Projection = getProjectionMatrix();
		glm::mat4 View = getViewMatrix();

		// �ٴ�(Floor) ������ ���� (Floor�� Texture Mapping�� �Ѵ�)
		glm::mat4 ModelFloor = scale(mat4(), vec3(20.0f, 1.0f, 20.0f)) * translate(mat4(), vec3(0.0f, -3.0f, 0.0f)) * glm::mat4(1.0f);
		glm::mat4 basicMVP = Projection * View * ModelFloor; // Remember, matrix multiplication is the other way around		

		//***************************
		//1. Viking ������ ���� ����
		//***************************

		//x,z��鿡 ��ġ ��Ų �� ���� (1�� Transition Matrix �� ����)
		vec3 gPosition1(0.0f, 1.0f, 0.0f);
		vec3 gPosition2(0.0f, 2.0f, 0.0f);
		vec3 gPosForVike(1.1f, 1.5f, 1.1f);
		vec3 gOrientForVike(3.14f / 5.0f, 3.14f / 5.0f, 0.0f);

		gOrientation1.x = 3.14f;
		gOrientation1.y = 0.0f;

		//��谪�� �Ѿ�� flag����
		if (gOrientation1.z > 1.2f)
			flag = 1;
		else if (gOrientation1.z < -1.2f)
			flag = 0;

		//1.2f�� �Ѿ�� +, -1.2f���� �۾����� -
		if (flag == 1) {
			gOrientation1.z -= 3.14159f / 2.0f * deltaTime * (cos(gOrientation1.z) * cos(gOrientation1.z));
		}
		else {
			gOrientation1.z += 3.14159f / 2.0f * deltaTime * (cos(gOrientation1.z) * cos(gOrientation1.z));
		}

		//1-1. ����ŷ ���� ������¸� ����� ���� Matrix ����
		glm::mat4 transMatForVikeAll = translate(mat4(), vec3(-2.0f, 0.0f, 2.0f));
		glm::mat4 transMatForVike1 = translate(mat4(), gPosition1); // A bit to the left
		glm::mat4 rotMatForVike1 = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
		glm::mat4 scalMatForVike1 = scale(mat4(), vec3(0.1f, 1.15f, 0.1f));
		glm::mat4 modelForVike1 = glm::mat4(1.0f);
		modelForVike1 = modelForVike1 * transMatForVikeAll * rotMatForVike1*transMatForVike1* scalMatForVike1;
		glm::mat4 MVPForVike = Projection * View * modelForVike1; // Remember, matrix multiplication is the other way around				

		//1-2. ����ŷ �� ������¸� ����� ���� Matrix ����.
		glm::mat4 transMatForVike2 = translate(mat4(), vec3(gPosition1.x, gPosition1.y - 0.15, gPosition1.z)); // A bit to the left
		glm::mat4 rotMatForVike2 = eulerAngleYXZ(0.0f, 0.0f, 0.0f);
		glm::mat4 scalMatForVike2 = scale(mat4(), vec3(0.6f, 0.1f, 0.6f));
		glm::mat4 modelForVike2 = glm::mat4(1.0f);
		modelForVike2 = modelForVike2 * transMatForVikeAll * scalMatForVike2 * transMatForVike2;
		glm::mat4 MVPForVike2 = Projection * View * modelForVike2; // Remember, matrix multiplication is the other way around

		//1-3. ����ŷ �Ʒ� ������¸� ����� ���� Matrix ����.
		glm::mat4 transMatForVike3 = translate(mat4(), vec3(gPosition2.x, gPosition2.y, gPosition2.z)); // A bit to the left
		glm::mat4 rotMatForVike3 = eulerAngleYXZ(gOrientation1.y, 0.0f, 0.0f);
		glm::mat4 scalMatForVike3 = scale(mat4(), vec3(1.5f, 0.3f, 0.3f));
		glm::mat4 modelForVike3 = glm::mat4(1.0f);
		modelForVike3 = modelForVike3 * transMatForVikeAll * rotMatForVike1 * transMatForVike3 * scalMatForVike3;
		glm::mat4 MVPForVike3 = Projection * View * modelForVike3; // Remember, matrix multiplication is the other way around

		//1-4. ����ŷ ���� ��� Matrix ����
		glm::mat4 rotMatForVike4 = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, 3.14f / 8.0f + gOrientation1.z);
		glm::mat4 modelForVike4 = glm::mat4(1.0f);
		modelForVike4 = modelForVike4 * transMatForVikeAll * rotMatForVike4* transMatForVike1* scalMatForVike1;
		glm::mat4 MVPForVike4 = Projection * View * modelForVike4; // Remember, matrix multiplication is the other way around

		//1-5. ����ŷ ���� ��� Matrix ����
		glm::mat4 rotMatForVike5 = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, -(3.14f / 8.0f) + gOrientation1.z);
		glm::mat4 modelForVike5 = glm::mat4(1.0f);
		modelForVike5 = modelForVike5 * transMatForVikeAll * rotMatForVike5* transMatForVike1 * scalMatForVike1;
		glm::mat4 MVPForVike5 = Projection * View * modelForVike5; // Remember, matrix multiplication is the other way around

		//1-6. ����ŷ õ���� ��ġ�� ���(1)
		glm::mat4 transMatForVike6 = translate(mat4(), vec3(-gPosForVike.x, -gPosForVike.y, -gPosForVike.z));
		glm::mat4 scalMatForVike6 = scale(mat4(), vec3(0.1f, 2.0f, 0.1f));
		glm::mat4 rotMatForVike6 = eulerAngleYXZ(gOrientForVike.y, gOrientForVike.x, gOrientForVike.z);
		glm::mat4 modelForVike6 = transMatForVikeAll * transMatForVike6 * rotMatForVike6 * scalMatForVike6 * glm::mat4(1.0f);
		glm::mat4 MVPForVike6 = Projection * View * modelForVike6; // Remember, matrix multiplication is the other way around				

		//1-6. ����ŷ õ���� ��ġ�� ���(2)
		glm::mat4 transMatForVike7 = translate(mat4(), vec3(-gPosForVike.x, -gPosForVike.y, gPosForVike.z));
		glm::mat4 rotMatForVike7 = eulerAngleYXZ(3.14f - gOrientForVike.y, gOrientForVike.x, gOrientForVike.z);
		glm::mat4 modelForVike7 = transMatForVikeAll * transMatForVike7 * rotMatForVike7 * scalMatForVike6 * glm::mat4(1.0f);
		glm::mat4 MVPForVike7 = Projection * View * modelForVike7; // Remember, matrix multiplication is the other way around				

		//1-6. ����ŷ õ���� ��ġ�� ���(3)		
		glm::mat4 transMatForVike8 = translate(mat4(), vec3(gPosForVike.x, -gPosForVike.y, -gPosForVike.z));
		glm::mat4 rotMatForVike8 = eulerAngleYXZ(3.14f - gOrientForVike.y, 3.14f - gOrientForVike.x, gOrientForVike.z);
		glm::mat4 modelForVike8 = transMatForVikeAll * transMatForVike8 * rotMatForVike8 * scalMatForVike6 * glm::mat4(1.0f);
		glm::mat4 MVPForVike8 = Projection * View * modelForVike8; // Remember, matrix multiplication is the other way around				

																   //1-6. ����ŷ õ���� ��ġ�� ���(4)	
		glm::mat4 transMatForVike9 = translate(mat4(), vec3(gPosForVike.x, -gPosForVike.y, gPosForVike.z));
		glm::mat4 rotMatForVike9 = eulerAngleYXZ(gOrientForVike.y, 3.14f - gOrientForVike.x, gOrientForVike.z);
		glm::mat4 modelForVike9 = transMatForVikeAll * transMatForVike9 * rotMatForVike9 * scalMatForVike6 * glm::mat4(1.0f);
		glm::mat4 MVPForVike9 = Projection * View * modelForVike9; // Remember, matrix multiplication is the other way around				

		//***************************
		//Viking ������ ���� ��
		//***************************

		//***************************
		//2. ȸ���� ������ ���� ����
		//***************************
		vec3 gPosForMGR(6.0f, 0.0f, 6.0f); // ȸ���� ���̱ⱸ ��ü�� ������ translation ����
		glm::mat4 transMatForMGRAll = translate(mat4(), gPosForMGR); // A bit to the left
		gOrientation2.x = 0.0f;
		gOrientation2.y += 3.141592f / 2.0f * deltaTime;
		gOrientation2.z = 0.0f;

		// ���� ��
		glm::mat4 transMatForMGR1 = translate(mat4(), vec3(0.0f, -2.0f, 0.0f)); // A bit to the left
		glm::mat4 rotMatForMGR1 = eulerAngleYXZ(gOrientation2.y, gOrientation2.x, gOrientation2.z);
		glm::mat4 scalMatForMGR1 = scale(mat4(), vec3(3.0f, 1.0f, 3.0f));
		glm::mat4 modelForMGR1 = glm::mat4(1.0f);
		modelForMGR1 = modelForMGR1 * transMatForMGRAll * scalMatForMGR1 * rotMatForMGR1 * transMatForMGR1;
		glm::mat4 MVPForMGR1 = Projection * View * modelForMGR1; // Remember, matrix multiplication is the other way around

		// �ظ� ��
		vec3 gPosForMGR3(0.0f, -3.0f, 0.0f);
		glm::mat4 transMatForMGR3 = translate(mat4(), gPosForMGR3); // A bit to the left
		glm::mat4 modelForMGR3 = glm::mat4(1.0f);
		modelForMGR3 = modelForMGR3 * transMatForMGRAll * scalMatForMGR1 * transMatForMGR3 * rotMatForMGR1;
		glm::mat4 MVPForMGR3 = Projection * View * modelForMGR3; // Remember, matrix multiplication is the other way around	

		// ����� ���̵� �κ�
		vec3 gPosForMGR4(0.0f, -2.5f, 0.0f);
		glm::mat4 transMatForMGR4 = translate(mat4(), gPosForMGR4); // A bit to the left
		glm::mat4 rotMatForMGR4 = eulerAngleYXZ(gOrientation2.y, 0.0f, 0.0f);
		glm::mat4 scalMatForMGR4 = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 modelForMGR4 = glm::mat4(1.0f);
		modelForMGR4 = modelForMGR4 * transMatForMGRAll * scalMatForMGR1 * rotMatForMGR4 * transMatForMGR4;
		glm::mat4 MVPForMGR4 = Projection * View * modelForMGR4; // Remember, matrix multiplication is the other way around	

		// ��� ��� ������ �κ�
		vec3 gPosForMGR5(0.0f, 0.0f, 0.0f);
		glm::mat4 transMatForMGR5 = translate(mat4(), gPosForMGR5); // A bit to the left
		glm::mat4 scalMatForMGR5 = scale(mat4(), vec3(0.1f, 4.0f, 0.1f));
		glm::mat4 modelForMGR5 = glm::mat4(1.0f);
		modelForMGR5 = modelForMGR5 * transMatForMGRAll * scalMatForMGR5 * rotMatForMGR4 * transMatForMGR5;
		glm::mat4 MVPForMGR5 = Projection * View * modelForMGR5; // Remember, matrix multiplication is the other way around	

		// ��� ���
		vec3 gPosForMGR6(0.0f, 1.5f, 0.0f);
		glm::mat4 transMatForMGR6 = translate(mat4(), gPosForMGR6); // A bit to the left
		glm::mat4 scalMatForMGR6 = scale(mat4(), vec3(2.0f, 1.0f, 2.0f));
		glm::mat4 modelForMGR6 = glm::mat4(1.0f);
		modelForMGR6 = modelForMGR6 * transMatForMGRAll * scalMatForMGR6 * rotMatForMGR4 * transMatForMGR6;
		glm::mat4 MVPForMGR6 = Projection * View * modelForMGR6; // Remember, matrix multiplication is the other way around	

		//Merry go round �� ���� ���� ��� 4���� ���� Matrix ����
		//��谪�� �Ѿ�� flag����. ȸ������ ���� ��� �����ϱ� ���� �ڵ�
		if (gPosForMGR7.z < -2.0f)
			flagForSub1 = 1;
		else if (gPosForMGR7.z > -1.0f)
			flagForSub1 = 0;

		//-5.0f���� �۾����� +, -1.0f���� Ŀ���� -
		if (flagForSub1 == 1)
			gPosForMGR7.z += 1.0f*deltaTime;
		else
			gPosForMGR7.z -= 1.0f*deltaTime;

		glm::mat4 transMatForY = translate(mat4(), vec3(0.0f, -3.0f, 0.0f));

		// merry go round�� �ִ� 4���� ���� ���� ��� Matrix ����
		glm::mat4 transMatForMGR7 = translate(mat4(), gPosForMGR7); // A bit to the left
		glm::mat4 rotMatForMGR7 = eulerAngleYXZ(gOrientForMGRSub.y, 1.57f, gOrientForMGRSub.z);
		glm::mat4 scalMatForMGR7 = scale(mat4(), vec3(0.4f, 1.6f, 0.4f));
		glm::mat4 modelForMGR7 = glm::mat4(1.0f);
		modelForMGR7 = modelForMGR7 * transMatForMGRAll * rotMatForMGR4 * transMatForY * rotMatForMGR7
			* transMatForMGR7 * rotMatForMGR7 * scalMatForMGR7;
		glm::mat4 MVPForMGR7 = Projection * View * modelForMGR7; // Remember, matrix multiplication is the other way around	


		//��谪�� �Ѿ�� flag����. ȸ������ ���� ��� �����ϱ� ���� �ڵ�
		if (gPosForMGR8.z < -2.0f)
			flagForSub2 = 1;
		else if (gPosForMGR8.z > -1.0f)
			flagForSub2 = 0;

		//-5.0f���� �۾����� +, -1.0f���� Ŀ���� -
		if (flagForSub2 == 1)
			gPosForMGR8.z += 1.0f*deltaTime;
		else
			gPosForMGR8.z -= 1.0f*deltaTime;

		glm::mat4 transMatForMGR8 = translate(mat4(), gPosForMGR8); // A bit to the left
		glm::mat4 modelForMGR8 = glm::mat4(1.0f);
		modelForMGR8 = modelForMGR8 * transMatForMGRAll * rotMatForMGR4 * transMatForY
			* rotMatForMGR7 * transMatForMGR8 * rotMatForMGR7 * scalMatForMGR7;
		glm::mat4 MVPForMGR8 = Projection * View * modelForMGR8;

		//��谪�� �Ѿ�� flag����. ȸ������ ���� ��� �����ϱ� ���� �ڵ�
		if (gPosForMGR9.z < -2.0f)
			flagForSub3 = 1;
		else if (gPosForMGR9.z > -1.0f)
			flagForSub3 = 0;

		//-5.0f���� �۾����� +, -1.0f���� Ŀ���� -
		if (flagForSub3 == 1)
			gPosForMGR9.z += 1.0f*deltaTime;
		else
			gPosForMGR9.z -= 1.0f*deltaTime;

		glm::mat4 transMatForMGR9 = translate(mat4(), gPosForMGR9); // A bit to the left
		glm::mat4 modelForMGR9 = glm::mat4(1.0f);
		modelForMGR9 = modelForMGR9 * transMatForMGRAll * rotMatForMGR4 * transMatForY
			* rotMatForMGR7 * transMatForMGR9 * rotMatForMGR7 * scalMatForMGR7;
		glm::mat4 MVPForMGR9 = Projection * View * modelForMGR9;

		//��谪�� �Ѿ�� flag����. ȸ������ ���� ��� �����ϱ� ���� �ڵ�
		if (gPosForMGR10.z < -2.0f)
			flagForSub4 = 1;
		else if (gPosForMGR10.z > -1.0f)
			flagForSub4 = 0;

		//-5.0f���� �۾����� +, -1.0f���� Ŀ���� -
		if (flagForSub4 == 1)
			gPosForMGR10.z += 1.0f*deltaTime;
		else
			gPosForMGR10.z -= 1.0f*deltaTime;

		glm::mat4 transMatForMGR10 = translate(mat4(), gPosForMGR10); // A bit to the left
		glm::mat4 modelForMGR10 = glm::mat4(1.0f);
		modelForMGR10 = modelForMGR10 * transMatForMGRAll * rotMatForMGR4 * transMatForY
			* rotMatForMGR7 * transMatForMGR10 * rotMatForMGR7 * scalMatForMGR7;
		glm::mat4 MVPForMGR10 = Projection * View * modelForMGR10;

		// ���� ����� ���� �ö� cube�� ���� Matrix
		glm::mat4 transMatForMGR11 = translate(mat4(), vec3(0.0f, 1.0f, 0.0f)); // A bit to the left
		glm::mat4 scalMatForMGR11 = scale(mat4(), vec3(0.5f, 0.5f, 0.5f));
		glm::mat4 modelForMGR11 = glm::mat4(1.0f);
		modelForMGR11 = modelForMGR11 * transMatForMGR11 * transMatForMGRAll * rotMatForMGR4 * transMatForY * rotMatForMGR7
			* transMatForMGR7 * rotMatForMGR7 * scalMatForMGR11;
		glm::mat4 MVPForMGR11 = Projection * View * modelForMGR11;

		// ���� ����� ���� �ö� cube�� ���� Matrix
		glm::mat4 modelForMGR12 = glm::mat4(1.0f);
		modelForMGR12 = modelForMGR12 * transMatForMGR11 * transMatForMGRAll * rotMatForMGR4 * transMatForY * rotMatForMGR7
			* transMatForMGR8 * rotMatForMGR7 * scalMatForMGR11;
		glm::mat4 MVPForMGR12 = Projection * View * modelForMGR12;

		// ���� ����� ���� �ö� cube�� ���� Matrix
		glm::mat4 modelForMGR13 = glm::mat4(1.0f);
		modelForMGR13 = modelForMGR13 * transMatForMGR11 * transMatForMGRAll * rotMatForMGR4 * transMatForY * rotMatForMGR7
			* transMatForMGR9 * rotMatForMGR7 * scalMatForMGR11;
		glm::mat4 MVPForMGR13 = Projection * View * modelForMGR13;

		// ���� ����� ���� �ö� cube�� ���� Matrix
		glm::mat4 modelForMGR14 = glm::mat4(1.0f);
		modelForMGR14 = modelForMGR14 * transMatForMGR11 * transMatForMGRAll * rotMatForMGR4 * transMatForY * rotMatForMGR7
			* transMatForMGR10 * rotMatForMGR7 * scalMatForMGR11;
		glm::mat4 MVPForMGR14 = Projection * View * modelForMGR14;

		//***************************
		//2. ȸ���� ������ ���� ��
		//***************************

		//***************************
		//3. �ѷ��ڽ��� ������ ���� ����
		//***************************
		if (angle <= 0.0f) angle = doublePi;
		if (angle2 <= 0.0f) angle2 = doublePi;
		if (angle3 <= 0.0f) angle3 = doublePi;
		if (angle4 <= 0.0f) angle4 = doublePi;

		// ���ӵ� ���� �� ���� �׽�Ʈ (Rail ���� �ִ� 4�� ť�꿡 ���� ��� �׽�Ʈ�Ѵ�.)
		angle -= 0.5f * deltaTime;
		angle2 -= 0.5f * deltaTime;
		angle3 -= 0.5f * deltaTime;
		angle4 -= 0.5f * deltaTime;

		result = doublePi - angle * 2.0f;
		height = 2.0f * (3.0f * sin(result) / result + 2.0f * cos(result));

		result2 = doublePi - angle2 * 2.0f;
		height2 = 2.0f * (3.0f * sin(result2) / result2 + 2.0f * cos(result2));

		result3 = doublePi - angle3 * 2.0f;
		height3 = 2.0f * (3.0f * sin(result3) / result3 + 2.0f * cos(result3));

		result4 = doublePi - angle4 * 2.0f;
		height4 = 2.0f * (3.0f * sin(result4) / result4 + 2.0f * cos(result4));

		gPosForRC.x = 10 * cos(angle);
		gPosForRC.y = 2.0f * (3.0f * sin(result) / result + 2.0f * cos(result));
		gPosForRC.z = 10 * sin(angle);

		gPosForRC2.x = 10 * cos(angle2);
		gPosForRC2.y = 2.0f * (3.0f * sin(result2) / result2 + 2.0f * cos(result2));
		gPosForRC2.z = 10 * sin(angle2);

		gPosForRC3.x = 10 * cos(angle3);
		gPosForRC3.y = 2.0f * (3.0f * sin(result3) / result3 + 2.0f * cos(result3));
		gPosForRC3.z = 10 * sin(angle3);

		gPosForRC4.x = 10 * cos(angle4);
		gPosForRC4.y = 2.0f * (3.0f * sin(result4) / result4 + 2.0f * cos(result4));
		gPosForRC4.z = 10 * sin(angle4);

		yDiff = gPosForRC.y - tempY;
		yDiff2 = gPosForRC2.y - tempY2;
		yDiff3 = gPosForRC3.y - tempY3;
		yDiff4 = gPosForRC4.y - tempY4;

		angVel = atan((yDiff / (0.5f * deltaTime) / 8.0f));
		angVel2 = atan((yDiff2 / (0.5f * deltaTime) / 8.0f));
		angVel3 = atan((yDiff3 / (0.5f * deltaTime) / 8.0f));
		angVel4 = atan((yDiff4 / (0.5f * deltaTime) / 8.0f));

		tempY = gPosForRC.y;
		tempY2 = gPosForRC2.y;
		tempY3 = gPosForRC3.y;
		tempY4 = gPosForRC4.y;

		// ���� ���ӵ� ������ �׽�Ʈ�� �߱� ������, angle ���� ���� ������ ������.
		angle += 0.5f * deltaTime;
		angle2 += 0.5f * deltaTime;
		angle3 += 0.5f * deltaTime;
		angle4 += 0.5f * deltaTime;

		// ���� ������ ������ Ȱ���ؼ� ���ӵ��� �����ϰ�, �̸� �̵��ӵ��� �����Ѵ�. (4�� Cube ��� ����)
		angle -= (0.5f - (0.25f * (angVel))) * deltaTime;
		angle2 -= (0.5f - (0.25f * (angVel2))) * deltaTime;
		angle3 -= (0.5f - (0.25f * (angVel3))) * deltaTime;
		angle4 -= (0.5f - (0.25f * (angVel4))) * deltaTime;

		result = doublePi - angle * 2.0f;
		height = 2.0f * (3.0f * sin(result) / result + 2.0f * cos(result));

		result2 = doublePi - angle2 * 2.0f;
		height2 = 2.0f * (3.0f * sin(result2) / result2 + 2.0f * cos(result2));

		result3 = doublePi - angle3 * 2.0f;
		height3 = 2.0f * (3.0f * sin(result3) / result3 + 2.0f * cos(result3));

		result4 = doublePi - angle4 * 2.0f;
		height4 = 2.0f * (3.0f * sin(result4) / result4 + 2.0f * cos(result4));

		// Cube ���� x,y,z ��ǥ ���� (4�� Cube ��� ����)
		gPosForRC.x = 10 * cos(angle);
		gPosForRC.y = 2.0f * (3.0f * sin(result) / result + 2.0f * cos(result));
		gPosForRC.z = 10 * sin(angle);

		gPosForRC2.x = 10 * cos(angle2);
		gPosForRC2.y = 2.0f * (3.0f * sin(result2) / result2 + 2.0f * cos(result2));
		gPosForRC2.z = 10 * sin(angle2);

		gPosForRC3.x = 10 * cos(angle3);
		gPosForRC3.y = 2.0f * (3.0f * sin(result3) / result3 + 2.0f * cos(result3));
		gPosForRC3.z = 10 * sin(angle3);

		gPosForRC4.x = 10 * cos(angle4);
		gPosForRC4.y = 2.0f * (3.0f * sin(result4) / result4 + 2.0f * cos(result4));
		gPosForRC4.z = 10 * sin(angle4);

		yDiff = gPosForRC.y - tempY;
		yDiff2 = gPosForRC2.y - tempY2;
		yDiff3 = gPosForRC3.y - tempY3;
		yDiff4 = gPosForRC4.y - tempY4;


		// Cube�� ���� ȸ�� vector ���� (4�� Cube ��� ����)
		if (angVel > 0.0f) // Cube �� Rail �󿡼� ��� ������ �ִ� ���
			gOrientForRC.x = -atan(yDiff / ((0.5f - (0.25f * (angVel))) * deltaTime) / 8.0f);
		else // Cube �� Rail �󿡼� �ϰ� ������ �ִ� ���
			gOrientForRC.x = atan(yDiff / ((0.5f - (0.25f * (angVel))) * deltaTime) / 4.0f);

		if (angVel2 > 0.0f)
			gOrientForRC2.x = -atan(yDiff2 / ((0.5f - (0.25f * (angVel2))) * deltaTime) / 8.0f);
		else
			gOrientForRC2.x = atan(yDiff2 / ((0.5f - (0.25f * (angVel2))) * deltaTime) / 4.0f);

		if (angVel3 > 0.0f)
			gOrientForRC3.x = -atan(yDiff3 / ((0.5f - (0.25f * (angVel3))) * deltaTime) / 8.0f);
		else
			gOrientForRC3.x = atan(yDiff3 / ((0.5f - (0.25f * (angVel3))) * deltaTime) / 4.0f);

		if (angVel4 > 0.0f)
			gOrientForRC4.x = -atan(yDiff4 / ((0.5f - (0.25f * (angVel4))) * deltaTime) / 8.0f);
		else
			gOrientForRC4.x = atan(yDiff4 / ((0.5f - (0.25f * (angVel4))) * deltaTime) / 4.0f);

		tempY = gPosForRC.y;
		tempY2 = gPosForRC2.y;
		tempY3 = gPosForRC3.y;
		tempY4 = gPosForRC4.y;

		// �ѷ��ڽ��� ��ü�� ��ü �̵�
		glm::mat4 transMatAllForRC = translate(mat4(), vec3(4.0f, 4.0f, 4.0f));

		// �ѷ��ڽ��� Rail ����
		glm::mat4 transMatForRC1 = translate(mat4(), vec3(0.0f, 0.15f, 0.0f));
		glm::mat4 modelForRC1 = glm::mat4(1.0f);
		glm::mat4 scalMatForRC1 = scale(mat4(), vec3(10.0f, 2.0f, 10.0f));
		modelForRC1 = modelForRC1 * transMatAllForRC * transMatForRC1 * scalMatForRC1;
		glm::mat4 MVPForRC1 = Projection * View * modelForRC1;

		// �ѷ��ڽ��Ϳ� ���� Cube ����		
		glm::mat4 modelForRC2 = glm::mat4(1.0f);
		glm::mat4 transMatForRC2 = translate(mat4(), vec3(10 * cos(angle), height + 0.75f, 10 * sin(angle)));
		glm::mat4 rotMatForRC2 = eulerAngleYXZ(gOrientForRC.y - angle, gOrientForRC.x, gOrientForRC.z);
		glm::mat4 scalMatForRC2 = scale(mat4(), vec3(0.5f, 0.5f, 1.0f));
		modelForRC2 = modelForRC2 * transMatAllForRC * transMatForRC2 * rotMatForRC2 *scalMatForRC2;
		glm::mat4 MVPForRC2 = Projection * View * modelForRC2;

		// �ѷ��ڽ��Ϳ� ���� Cube ���� (2)
		glm::mat4 modelForRC2_2 = glm::mat4(1.0f);
		glm::mat4 transMatForRC2_2 = translate(mat4(), vec3(10 * cos(angle2), height2 + 0.75f, 10 * sin(angle2)));
		glm::mat4 rotMatForRC2_2 = eulerAngleYXZ(gOrientForRC2.y - angle2, gOrientForRC2.x, gOrientForRC2.z);
		modelForRC2_2 = modelForRC2_2 * transMatAllForRC * transMatForRC2_2 * rotMatForRC2_2 * scalMatForRC2;
		glm::mat4 MVPForRC2_2 = Projection * View * modelForRC2_2;

		// �ѷ��ڽ��Ϳ� ���� Cube ���� (3)
		glm::mat4 modelForRC2_3 = glm::mat4(1.0f);
		glm::mat4 transMatForRC2_3 = translate(mat4(), vec3(10 * cos(angle3), height3 + 0.75f, 10 * sin(angle3)));
		glm::mat4 rotMatForRC2_3 = eulerAngleYXZ(gOrientForRC3.y - angle3, gOrientForRC3.x, gOrientForRC3.z);
		modelForRC2_3 = modelForRC2_3 * transMatAllForRC * transMatForRC2_3 * rotMatForRC2_3 * scalMatForRC2;
		glm::mat4 MVPForRC2_3 = Projection * View * modelForRC2_3;

		// �ѷ��ڽ��Ϳ� ���� Cube ���� (4)
		glm::mat4 modelForRC2_4 = glm::mat4(1.0f);
		glm::mat4 transMatForRC2_4 = translate(mat4(), vec3(10 * cos(angle4), height4 + 0.75f, 10 * sin(angle4)));
		glm::mat4 rotMatForRC2_4 = eulerAngleYXZ(gOrientForRC4.y - angle4, gOrientForRC4.x, gOrientForRC4.z);
		modelForRC2_4 = modelForRC2_4 * transMatAllForRC * transMatForRC2_4 * rotMatForRC2_4 * scalMatForRC2;
		glm::mat4 MVPForRC2_4 = Projection * View * modelForRC2_4;

		// Rail ��ħ�� cube ���� (1)
		glm::mat4 modelForRC4 = glm::mat4(1.0f);
		glm::mat4 transMatForRC4 = translate(mat4(), vec3(10.0f * cos(doublePi),
			2 * (3.0f * sin(doublePi) / doublePi + 2.0f * cos(doublePi)) - 6.0f, 10 * sin(doublePi)));
		glm::mat4 scalMatForRC4 = scale(mat4(), vec3(0.5f, 6.0f, 0.5f));
		modelForRC4 = modelForRC4 * transMatAllForRC * transMatForRC4 *scalMatForRC4;
		glm::mat4 MVPForRC4 = Projection * View * modelForRC4;

		// Rail ��ħ�� cube ���� (2)
		glm::mat4 modelForRC5 = glm::mat4(1.0f);
		glm::mat4 transMatForRC5 = translate(mat4(), vec3(10.0f * cos(doublePi / 2.0f),
			1.0f, 10 * sin(doublePi / 2.0f)));
		glm::mat4 scalMatForRC5 = scale(mat4(), vec3(0.5f, 9.0f, 0.5f));
		modelForRC5 = modelForRC5 * transMatAllForRC * transMatForRC5 *scalMatForRC5;
		glm::mat4 MVPForRC5 = Projection * View * modelForRC5;

		// Rail ��ħ�� cube ���� (3)
		glm::mat4 modelForRC6 = glm::mat4(1.0f);
		glm::mat4 transMatForRC6 = translate(mat4(), vec3(10.0f * cos(doublePi / 4.0f - 0.125f),
			2 * (3.0f * sin(-doublePi / 2.0f) / (-doublePi / 2.0f) + 2.0f * cos(-doublePi / 2.0f)) - 6.15f, 10 * sin((doublePi / 4.0f) - 0.125f)));
		glm::mat4 scalMatForRC6 = scale(mat4(), vec3(0.5f, 6.0f, 0.5f));
		modelForRC6 = modelForRC6 * transMatAllForRC * transMatForRC6 *scalMatForRC6;
		glm::mat4 MVPForRC6 = Projection * View * modelForRC6;

		// Rail ��ħ�� cube ���� (4)
		glm::mat4 modelForRC7 = glm::mat4(1.0f);
		glm::mat4 transMatForRC7 = translate(mat4(), vec3(10.0f * cos(doublePi / 4.0f * 3.0f + 0.125f),
			2 * (3.0f * sin(doublePi / 2.0f) / (doublePi / 2.0f) + 2.0f * cos(doublePi / 2.0f)) - 6.15f, 10 * sin(doublePi / 4.0f * 3.0f + 0.125f)));
		glm::mat4 scalMatForRC7 = scale(mat4(), vec3(0.5f, 6.0f, 0.5f));
		modelForRC7 = modelForRC7 * transMatAllForRC * transMatForRC7 *scalMatForRC7;
		glm::mat4 MVPForRC7 = Projection * View * modelForRC7;

		//***************************
		//3. �ѷ��ڽ��� ������ ���� ��
		//***************************

		//*********************************
		// �� �κ� ���ķδ� ������ ��Ʈ�Դϴ�.
		//*********************************

		// �ٴ� �׸���
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &basicMVP[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureFloor);
		glUniform1i(TextureID, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, rectVertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// Texture �̹����� color�� ����� �ÿ� layout 2�� ����ϹǷ� �Ʒ��� ���� ����Ѵ�.
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, rectUVBuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 2 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		//***********************
		// Viking Rendering ����
		//***********************
		// ����ŷ õ���� ��ġ�� 4�� ��� �׸��� (MVPForVike6,MVPForVike7, MVPForVike8, MVPForVike9)
		// ����ŷ ���� ��� 2�� �׸���(�밢�� ��� �׸���) (MVPForVike4, MVPForVike5)
		// ����ŷ �Ʒ� ��� �׸��� (MVPForVike3)
		// ����ŷ �� ��� �׸��� (MVPForVike2)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForVike2[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureYellow); // viking �� �ֻ�� �κ��� yellow texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForVike3[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWood); // viking �� �ֻ�� �κ��� yellow texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForVike4[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWood); // viking �� �ֻ�� �κ��� yellow texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForVike5[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWood); // viking �� �ֻ�� �κ��� yellow texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForVike6[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWood); // viking �� �ֻ�� �κ��� yellow texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForVike7[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWood); // viking �� �ֻ�� �κ��� yellow texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForVike8[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWood); // viking �� �ֻ�� �κ��� yellow texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForVike9[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWood); // viking �� �ֻ�� �κ��� yellow texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		//***********************
		// Viking Rendering ������
		//***********************

		//***********************
		// Merry-go-round Rendering ����
		//***********************
		// 2-1. ȸ���� ����� �� �� �׸���. (MVPForMGR1)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForMGR1[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureYellow); // viking �� �ֻ�� �κ��� yellow texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, circlebuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, circleUV);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(g_circle_buffer_data) / sizeof(GLfloat));
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		// 2-2. ȸ���� ����� �� �� �׸���.
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForMGR3[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureYellow); // viking �� �ֻ�� �κ��� yellow texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, circlebuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, circleUV);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(g_circle_buffer_data) / sizeof(GLfloat));
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		// 2-3. ����� ���̵� �׸��� with Texture Wood
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForMGR4[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWood); // viking �� �ֻ�� �κ��� yellow texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, sidebuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, side_uv_buffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 18 * 36);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		//2-4 2��° ����� �׸��� with texture
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForMGR5[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWood); // viking �� �ֻ�� �κ��� yellow texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, sidebuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, side_uv_buffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 18 * 36);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		// 2-5. ��� �׸���(Texture)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForMGR6[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureYellow); // viking �� �ֻ�� �κ��� yellow texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, umbrellaBuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, umbrellaUV);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 8 * 9); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		//2-6. ���� ����� �׸��� with texture
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForMGR7[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWood); // viking �� �ֻ�� �κ��� yellow texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, sidebuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, side_uv_buffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 18 * 36);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		//2-7. ���� ����� �׸��� with texture
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForMGR8[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWood); // viking �� �ֻ�� �κ��� yellow texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, sidebuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, side_uv_buffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 18 * 36);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		//2-8. ���� ����� �׸��� with texture
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForMGR9[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWood); // viking �� �ֻ�� �κ��� yellow texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, sidebuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, side_uv_buffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 18 * 36);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		//2-9. ���� ����� �׸���
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForMGR10[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWood); // viking �� �ֻ�� �κ��� yellow texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, sidebuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, side_uv_buffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 18 * 36);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		//2-10. ����� ���� �ö� ť�� �׸���
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForMGR11[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, TextureYellow); // ȸ���� cube �κ��� strip texture�� mapping �Ѵ�.
		glBindTexture(GL_TEXTURE_2D, TextureStrip); // ȸ���� cube �κ��� strip texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		//2-11. ����� ���� �ö� ť�� �׸���
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForMGR12[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, TextureYellow); // ȸ���� cube �κ��� strip texture�� mapping �Ѵ�.
		glBindTexture(GL_TEXTURE_2D, TextureStrip); // ȸ���� cube �κ��� strip texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		//2-12. ����� ���� �ö� ť�� �׸���
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForMGR13[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, TextureYellow); // ȸ���� cube �κ��� strip texture�� mapping �Ѵ�.
		glBindTexture(GL_TEXTURE_2D, TextureStrip); // ȸ���� cube �κ��� strip texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		//2-13. ����� ���� �ö� ť�� �׸���
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForMGR14[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureStrip); // ȸ���� cube �κ��� strip texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		//***********************
		// Merry-go-round Rendering ������
		//***********************

		//***********************
		// 3. Roller Coaster Rendering ����
		//***********************

		//3-1. Rail �׸���
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForRC1[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), false);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, railBuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, railColor);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 18 * 37);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		//3-2. Roller Coaster Cube �׸���
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForRC2[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureStrip); // �ѷ��ڽ�Ʈ cube �κ��� strip texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		// 3-3 Roller Coaster Cube �׸��� (2)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForRC2_2[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, TextureYellow); // �ѷ��ڽ�Ʈ cube �κ��� strip texture�� mapping �Ѵ�.
		glBindTexture(GL_TEXTURE_2D, TextureStrip); // �ѷ��ڽ�Ʈ cube �κ��� strip texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		// 3-4 Roller Coaster Cube �׸��� (2)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForRC2_3[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, TextureYellow); // �ѷ��ڽ�Ʈ cube �κ��� strip texture�� mapping �Ѵ�.
		glBindTexture(GL_TEXTURE_2D, TextureStrip); // �ѷ��ڽ�Ʈ cube �κ��� strip texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		// 3-5 Roller Coaster Cube �׸��� (2)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForRC2_4[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, TextureYellow); // �ѷ��ڽ�Ʈ cube �κ��� strip texture�� mapping �Ѵ�.
		glBindTexture(GL_TEXTURE_2D, TextureStrip); // �ѷ��ڽ�Ʈ cube �κ��� strip texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		// 3-6 Roller Coaster Cube �׸��� (Rail ��ħ) (1)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForRC4[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWood); // �ѷ��ڽ�Ʈ cube �κ��� strip texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		// 3-7 Roller Coaster Cube �׸��� (Rail ��ħ) (2)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForRC5[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWood); // �ѷ��ڽ�Ʈ cube �κ��� strip texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		// 3-8 Roller Coaster Cube �׸��� (Rail ��ħ) (3)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForRC6[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWood); // �ѷ��ڽ�Ʈ cube �κ��� strip texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		// 3-9 Roller Coaster Cube �׸��� (Rail ��ħ) (4)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPForRC7[0][0]);
		glUniform1i(glGetUniformLocation(programID, "isTexture"), true);

		// Texture setting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureWood); // �ѷ��ڽ�Ʈ cube �κ��� strip texture�� mapping �Ѵ�.
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		//***********************
		// Roller Coaster Rendering ��
		//***********************		

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &rectVertexBuffer);
	glDeleteBuffers(1, &rectUVBuffer);
	glDeleteBuffers(1, &circlebuffer);
	glDeleteBuffers(1, &circleColor);
	glDeleteBuffers(1, &circleUV);
	glDeleteBuffers(1, &sidebuffer);
	glDeleteBuffers(1, &side_color_buffer);
	glDeleteBuffers(1, &side_uv_buffer);
	glDeleteBuffers(1, &umbrellaBuffer);
	glDeleteBuffers(1, &umbrellaColor);
	glDeleteBuffers(1, &umbrellaUV);
	glDeleteBuffers(1, &railBuffer);
	glDeleteBuffers(1, &railColor);
	glDeleteProgram(programID);
	glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

// Circle�� ���� Vertex Data �����
GLfloat* makeCircleVertexData(GLfloat *arr, GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides)
{
	GLint numberOfVertices = numberOfSides + 2;
	GLfloat doublePi = 2.0f * 3.141592f;
	GLfloat *allCircleVertices = arr;

	allCircleVertices[0] = 0.0f;
	allCircleVertices[1] = y;
	allCircleVertices[2] = 0.0f;

	for (int i = 0; i < numberOfVertices; i++)
	{
		allCircleVertices[3 * (i + 1)] = x + (radius * cos(i * doublePi / numberOfSides));
		allCircleVertices[3 * (i + 1) + 1] = y;
		allCircleVertices[3 * (i + 1) + 2] = z + (radius * sin(i * doublePi / numberOfSides));
	}
	return allCircleVertices;
}

// Cylinder Side �׸���
GLfloat *makeCylinderSide(GLfloat *arr, GLfloat radius, GLint numberOfSides) {

	GLfloat doublePi = 2.0f * 3.141592f;
	GLfloat *allSideVertices = arr;

	for (int i = 0; i < numberOfSides; i++)
	{
		// x,y,z �迭���� ������ ����
		// return �� ���ۿ� ���� ���� ����
		allSideVertices[18 * i] = (radius * cos(i * doublePi / numberOfSides));
		allSideVertices[18 * i + 1] = radius / 2.0f;
		allSideVertices[18 * i + 2] = (radius * sin(i * doublePi / numberOfSides));

		allSideVertices[18 * i + 3] = (radius * cos(i * doublePi / numberOfSides));
		allSideVertices[18 * i + 4] = -radius / 2.0f;
		allSideVertices[18 * i + 5] = (radius * sin(i * doublePi / numberOfSides));

		allSideVertices[18 * i + 6] = (radius * cos((i + 1) * doublePi / numberOfSides));
		allSideVertices[18 * i + 7] = radius / 2.0f;
		allSideVertices[18 * i + 8] = (radius * sin((i + 1) * doublePi / numberOfSides));

		allSideVertices[18 * i + 9] = (radius * cos((i + 1) * doublePi / numberOfSides));
		allSideVertices[18 * i + 10] = radius / 2.0f;
		allSideVertices[18 * i + 11] = (radius * sin((i + 1) * doublePi / numberOfSides));

		allSideVertices[18 * i + 12] = (radius * cos(i * doublePi / numberOfSides));
		allSideVertices[18 * i + 13] = -radius / 2.0f;
		allSideVertices[18 * i + 14] = (radius * sin(i * doublePi / numberOfSides));

		allSideVertices[18 * i + 15] = (radius * cos((i + 1) * doublePi / numberOfSides));
		allSideVertices[18 * i + 16] = -radius / 2.0f;
		allSideVertices[18 * i + 17] = (radius * sin((i + 1) * doublePi / numberOfSides));
	}
	return allSideVertices;
}

// Cylinder Color �׸���
GLfloat *makeCylinderColor(GLfloat *arr, GLint numberOfSides, GLfloat r, GLfloat g, GLfloat b) {
	GLfloat *cylinderColor = arr;
	for (int i = 0; i < numberOfSides; i++)
	{
		cylinderColor[18 * i] = r;
		cylinderColor[18 * i + 1] = g;
		cylinderColor[18 * i + 2] = b;

		cylinderColor[18 * i + 3] = r;
		cylinderColor[18 * i + 4] = g;
		cylinderColor[18 * i + 5] = b;

		cylinderColor[18 * i + 6] = r;
		cylinderColor[18 * i + 7] = g;
		cylinderColor[18 * i + 8] = b;

		cylinderColor[18 * i + 9] = r;
		cylinderColor[18 * i + 10] = g;
		cylinderColor[18 * i + 11] = b;

		cylinderColor[18 * i + 12] = r;
		cylinderColor[18 * i + 13] = g;
		cylinderColor[18 * i + 14] = b;

		cylinderColor[18 * i + 15] = r;
		cylinderColor[18 * i + 16] = g;
		cylinderColor[18 * i + 17] = b;
	}
	return cylinderColor;
}

// Cylinder UV Buffer �׸���
GLfloat *makeCylinderUVBuffer(GLfloat *arr, GLint numberOfSides) {
	GLfloat *allSideUVs = arr;

	for (int i = 0; i < numberOfSides; i++)
	{
		// ����� ���̵忡 ���� UV Buffer ����

		allSideUVs[12 * i] = ((float)i) / numberOfSides;
		allSideUVs[12 * i + 1] = 1.0f;

		allSideUVs[12 * i + 2] = ((float)i) / numberOfSides;
		allSideUVs[12 * i + 3] = 0.0f;

		allSideUVs[12 * i + 4] = ((float)i + 1) / numberOfSides;
		allSideUVs[12 * i + 5] = 1.0f;

		allSideUVs[12 * i + 6] = ((float)i + 1) / numberOfSides;
		allSideUVs[12 * i + 7] = 1.0f;

		allSideUVs[12 * i + 8] = ((float)i) / numberOfSides;
		allSideUVs[12 * i + 9] = 0.0f;

		allSideUVs[12 * i + 10] = ((float)i + 1) / numberOfSides;
		allSideUVs[12 * i + 11] = 0.0f;
	}
	return allSideUVs;
}

// vertex ������ �°� ������ �÷��� �޾� Color ���� ����
GLfloat *makeColorBuffer(GLfloat *arr, GLint vertexNum, GLfloat red, GLfloat green, GLfloat blue) {
	//GLfloat *allColorVertices = new GLfloat[vertexNum * 3];
	GLfloat *allColorVertices = arr;
	for (int i = 0; i < vertexNum; i++)
	{
		allColorVertices[3 * i] = red;
		allColorVertices[3 * i + 1] = green;
		allColorVertices[3 * i + 2] = blue;
	}
	return allColorVertices;
}

// ��� ����� vertex data ����
GLfloat *makeUmbrella(GLfloat *arr, GLfloat y, GLfloat radius, GLint numberOfSides) {
	GLint numberOfVertices = numberOfSides + 1;
	GLfloat doublePi = 2.0f * 3.141592f;
	GLfloat *allUmbrellaVertices = arr;
	for (int i = 0; i < numberOfVertices; i++)
	{
		allUmbrellaVertices[9 * i] = 0.0f;
		allUmbrellaVertices[9 * i + 1] = y;
		allUmbrellaVertices[9 * i + 2] = 0.0f;

		allUmbrellaVertices[9 * i + 3] = (radius * cos(i * doublePi / numberOfSides));
		allUmbrellaVertices[9 * i + 4] = 0.0f;
		allUmbrellaVertices[9 * i + 5] = (radius * sin(i * doublePi / numberOfSides));

		allUmbrellaVertices[9 * i + 6] = (radius * cos((i + 1) * doublePi / numberOfSides));
		allUmbrellaVertices[9 * i + 7] = 0.0f;
		allUmbrellaVertices[9 * i + 8] = (radius * sin((i + 1) * doublePi / numberOfSides));
	}
	return allUmbrellaVertices;
}

// ��� ����� color buffer ����
GLfloat *makeUmbrellaColor(GLfloat *arr, GLint numberOfSides, GLfloat red, GLfloat green, GLfloat blue) {
	GLfloat *allUmbrellaColors = NULL;
	allUmbrellaColors = arr;

	for (int i = 0; i < numberOfSides; i++)
	{
		allUmbrellaColors[9 * i] = red;
		allUmbrellaColors[9 * i + 1] = green;
		allUmbrellaColors[9 * i + 2] = blue;

		allUmbrellaColors[9 * i + 3] = red;
		allUmbrellaColors[9 * i + 4] = green;
		allUmbrellaColors[9 * i + 5] = blue;

		allUmbrellaColors[9 * i + 6] = red;
		allUmbrellaColors[9 * i + 7] = green;
		allUmbrellaColors[9 * i + 8] = blue;
	}
	return allUmbrellaColors;
}

// ��� ����� uv buffer ����
GLfloat *makeUmbrellaUV(GLfloat *arr, GLint numberOfSides) {
	GLfloat *allUmbrellaUVs = NULL;
	GLfloat doublePi = 2.0f * 3.141592f;
	allUmbrellaUVs = arr;

	for (int i = 0; i < numberOfSides; i++)
	{
		allUmbrellaUVs[6 * i] = 0.5f;
		allUmbrellaUVs[6 * i + 1] = 0.5f;

		allUmbrellaUVs[6 * i + 2] = 0.5f + (0.5 * cos(i * doublePi / numberOfSides));
		allUmbrellaUVs[6 * i + 3] = 0.5f + (0.5 * sin(i * doublePi / numberOfSides));

		allUmbrellaUVs[6 * i + 4] = 0.5f + (0.5 * cos((i + 1) * doublePi / numberOfSides));
		allUmbrellaUVs[6 * i + 5] = 0.5f + (0.5 * sin((i + 1) * doublePi / numberOfSides));

	}
	return allUmbrellaUVs;
}

// Circle�� ���� UV buffer ����
GLfloat *makeCircleUVBuffer(GLfloat *arr, GLint sideNums) {
	GLfloat *allCircleUVs = NULL;

	GLint numberOfVertices = sideNums + 2;
	GLfloat doublePi = 2.0f * 3.141592f;
	allCircleUVs = arr;

	allCircleUVs[0] = 0.5f;
	allCircleUVs[1] = 0.5f;

	for (int i = 0; i < numberOfVertices; i++)
	{
		allCircleUVs[2 * (i + 1)] = 0.5f + (0.5 * cos(i * doublePi / sideNums));
		allCircleUVs[2 * (i + 1) + 1] = 0.5f + (0.5 * sin(i * doublePi / sideNums));
	}
	return allCircleUVs;
}

// roller coaster rail vertex buffer ����� �Լ�
GLfloat *makeRailVertexBuffer(GLfloat *arr, GLint numberOfSides) {
	GLfloat *allRailVertexBuffer = NULL;
	allRailVertexBuffer = arr;

	GLfloat doublePi = 2.0f * 3.141592f; // -2 * 3.141592 ~ 2 * 3.141592
	GLfloat angle = 0.0f; // -2 * 3.141592 ~ 2 * 3.141592
	GLfloat nextangle = 0.0f; // -2 * 3.141592 ~ 2 * 3.141592
	GLfloat segment = 4.0f * 3.141592f / ((float)numberOfSides);
	GLfloat result = 0.0f, nextResult = 0.0f;
	GLfloat height = 0.0f, nextheight = 0.0f;

	for (int i = 0; i < numberOfSides; i++)
	{
		if (i > 0) angle += segment;
		nextangle += segment;

		height = -2.0f * 3.141592f + angle;
		nextheight = -2.0f * 3.141592f + nextangle;

		result = 3.0f * sin(height) / height + 2.0f * cos(height);
		nextResult = 3.0f * sin(nextheight) / nextheight + 2.0f * cos(nextheight);

		allRailVertexBuffer[18 * i] = 0.0 + (0.95f * cos(i * doublePi / numberOfSides));
		allRailVertexBuffer[18 * i + 1] = result;
		allRailVertexBuffer[18 * i + 2] = 0.0 + (0.95f * sin(i * doublePi / numberOfSides));

		allRailVertexBuffer[18 * i + 3] = 0.0 + (1.05f * cos(i * doublePi / numberOfSides));
		allRailVertexBuffer[18 * i + 4] = result;
		allRailVertexBuffer[18 * i + 5] = 0.0 + (1.05f * sin(i * doublePi / numberOfSides));

		allRailVertexBuffer[18 * i + 6] = 0.0 + (0.95f * cos((i + 1) * doublePi / numberOfSides));
		allRailVertexBuffer[18 * i + 7] = nextResult;
		allRailVertexBuffer[18 * i + 8] = 0.0 + (0.95f * sin((i + 1) * doublePi / numberOfSides));

		allRailVertexBuffer[18 * i + 9] = 0.0 + (0.95f * cos((i + 1) * doublePi / numberOfSides));
		allRailVertexBuffer[18 * i + 10] = nextResult;
		allRailVertexBuffer[18 * i + 11] = 0.0 + (0.95f * sin((i + 1) * doublePi / numberOfSides));

		allRailVertexBuffer[18 * i + 12] = 0.0 + (1.05f * cos((i + 1) * doublePi / numberOfSides));
		allRailVertexBuffer[18 * i + 13] = nextResult;
		allRailVertexBuffer[18 * i + 14] = 0.0 + (1.05f * sin((i + 1) * doublePi / numberOfSides));

		allRailVertexBuffer[18 * i + 15] = 0.0 + (1.05f * cos(i * doublePi / numberOfSides));
		allRailVertexBuffer[18 * i + 16] = result;
		allRailVertexBuffer[18 * i + 17] = 0.0 + (1.05f * sin(i * doublePi / numberOfSides));
	}

	return allRailVertexBuffer;
}

// roller coaster rail vertex buffer ����� �Լ�
GLfloat *makeRailColorBuffer(GLfloat *arr, GLint numberOfSides, GLfloat red, GLfloat green, GLfloat blue) {
	GLfloat *allRailColorBuffer = NULL;
	allRailColorBuffer = arr;

	for (int i = 0; i < numberOfSides; i++)
	{

		allRailColorBuffer[18 * i] = red;
		allRailColorBuffer[18 * i + 1] = green;
		allRailColorBuffer[18 * i + 2] = blue;

		allRailColorBuffer[18 * i + 3] = red;
		allRailColorBuffer[18 * i + 4] = green;
		allRailColorBuffer[18 * i + 5] = blue;

		allRailColorBuffer[18 * i + 6] = red;
		allRailColorBuffer[18 * i + 7] = green;
		allRailColorBuffer[18 * i + 8] = blue;

		allRailColorBuffer[18 * i + 9] = red;
		allRailColorBuffer[18 * i + 10] = green;
		allRailColorBuffer[18 * i + 11] = blue;

		allRailColorBuffer[18 * i + 12] = red;
		allRailColorBuffer[18 * i + 13] = green;
		allRailColorBuffer[18 * i + 14] = blue;

		allRailColorBuffer[18 * i + 15] = red;
		allRailColorBuffer[18 * i + 16] = green;
		allRailColorBuffer[18 * i + 17] = blue;
	}

	return allRailColorBuffer;
}
