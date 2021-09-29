uniform sampler2D grifonTexSampler;

varying vec2 grifonTexCoord;
varying vec3 vertexCoord;
varying vec3 normals;

void main(){
//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    gl_FragColor = texture2D(grifonTexSampler, grifonTexCoord);
}
