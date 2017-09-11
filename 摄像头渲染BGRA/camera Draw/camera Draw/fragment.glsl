varying highp vec2 textureCoordinate;
precision mediump float;
uniform sampler2D luminanceTexture;
void main(){
    vec4 color = texture2D(luminanceTexture, textureCoordinate);
    gl_FragColor = vec4(color.x, color.y, color.z, color.w);
}
