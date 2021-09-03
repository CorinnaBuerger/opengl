#ifndef _VAO_HH_
#define _VAO_HH_

#include "common.hh"
#include "vertex.hh"
#include "box.hh"

struct Vao {
private:
    GLuint vao_id, vbo_id, ebo_id;

public:
    Vao(GLuint ebo_id, Box box)
        : vao_id { 0 }, vbo_id { 0 }, ebo_id { ebo_id }
    {
        glGenVertexArrays(1, &this->vao_id);
        glBindVertexArray(this->vao_id);

        glGenBuffers(1, &this->vbo_id);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
        glBufferData(GL_ARRAY_BUFFER, box.vertices.size()*sizeof(Vertex),
                     box.vertices.data(), GL_STATIC_DRAW);

        Vertex::enable_as_attrib();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_id);
    }

    ~Vao() {
        GLuint buffer_ids[] { this->vbo_id, this->ebo_id };
        glDeleteBuffers(std::size(buffer_ids), buffer_ids);

        glDeleteVertexArrays(1, &this->vao_id);
    }

    void bind(void)
    {
        glBindVertexArray(this->vao_id);
    }

    void draw(void)
    {
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
};

#endif /* _VAO_HH_ */
