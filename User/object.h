#ifndef __OBJECT_H
#define	__OBJECT_H

#include "stm32f10x.h"
#include "bsp_usart.h"
#include "stdlib.h"
#include "fonts.h"

typedef struct OBJ_IMAGE {
    const uint8_t *pointer;
    uint16_t width;
    uint16_t height;
} OBJ_IMAGE;

typedef struct IMAGE {
    OBJ_IMAGE Background;
    OBJ_IMAGE Hero;
    OBJ_IMAGE Enemy;
    OBJ_IMAGE BOOM1;
    OBJ_IMAGE BOOM2;
    OBJ_IMAGE Destroy;
} IMAGE;

typedef struct OBJ_Bullet {
    uint16_t loc_x;
    uint16_t loc_y;
    uint8_t status;
    uint8_t speed;
} OBJ_Bullet;

typedef struct OBJ_ENEMY {
    uint16_t loc_x;
    uint16_t loc_y;
    uint8_t status;
    uint8_t speed;
    uint8_t life;
} OBJ_ENEMY;

typedef struct OBJ_HERO {
    uint16_t loc_x;
    uint16_t loc_y;
    uint8_t status;
    uint8_t speed;
    uint8_t life;
} OBJ_HERO;

typedef struct OBJ_ENEMY_LIST {
    uint8_t lenth;
    struct OBJ_ENEMY enemy;
    struct OBJ_ENEMY_LIST *next;
} OBJ_ENEMY_LIST;

typedef enum { 
    Dead = 0,
    Alive = 1,
    Boom1 = 2,
    Boom2 = 3,
    Destroy = 4
} Status_TypeDef;

extern OBJ_HERO Hero;
extern OBJ_ENEMY_LIST *EnemyList;
extern IMAGE IMAGE_LIB;

int8_t Obj_Init(void);
int8_t Create_Hero(void);
int8_t Create_Enemy(void);

#endif
