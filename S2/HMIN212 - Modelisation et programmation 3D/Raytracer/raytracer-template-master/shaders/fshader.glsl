#version 150

uniform vec4 AmbientProduct;
uniform vec4 DiffuseProduct;
uniform vec4 SpecularProduct;
uniform vec4 LightPosition;
uniform float Shininess;

uniform mat4 ModelViewLight;

in vec4 pos;
in vec4 N;

out vec4 fragColor;

void main()
{
  
  vec4 L;
  if(LightPosition.w == 0.0){
    L = normalize(LightPosition - pos); L.w = 0;
  }else{
    L = normalize((ModelViewLight*LightPosition) - pos ); L.w = 0;
  }
  
  vec4 V = normalize( -pos);  V.w = 0.0;
  vec4 R = normalize(-reflect(L,N));
  
  // Compute terms in the illumination equation
  vec4 ambient = AmbientProduct;
  
  float Kd = max( dot(L, N), 0.0 );
  vec4  diffuse = Kd*DiffuseProduct;
  
  float Ks = pow( max(dot(V, R), 0.0), Shininess );
  
  //Phong/Blinn used the half angle approx:
  //float Ks = pow( max(dot(N, normalize( L + V ) ), 0.0), Shininess );
  
  vec4  specular = Ks * SpecularProduct;
  
  if( dot(L, N) < 0.0 ) {
    fragColor = vec4(0.0, 0.0, 0.0, 1.0);
  }else{
    fragColor = ambient + diffuse + specular;
  }
  
}

