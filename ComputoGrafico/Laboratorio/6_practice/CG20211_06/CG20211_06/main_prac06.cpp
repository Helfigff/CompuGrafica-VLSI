/*---------------------------------------------------------*/
/* ----------------   Pr�ctica 6 --------------------------*/
/*-----------------    2021-1   ---------------------------*/
/*------------- Alumno: Alfonso Murrieta Villegas----------*/
#include "esfera.h"

Esfera my_sphere(1.0);

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void display(Shader);
void getResolution(void);
void animate(void);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f;

//For planets 
float	
	sol = 0.0f,
	mercurio = 0.0f,

	year = 0.0f,
	day = 0.0f,
	moon = 0.0f,
	mars_year = 0.0f,
	jupiter_year = 0.0f,
	saturno= 0.0f;

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{	
		GLfloat vertices[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V4

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V7

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V1
	};

	unsigned int indices[] =	//I am not using index for this session
	{
		0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void animate(void)
{
	if (360 > sol)
		sol += 0.1f;
	else
		sol = 0.0f;

	//Movements for each entity 
	mercurio += 0.4;
	
	year -= 0.9f;
	day += 0.7f;

	moon += 0.4f;
	mars_year -= 0.5f;

	jupiter_year += 0.4f;

	saturno += 0.35;
}

void display(Shader shader)
{

	shader.use();

	// create transformations and Projection
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ));
	view = glm::rotate(view, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Colocar c�digo aqu�
	model = glm::rotate(model, glm::radians(sol), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	my_sphere.render();	//Sun

	//PLANETAS
	glm::mat4 temp = glm::mat4(1.0f);

	//MERCURIO
	//Simulaci�n movimiento de translaci�n en mercurio
	model = glm::rotate(glm::mat4(1.0f), glm::radians(mercurio), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
	my_sphere.render();	

	//TIERRA
	model = glm::rotate(glm::mat4(1.0f), glm::radians(year), glm::vec3(0.0f, 1.0f, 0.0f));
	temp = model = glm::translate(model, glm::vec3(11.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(day), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
	my_sphere.render();


	//Sat�lite de la Tierra (Luna)
	model = glm::rotate(model, glm::radians(moon), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 2.7f, 0.0f));
	model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.9f, 0.9f, 0.9f));
	my_sphere.render();


	//MARTE 
	//traslaci�n del planeta en z,entre tierra y jupiter
	model = glm::rotate(glm::mat4(1.0f), glm::radians(mars_year), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(15.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(day), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	my_sphere.render();


	//JUPITER
	//Desplazamiento respecto al plano
	model = glm::rotate(glm::mat4(1.0f), glm::radians(165.5f), glm::vec3(0.0f, 0.0f, 1.0f));
	//Traslaci�n del planeta
	model = glm::rotate(model, glm::radians(jupiter_year), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(22.4f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.9f, 1.9f, 1.9f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
	my_sphere.render();


	//SATURNO 29.3 desfase en Y y 31.3 de distancia respecto al sol
	//Desplazamiento respecto al plano
	float desfase = 29.3;
	model = glm::rotate(glm::mat4(1.0f), glm::radians(180-desfase), glm::vec3(0.0f, 1.0f, 0.0f));

	//Traslaci�n del planeta
	model = glm::rotate(model, glm::radians(saturno), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(31.3f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));

	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.5f, 0.5f, 0.0f));
	my_sphere.render();

	glBindVertexArray(0);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 6", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	my_sphere.init();
	glEnable(GL_DEPTH_TEST);
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);
		animate();

        // render
        // Backgound color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi funci�n de dibujo
		display(projectionShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	//~Esfera();
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX += 0.08f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ -= 0.08f;

	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rotX += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotX -= 0.18f;
	

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}