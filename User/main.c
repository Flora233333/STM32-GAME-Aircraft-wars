/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
/* ������Ӳ��bspͷ�ļ� */
#include "bsp_ili9341_lcd.h"
#include "bsp_xpt2046_lcd.h"
#include "bsp_usart.h"
#include "bsp_beep.h"
#include "object.h"
/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
static TaskHandle_t AppTaskCreate_Handle = NULL;/* ���������� */
static TaskHandle_t Print_Handle = NULL;
static TaskHandle_t Bullet_Handle = NULL;
static TaskHandle_t Create_Handle = NULL;
static TaskHandle_t Updata_Handle = NULL;
static TaskHandle_t Touch_Handle = NULL;
static TaskHandle_t Beep_Handle = NULL;

//static TaskHandle_t Print_TaskHandle = NULL;
//static TaskHandle_t Test_Task_Handle = NULL;/* LED������ */
//static TaskHandle_t KEY_Task_Handle = NULL;/* KEY������ */

/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */

SemaphoreHandle_t xBeepOn;

/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */




/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void);/* ���ڴ������� */
static void Print_Image(void *parm);
static void Bullet_Manage(void *parm);
static void Create_EnemyFlight(void *parm);
static void Updata_location(void *parm);
static void Touch_Detect(void *parm);
static void Beep_Control(void *parm);

static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */

/*****************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
            �ڶ���������APPӦ������
            ������������FreeRTOS����ʼ���������
  ****************************************************************/
int main(void)
{	
    BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    
    /* ������Ӳ����ʼ�� */
    BSP_Init();

    xBeepOn = xSemaphoreCreateBinary(); //��ɱ�����ź���
        
    /* ����AppTaskCreate���� */
    xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                            (const char*    )"AppTaskCreate",/* �������� */
                            (uint16_t       )512,  /* ����ջ��С */
                            (void*          )NULL,/* ������ں������� */
                            (UBaseType_t    )1, /* ��������ȼ� */
                            (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
                            
                            
    /* ����������� */  
    if(xReturn == pdPASS)						
        vTaskStartScheduler();   /* �������񣬿������� */
    else
        return -1;  
    
    while(1);   /* ��������ִ�е����� */    
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
    BaseType_t xReturn1 = pdPASS; /* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    BaseType_t xReturn2 = pdPASS;
    BaseType_t xReturn3 = pdPASS;
    BaseType_t xReturn4 = pdPASS;
    BaseType_t xReturn5 = pdPASS;
    BaseType_t xReturn6 = pdPASS;
  
    taskENTER_CRITICAL();           //�����ٽ���

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
        printf("��������ɹ�!\r\n");
   
    vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

/*
    TODO:
    3.�ɻ�֮�����ײ���
    4.�л������ӵ� AC
    5.����������ʾ
    6.�������˱�ը
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
  * @ ������  �� BSP_Init
  * @ ����˵���� �弶�����ʼ�������а����ϵĳ�ʼ�����ɷ��������������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
    ILI9341_Init();
    
    XPT2046_Init();
    Calibrate_or_Get_TouchParaWithFlash(6,0);
    
	/* ���ڳ�ʼ��	*/
	USART_Config();
    
    BEEP_GPIO_Config();
    
    ILI9341_GramScan(6); //LCDģʽ��
    
    Obj_Init();
    
    LCD_SetFont(&Font16x24);
    LCD_SetColors(RED, BLACK);
    ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
}

/********************************END OF FILE****************************/
