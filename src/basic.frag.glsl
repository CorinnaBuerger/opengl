#version 330 core

struct Material {
    sampler2D ambient, diffuse, specular;
    float     shininess;
};

struct Point_light {
    vec3 position;
    vec3 ambient, diffuse, specular;
};

struct Dir_light {
    vec3 direction;
    vec3 ambient, diffuse, specular;
};

out vec4 frag_color;

// Varyings.
in vec3 v_normal;
in vec3 v_frag_pos;
in vec2 v_uv;

uniform vec3        u_lighting_color;
uniform vec3        u_view_pos;
uniform Material    u_material;
uniform Point_light u_point_light;
uniform Dir_light   u_dir_light;

vec3 calc_dir_light(Dir_light light, vec3 normal, vec3 view_dir)
{
    vec3 lighting_dir = normalize(-light.direction);

    // Ambient lighting.
    vec3 ambient = light.ambient * vec3(texture(u_material.ambient, v_uv));
    // Diffuse lighting.
    float dot_product = max(dot(normal, lighting_dir), 0.0);
    vec3 diffuse = (
        light.diffuse * dot_product * vec3(texture(u_material.diffuse, v_uv))
    );
    // Specular Lighting.
    vec3 reflect_dir = reflect(-lighting_dir, normal);
    float dot_product_spec = pow(max(dot(view_dir, reflect_dir), 0.0),
                                 u_material.shininess);
    vec3 specular = (
        light.specular * dot_product_spec * vec3(texture(u_material.specular, v_uv))
    );

    return ambient + diffuse + specular;
}

vec3 calc_point_light(Point_light light, vec3 normal, vec3 view_dir, vec3 frag_pos)
{
    vec3 lighting_dir = normalize(light.position - frag_pos);

    // Ambient lighting.
    vec3 ambient = light.ambient * vec3(texture(u_material.ambient, v_uv));
    // Diffuse lighting.
    float dot_product = max(dot(normal, lighting_dir), 0.0);
    vec3 diffuse = (
        light.diffuse * dot_product * vec3(texture(u_material.diffuse, v_uv))
    );
    // Specular lighting.
    vec3 reflect_dir = reflect(-lighting_dir, normal);
    float dot_product_spec = pow(max(dot(view_dir, reflect_dir), 0.0),
                                 u_material.shininess);
    vec3 specular = (
        light.specular * dot_product_spec * vec3(texture(u_material.specular, v_uv))
    );

    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm = normalize(v_normal);
    vec3 view_dir = normalize(u_view_pos - v_frag_pos);
    vec3 output_color = vec3(0.0);
    output_color += calc_dir_light(u_dir_light, norm, view_dir);
    output_color += calc_point_light(u_point_light, norm, view_dir, v_frag_pos);

    frag_color = vec4(output_color, 1.0);
}
