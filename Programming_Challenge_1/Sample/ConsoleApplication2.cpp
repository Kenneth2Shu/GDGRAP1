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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float height = 600.f;
float width = 600.f;

float x_mod = 0;
float y_mod = 0;

float translate_x = 0;
float translate_y = 0;
float translate_z = -1;
float scale_x = 0.5;
float scale_y = 0.5;
float scale_z = 0.5;
float theta = 0.01;
float axis_x = 1;
float axis_y = 1;
float axis_z = 1;

float nFOV = 60.0f;
float camera_x = 0;
float camera_y = 0;
float camera_z = 0;
float projection_z = 100.0f;

void Key_Callback(
    GLFWwindow* window,
    int key, //key code
    int scancode, //physical position
    int action, //press or release
    int mod //modifier keys (ctrl, shft, alt, etc)
)

{
    //original
    /*
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        //y_mod += 0.1f;
        translate_y += 0.1f;
    }
    */
    if (key == GLFW_KEY_D && action == GLFW_REPEAT) {
        //x_mod += 0.1f;
        translate_x += 0.01f;
    }

    if (key == GLFW_KEY_A && action == GLFW_REPEAT) {
        //x_mod -= 0.1f;
        translate_x -= 0.01f;
    }

    if (key == GLFW_KEY_W && action == GLFW_REPEAT) {
        //y_mod += 0.1f;
        translate_y += 0.01f;
    }

    if (key == GLFW_KEY_S && action == GLFW_REPEAT) {
        //y_mod -= 0.1f;
        translate_y -= 0.01f;
    }

    if (key == GLFW_KEY_E && action == GLFW_REPEAT) {
        scale_x += 0.1f;
        scale_y += 0.1f;
        scale_z += 0.1f;
    }

    if (key == GLFW_KEY_Q && action == GLFW_REPEAT) {
        scale_x -= 0.1f;
        scale_y -= 0.1f;
        scale_z -= 0.1f;
    }

    if (key == GLFW_KEY_Z && action == GLFW_REPEAT) {
        //translate_z += 0.1f;
        camera_z += 0.1f;
        std::cout << camera_z << "\n";
    }

    if (key == GLFW_KEY_X && action == GLFW_REPEAT) {
        //translate_z -= 0.1f;
        camera_z -= 0.1f;
        std::cout << camera_z << "\n";
    }

    if (key == GLFW_KEY_T && action == GLFW_REPEAT) {
        //translate_z += 0.1f;
        camera_y += 0.1f;
        std::cout << camera_y << "\n";
    }

    if (key == GLFW_KEY_G && action == GLFW_REPEAT) {
        //translate_z -= 0.1f;
        camera_y -= 0.1f;
        std::cout << camera_y << "\n";
    }

    if (key == GLFW_KEY_UP && action == GLFW_REPEAT) {
        axis_x += 50.0f;
        theta += 1;
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) {
        axis_y -= 50.0f;
        theta += 1;
    }

    if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
        axis_y += 50.0f;
        theta += 1;
    }

    if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
        axis_x -= 50.0f;
        theta += 1;
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
    window = glfwCreateWindow(width, height, "Kevin Shu Too", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //initialize the library
    gladLoadGL();

    int img_width,//width of texture
        img_height,//height of texture
        colorChannels;//num of color channels

    //right side up
    stbi_set_flip_vertically_on_load(true);

    //for the texture
    
    /*
    unsigned char* tex_bytes = stbi_load("3D/ayaya.png",
                                        &img_width,
                                        &img_height,
                                        &colorChannels,
                                        0);*/

    unsigned char* tex_bytes = stbi_load("3D/dino.png",
                                        &img_width,
                                        &img_height,
                                        &colorChannels,
                                        0);
    
    //openGL reference to texture
    GLuint texture;
    //generate a reference
    glGenTextures(1, &texture);
    //set current texture to Texture0
    //texture 0 for now, add more ltr
    glActiveTexture(GL_TEXTURE0);
    //bind next tasks to Texture0
    //similar to VBO
    glBindTexture(GL_TEXTURE_2D, texture);

    //assign the loaded texture to openGL reference
    glTexImage2D(GL_TEXTURE_2D,
                0,
                GL_RGBA,//target color format of the texture, png has alpha, jpeg only does rgb
                img_width,
                img_height,
                0,//border
                GL_RGBA,//color format, should be same as target color format
                GL_UNSIGNED_BYTE,
                tex_bytes);//loaded texture in bytes

    //mipmap is a smaller version of the texture
    glGenerateMipmap(GL_TEXTURE_2D);
    //clean up
    stbi_image_free(tex_bytes);

    //enable depth testing
    glEnable(GL_DEPTH_TEST);

    //for limiting screen
    
    glViewport(0,//min x
               0,//min y
               width,//width
               height);//height

    //create projection matrix for orthographic matrix
    //glm::mat4 projection = glm::ortho(-2.0f, //left most
    //                                    2.0f, //right most
    //                                    -2.0f, //bottom most
    //                                    2.0f, //top most
    //                                    -1.f, //z near
    //                                    1.0f); //z far
    //setting these values make it look like rabbit shrinks since original bounds are -1 and 1, so -2 and 2 makes it bigger

    //glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -3.0f),
    //                            glm::vec3(0.0f, 0.0f, 0.0f),
    //                           glm::vec3(0.0f, 1.0f, 0.0f));

    //for perspective camera
    glm::mat4 projection = glm::perspective(
        glm::radians(nFOV), //fov
        height / width, //aspect ratio
        //camera_z,
        0.1f, // near
        100.f // far
        //these two z values are not actual coordinates but distance from camera
    );
    //positive x, towards us
    //negative x, away
    //camera rn is at z = 0

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

    std::string path = "3D/myCube.obj";
    //std::string path = "3D/bunny.obj";
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

    GLfloat UV[]{
        0.f, 1.f,
        0.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        0.f, 1.f,
        0.f, 0.f
    };//handles how many tiles, ex. 3 gives 3 tiles

    /*
    GLfloat UV[]{
        0.f, 1.f,
        0.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        0.f, 1.f,
        0.f, 0.f
    };
    */

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

    GLuint VAO, VBO, EBO, VBO_UV;
    glGenVertexArrays(1, &VAO);//makes 1 vao
    glGenBuffers(1, &VBO);//makes 1 vbo
    glGenBuffers(1, &VBO_UV);//for UVs for the texture
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

    //bind UV buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);

    //add UV buffer data
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * (sizeof(UV) / sizeof(UV[0])),//float * size of UV array
        &UV[0],// uv array earlier
        GL_DYNAMIC_DRAW);

    //add how to interpret the array
    glVertexAttribPointer(
            2,//for tex coordinates or UV
            2, //2 since U and V, 2 components
            GL_FLOAT,//array of gl floats
            GL_FALSE,
            2 * sizeof(float),// every 2 index, can also use GL_FLOAT aside from float
            (void*)0
    );

    //enable 2 for UV / Tex coordinates
    glEnableVertexAttribArray(2);

    //tell opengl done working with vao and vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clears up depth buffer as well

        //camera position
        //glm::vec3 cameraPos = glm::vec3(0, 0, 1.0f);
        // moves with camera
        glm::vec3 cameraPos = glm::vec3(translate_x, 0, 1.0f);
        //psoition matrix is translation
        glm::mat4 cameraPosMatrix = glm::translate(
            glm::mat4(1.0f),//initialize identity matrix
            cameraPos * -1.0f//since position matrix is negative
        );

        //up direction of the world, literally pointing up on y axis
        glm::vec3 worldUp = glm::normalize(glm::vec3(0, 1.0f, 0));

        //camera center aka target in the world
        //glm::vec3 cameraCenter = glm::vec3(0, 2.0f, 0);

        //camera moves when object moves
        //glm::vec3 cameraCenter = glm::vec3(translate_x, 2.0f, 0);

        //movable camera center
        glm::vec3 cameraCenter = glm::vec3(camera_x, camera_y, camera_z);

        //start with forward
        glm::vec3 F = (cameraCenter - cameraPos);
        F = glm::normalize(F);

        //next is right
        glm::vec3 R = glm::cross(F, worldUp);

        //up vector
        glm::vec3 U = glm::cross(R, F);

        //construct matrix
        glm::mat4 cameraOrientation = glm::mat4(1.0f);

        //matrix[Col][Row]
        //R top row
        cameraOrientation[0][0] = R.x;
        cameraOrientation[1][0] = R.y;
        cameraOrientation[2][0] = R.z;
        //U middle row
        cameraOrientation[0][1] = U.x;
        cameraOrientation[1][1] = U.y;
        cameraOrientation[2][1] = U.z;
        //F bottom row
        cameraOrientation[0][2] = -F.x;
        cameraOrientation[1][2] = -F.y;
        cameraOrientation[2][2] = -F.z;

        glm::mat4 viewMatrix = cameraOrientation * cameraPosMatrix;
        //can also use
        /*
        glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraCenter, worldUp);
        */

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc,
            1,
            GL_FALSE,
            glm::value_ptr(viewMatrix));

        //from week 2
        //unsigned int xLoc = glGetUniformLocation(shaderProgram, "x");//note: same name as variable in vert shader
        //glUniform1f(xLoc, x_mod);

        //unsigned int yLoc = glGetUniformLocation(shaderProgram, "y");
        //glUniform1f(yLoc, y_mod);

        //for week 3
        //from identity, then translate, then scale, the rotate

        //for camera
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc,
            1,
            GL_FALSE,
            glm::value_ptr(projection));

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

        //get location of tex 0 in fragment shader
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        //tell opengl to use the texture
        glBindTexture(GL_TEXTURE_2D, texture);
        //use the texture at 0
        glUniform1i(tex0Address, 0);

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