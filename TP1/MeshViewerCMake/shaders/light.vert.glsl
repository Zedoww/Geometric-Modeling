#version 120
attribute vec3 position;
attribute vec3 normal;
uniform mat4 myprojection_matrix;
uniform mat4 myview_matrix;
uniform mat3 mynormal_matrix;
varying vec3 vNormal;

void main(void) {
    gl_Position = myprojection_matrix * myview_matrix * vec4(position, 1.0);
    vNormal = mynormal_matrix * normal;
}
