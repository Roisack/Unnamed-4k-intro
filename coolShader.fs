varying vec2 texCoord;
uniform float time;

void main()
{
    vec4 col;
    col.r = sin(texCoord.s);
    col.g = sin(texCoord.t);
    col.b = sin(time/10.0);
	gl_FragColor = col;
}
