precision mediump float;

uniform sampler2D myTexture;
uniform sampler2D myTexture2;

varying vec2 myTex;

void main(void)
{
    mat3 colorConversionMatrix = mat3( 1.164,  1.164, 1.164,
                                       0.0,   -0.213, 2.112,
                                       1.793, -0.533, 0.0 );
    vec3 yuv;
    vec3 rgb;
    
    yuv.x  = texture2D(myTexture, myTex).r - (16.0/255.0);
    yuv.yz = texture2D(myTexture2, myTex).ra - vec2(0.5, 0.5);
    
    rgb = colorConversionMatrix * yuv;
    
    gl_FragColor = vec4(rgb, 1.0 );

}
