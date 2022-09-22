#include "object.h"

OBJ_HERO Hero;
OBJ_ENEMY_LIST *EnemyList;
IMAGE IMAGE_LIB;

int8_t Obj_Init(void) {
    IMAGE_LIB.Background.pointer = background;
    IMAGE_LIB.Background.width = 240;
    IMAGE_LIB.Background.height = 320;
    
    EnemyList = (OBJ_ENEMY_LIST*)malloc(sizeof(OBJ_ENEMY_LIST));

    if(EnemyList == NULL)
        return -1;

    EnemyList->next = NULL;
    EnemyList->lenth = 0;

    return 0;
}

int8_t Create_Hero(void) {
    Hero.loc_x = 100;
    Hero.loc_y = 100;
    Hero.speed = 5;
    Hero.status = Alive;
    Hero.life = 3;
    Hero.dir = 0;
    return 0;
}

int8_t Create_Enemy(void) {
    OBJ_ENEMY_LIST *p = NULL;
    OBJ_ENEMY_LIST *obj = (OBJ_ENEMY_LIST*)malloc(sizeof(OBJ_ENEMY_LIST));

    if(obj == NULL)
        return -1;

    obj->enemy.loc_x = 10;
    obj->enemy.loc_y = 10;
    obj->enemy.speed = 3;
    obj->enemy.status = Alive;
    obj->enemy.life = 3;
    obj->next = NULL;

    if(EnemyList->next == NULL) 
    {
        EnemyList->next = obj;
    }
    else
    {
        p = EnemyList->next;
        while(1) 
        {
            if(p->next == NULL)
            {
                p->next = obj;
                break;
            }
            else
            {
                p = p->next;
            }
        }

    }
   
    return 0;
}
