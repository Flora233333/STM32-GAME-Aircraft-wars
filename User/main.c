/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/ 
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
/* 开发板硬件bsp头文件 */
#include "bsp_ili9341_lcd.h"
#include "bsp_xpt2046_lcd.h"
#include "bsp_usart.h"
#include "bsp_beep.h"
#include "object.h"
/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
static TaskHandle_t AppTaskCreate_Handle = NULL;/* 创建任务句柄 */
static TaskHandle_t Print_Handle = NULL;
static TaskHandle_t Bullet_Handle = NULL;
static TaskHandle_t Create_Handle = NULL;
static TaskHandle_t Updata_Handle = NULL;
static TaskHandle_t Touch_Handle = NULL;
static TaskHandle_t Beep_Handle = NULL;

//static TaskHandle_t Print_TaskHandle = NULL;
//static TaskHandle_t Test_Task_Handle = NULL;/* LED任务句柄 */
//static TaskHandle_t KEY_Task_Handle = NULL;/* KEY任务句柄 */

/********************************** 内核对象句柄 *********************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
 */

SemaphoreHandle_t xBeepOn;

/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */




/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void AppTaskCreate(void);/* 用于创建任务 */
static void Print_Image(void *parm);
static void Bullet_Manage(void *parm);
static void Create_EnemyFlight(void *parm);
static void Updata_location(void *parm);
static void Touch_Detect(void *parm);
static void Beep_Control(void *parm);

static void BSP_Init(void);/* 用于初始化板载相关资源 */

/*****************************************************************
  * @brief  主函数
  * @param  无
  * @retval 无
  * @note   第一步：开发板硬件初始化 
            第二步：创建APP应用任务
            第三步：启动FreeRTOS，开始多任务调度
  ****************************************************************/
int main(void)
{	
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    
    /* 开发板硬件初始化 */
    BSP_Init();

    xBeepOn = xSemaphoreCreateBinary(); //击杀互斥信号量
        
    /* 创建AppTaskCreate任务 */
    xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* 任务入口函数 */
                            (const char*    )"AppTaskCreate",/* 任务名字 */
                            (uint16_t       )512,  /* 任务栈大小 */
                            (void*          )NULL,/* 任务入口函数参数 */
                            (UBaseType_t    )1, /* 任务的优先级 */
                            (TaskHandle_t*  )&AppTaskCreate_Handle);/* 任务控制块指针 */ 
                            
                            
    /* 启动任务调度 */  
    if(xReturn == pdPASS)						
        vTaskStartScheduler();   /* 启动任务，开启调度 */
    else
        return -1;  
    
    while(1);   /* 正常不会执行到这里 */    
}


/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
    BaseType_t xReturn1 = pdPASS; /* 定义一个创建信息返回值，默认为pdPASS */
    BaseType_t xReturn2 = pdPASS;
    BaseType_t xReturn3 = pdPASS;
    BaseType_t xReturn4 = pdPASS;
    BaseType_t xReturn5 = pdPASS;
    BaseType_t xReturn6 = pdPASS;
  
    taskENTER_CRITICAL();           //进入临界区

    xReturn1 = xTaskCreate((TaskFunction_t )Print_Image,  
                        (const char*    )"Print_Image",
                        (uint16_t       )512,  
                        (void*          )NULL,
                        (UBaseType_t    )2, 
                        (TaskHandle_t*  )&Print_Handle);

    xReturn2 = xTaskCreate((TaskFunction_t )Bullet_Manage,  
                        (const char*    )"Bullet_Manage",
                        (uint16_t       )512,  
                        (void*          )NULL,
                        (UBaseType_t    )2, 
                        (TaskHandle_t*  )&Bullet_Handle);

    xReturn3 = xTaskCreate((TaskFunction_t )Create_EnemyFlight,  
                        (const char*    )"Create_EnemyFlight",
                        (uint16_t       )512,  
                        (void*          )NULL,
                        (UBaseType_t    )2, 
                        (TaskHandle_t*  )&Create_Handle);

    xReturn4 = xTaskCreate((TaskFunction_t )Updata_location,  
                        (const char*    )"Updata_location",
                        (uint16_t       )512,  
                        (void*          )NULL,
                        (UBaseType_t    )2, 
                        (TaskHandle_t*  )&Updata_Handle);

    xReturn5 = xTaskCreate((TaskFunction_t )Touch_Detect,  
                        (const char*    )"Touch_Detect",
                        (uint16_t       )128,  
                        (void*          )NULL,
                        (UBaseType_t    )2, 
                        (TaskHandle_t*  )&Touch_Handle);
                        
    xReturn6 = xTaskCreate((TaskFunction_t )Beep_Control,  
                        (const char*    )"Beep_Control",
                        (uint16_t       )128,  
                        (void*          )NULL,
                        (UBaseType_t    )2, 
                        (TaskHandle_t*  )&Beep_Handle);
                        
    if(xReturn1 == pdPASS && xReturn2 == pdPASS && xReturn3 == pdPASS && 
    xReturn4 == pdPASS && xReturn5 == pdPASS && xReturn6 == pdPASS)
        printf("创建任务成功!\r\n");
   
    vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
  
    taskEXIT_CRITICAL();            //退出临界区
}

