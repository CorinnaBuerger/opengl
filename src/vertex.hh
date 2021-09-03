#ifndef _VERTEX_HH_
#define _VERTEX_HH_

#include "common.hh"

struct Vertex {
    float x, y, z; // coordinates
    float n1, n2, n3; // normals
    float u, v;    // texture coordinates

    Vertex(float x, float y, float z, float n1, float n2, float n3, float u, float v)
        : x { x }, y { y }, z { z },
          n1 { n1 }, n2 { n2 }, n3 { n3 },
          u { u }, v { v }
    {
    }

    glm::vec3 get_coords(void)
    {
        return glm::vec3 { this->x, this->y, this->z };
    }

    glm::vec3 get_normal(void)
    {
        return glm::vec3 { this->n1, this->n2, this->n3 };
    }

    glm::vec2 get_uvs(void)
    {
        return glm::vec2 { this->u, this->v };
    }

    // Sets and enables vertex attributes according to the specification above.
    static void enable_as_attrib(void)
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)offsetof(Vertex, x));
        glEnableVertexAttribArray(0);

        // normal attribute at 1
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)offsetof(Vertex, n1));
        glEnableVertexAttribArray(1);

        // texture attribute at 2
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)offsetof(Vertex, u));
        glEnableVertexAttribArray(2);
    }
};

#endif /* _VERTEX_HH_ */
