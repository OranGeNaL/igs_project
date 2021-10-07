uniform sampler2D grifonTexSampler;
varying vec2 grifonTexCoord;
varying vec3 vertexCoord;
varying vec3 normals;

attribute vec3 vertexNormal;
attribute vec2 grifonTexSave;

void main() {
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    //normals
    vertexCoord = gl_Position.xyz;
    // mat4 cal;
    // cal = inverse(cal);
    normals = normalize(vertexNormal);
    grifonTexCoord = grifonTexSave;
}
