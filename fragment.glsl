varying vec3 translatedNormals;
varying vec2 translatedTexCoords;
varying vec3 translatedVertexCoords;

uniform sampler2D tex;
uniform sampler2D grifonTexSampler;


uniform vec3 lightSourcePosition;
uniform vec3 ambientLightColor;
uniform vec3 diffuseLightColor;
uniform vec3 specularLightColor;
uniform float ambientLightIntensivity;

uniform vec3 ambientMaterialColor;
uniform vec3 diffuseMaterialColor;
uniform vec3 specularMaterialColor;
uniform float materialShiness;

uniform vec3 viewerPosition;


void main(){
    vec4 color = texture2D(tex, translatedTexCoords);
    vec3 ambientResult = ambientLightColor * ambientLightIntensivity;

    vec3 normalizedNormals = normalize(translatedNormals);
    vec3 normalizedLightPosition = normalize(translatedVertexCoords - lightSourcePosition);
    float diffuseImpact = max(dot(normalizedNormals, -normalizedLightPosition), 0.0);
    vec3 diffuserResult = diffuseLightColor * color.xyz * diffuseImpact;

    vec3 reflected = reflect(normalizedLightPosition, normalizedNormals);
    vec3 modelToViewerVector = normalize(viewerPosition - translatedVertexCoords);
    float specularImpact = pow(max(dot(reflected, modelToViewerVector), 0.0), materialShiness);
    vec3 specularResult = specularLightColor * specularMaterialColor * specularImpact;

    vec3 result = (ambientResult + diffuserResult + specularResult);

    gl_FragColor = vec4(result, 1.0);
}
