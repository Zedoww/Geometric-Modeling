#version 120
varying vec3 vNormal;
uniform vec4 kd;
uniform int type;

void main(void) {
    if (type != 0) {
        gl_FragColor = kd;
        return;
    }
    vec3 L = normalize(vec3(0.5, 0.8, 1.0));
    float d = abs(dot(normalize(vNormal), L));
    float shade = 0.25 + 0.75 * d;
    gl_FragColor = vec4(kd.rgb * shade, kd.a);
}
