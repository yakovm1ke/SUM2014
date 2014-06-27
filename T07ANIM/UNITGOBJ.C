 /* FILENAME: GOBJ.C
 * PROGRAMMER: MY6
 * PURPOSE: GObj Unit
 * LAST UPDATE: 17.06.2014
 */

#include "anim.h"
#include "render.h"
#include "geom.h"

/* Структура описания объекта анимации */
typedef struct tagmy6UNIT_GOBJ
{
  MY6_UNIT_BASE_FIELDS; /* Включение базовых полей */
  my6GEOM Road;
  my6GEOM Car;
} my6UNIT_GOBJ;

MATR WVP;

FLT RoadR = 20000;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_GOBJ *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

static VOID GObjUnitInit( my6UNIT_GOBJ *Unit, my6ANIM *Ani )
{
  my6PRIM prim = {0};
  my6MATERIAL mtl = {0};

  memset(&Unit->Road, 0, sizeof(my6GEOM));
  memset(&Unit->Car, 0, sizeof(my6GEOM));

  MY6_GeomLoad(&Unit->Car, "x6.object");
  MY6_GeomTransform(&Unit->Car, MatrRotateX(-90));
  MY6_GeomTransform(&Unit->Car, MatrRotateY(180));
  MY6_GeomTransform(&Unit->Car, MatrTranslate(2.7, 0.87, 0));


  MY6_PrimCreateCylinder(&prim, 200, RoadR, 2000, 5);

  mtl.Ka = VecSet(0.1, 0.1, 0.1);
  mtl.Kd = VecSet(1, 1, 1);
  mtl.Ks = VecSet(0, 0, 0);
  mtl.Phong = 30;
  mtl.Trans = 1;
  mtl.TexNo = 0;
  mtl.MapD[0] = 0;
  strcpy(mtl.MapD, "r.bmp");
  strcpy(mtl.Name, "Road material");
  prim.Mtl = MY6_GeomAddMaterial(&Unit->Road, &mtl);

  MY6_GeomAddPrim(&Unit->Road, &prim);

} /* End of 'GObjUnitInit' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_GOBJ *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

static VOID GObjUnitResponse( my6UNIT_GOBJ *Unit, my6ANIM *Ani )
{
} /* End of 'GObjUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_GOBJ *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

static VOID GObjUnitRender( my6UNIT_GOBJ *Unit, my6ANIM *Ani )
{
  INT loc;
  static INT ShadProgId;
  static DBL rotatey, rotatex, scale, tr, time, full_shift = 0, vert_shift, hor_shift = 0, v = 0;
  static DBL rot1 = 0;
  static DBL rot = 0;

  rotatey += Ani->JX * 4;
  rotatex += Ani->JY * 4;
  scale += Ani->JR / 50;

  vert_shift += (Ani->JPOV == 1) * Ani->DeltaTime * 10.0 - (Ani->JPOV == 5) * Ani->DeltaTime * 10.0;
  hor_shift += Ani->JX * Ani->DeltaTime * 6.0;
  hor_shift += Ani->Keys['D'] * Ani->DeltaTime * 6.0;
  hor_shift -= Ani->Keys['A'] * Ani->DeltaTime * 6.0;

  if(v > 0)
  {
    rot = Ani->JX * 6;
    if(Ani->Keys['D'])
    {
      rot1 += 6;
      if(rot1 > 500)
        rot1 = 500;
    }

    if(Ani->Keys['A'])
    {
      rot1 -= 6;
      if(rot1 < -500)
        rot1 = -500;
    }
    rot += rot1 * Ani->DeltaTime;
  }
  else
  {
    rot = -Ani->JX * 6;
    if(Ani->Keys['D'])
    {
      rot1 -= 6;
      if(rot1 < -500)
        rot1 = -500;
    }

    if(Ani->Keys['A'])
    {
      rot1 += 6;
      if(rot1 > 500)
        rot1 = 500;
    }
    rot += rot1 * Ani->DeltaTime;
  }
  /*if((Ani->Keys['A'] && Ani->Keys['D']) || ((Ani->Keys['D'] == 0) && (Ani->Keys['A'] == 0)))
  {
    if(rot1 == 0)
      rot = 0;
    while(rot1 != 0)
    {
      if(rot1 > 0)
        rot1 -= 6;
      else
        rot1 += 6;
    }
    rot += rot1 * Ani->DeltaTime;
  }*/

  /*if(Ani->Keys['A'] && Ani->Keys['D'])
  {
    if(rot1 > 0)
    {
      while (rot1 != 0)
        rot1 -= 6;
      rot = rot1 * Ani->DeltaTime;
    }
    else
    {
      while (rot1 != 0)
        rot1 -= 6;
      rot = rot1 * Ani->DeltaTime;
    }
  }*/

  if(vert_shift < -4)
    vert_shift = -4;
  if(vert_shift > 10)
    vert_shift = 10;


  Ani->MatrView = MatrViewLookAt(VecSet(2.7, 3.5 + vert_shift, 30), VecSet(2.7, 1, 0), VecSet(0, 1, 0));

  Ani->MatrProjection = MatrProjection(-Ani->Wp / 2, Ani->Wp / 2,
                                       -Ani->Hp / 2, Ani->Hp / 2,
                                        Ani->ProjDist, Ani->FarClip);

  //Ani->MatrWorld = MatrScale(1 + scale, 1 + scale, 1 + scale);
  //Ani->MatrWorld = MatrMulMatr(Ani->MatrWorld, MatrRotateY(rotatey));
  //Ani->MatrWorld = MatrMulMatr(Ani->MatrWorld, MatrRotateX(rotatex));

  WVP = MatrMulMatr(Ani->MatrWorld, MatrMulMatr(Ani->MatrView, Ani->MatrProjection));

  glLoadMatrixf(WVP.A[0]);

  if (Ani->JButs[5])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  time += Ani->GlobalDeltaTime;
  if (time > 1)
  {
    time = 0;
    MY6_ShadProgClose(MY6_ShaderProg);
    MY6_ShaderProg = MY6_ShadProgInit("a.vert", "a.frag");
  }

  glUseProgram(MY6_ShaderProg);

  loc = glGetUniformLocation(MY6_ShaderProg, "J0");
  if (loc != -1)
    glUniform1f(loc, Ani->JButs[0]);
  loc = glGetUniformLocation(MY6_ShaderProg, "J1");
  if (loc != -1)
    glUniform1f(loc, Ani->JButs[1]);



  /* Draw road */
  v += Ani->JZ * 0.01;
  v -= Ani->Keys['W'] * 0.01;
  v += Ani->Keys['S'] * 0.01;
  full_shift += v * Ani->DeltaTime;
  if(Ani->JButsClick[7] || Ani->Keys[' '])
    v = 0;
  if(v < -0.8)
    v = -0.8;
  if(v > 2)
    v = 2;
  if(hor_shift > 10)
    hor_shift = 10;
  if(hor_shift < -15)
    hor_shift = -15;
  if (v == 0)
  {
    hor_shift -= Ani->JX * Ani->DeltaTime * 6.0;
    hor_shift -= Ani->Keys['D'] * Ani->DeltaTime * 6.0;
    hor_shift += Ani->Keys['A'] * Ani->DeltaTime * 6.0;
  }

  Ani->MatrWorld = MatrMulMatr(MatrRotateX(-full_shift), MatrTranslate(-hor_shift, -RoadR, 0));
  MY6_GeomDraw(&Unit->Road);

  Ani->MatrWorld = MatrRotateY(rot);
  MY6_GeomDraw(&Unit->Car);

  Ani->MatrWorld = MatrRotateY(rot);
  Ani->MatrWorld = MatrMulMatr(Ani->MatrWorld, MatrScale(1, 0, 1));
  Ani->MatrWorld = MatrMulMatr(Ani->MatrWorld, MatrTranslate(0, 0.02, 0));

  Unit->Car.Data[0] = 1;
  MY6_GeomDraw(&Unit->Car);
  Unit->Car.Data[0] = 0;

  glUseProgram(0);
} /* End of 'GObjUnitRender' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_GOBJ *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID GObjUnitClose( my6UNIT_GOBJ *Unit, my6ANIM *Ani )
{
  MY6_GeomFree(&Unit->Road);
} /* End of 'GObjUnitClose' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (my6UNIT *) указатель на созданный объект анимации.
 */

my6UNIT * MY6_GObjUnitCreate( VOID )
{
  my6UNIT_GOBJ *Unit;

  if ((Unit = (my6UNIT_GOBJ *)MY6_AnimUnitCreate(sizeof(my6UNIT_GOBJ))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)GObjUnitInit;
  Unit->Close = (VOID *)GObjUnitClose;
  Unit->Response = (VOID *)GObjUnitResponse;
  Unit->Render = (VOID *)GObjUnitRender;
  return (my6UNIT *)Unit;
} /* End of 'MY6_GObjUnitCreate' function */

/* END OF 'UNITGOBJ.C' FILE */
