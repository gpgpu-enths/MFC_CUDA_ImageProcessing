#include "stdafx.h"
#include "OpenGLControl.h"
#include ".\openglcontrol.h"

#include "Utility.h"


void InitTexture();
void BuildVBO();

// Global Vars
GLuint VBO, VAO;

void error_callback (int error, const char *description)
{
	fputs(description, stderr);
}



//
//void BuildVBO()
//{
//	// Set up vertex data (and buffer(s)) and attribute pointers
//    GLfloat vertices[] = {
//        // Positions         // Colors
//        0.2f,  0.1f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Right
//       -0.2f,  0.1f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Left
//        0.0f,  0.6f, 0.0f,   0.0f, 0.0f, 1.0f,   // Top 
//
//	    0.2f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f,  // Bottom Right
//	   -0.2f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom Left
//		0.0f, -1.3f, 0.0f, 0.0f, 0.0f, 1.0f,   // Top
//
//		0.3f,  0.1f, 0.0f, 1.0f, 0.0f, 0.0f,  // Bottom Right
//		0.3f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom Left
//		0.6f,  -0.3f, 0.0f, 0.0f, 0.0f, 1.0f,   // Top
//
//		-0.3f, 0.1f, 0.0f, 1.0f, 0.0f, 0.0f,  // Bottom Right
//		-0.3f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom Left
//		-0.6f, -0.3f, 0.0f, 0.0f, 0.0f, 1.0f,   // Top
//
//	   -0.2f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // 
//	    0.2f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  // 
//	    0.2f,  -0.4f, 0.0f, 0.0f, 0.0f, 1.0f,   // 
//	   -0.2f,  -0.4f, 0.0f,  0.0f, 0.0f, 1.0f   // 
//
//    };
//
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//
//    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    // Position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(0);
//    // Color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//    glEnableVertexAttribArray(1);
//
//    glBindVertexArray(0); // Unbind VAO
//}

Shader *ourShader;
void GLInit()
{
	glfwSetErrorCallback (error_callback);	// This is the only callback called before glfwInit
	
    // Init GLFW
    if (!glfwInit())
		exit(EXIT_FAILURE);


	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);



    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();


	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glClearDepth(1.0f);
	//glEnable(GL_DEPTH_TEST);	// Enable depth testing (i.e. enable hidden surface removal)
	//glDepthFunc(GL_LESS);		// This is the default depth test function

	InitTexture();

	//// Build and compile our shader programs
    //ourShader = new Shader("basic.vs", "basic.frag");
	ourShader = new Shader("ImageProc.vs", "ImageProc.frag");

	BuildVBO();
}

CTexture tex1;
void InitTexture()
{
	tex1 = CTexture();
	//tex1.LoadTexture2D("dice_texture1.png");
	/*tex1.LoadTexture2D("images/NLM_lg.png", CV_LOAD_IMAGE_COLOR, GL_NEAREST, GL_NEAREST, GL_CLAMP);*/

	tex1.LoadTexture2D("Images/NLM_lg.png", CV_LOAD_IMAGE_COLOR, GL_NEAREST, GL_NEAREST, GL_CLAMP);



}

CMesh rect;
void BuildVBO()
{
	rect = gls2Rect(0.0f, 1.0f, 1.0f, 1, 1);
}

COpenGLControl::COpenGLControl(void)
{
	m_fPosX = 0.0f;		// X position of model in camera view
	m_fPosY = 0.0f;		// Y position of model in camera view
	m_fZoom = 10.0f;	// Zoom on model in camera view
	m_fRotX = 0.0f;		// Rotation on model in camera view
	m_fRotY	= 0.0f;		// Rotation on model in camera view
	m_bIsMaximized = false;
}

COpenGLControl::~COpenGLControl(void)
{
}

BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void COpenGLControl::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	ValidateRect(NULL);
}

void COpenGLControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;

	// Map the OpenGL coordinates.
	glViewport(0, 0, cx, cy);

	// Projection view
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	// Set our current view perspective
	gluPerspective(35.0f, (float)cx / (float)cy, 0.01f, 2000.0f);

	// Model view
	glMatrixMode(GL_MODELVIEW);

	switch (nType)
	{
		// If window resize token is "maximize"
		case SIZE_MAXIMIZED:
		{
			// Get the current window rect
			GetWindowRect(m_rect);

			// Move the window accordingly
			MoveWindow(6, 6, cx - 14, cy - 14);

			// Get the new window rect
			GetWindowRect(m_rect);

			// Store our old window as the new rect
			m_oldWindow = m_rect;

			break;
		}

		// If window resize token is "restore"
		case SIZE_RESTORED:
		{
			// If the window is currently maximized
			if (m_bIsMaximized)
			{
				// Get the current window rect
				GetWindowRect(m_rect);

				// Move the window accordingly (to our stored old window)
				MoveWindow(m_oldWindow.left, m_oldWindow.top - 18, m_originalRect.Width() - 4, m_originalRect.Height() - 4);

				// Get the new window rect
				GetWindowRect(m_rect);

				// Store our old window as the new rect
				m_oldWindow = m_rect;
			}
		
			break;
		}
	}
}

int COpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

	oglInitialize();
	GLInit();


	return 0;
}

void COpenGLControl::OnDraw(CDC *pDC)
{
	// If the current view is perspective...
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -m_fZoom);
	glTranslatef(m_fPosX, m_fPosY, 0.0f);
	glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);
}

void COpenGLControl::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
		case 1:
		{
			// Clear color and depth buffer bits
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Draw OpenGL scene
			oglDrawScene();

			// Swap buffers
			SwapBuffers(hdc);

			break;
		}

		default:
			break;
	}

	CWnd::OnTimer(nIDEvent);
}

