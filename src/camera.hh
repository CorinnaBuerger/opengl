#ifndef _CAMERA_HH_
#define _CAMERA_HH_

#include "common.hh"

class Camera {
private:
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 x_axis;
    std::string name;
    float yaw = -90.0f, pitch = 0.0f;

    static constexpr float sensitivity = 0.1f;
    static constexpr glm::vec3 world_up { glm::vec3(0.0, 1.0, 0.0) };

public:
    float zoom;
    glm::vec3 pos;

    Camera(glm::vec3 front, glm::vec3 up, glm::vec3 x_axis,
           std::string name, float zoom, glm::vec3 pos)
        : front { front }, up { up },
          x_axis { x_axis }, name { std::move(name) },
          zoom { zoom }, pos { pos }
    {
    }

    Camera(const Camera&) = default;
    Camera(Camera&&)      = default;
    Camera& operator=(const Camera&) = default;
    Camera& operator=(Camera&&)      = default;

    virtual ~Camera(void) = default;

    glm::mat4 get_matrix(void)
    {
        return glm::lookAt(this->pos,
                           this->pos + this->front,
                           this->up);
    }

    void process_mouse_callback(double x_offset, double y_offset)
    {
        x_offset *= this->sensitivity;
        y_offset *= this->sensitivity;

        this->yaw   += x_offset;
        this->pitch += y_offset;

        if (this->pitch >  89.0f) this->pitch = 89.0f;
        if (this->pitch < -89.0f) this->pitch = -89.0f;

        glm::vec3 direction;
        direction.x  = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y  = sin(glm::radians(pitch));
        direction.z  = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->front  = glm::normalize(direction);
        this->x_axis = glm::normalize(glm::cross(this->front, this->world_up));
        this->up     = glm::normalize(glm::cross(this->x_axis, this->front));
    }

    void process_keyboard_input(GLFWwindow* window, float dt)
    {
        float camera_speed = 2.5f * dt;
        float zoom_offset  = 1.0f;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
                   glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            this->pos += this->front * camera_speed;
        } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
                   glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            this->pos -= this->front * camera_speed;
        } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
                   glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            this->pos -= this->x_axis * camera_speed;
        } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
                   glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            this->pos += this->x_axis * camera_speed;
        } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            this->pos += this->up * camera_speed;
        } else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            this->pos -= this->up * camera_speed;
        } else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            this->zoom += zoom_offset;
            if (this->zoom > 45.0f) this->zoom = 45.0f;
        } else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            this->zoom -= zoom_offset;
            if (this->zoom < 1.0f) this->zoom = 1.0f;
        }
    }
};

#endif /* _CAMERA_HH_ */
