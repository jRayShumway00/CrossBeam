#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

/* Compiling the Vertex and Fragment Shaders */
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type); // Assign the id with glCreateShader
    const char* src = source.c_str(); // Converting data string to role string.
    glShaderSource(id, 1, &src, nullptr); // Replaces source code in a shader object.
    glCompileShader(id); // Compile the shader!

    /* ERROR HANDLING */
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); // i = integer, v = vector (pointer)
    
    // Check if result is false:
    if (result == GL_FALSE)
    {
        int length; // Var for error length.
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); // Query the length. Store data to int length.
        char* message = (char*)_malloca(length * sizeof(char)); // Allocate on stack dynamically.
        glGetShaderInfoLog(id, length, &length, message); // Return the information log for the shader object.
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl; // Print error message.
        std::cout << message << std::endl; // Print the log.
        glDeleteShader(id);
        return 0;
    }
    /* END ERROR HANDLING */

    return id;
}

/* Creating vertexShader and fragmentShader */
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram(); // Assign the create program object to a var.
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); // Var for the vertexShader
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); // Var for the fragmentShader

    glAttachShader(program, vs); // Attach the vertexShader
    glAttachShader(program, fs); // Attach the fragmentShader
    glLinkProgram(program); // Link the program var.

    
    /* Can Be Removed if needed. */
    glDetachShader(program, vs); // Shader has been linked so we can detach it.
    glDetachShader(program, fs); // Shader has been linked so we can detach it.


    glValidateProgram(program); // Validate the program var.

    glDeleteShader(vs); // The shader's have been linked already so we can delete them since the program has already been create.
    glDeleteShader(fs); // The shader's have been linked already so we can delete them since the program has already been create.

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "Crossbeam", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "ERROR!" << std::endl;

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    /* First Buffer */
    // Data for all vertices in the triangle.
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    unsigned int buffer; // Create int variable for buffer id.
    glGenBuffers(1, &buffer); // Generate buffer object names.
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // Bind the named buffer object.
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); // Creates and initializes the buffer object's data store.

    glEnableVertexAttribArray(0); // Enable the vertex attribute array.
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); // Define the array of the generic vertex attribute data.
    /* End First Buffer Zone */

    /* Shader */
    // Vertex Shader Code:
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    // Fragment Shader Code:
    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader); // Assign Shaders
    glUseProgram(shader); // Use the specified shader program!
    /* End Shader */

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Render the triangle primitive from the array data! */
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup and delete the shader program:
    glDeleteProgram(shader);

    // Terminate the application window.
    glfwTerminate();
    return 0;
}