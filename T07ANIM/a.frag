/* FILE NAME   : a.frag
 * PURPOSE     : Simple fragment shader.
 * PROGRAMMER  : MY6.
 * LAST UPDATE : 17.06.2014
 */

#version 420

/* Выходное значение цвета рисования */
out vec4 OutColor;

/* Глобальные данные */
uniform float Time;
uniform vec4 LightPosition;
uniform vec4 LightColor;
uniform vec3 ViewDir;

/* Материал */
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Phong;
uniform float Trans;

uniform vec4 UseColor;
uniform mat4 MatrWVP;
uniform mat4 MatrWorldInverseTranspose;
uniform mat4 MatrWorld;

uniform int Joy[10];
uniform float J0;
uniform float J1;
uniform float Data[10];


/* Текстура */
uniform sampler2D DrawTexture;

/* Входные данные */
in vec4 DrawColor;
in vec3 DrawPos;
in vec2 DrawTexCoord;
in vec3 DrawNormal;
in vec4 CameraPos;

vec3 Illum( vec3 N )
{
  vec4 texc = texture2D(DrawTexture, DrawTexCoord.xy);
  vec3 color = Ka;
  vec3 Dir = mat3(MatrWorld) * ViewDir; 

  vec3 lPos = vec3(0, 3, 0);
  vec3 l = normalize(lPos - DrawPos);

  N = faceforward(N, ViewDir, N);
  float nl = dot(N, l);
  if (nl > 0)
    color += (texc.xyz * 1 + Kd + vec3(0.3, 0.63, 0.3)) * nl;

  vec3 R = reflect(Dir, N);
  R = Dir - N * (2 * dot(Dir, N));
  float rl = dot(R, l);
  if (rl > 0)
    color += Ks * pow(dot(R, l), 14);

  return color;
}


/* Main function */
void main( void )
{
  vec4 texc = texture2D(DrawTexture, DrawTexCoord.xy);
  if (J0 == 1)
    OutColor = vec4(1, 1, 1, 1);
  else
    OutColor = vec4(texc.xyz, 1); //vec4(DrawNormal.xyz / 10, Trans);
  if (Data[0] == 1)
    OutColor = vec4(0, 0, 0, 1);
  //OutColor = vec4(DrawNormal.xyz, Trans);
  /*
  float start = 2.5, end = -0.61;
  float dist = CameraPos.z;
  if (dist < -1)
    discard;
  float t = 0.5;
  if (dist > start)
    t = 1;
  else
    if (dist < end)
      t = 0;
    else
      t = 1 - (dist - start) / (end - start);
  OutColor = vec4(0.3, 0.5, 0.7, 1) * (1 - t) + vec4(Illum(DrawNormal).xyz, Trans) * t;
  OutColor = vec4(1, 0, 1, 1);
  */
} /* End of 'main' function */

/* End of 'a.frag' file */
