varying highp vec2 textureCoordinate;
precision mediump float;
uniform sampler2D luminanceTexture;
void main(){
    // 本来的色值
    vec4 color = texture2D(luminanceTexture, textureCoordinate);
    
    // 亮度系数值
    vec3 lum = vec3(0.2125,0.7154,0.0721);
    // 这里获取的矩阵为2*2的(就是当前点的前后左右)
    // 如果不存在边则两个点颜色很接近，过滤器返回一个较小的值，否则就可判断出边缘的存在。
    // 本来需要传入宽高进来的,但是这里省略了,直接除了600.0
    float color00 = dot(texture2D(luminanceTexture,textureCoordinate + vec2(-1.0,-1.0)/600.0).xyz,lum);
    float color01 = dot(texture2D(luminanceTexture,textureCoordinate + vec2(-1.0, 1.0)/600.0).xyz,lum);
    float color10 = dot(texture2D(luminanceTexture,textureCoordinate + vec2( 1.0,-1.0)/600.0).xyz,lum);
    float color11 = dot(texture2D(luminanceTexture,textureCoordinate + vec2( 1.0, 1.0)/600.0).xyz,lum);
    
    //GX为水平过滤器，GY为垂直过滤器，垂直过滤器就是水平过滤器旋转90度。
    float Gx = color00 - color11;
    float Gy = color01 - color10;
    
    //求出图像的每一个像素的横向及纵向灰度值通过以下公式结合，来计算该点灰度的大小
    float g = abs(Gx)+abs(Gy);
    // 这里是为了增强边缘颜色
//    g = g * 2.0;
    
    // 边缘颜色为红色  红色的rbg 是255,23,140
    vec4 addColor = vec4(g*(255.0/255.0),g*(23.0/255.0),g*(140.0/255.0),1);
    gl_FragColor = vec4(color.r+addColor.r, color.g+addColor.g,color.b+addColor.b, color.w);
}
