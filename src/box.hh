#ifndef _BOX_HH_
#define _BOX_HH_

#include "vertex.hh"
#include "common.hh"

struct Box {
    static const size_t NUM_VERTICES = 24;

    std::vector<Vertex> vertices;

    Box(std::initializer_list<Vertex> init_list)
        : vertices { init_list }
    {
        assert(vertices.size() == NUM_VERTICES && "wrong number of vertices");
    };
};

#endif /* _BOX_HH_ */
