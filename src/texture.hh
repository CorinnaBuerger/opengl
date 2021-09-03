#ifndef _TEXTURE_HH_
#define _TEXTURE_HH_

#include "common.hh"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "logger.hh"

Logger texture_logger {};

struct Texture {
    unsigned int tex_id;
    int width, height, nr_channels, texture_unit;

    Texture(std::string_view file, int texture_unit)
        : tex_id { 0 }, width { 0 }, height { 0 },
          nr_channels { 3 }, texture_unit { texture_unit }
    {
        unsigned char* data;

        glGenTextures(1, &this->tex_id);
        glActiveTexture((GL_TEXTURE0 + this->texture_unit));
        glBindTexture(GL_TEXTURE_2D, this->tex_id);

        // set texture wrapping/filtering options (on currently bound texture)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load and generate the texture
        stbi_set_flip_vertically_on_load(true);
        data = stbi_load(file.data(), &width, &height, &nr_channels, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            texture_logger.panic("Could not load texture");
        }
        stbi_image_free(data);
    }

    ~Texture(void) {
        glDeleteTextures(1, &this->tex_id);
    }

    void bind(void)
    {
        glActiveTexture((GL_TEXTURE0 + this->texture_unit));
        glBindTexture(GL_TEXTURE_2D, this->tex_id);
    }
};

#endif /* _TEXTURE_HH_ */
