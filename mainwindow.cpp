#include "mainwindow.h"
#include <QMessageBox>

// --- Vertex Shader (OpenGL 3.3 Core) ---
static const char *vertexShaderSource = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}
)";

// --- Fragment Shader (OpenGL 3.3 Core) ---
static const char *fragmentShaderSource = R"(#version 330 core
in vec3 ourColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}
)";

// ==========================================
// GLWidget Implementation
// ==========================================

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

GLWidget::~GLWidget()
{
    // Always make context current before releasing OpenGL resources
    makeCurrent();
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_shaderProgram) glDeleteProgram(m_shaderProgram);
    doneCurrent();
}

void GLWidget::initializeGL()
{
    // Initialize OpenGL 3.3 Core function pointers
    initializeOpenGLFunctions();

    // 1. Compile Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // 2. Compile Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // 3. Link Shader Program
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);

    // Cleanup individual shaders once linked into program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 4. Triangle vertex data: [Position (X, Y, Z), Color (R, G, B)]
    float vertices[] = {
        // Positions          // Colors
        0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Top: Red
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom-Left: Green
        0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // Bottom-Right: Blue
    };

    // 5. Setup VAO and VBO
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Attribute 0: Positions (3 floats, stride = 6 * sizeof(float), offset = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Attribute 1: Colors (3 floats, stride = 6 * sizeof(float), offset = 3 * sizeof(float))
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLWidget::paintGL()
{
    // Dark background
    glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw triangle using modern OpenGL 3.3 Core
    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

// ==========================================
// MainWindow Implementation
// ==========================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Qt 6.12 Static - OpenGL 3.3 Core");
    resize(400, 300);

    // Put OpenGL widget as the central widget of the window
    m_glWidget = new GLWidget(this);
    setCentralWidget(m_glWidget);
}

MainWindow::~MainWindow()
{
}
