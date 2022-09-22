/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
/* ������Ӳ��bspͷ�ļ� */
#include "bsp_ili9341_lcd.h"
#include "bsp_usart.h"
#include "object.h"
#include "Key.h"
/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
static TaskHandle_t AppTaskCreate_Handle = NULL;/* ���������� */
static TaskHandle_t Print_Handle = NULL;
static TaskHandle_t Uart_Handle = NULL;
static TaskHandle_t Create_Handle = NULL;
static TaskHandle_t Updata_Handle = NULL;
static TaskHandle_t Key_Handle = NULL;
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
static void Uart_Send(void *parm);
static void Create_EnemyFlight(void *parm);
static void Updata_location(void *parm);
static void Press_Key(void *parm);

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
  
    taskENTER_CRITICAL();           //�����ٽ���

    xReturn1 = xTaskCreate((TaskFunction_t )Print_Image,  
                        (const char*    )"Print_Image",
                        (uint16_t       )512,  
                        (void*          )NULL,
                        (UBaseType_t    )2, 
                        (TaskHandle_t*  )&Print_Handle);

    xReturn2 = xTaskCreate((TaskFunction_t )Uart_Send,  
                        (const char*    )"Uart_Send",
                        (uint16_t       )512,  
                        (void*          )NULL,
                        (UBaseType_t    )2, 
                        (TaskHandle_t*  )&Uart_Handle);

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

    xReturn5 = xTaskCreate((TaskFunction_t )Press_Key,  
                        (const char*    )"Press_Key",
                        (uint16_t       )128,  
                        (void*          )NULL,
                        (UBaseType_t    )2, 
                        (TaskHandle_t*  )&Key_Handle);
                        
    if(xReturn1 == pdPASS && xReturn2 == pdPASS &&
        xReturn3 == pdPASS && xReturn4 == pdPASS && xReturn5 == pdPASS)
        printf("��������ɹ�!\r\n");
   
    vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

static void Print_Image(void *parm) {

  //uint8_t status = 0;
  //uint8_t flag = 0;

    while(1) {
        taskENTER_CRITICAL();  
       
        LCD_DrawPicure(0, 0, &IMAGE_LIB.Background);

        taskEXIT_CRITICAL();  
    }
}

static void Uart_Send(void *parm) {

    //uint8_t status = 0;
    //uint8_t flag = 0;

    while(1) {
      
    
      
    }
}

static void Create_EnemyFlight(void *parm) {

    //uint8_t status = 0;
    //uint8_t flag = 0;

    while(1) {
      
    
      
    }
}

static void Updata_location(void *parm) {

    //uint8_t status = 0;
    //uint8_t flag = 0;

    while(1) {
      
   
      
    }
}

static void Press_Key(void *parm) {

    //uint8_t status = 0;
    uint8_t flag = 0;

    while(1) {
        vTaskSuspendAll();

        flag = Read_Key();
        Hero.dir = flag;

        xTaskResumeAll();
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
	/* ���ڳ�ʼ��	*/
	USART_Config();
    
    Obj_Init();
    
    ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
    LCD_SetColors(BLACK, WHITE);
}

/********************************END OF FILE****************************/
