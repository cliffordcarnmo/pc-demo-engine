#version 430
uniform float time;
out vec4 fragColor;
in vec2 fragCoord;
void main() {
vec3 c;
vec2 r = vec2(1.5, 1.5);
float l, z = time*2;
for (int i = 0; i<3; i++) {
vec2 uv, p = fragCoord.xy / r;
uv = p;
p -= .1;
p.x *= r.x / r.y;
z += .09;
l = length(p);
uv += p / l*(sin(z) + 1.)*abs(sin(l*19. - z*1.));
c[i] = .03 / length(abs(mod(uv, 1.) - .5));
}

fragColor = vec4(c / l, time);
}
