#include "object.h"

OBJ_HERO Hero;
//OBJ_ENEMY_LIST *EnemyList;
IMAGE IMAGE_LIB;
OBJ_Bullet Bullets[20];
OBJ_Bullet Enemy_Bullets[20];
OBJ_ENEMY Enemys[5];

int8_t Obj_Init(void) {
    uint8_t i = 0;

    Image_Init();

    Create_Hero();

    for (i = 0; i < 20; i++) {
        OBJ_Bullet bullet;
        bullet.status = Destroy;
        Bullets[i] = bullet;
    }

    for (i = 0; i < 20; i++) {
        OBJ_Bullet bullet;
        bullet.status = Destroy;
        Enemy_Bullets[i] = bullet;
    }

     for (i = 0; i < 5; i++) {
        OBJ_ENEMY enemy;
        enemy.status = Destroy;
        enemy.hasDead = 1;
        Enemys[i] = enemy;
    }
    
    // EnemyList = (OBJ_ENEMY_LIST*)malloc(sizeof(OBJ_ENEMY_LIST));

    // if(EnemyList == NULL)
    //     return -1;

    // EnemyList->next = NULL;
    // EnemyList->lenth = 0;

    return 0;
}

void Image_Init(void) {
    IMAGE_LIB.Background.pointer = background;
    IMAGE_LIB.Background.width = 240;
    IMAGE_LIB.Background.height = 320;

    IMAGE_LIB.Enemy.pointer = enemy;
    IMAGE_LIB.Enemy.width = 50;
    IMAGE_LIB.Enemy.height = 70;

    IMAGE_LIB.BOOM1.pointer = boom1;
    IMAGE_LIB.BOOM1.width = 50;
    IMAGE_LIB.BOOM1.height = 70;

    IMAGE_LIB.BOOM2.pointer = boom2;
    IMAGE_LIB.BOOM2.width = 50;
    IMAGE_LIB.BOOM2.height = 70;

    IMAGE_LIB.Hero.pointer = flight;
    IMAGE_LIB.Hero.width = 40;
    IMAGE_LIB.Hero.height = 50;

    IMAGE_LIB.Bullet.pointer = bullet;
    IMAGE_LIB.Bullet.width = 6;
    IMAGE_LIB.Bullet.height = 12;
}

int8_t Create_Hero(void) {
    Hero.loc_x = 100;
    Hero.loc_y = 200;
    Hero.speed = 5;
    Hero.status = Alive;
    Hero.life = 3;
    Hero.dir = 0;
    Hero.score = 0;
    return 0;
}

int8_t Create_Bullet(void) {
    uint8_t i = 0;
    int8_t flag = -1;
    OBJ_Bullet bullet;
    bullet.loc_x = Hero.loc_x + 17;
    bullet.loc_y = Hero.loc_y - 12;
    bullet.speed = 5;
    bullet.status = Alive;

    for(i = 0;i < 20; i++) {
        if(Bullets[i].status == Destroy) {
            Bullets[i] = bullet;
            flag = 1;
            break;
        }
    }

    return flag;
}

int8_t Enemy_Bullet(OBJ_ENEMY *enemy) {
    uint8_t i = 0;
    int8_t flag = -1;
    OBJ_Bullet bullet;

    bullet.loc_x = enemy->loc_x + 25;
    bullet.loc_y = enemy->loc_y + 65;  
    bullet.speed = 2;
    bullet.status = Alive;

    for(i = 0; i < 20; i++) {
        if(Enemy_Bullets[i].status == Destroy) {
            Enemy_Bullets[i] = bullet;
            flag = 1;
            break;
        }
    }

    return flag;
}

uint8_t random_number[] = {20, 170, 80, 30, 120, 50, 180, 140, 100, 10, 80, 130};

int8_t Create_Enemy(void) {
    uint8_t i = 0;
    int8_t flag = -1;
    static uint8_t num = 0;
    OBJ_ENEMY enemy;
    enemy.loc_x = random_number[num];
    enemy.loc_y = -60;

    if(++num >= 12)
        num = 0;

    enemy.speed = 1;
    enemy.status = Alive;
    enemy.life = 8;
    enemy.hasDead = 0;

    for(i = 0;i < 5; i++) {
        if(Enemys[i].status == Destroy) {
            Enemys[i] = enemy;
            flag = 1;
            break;
        }
    }

    return flag;
}


// int8_t Create_Enemy(void) {
//     OBJ_ENEMY_LIST *p = NULL;
//     OBJ_ENEMY_LIST *obj = (OBJ_ENEMY_LIST*)malloc(sizeof(OBJ_ENEMY_LIST));

//     if(obj == NULL)
//         return -1;

//     obj->enemy.loc_x = 10;
//     obj->enemy.loc_y = 10;
//     obj->enemy.speed = 3;
//     obj->enemy.status = Alive;
//     obj->enemy.life = 3;
//     obj->next = NULL;

//     if(EnemyList->next == NULL) 
//     {
//         EnemyList->next = obj;
//     }
//     else
//     {
//         p = EnemyList->next;
//         while(1) 
//         {
//             if(p->next == NULL)
//             {
//                 p->next = obj;
//                 break;
//             }
//             else
//             {
//                 p = p->next;
//             }
//         }

//     }
   
//     return 0;
// }
