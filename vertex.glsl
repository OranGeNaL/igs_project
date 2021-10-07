uniform sampler2D grifonTexSampler;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

attribute vec3 glPoint;
attribute vec3 glNormal;
attribute vec2 glTextureCoord;

varying vec3 translatedNormals;
varying vec2 translatedTexCoords;
varying vec3 translatedVertexCoords;


void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(glPoint, 1.0);
    translatedVertexCoords = gl_Position.xyz;
    translatedNormals = (modelMatrix * vec4(glNormal, 1.0)).xyz;
    translatedTexCoords = glTextureCoord;
}
