/* FILENAME: IMAGE.H
 * PROGRAMMER: MY6
 * PURPOSE: Image handle declaration module.
 * LAST UPDATE: 10.06.2014
 */

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "def.h"

/* Структура хранения изображения */
typedef struct tagIMAGE
{
  INT W, H;    /* Размер изображения */
  HBITMAP hBm; /* Дескриптор загруженного изображения */
  HDC hDC;
  DWORD *Bits; /* Цвета точек */
} IMAGE;

/* Функция загрузки изображения */
BOOL ImageLoad( IMAGE *Img, CHAR *FileName );

/* Функция освобождения памяти из-под изображения */
VOID ImageFree( IMAGE *Img );

/* Функция получения цвета точки изображения */
DWORD ImageGetP( IMAGE *Img, INT X, INT Y );

VOID ImageDraw( IMAGE *Img, HDC hDC, INT X, INT Y, INT LogOp );

#endif /* __IMAGE_H_ */

/* END OF 'IMAGE.H' FILE */