void COpenGLControl::OnMouseMove(UINT nFlags, CPoint point)
{
	int diffX = (int)(point.x - m_fLastX);
	int diffY = (int)(point.y - m_fLastY);
	m_fLastX  = (float)point.x;
	m_fLastY  = (float)point.y;

	// Left mouse button
	if (nFlags & MK_LBUTTON)
	{
		m_fRotX += (float)0.5f * diffY;

		if ((m_fRotX > 360.0f) || (m_fRotX < -360.0f))
		{
			m_fRotX = 0.0f;
		}

		m_fRotY += (float)0.5f * diffX;

		if ((m_fRotY > 360.0f) || (m_fRotY < -360.0f))
		{
			m_fRotY = 0.0f;
		}
	}

	// Right mouse button
	else if (nFlags & MK_RBUTTON)
	{
		m_fZoom -= (float)0.1f * diffY;
	}

	// Middle mouse button
	else if (nFlags & MK_MBUTTON)
	{
		m_fPosX += (float)0.05f * diffX;
		m_fPosY -= (float)0.05f * diffY;
	}

	OnDraw(NULL);

	CWnd::OnMouseMove(nFlags, point);
}

void COpenGLControl::oglCreate(CRect rect, CWnd *parent)
{
	CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_OWNDC, NULL, (HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

	CreateEx(0, className, _T("OpenGL"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);

	// Set initial variables' values
	m_oldWindow	   = rect;
	m_originalRect = rect;

	hWnd = parent;
}

void COpenGLControl::oglInitialize(void)
{
	// Initial Setup:
	//
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32, // bit depth
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		16, // z-buffer depth
		0, 0, 0, 0, 0, 0, 0,
	};

	// Get device context only once.
	hdc = GetDC()->m_hDC;
	
	// Pixel format.
	m_nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, m_nPixelFormat, &pfd);

	// Create the OpenGL Rendering Context.
	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	// Basic Setup:
	//
	// Set color to use when clearing the background.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	// Turn on backface culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	
	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Send draw request
	OnDraw(NULL);
}

void COpenGLControl::oglDrawScene(void)
{
	int procType = 1; // RGB  -> illuminace
	//int procType = 2; // RGB negative
	//int procType = 3; // RGB -> illuminace -> negative
	//int procType = 4; // Color filter
	//int procType = 5; // Embossing
	//int procType = 6; // Edge detection





	string imgPath = Utility::GetImagePath();
	if (!imgPath.empty())
	{
		tex1 = CTexture();
		tex1.LoadTexture2D(imgPath, CV_LOAD_IMAGE_COLOR, GL_NEAREST, GL_NEAREST, GL_CLAMP);
	}


	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1.uiTexture);
	glUniform1i(glGetUniformLocation(ourShader->Program, "ourTexture1"), 0);

	ourShader->Use();

	// Get uniform locations of transformation
	GLint modelLoc = glGetUniformLocation(ourShader->Program, "model");
	GLint viewLoc = glGetUniformLocation(ourShader->Program, "view");
	GLint projLoc = glGetUniformLocation(ourShader->Program, "projection");
	GLint transLoc = glGetUniformLocation(ourShader->Program, "transform");

	// Apply model, view, and perspective transformations
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 transform;

	// =============================================================================================
	//
	//	Set a virtual camera (i.e. view and projection).
	//	view and projection (usually these are the same for all objects, i.e. one camera for the scene)
	//  Note:	currently we set the view and projection matrix each frame, 
	//			but since the view and projection matrix rarely changes,
	//		 i	t's often best practice to set them outside the main loop only once.
	//
	//===============================================================================================
	view = glm::lookAt(glm::vec3(0, 0, 85.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	projection = glm::perspective(glm::radians(50.0f), (GLfloat)m_originalRect.Width() / (GLfloat)m_originalRect.Height(), 0.1f, 100.0f); // foy is in radians
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//=================================================================================================
	//
	//	Apply a model transformation to each object in the scene
	//
	//=================================================================================================

	//=================================================================================================
	//### BIND ORIGINAL IMAGE #################################################
	//=================================================================================================
	GLint procTypeLoc = glGetUniformLocation(ourShader->Program, "procType");
	GLint filterColorLoc = glGetUniformLocation(ourShader->Program, "filterColor");

	// Transfome object 1 and draw
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.51f, 0.0f, 0.0f));
	transform = projection * view * model;
	// Pass the matrices to the shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glUniform1i(procTypeLoc, 0);
	rect.Draw();
	//=================================================================================================



	//=================================================================================================
	//### BIND PROCESSED IMAGE #################################################
	//=================================================================================================

	// Transfome object 1 and draw
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.51f, 0.0f, 0.0f));
	transform = projection * view * model;
	// Pass the matrices to the shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(transform));
	// set image processing method

	// glUniform1i(procTypeLoc, 1);	// 1 --> RGB to illuminance
	//glUniform1i(procTypeLoc, 2);		// 2 --> filter color
	glUniform1i(procTypeLoc, procType);
	float r = 0, g = 0, b = 0;
	////ColorNameToRGB ("Sepia", r, g, b);
	//ColorNameToRGB("SkyBlue", r, g, b);
	glUniform3f(filterColorLoc, r, g, b);
	rect.Draw();

	//=================================================================================================




	//ourShader->Use();
	//glBindVertexArray(VAO);
 //   glDrawArrays(GL_TRIANGLES, 0, 12);
	//glDrawArrays(GL_QUADS, 12, 4);
 //   glBindVertexArray(0);
}

