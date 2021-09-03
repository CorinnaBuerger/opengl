#include <array>
#include <cmath>
#include <concepts>
#include <vector>

#include "box.hh"
#include "camera.hh"
#include "common.hh"
#include "debug.hh"
#include "logger.hh"
#include "shader.hh"
#include "texture.hh"
#include "vao.hh"
#include "vertex.hh"

struct Color : public glm::vec3 {};
Color global_clear_color;

Logger global_logger;

unsigned int indices[] {
    // front
    0, 1, 2,
    0, 1, 3,
    // back
    4, 5, 6,
    4, 5, 7,
    // left
    8, 9, 10,
    8, 9, 11,
    // right
    12, 13, 14,
    12, 13, 15,
    // top
    16, 17, 18,
    16, 17, 19,
    // bottom
    20, 21, 22,
    20, 21, 23,

};

Box box {
    // front                       // normals          // texture coords
    Vertex { -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f },
    Vertex {  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f },
    Vertex {  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f },
    Vertex { -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f },
    // back
    Vertex { -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f },
    Vertex {  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f },
    Vertex {  0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f },
    Vertex { -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f },
    // left
    Vertex { -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f },
    Vertex { -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f },
    Vertex { -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f },
    Vertex { -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f },
    // right
    Vertex {  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f },
    Vertex {  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f },
    Vertex {  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f },
    Vertex {  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f },
    // top
    Vertex { -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f },
    Vertex {  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f },
    Vertex {  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f },
    Vertex { -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f },
    // bottom
    Vertex { -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f },
    Vertex {  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f },
    Vertex {  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f },
    Vertex { -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f },
};

glm::vec3 camera_pos    { glm::vec3(0.0f, 0.0f,  3.0f) };
glm::vec3 camera_front  { glm::vec3(0.0f, 0.0f, -1.0f) };
glm::vec3 camera_up     { glm::vec3(0.0f, 1.0f,  0.0f) };
glm::vec3 camera_x      { glm::normalize(glm::cross(camera_front, camera_up)) };
Camera camera {camera_front, camera_up, camera_x, "camera_basic", 45.0f, camera_pos };
float last_x            = 512.0f;
float last_y            = 400.0f;
bool  first_mouse       = true;

