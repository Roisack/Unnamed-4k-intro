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
"   col.r = texCoord.s;"
"   col.g = texCoord.t;"
"   col.b = col.r / col.g;"
"   gl_FragColor = col;"
"}";
