attribute vec4 position;
attribute vec2 inputTextureCoordinate;
varying vec2 textureCoordinate;
void main()
{
    gl_Position = position;
    textureCoordinate = vec2(1.0 - inputTextureCoordinate.y,1.0 - inputTextureCoordinate.x);
}
