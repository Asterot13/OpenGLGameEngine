#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
    
    std::string VertexShaderCode = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;

        out vec3 vColor;

        void main()
        {
            vColor = color;
            gl_Position = vec4(position.x, position.y, position.z, 1.0);
        }
    )";
    
    // Creating and compiling vector shader
    GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* VertexShaderCStr = VertexShaderCode.c_str();
    glShaderSource(VertexShader, 1, &VertexShaderCStr, nullptr);
    glCompileShader(VertexShader);
    
    GLint success;
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(VertexShader, 512, nullptr, infoLog);
        std::cerr << "Failed to compile vertex shader: " << infoLog << '\n';
    }
    
    std::string FragmentShaderCode = R"(
        #version 330 core
        out vec4 FragColor;

        uniform vec4 uColor;

        in vec3 vColor;

        void main()
        {
            FragColor = vec4(vColor, 1.0f) * uColor;
        }
    )";
    
    // Creating and compiling fragment shader
    GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* FragmentShaderCStr = FragmentShaderCode.c_str();
    glShaderSource(FragmentShader, 1, &FragmentShaderCStr, nullptr);
    glCompileShader(FragmentShader);
    
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(FragmentShader, 512, nullptr, infoLog);
        std::cerr << "Failed to compile fragment shader: " << infoLog << '\n';
    }
    
    GLuint ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);
    
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(ShaderProgram, 512, nullptr, infoLog);
        std::cerr << "Failed to link shader program: " << infoLog << '\n';   
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
    
    std::vector<float> vertices = 
    {
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f
    };
    
    std::vector<unsigned int> indices = 
    {
        0, 1, 3,
        1, 2, 3
    };
    
    GLuint VBO; // vertex buffer object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    GLuint EBO; //element buffer object
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    GLuint VAO; // vertex array object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    GLint uColorLoc = glGetUniformLocation(ShaderProgram, "uColor");
    
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(ShaderProgram);
        glUniform4f(uColorLoc, 0.0f, 1.0f, 0.0f, 1.0f);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}