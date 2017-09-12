varying highp vec2 textureCoordinate;
precision mediump float;
uniform sampler2D luminanceTexture;
void main(){
    // 本来的色值
    vec4 color = texture2D(luminanceTexture, textureCoordinate);
    
    
    // rbg相加/3
    // 不懂的可以百度百科 三基色原理
    float c = (color.r + color.g + color.b)/3.0;
    
    gl_FragColor = vec4(c, c,c, color.w);
}
