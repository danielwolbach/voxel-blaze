namespace consts {
    const char *vertex_shader_source = R""""(
    #version 440 core

    layout (location = 0) in vec3 in_position;
    layout (location = 1) in vec3 in_color;
    out vec3 pass_color;
    uniform mat4 model_transform;
    uniform mat4 view_transform;
    uniform mat4 projection_transform;

    void main()
    {
        pass_color = in_color;
        gl_Position = projection_transform * view_transform * model_transform * vec4(in_position, 1.0);
    }
    )"""";

    const char *fragment_shader_source = R""""(
    #version 440 core

    in vec3 pass_color;
    out vec4 out_color;

    void main()
    {
        out_color = vec4(pass_color, 1.0f);
    }
    )"""";
}