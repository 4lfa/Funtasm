#include "Demo.h"

Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildColoredPlane();
	BuildSkyBox();
	BuildSkyBoxR();
	BuildTiang();
	BuildTiang2();
	BuildPalang();
	BuildPapan();
	BuildTali();
	BuildTali2();
	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	
	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		FloatCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		FloatCamera(-CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth/2;
	double midY = screenHeight/2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);


	

}

void Demo::Update(double deltaTime) {
	
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	
	DrawColoredPlane();
	DrawSkyBox();
	DrawSkyBoxR();
	DrawTiang();
	DrawTiang2();
	DrawPalang();
	DrawPapan();
	DrawTali();
	DrawTali2();
	glDisable(GL_DEPTH_TEST);
}


//Kiri
void Demo::BuildTiang() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("woodV.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		0, -1, 0.25, 0, 0, // 0
		0.25, -1, 0.25, 1, 0, // 1
		0.25, 3, 0.25, 1, 1, // 2
		0, 3, 0.25, 0, 1, // 3
		// right
		0.25, 3, 0.25, 0, 0, // 4
		0.25, 3, 0, 1, 0, // 5
		0.25, -1, 0, 1, 1, // 6
		0.25, -1, 0.25, 0, 1, // 7
		// back
		0, -1, 0, 0, 0, // 8 
		0.25, -1, 0, 1, 0, // 9
		0.25, 3, 0, 1, 1, // 10
		0, 3, 0, 0, 1, // 11
		// left
		0, -1, 0, 0, 0, // 12
		0, -1, 0.25, 1, 0, // 13
		0, 3, 0.25, 1, 1, // 14
		0, 3, 0, 0, 1, // 15
		// upper
		0.25, 3, 0.25, 0, 0,  // 16
		0, 3, 0.25, 1, 0,  // 17
		0, 3, 0, 1, 1,  // 18
		0.25, 3, 0, 0, 1,  // 19
		// bottom
		0, -1, 0, 0, 0, // 20
		0.25, -1, 0, 1, 0, // 21
		0.25, -1, 0.25, 1, 1, // 22
		0, -1, 0.25, 0, 1, // 23
	};
	
	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   //front
		4,  5,  6,  4,  6,  7,   //right
		8,  9,  10, 8,  10, 11,  //back
		12, 13, 14, 12, 14, 15,  //left
		16, 17, 18, 16, 18, 19,  //upper
		20, 21, 22, 20, 22, 23,  //bottom
	};
	
	

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::DrawTiang()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;

	model = glm::rotate(model, angle, glm::vec3(4.825, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

//Kanan
void Demo::BuildTiang2() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("woodV.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	
	float vertices[] = {
		// 2front
		1.5, -1, 0.25, 0, 0, // 24
		1.75, -1, 0.25, 1, 0, // 25
		1.75, 3, 0.25, 1, 1, // 26
		1.5, 3, 0.25, 0, 1, // 27
		// 2right
		1.75, 3, 0.25, 0, 0, // 28
		1.75, 3, 0, 1, 0, // 29
		1.75, -1, 0, 1, 1, // 30
		1.75, -1, 0.25, 0, 1, // 31
		// 2back
		1.5, -1, 0, 0, 0, // 32
		1.75, -1, 0, 1, 0, // 33
		1.75, 3, 0, 1, 1, // 34
		1.5, 3, 0, 0, 1, // 35
		// 2left
		1.5, -1, 0, 0, 0, // 36
		1.5, -1, 0.25, 1, 0, // 37
		1.5, 3, 0.25, 1, 1, // 38
		1.5, 3, 0, 0, 1, // 39
		// 2upper
		1.75, 3, 0.25, 0, 0,  // 40
		1.5, 3, 0.25, 1, 0,  // 41
		1.5, 3, 0, 1, 1,  // 42
		1.75, 3, 0, 0, 1,  // 43
		// 2bottom
		1.5, -1, 0, 0, 0, // 44
		1.75, -1, 0, 1, 0, // 45
		1.75, -1, 0.25, 1, 1, // 46
		1.5, -1, 0.25, 0, 1, // 47
	};
	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   //front
		4,  5,  6,  4,  6,  7,   //right
		8,  9,  10, 8,  10, 11,  //back
		12, 13, 14, 12, 14, 15,  //left
		16, 17, 18, 16, 18, 19,  //upper
		20, 21, 22, 20, 22, 23,  //bottom
	};



	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::DrawTiang2()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO3); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;

	model = glm::rotate(model, angle, glm::vec3(4.825, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

//atas
void Demo::BuildPalang() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("woodH.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------


	float vertices[] = {
		// 2front
		0, 3, 0.25, 0, 0, // 24
		1.75, 3, 0.25, 1, 0, // 25
		1.75, 3.25, 0.25, 1, 1, // 26
		0, 3.25, 0.25, 0, 1, // 27
		// 2right
		1.75, 3.25, 0.25, 0, 0, // 28
		1.75, 3.25, 0, 1, 0, // 29
		1.75, 3, 0, 1, 1, // 30
		1.75, 3, 0.25, 0, 1, // 31
		// 2back
		0, 3, 0, 0, 0, // 32
		1.75, 3, 0, 1, 0, // 33
		1.75, 3.25, 0, 1, 1, // 34
		0, 3.25, 0, 0, 1, // 35
		// 2left
		0, 3, 0, 0, 0, // 36
		0, 3, 0.25, 1, 0, // 37
		0, 3.25, 0.25, 1, 1, // 38
		0, 3.25, 0, 0, 1, // 39
		// 2upper
		1.75, 3.25, 0.25, 0, 0,  // 40
		0, 3.25, 0.25, 1, 0,  // 41
		0, 3.25, 0, 1, 1,  // 42
		1.75, 3.25, 0, 0, 1,  // 43
		// 2bottom
		0, 3, 0, 0, 0, // 44
		1.75, 3, 0, 1, 0, // 45
		1.75, 3, 0.25, 1, 1, // 46
		0, 3, 0.25, 0, 1, // 47
	};
	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   //front
		4,  5,  6,  4,  6,  7,   //right
		8,  9,  10, 8,  10, 11,  //back
		12, 13, 14, 12, 14, 15,  //left
		16, 17, 18, 16, 18, 19,  //upper
		20, 21, 22, 20, 22, 23,  //bottom
	};



	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glGenBuffers(1, &EBO5);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO5);

	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::DrawPalang()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO5); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;

	model = glm::rotate(model, angle, glm::vec3(4.825, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void Demo::BuildPapan() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("woodH.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------


	float vertices[] = {
		// front
		0.5, 0.2, 0.5, 0, 0, // 24
		1.25, 0.2, 0.5, 1, 0, // 25
		1.25, 0.25, 0.5, 1, 1, // 26
		0.5, 0.25, 0.5, 0, 1, // 27
		// right
		1.25, 0.25, 0.5, 0, 0, // 28
		1.25, 0.25, -0.25, 1, 0, // 29
		1.25, 0.2, -0.25, 1, 1, // 30
		1.25, 0.2, 0.5, 0, 1, // 31
		// back
		0.5, 0.2, -0.25, 0, 0, // 32
		1.25, 0.2, -0.25, 1, 0, // 33
		1.25, 0.25, -0.25, 1, 1, // 34
		0.5, 0.25, -0.25, 0, 1, // 35
		// left
		0.5, 0.2, -0.25, 0, 0, // 36
		0.5, 0.2, 0.5, 1, 0, // 37
		0.5, 0.25, 0.5, 1, 1, // 38
		0.5, 0.25, -0.25, 0, 1, // 39
		// upper
		1.25, 0.25, 0.5, 0, 0,  // 40
		0.5, 0.25, 0.5, 1, 0,  // 41
		0.5, 0.25, -0.25, 1, 1,  // 42
		1.25, 0.25, -0.25, 0, 1,  // 43
		// bottom
		0.5, 0.2, -0.25, 0, 0, // 44
		1.25, 0.2, -0.25, 1, 0, // 45
		1.25, 0.2, 0.5, 1, 1, // 46
		0.5, 0.2, 0.5, 0, 1, // 47
	};
	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   //front
		4,  5,  6,  4,  6,  7,   //right
		8,  9,  10, 8,  10, 11,  //back
		12, 13, 14, 12, 14, 15,  //left
		16, 17, 18, 16, 18, 19,  //upper
		20, 21, 22, 20, 22, 23,  //bottom
	};



	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glGenBuffers(1, &EBO4);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::DrawPapan()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO4); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;

	model = glm::rotate(model, angle, glm::vec3(4.825, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

//Kiri
void Demo::BuildTali() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture6);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("ropeV.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		0.525, 0.2, 0.17, 0, 0, // 0
		0.5, 0.2, 0.17, 1, 0, // 1
		0.5, 3, 0.17, 1, 1, // 2
		0.525, 3, 0.17, 0, 1, // 3
		// right
		0.5, 3, 0.17, 0, 0, // 4
		0.5, 3, 0.18, 1, 0, // 5
		0.5, 0.2, 0.18, 1, 1, // 6
		0.5, 0.2, 0.17, 0, 1, // 7
		// back
		0.525, 0.2, 0.18, 0, 0, // 8 
		0.5, 0.2, 0.18, 1, 0, // 9
		0.5, 3, 0.18, 1, 1, // 10
		0.525, 3, 0.18, 0, 1, // 11
		// left
		0.5, 0.2, 0.18, 0, 0, // 12
		0.5, 0.2, 0.17, 1, 0, // 13
		0.5, 3, 0.17, 1, 1, // 14
		0.5, 3, 0.18, 0, 1, // 15
		// upper
		0.5, 3, 0.17, 0, 0,  // 16
		0.525, 3, 0.17, 1, 0,  // 17
		0.525, 3, 0.18, 1, 1,  // 18
		0.5, 3, 0.18, 0, 1,  // 19
		// bottom
		0.525, 0.2, 0.18, 0, 0, // 20
		0.5, 0.2, 0.18, 1, 0, // 21
		0.5, 0.2, 0.17, 1, 1, // 22
		0.525, 0.2,0.17, 0, 1, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   //front
		4,  5,  6,  4,  6,  7,   //right
		8,  9,  10, 8,  10, 11,  //back
		12, 13, 14, 12, 14, 15,  //left
		16, 17, 18, 16, 18, 19,  //upper
		20, 21, 22, 20, 22, 23,  //bottom
	};



	glGenVertexArrays(1, &VAO6);
	glGenBuffers(1, &VBO6);
	glGenBuffers(1, &EBO6);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO6);

	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO6);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::DrawTali()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO6); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;

	model = glm::rotate(model, angle, glm::vec3(4.825, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

//Kanan
void Demo::BuildTali2() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture7);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("ropeV.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------


	float vertices[] = {
		// format position, tex coords
		// front
		1.25, 0.2, 0.17, 0, 0, // 0
		1.225, 0.2, 0.17, 1, 0, // 1
		1.225, 3, 0.17, 1, 1, // 2
		1.25, 3, 0.17, 0, 1, // 3
		// right
		1.225, 3, 0.17, 0, 0, // 4
		1.2255, 3, 0.18, 1, 0, // 5
		1.225, 0.2, 0.18, 1, 1, // 6
		1.225, 0.2, 0.17, 0, 1, // 7
		// back
		1.25, 0.2, 0.18, 0, 0, // 8 
		1.225, 0.2, 0.18, 1, 0, // 9
		1.225, 3, 0.18, 1, 1, // 10
		1.25, 3, 0.18, 0, 1, // 11
		// left
		1.225, 0.2, 0.18, 0, 0, // 12
		1.225, 0.2, 0.17, 1, 0, // 13
		1.225, 3, 0.17, 1, 1, // 14
		1.225, 3, 0.18, 0, 1, // 15
		// upper
		1.225, 3, 0.17, 0, 0,  // 16
		1.25, 3, 0.17, 1, 0,  // 17
		1.25, 3, 0.18, 1, 1,  // 18
		1.225, 3, 0.18, 0, 1,  // 19
		// bottom
		1.25, 0.2, 0.18, 0, 0, // 20
		1.225, 0.2, 0.18, 1, 0, // 21
		1.225, 0.2, 0.17, 1, 1, // 22
		1.25, 0.2,0.17, 0, 1, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   //front
		4,  5,  6,  4,  6,  7,   //right
		8,  9,  10, 8,  10, 11,  //back
		12, 13, 14, 12, 14, 15,  //left
		16, 17, 18, 16, 18, 19,  //upper
		20, 21, 22, 20, 22, 23,  //bottom
	};



	glGenVertexArrays(1, &VAO7);
	glGenBuffers(1, &VBO7);
	glGenBuffers(1, &EBO7);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO7);

	glBindBuffer(GL_ARRAY_BUFFER, VBO7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO7);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::DrawTali2()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO7); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;

	model = glm::rotate(model, angle, glm::vec3(4.825, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


//Skybox
//SkyboxBawah
void Demo::BuildColoredPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("tanah.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -0.5, -50.0,  0,  0,
		 50.0, -0.5, -50.0, 50,  0,
		 50.0, -0.5,  50.0, 50, 50,
		-50.0, -0.5,  50.0,  0, 50,
	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}
void Demo::DrawColoredPlane()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
//EndSkyboxBawah
//KelilingSkybox
void Demo::BuildSkyBox()
{
	// Load and create a texture 
	glGenTextures(1, &texture8);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("sky.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	float vertices[] = {
		// format position, tex coords
		// Front
		50.0  , -0.75, 50.0, 0, 0, // 0
		-50.0 , -0.75, 50.0, 1, 0, // 1
		-50.0 , 50.0, 50.0, 1, 1, // 2
		50.0  , 50.0, 50.0, 0, 1, // 3
		//Right
		50.0, -50.0, -50.0, 0, 0, // 4
		50.0,-0.75, 50.0,  1, 0, // 5
		50.0, 50.0, 50.0, 1, 1, // 6
		50.0,50.0, -50.0, 0, 1, // 7
		//back
		50.0  , -0.75, -50.0, 0, 0, // 8
		-50.0 , -0.75, -50.0, 1, 0, // 9
		-50.0 , 50.0, -50.0, 1, 1, // 10
		50.0  , 50.0, -50.0, 0, 1, // 11
		//left
		-50.0, -0.75, -50.0, 0, 0, //12
		-50.0,-0.75, 50.0,  1, 0, // 13
		-50.0, 50.0, 50.0, 1, 1, // 14
		-50.0,50.0, -50.0, 0, 1, // 15
	};

	unsigned int indices[] = { 
		0,  1,  2,  0,  2,  3,   //front
		4,  5,  6,  4,  6,  7,   //right
		8,  9,  10, 8,  10, 11,  //back
		12, 13, 14, 12, 14, 15,  //left

	};


	glGenVertexArrays(1, &VAO8);
	glGenBuffers(1, &VBO8);
	glGenBuffers(1, &EBO8);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO8);

	glBindBuffer(GL_ARRAY_BUFFER, VBO8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO8);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::DrawSkyBox()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO8); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;

	model = glm::rotate(model, angle, glm::vec3(4.825, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
//EndKelilingSkybox
//Skybox atas
void Demo::BuildSkyBoxR()
{
	// Load and create a texture 
	glGenTextures(1, &texture9);
	glBindTexture(GL_TEXTURE_2D, texture9);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("sun.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// up
		-50.0, 50, -50.0,  0,  0, //0
		 50.0, 50, -50.0, 1,  0, //1
		 50.0, 50,  50.0, 1, 1, //2
		-50.0, 50,  50.0,  0, 1, //3


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO9);
	glGenBuffers(1, &VBO9);
	glGenBuffers(1, &EBO9);

	glBindVertexArray(VAO9);

	glBindBuffer(GL_ARRAY_BUFFER, VBO9);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO9);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO

}
void Demo::DrawSkyBoxR()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture9);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO9); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
//EndSkybox atas
//EndSkybox



//Camera
void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 1.0f;
	posCamZ = 8.0f;
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.001f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::FloatCamera(float speed)
{
	float y = viewCamY - posCamY;
	float orthoY = y;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamY = posCamY + orthoY * speed;
	viewCamY = viewCamY + orthoY * speed;
}

void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}

//main
int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Final Project: Funtasm", 800, 600, false, false);
}