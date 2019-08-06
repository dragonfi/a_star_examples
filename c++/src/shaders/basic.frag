#version 120

precision highp float;

in vec4 ex_Color;

void main(void) {
    gl_FragColor = vec4(ex_Color);
}