/*
    TODO:
    3.飞机之间的碰撞检测
    4.敌机发射子弹 AC
    5.主机受伤显示
    6.主机受伤爆炸
*/

static void Print_Image(void *parm) {

  //uint8_t status = 0;
  //uint8_t flag = 0;
    uint8_t i = 0;

    while(1) {

        taskENTER_CRITICAL();  
       
        LCD_DrawPicure(0, 0, &IMAGE_LIB.Background);
        
        if(Hero.loc_x < 5)
            Hero.loc_x = 5;
        if(Hero.loc_x > 190)
            Hero.loc_x = 190;
        
        if(Hero.loc_y < 5)
            Hero.loc_y = 5;
        if(Hero.loc_y > 270)
            Hero.loc_y = 270;
        
        LCD_MixPicure(Hero.loc_x, Hero.loc_y, &IMAGE_LIB.Hero, &IMAGE_LIB.Background);

        for (i = 0; i < 20; i++) {
            if(Bullets[i].status == Alive)
                LCD_MixPicure(Bullets[i].loc_x, Bullets[i].loc_y, &IMAGE_LIB.Bullet, &IMAGE_LIB.Background);
        }

         for (i = 0; i < 20; i++) {
            if(Enemy_Bullets[i].status == Alive)
                LCD_MixPicure(Enemy_Bullets[i].loc_x, Enemy_Bullets[i].loc_y, &IMAGE_LIB.Bullet, &IMAGE_LIB.Background);
        }
        
        for (i = 0; i < 5; i++) {
            
            switch (Enemys[i].status)
            {
            case Alive:
                LCD_MixPicure(Enemys[i].loc_x, Enemys[i].loc_y, &IMAGE_LIB.Enemy, &IMAGE_LIB.Background);
                break;

            case Boom1:
                LCD_MixPicure(Enemys[i].loc_x, Enemys[i].loc_y, &IMAGE_LIB.BOOM1, &IMAGE_LIB.Background);

                Enemys[i].status = Boom2;
                break;

            case Boom2:
                LCD_MixPicure(Enemys[i].loc_x, Enemys[i].loc_y, &IMAGE_LIB.BOOM2, &IMAGE_LIB.Background);
            
                vTaskDelay(50);
                Enemys[i].status = Destroy;
                break;

            case Destroy:
                break;

            default:
                break;
            } 

            //ILI9341_DispChar_EN(100, 100, Hero.score + '0');

        }

        
        taskEXIT_CRITICAL();

        vTaskDelay(10);  
    }
}

static void Bullet_Manage(void *parm) {

    //uint8_t status = 0;
    //uint8_t flag = 0;
    //uint8_t i = 0;

    while(1) {

        Create_Bullet();

        vTaskDelay(80); 
    }
}

