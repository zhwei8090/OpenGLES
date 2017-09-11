#version 300 es
precision mediump float;
in vec2 v_texCoord;
layout(location = 0)out vec4 outColor;
uniform sampler2D s_texture;
void main()
{
    outColor = texture(s_texture,v_texCoord);
//    if (outColor.b >= 0.5 &&
//        outColor.g >= 0.5 &&
//        outColor.r >= 0.5 ) {
//        outColor.a = 0;
//    }
//    gl_FragColor = 1;
}
