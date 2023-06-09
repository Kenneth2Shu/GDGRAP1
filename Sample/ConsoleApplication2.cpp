#define _USE_MATH_DEFINES

#include <glad/glad.h>
//above glfw
#include <GLFW/glfw3.h>
#include <cmath>

//glm headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION

#include "tiny_obj_loader.h"

#include "string"
#include "iostream"

float x_mod = 0;
float y_mod = 0;

float translate_x = 0;
float translate_y = 0;
float translate_z = 0;
float scale_x = 1;
float scale_y = 1;
float scale_z = 1;
float theta = 1;
float axis_x = 1;
float axis_y = 1;
float axis_z = 1;

void Key_Callback(
    GLFWwindow* window,
    int key, //key code
    int scancode, //physical position
    int action, //press or release
    int mod //modifier keys (ctrl, shft, alt, etc)
)

{
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        //x_mod += 0.1f;
        translate_x += 0.1f;
    }

    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        //x_mod -= 0.1f;
        translate_x -= 0.1f;
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        //y_mod += 0.1f;
        translate_y += 0.1f;
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        //y_mod -= 0.1f;
        translate_y -= 0.1f;
    }

    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        axis_y -= 10.0f;
        axis_z -= 10.0f;
    }

    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        axis_y += 10.0f;
        axis_z += 10.0f;
    }

    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        scale_x += 0.2f;
        scale_y += 0.2f;
        scale_z += 0.2f;
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        scale_x -= 0.2f;
        scale_y -= 0.2f;
        scale_z -= 0.2f;
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float Pi = 3.1415;

    //3x3 identity matrix
    glm::mat3 identity_matrix3 = glm::mat3(1.0f);
    //4x4 identity matrix
    glm::mat4 identity_matrix4 = glm::mat4(1.0f);

    //3d translation matrix
    glm::mat4 translation =
        glm::translate(identity_matrix4, //identity matrix
            glm::vec3(translate_x,
                      translate_y,
                      translate_z)
        );

    //3d scale matrix
    glm::mat4 scale =
        glm::scale(identity_matrix4, //identity matrix
            glm::vec3(scale_x,
                      scale_y,
                      scale_z)
        );

    //3d rotation matrix
    glm::mat4 rotation =
        glm::rotate(identity_matrix4, //identity matrix
            glm::radians(theta), //angle of rotation
            glm::vec3(axis_x,//normalized axis vector
                      axis_y,
                      axis_z)
        );

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Kevin Shu Too", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glfwSetKeyCallback(window, Key_Callback);

    //vertex shader
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    //fragment shader
    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    //compile shaders

    //create shader
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    //assign source
    glShaderSource(vertShader, 1, &v, NULL);
    //compile shader
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);

    std::string path = "3D/bunny.obj";
    //several variables
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;
    
    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    //define array of vertices
    //3 sets of coordinates for each point
    GLfloat vertices[]{
        //x   y     z
        0.f, 0.5f, 0.f,//pt 0
        -0.5f, -0.5f, 0.f,//pt 1
        0.5f, -0.5f, 0.f//pt 2
    };

    GLuint indices[]{
        0, 1, 2,//triangle a
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);//makes 1 vao
    glGenBuffers(1, &VBO);//makes 1 vbo
    glGenBuffers(1, &EBO);//makes 1 ebo

    //this tells gl we're working on this VAO
    glBindVertexArray(VAO);
    //this assigns this vbo to the vao above
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//what kind and what name
    //add data to the vbo
    glBufferData(
        GL_ARRAY_BUFFER,//type of buffer
        sizeof(GLfloat) * attributes.vertices.size(),//size in bytes
        &attributes.vertices[0],//array itself
        GL_STATIC_DRAW//static for not moving, dynamic for moving
    );

    //tells gl what to do with data above
    glVertexAttribPointer(
        0,//0 as position
        3,//how many images, 3 for xyz
        GL_FLOAT,//what array it is
        GL_FALSE, //nomrlaization
        3 * sizeof(GL_FLOAT),//size in bytes
        (void*)0//to be discussed
    );
    //enables index 0
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );

    //tell opengl done working with vao and vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //from week 2
        //unsigned int xLoc = glGetUniformLocation(shaderProgram, "x");//note: same name as variable in vert shader
        //glUniform1f(xLoc, x_mod);

        //unsigned int yLoc = glGetUniformLocation(shaderProgram, "y");
        //glUniform1f(yLoc, y_mod);

        //for week 3
        //from identity, then translate, then scale, the rotate

        glm::mat4 transformation_matrix = glm::translate(
            identity_matrix4,
            glm::vec3(translate_x, translate_y, translate_z)
        );

        transformation_matrix = glm::scale(
            transformation_matrix,
            glm::vec3(scale_x, scale_y, scale_z)// do not set any to 0
        );

        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta),
            glm::normalize(glm::vec3(axis_x, axis_y, axis_z))//needs at least 1 value greater than 0
        );

        //get location
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        //assign matrix to vertex shader
        glUniformMatrix4fv(transformLoc,//address
            1,
            GL_FALSE,
            glm::value_ptr(transformation_matrix));

        //declare which shader the program will use
        glUseProgram(shaderProgram);
        
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);//what type of primitive, 1st index, how many points expected
        glDrawElements(
            GL_TRIANGLES,
            mesh_indices.size(),
            GL_UNSIGNED_INT,
            0
        );

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    //cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}