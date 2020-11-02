#version 330

smooth in vec2 quad_coord;

out vec4 frag_color;

const float blur = 0.005;
const float radius = 1.0;

void main()
{
    float col = smoothstep(radius, radius - blur, abs(length(quad_coord)));

    frag_color = vec4(col);
}