void framebuffer_size_callback(GLFWwindow* /* window */, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* /*window */, double x_pos, double y_pos)
{
    if (first_mouse) {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    double x_offset { x_pos - last_x };
    double y_offset { last_y - y_pos };
    last_x = x_pos;
    last_y = y_pos;

    camera.process_mouse_callback(x_offset, y_offset);
}

// DT is in seconds since last frame (usually 0,016).
void process_input(GLFWwindow* window, float dt)
{
    glfwPollEvents();
    camera.process_keyboard_input(window, dt);
}

int main(void)
{
    GLenum err_glfw = glfwInit();
    if (GLFW_TRUE != err_glfw)
        global_logger.panic("unable to initialize GLFW");
    else
        global_logger.log("using GLFW ", glfwGetVersionString());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1024, 800,
                                          "Coco's first OpenGL window",
                                          NULL, NULL);

    if (!window) global_logger.panic("unable to create a window with GLFW3");
    glfwMakeContextCurrent(window);

    // Prior to this, you _cannot_ call any OpenGL functions!
    GLenum err = glewInit();
    if (GLEW_OK != err) global_logger.panic(glewGetErrorString(err));
    else global_logger.log("using GLEW ", glewGetString(GLEW_VERSION));

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);
    set_debug_callback(&global_logger);

    // Texture
    Texture texture1 { "./textures/container2.jpg", 0 };
    Texture texture2 { "./textures/shiba.jpg", 1 };

    // _One_ element buffer object for both triangles.
    GLuint ebo_id;
    glGenBuffers(1, &ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
                 indices, GL_STATIC_DRAW);

    Vao vao_container { ebo_id, box };
    Vao vao_light     { ebo_id, box };

    // Shader.
    Shader shader_basic { "./src/basic.vert.glsl", "./src/basic.frag.glsl" };
    Shader shader_lighting { "./src/lighting.vert.glsl", "./src/lighting.frag.glsl" };

    glEnable(GL_DEPTH_TEST); // makes sure foreground is displayed on background

    // Mouse movements.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    // Render loop.
    float last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float current_time = glfwGetTime();
        float delta_time = current_time - last_time;
        last_time = current_time;
        process_input(window, delta_time);

        // Render a new frame.
        glClearColor(global_clear_color.r, global_clear_color.g,
                     global_clear_color.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Lighting.
        shader_lighting.use();
        vao_light.bind();

        Color u_lighting_color;
        u_lighting_color.r = 0.8;
        u_lighting_color.g = 0.3;
        u_lighting_color.b = 1.0;

        glm::mat4 u_model_lighting { glm::mat4(1.0f) };
        glm::vec3 pos_lighting { glm::vec3(0.0f, 0.0f, 0.0f) };
        u_model_lighting = glm::translate(u_model_lighting, pos_lighting);
        u_model_lighting = glm::scale(u_model_lighting, glm::vec3(0.2f));

        glm::mat4 u_view_lighting;
        u_view_lighting = camera.get_matrix();

        glm::mat4 u_projection {
            glm::perspective(glm::radians(camera.zoom), 1024.0f / 800.0f, 0.1f, 100.0f)
        };

        shader_lighting.set_uniform("u_model", u_model_lighting);
        shader_lighting.set_uniform("u_view", u_view_lighting);
        shader_lighting.set_uniform("u_projection", u_projection);
        shader_lighting.set_uniform("u_lighting_color", u_lighting_color);

        vao_light.draw();

        shader_basic.use();
        vao_container.bind();

        float u_shininess          = 32.0f;
        Color u_lighting_intensity { glm::vec3(0.5f, 0.5f, 0.5f) };
        Color u_lighting_diffuse   { u_lighting_intensity * u_lighting_color };
        Color u_lighting_ambient   { u_lighting_diffuse * glm::vec3(0.2f, 0.2f, 0.2f) };
        Color u_lighting_specular  { glm::vec3(1.0f, 1.0f, 1.0f) };
        glm::vec3 dir_lighting     { glm::vec3(0.0f, -1.0f, 0.0f) };

        glm::mat4 u_view;
        u_view = camera.get_matrix();

        shader_basic.set_uniform("u_view", u_view);
        shader_basic.set_uniform("u_projection", u_projection);
        shader_basic.set_uniform("u_view_pos", camera.pos);
        shader_basic.set_uniform("u_material.ambient", 0);
        shader_basic.set_uniform("u_material.diffuse", 0);
        shader_basic.set_uniform("u_material.specular", 1);
        shader_basic.set_uniform("u_material.shininess", u_shininess);
        shader_basic.set_uniform("u_point_light.ambient", u_lighting_ambient);
        shader_basic.set_uniform("u_point_light.diffuse", u_lighting_diffuse);
        shader_basic.set_uniform("u_point_light.specular", u_lighting_specular);
        shader_basic.set_uniform("u_point_light.position", pos_lighting);
        shader_basic.set_uniform("u_dir_light.ambient", u_lighting_ambient);
        shader_basic.set_uniform("u_dir_light.diffuse", u_lighting_diffuse);
        shader_basic.set_uniform("u_dir_light.specular", u_lighting_specular);
        shader_basic.set_uniform("u_dir_light.direction", dir_lighting);

        texture1.bind();
        texture2.bind();

        std::vector<glm::vec3> cube_pos {
            glm::vec3( -3.0f, -1.0f,   0.0f),
            glm::vec3(  2.0f,  0.0f,   0.0f),
            glm::vec3( -2.0f,  3.5f,   3.0f),
            glm::vec3(  2.5f,  2.5f,  -2.0f),
            glm::vec3(  0.0f,  0.0f,  -2.0f),
            glm::vec3( -3.0f,  3.6f,  -3.0f),
            glm::vec3(  2.0f, -3.0f,   1.0f),
            glm::vec3(  0.0f, -2.0f,   0.0f),
            glm::vec3( -1.0f, -2.0f,  -2.0f),
        };

        for (size_t i = 0; i < cube_pos.size(); i++) {
            glm::mat4 u_model { glm::mat4(1.0f) };
            u_model = glm::translate(u_model, cube_pos[i]);
            float angle = 50.0f * (i + 1);
            u_model = glm::rotate(u_model, glm::radians(angle),
                               glm::vec3(1.0f, 0.3f, 0.5f));

            shader_basic.set_uniform("u_model", u_model);
            vao_container.draw();
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    global_logger.log("app shut down successfully");

    return 0;
}

