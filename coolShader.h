// TODO: Make an utility that makes this more compressed

const char* coolShader_vs = 
"varying vec2 texCoord;"
"void main()"
"{"
"    texCoord = gl_MultiTexCoord0.xy;"
"    gl_Position = ftransform();"
"}";

const char* coolShader_fs = 
"varying vec2 texCoord;"
"uniform float time;"
"void main()"
"{"
"   vec4 col;"
"   col.r = sin(time/10.0);"
"   col.g = sin(time/10.0);"
"   col.b = sin(time/10.0);"
"   gl_FragColor = col;"
"}";