static void Create_EnemyFlight(void *parm) {

    uint8_t i = 0;
    int8_t flag = 0;
    int16_t time = 800;
    static uint8_t num = 0;
    
    while(1) {

        flag = Create_Enemy();
        
        if(flag == 1) {
            num++;
            flag = 0;
            if(num >= 7) {
                num = 0;
                time -= 100;
                if(time < 300) {
                    time = 300;
                }
            }
        }

        for(i = 0; i < 5; i++) {
            if(Enemys[i].status == Alive) {
                Enemy_Bullet(&Enemys[i]);
                
            }
        }
        
        vTaskDelay(time);
    }
}

static void Updata_location(void *parm) {

    //uint8_t status = 0;
    //uint8_t flag = 0;
    uint8_t i = 0;
    uint8_t j = 0;

    while(1) {
        
        vTaskSuspendAll();
        
        for (i = 0; i < 20; i++) {
            if(Bullets[i].status == Alive && Bullets[i].loc_y < 5) {
                Bullets[i].status = Destroy;
            }
        }

        for (i = 0; i < 20; i++) {
            if(Enemy_Bullets[i].status == Alive && Enemy_Bullets[i].loc_y > 300)
                Enemy_Bullets[i].status = Destroy;
        }
        
        for (i = 0; i < 5; i++) {
            if((Enemys[i].loc_y > 260 || Enemys[i].life <= 0) && Enemys[i].hasDead == 0) {
                Enemys[i].status = Boom1;
                Enemys[i].hasDead = 1;
                
                xSemaphoreGive(xBeepOn);
            }
        }

        for (i = 0; i < 20; i++) {
            
            if(Bullets[i].status == Alive) {
                
                for (j = 0; j < 5; j++) {

                    if(Enemys[j].status == Alive) {

                        if(Bullets[i].loc_x >= Enemys[j].loc_x && Bullets[i].loc_x <= Enemys[j].loc_x + 50) {

                            if(Bullets[i].loc_y <= Enemys[j].loc_y + 65) {
                                Enemys[j].life--;
                                Bullets[i].status = Destroy;
                                
                            }
                        }
                    }
                }
            }
        }

        for (i = 0; i < 20; i++) {
            if(Bullets[i].status == Alive)
                Bullets[i].loc_y -= Bullets[i].speed;
        }

        for (i = 0; i < 20; i++) {
            if(Enemy_Bullets[i].status == Alive)
                Enemy_Bullets[i].loc_y += Enemy_Bullets[i].speed;
        }

        for (i = 0; i < 5; i++) {
            if(Enemys[i].status == Alive)
                Enemys[i].loc_y += Enemys[i].speed;
        }

        xTaskResumeAll();
        
        vTaskDelay(10);
    }
}

static void Touch_Detect(void *parm) {

    //uint8_t status = 0;
    //uint8_t flag = 0;

    while(1) {
        vTaskSuspendAll();

        XPT2046_TouchEvenHandler();

        xTaskResumeAll();
    }
}

static void Beep_Control(void *parm) {

    while(1) {
        xSemaphoreTake(xBeepOn, portMAX_DELAY);
        //BEEP(ON);
        vTaskDelay(30);
        //BEEP(OFF);
    }

}



/***********************************************************************
  * @ 函数名  ： BSP_Init
  * @ 功能说明： 板级外设初始化，所有板子上的初始化均可放在这个函数里面
  * @ 参数    ：   
  * @ 返回值  ： 无
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
	 * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
	 * 都统一用这个优先级分组，千万不要再分组，切忌。
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
    ILI9341_Init();
    
    XPT2046_Init();
    Calibrate_or_Get_TouchParaWithFlash(6,0);
    
	/* 串口初始化	*/
	USART_Config();
    
    BEEP_GPIO_Config();
    
    ILI9341_GramScan(6); //LCD模式六
    
    Obj_Init();
    
    LCD_SetFont(&Font16x24);
    LCD_SetColors(RED, BLACK);
    ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
}

/********************************END OF FILE****************************/
