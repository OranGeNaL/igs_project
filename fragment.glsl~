uniform sampler2D grifonTexSampler;

varying vec2 grifonTexCoord;
varying vec3 vertexCoord;
varying vec3 normals;

//Фоновый свет
uniform vec3 ambientLightColor; //Цвет фонового света
uniform float ambientLightIntensivity;//Интенсивность фонового света


void main(){
//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    vec4 texturedObjectColor = texture2D(grifonTexSampler, grifonTexCoord);//Расчет цвета пикселя с наложенной текстурой
    vec3 calculatedAmbientColor = ambientLightColor * ambientLightIntensivity;//Расчет цвета фонового света с учетом его интенсивности
    vec4 resultObjectColor = texturedObjectColor * vec4(calculatedAmbientColor, 1.0);//Расчет цвета пикселя с применением фонового света
    
    
    gl_FragColor = resultObjectColor;
    // gl_FragColor = vec4(normals, 1.0);
}
