#include "can.h" 


/*
 * ��������CAN_GPIO_Config
 * ����  ��CAN��GPIO ����,PB8�������룬PB9�������
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_GPIO_Config(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
   	
  	/*����ʱ������*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

  	/*IO����*/
//	GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
	
	/* Configure CAN pin: RX PB8*/									          
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	             // ��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	/* Configure CAN pin: TX PB9 */									               
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		         // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
		GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
 * ��������CAN_NVIC_Config
 * ����  ��CAN��NVIC ����,��1���ȼ��飬0��0���ȼ�
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_NVIC_Config(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure;
		/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	 	/*�ж�����*/
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	   //CAN1 RX0�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		   //��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			   //�����ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * ��������CAN_Mode_Config
 * ����  ��CAN��ģʽ ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_Mode_Config(void)
{
   	CAN_InitTypeDef        CAN_InitStructure;
	 	/************************CANͨ�Ų�������**********************************/
	/*CAN�Ĵ�����ʼ��*/
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	/*CAN��Ԫ��ʼ��*/
		CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  ʱ�䴥��ͨ��ģʽʹ��
    CAN_InitStructure.CAN_ABOM=DISABLE;			   //MCR-ABOM  �Զ����߹��� 
    CAN_InitStructure.CAN_AWUM=DISABLE;			   //MCR-AWUM  �Զ�����ģʽ
    CAN_InitStructure.CAN_NART=DISABLE;			   //MCR-NART  ��ֹ�����Զ��ش�	  DISABLE-�Զ��ش�
    CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  ����FIFO ����ģʽ  DISABLE-���ʱ�±��ĻḲ��ԭ�б���  
    CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  ����FIFO���ȼ� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ�� 
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //��������ģʽ
    CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;		   //BTR-SJW ����ͬ����Ծ���� 2��ʱ�䵥Ԫ
    CAN_InitStructure.CAN_BS1=CAN_BS1_9tq;		   //BTR-TS1 ʱ���1 ռ����6��ʱ�䵥Ԫ
    CAN_InitStructure.CAN_BS2=CAN_BS2_8tq;		   //BTR-TS1 ʱ���2 ռ����3��ʱ�䵥Ԫ
    CAN_InitStructure.CAN_Prescaler =4;		   ////BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤�� 36/(1+6+3)/4=0.5Mbps
		CAN_Init(CAN1, &CAN_InitStructure);
}

/*
 * ��������CAN_Filter_Config
 * ����  ��CAN�Ĺ����� ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_Filter_Config(void)
{
   CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/*CAN��������ʼ��*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;						//��������0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//�����ڱ�ʶ������λģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//������λ��Ϊ����32λ��
	/* ʹ�ܱ��ı�ʾ�����������ձ�ʾ�������ݽ��бȶԹ��ˣ���չID�������µľ����������ǵĻ��������FIFO0�� */

    CAN_FilterInitStructure.CAN_FilterIdHigh= (((u32)0x1330<<3)&0xFFFF0000)>>16;				
    CAN_FilterInitStructure.CAN_FilterIdLow= (((u32)0x1330<<3)|CAN_ID_EXT | CAN_RTR_REMOTE)&0xFFFF; 
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (((u32)0xfff0<<3)&0xFFFF0000)>>16;	
    CAN_FilterInitStructure.CAN_FilterMaskIdLow= (((u32)0xfff0<<3)|CAN_ID_EXT | CAN_RTR_REMOTE)&0xFFFD;			
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;			
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);
	/*CANͨ���ж�ʹ��*/
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

/*
 * ��������CAN_Config
 * ����  ����������CAN�Ĺ���
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void CAN_Config(void)
{
  CAN_GPIO_Config();
  CAN_NVIC_Config();
  CAN_Mode_Config();
  CAN_Filter_Config();   
}

/*
 * ��������CAN_SetMsg
 * ����  ��CANͨ�ű�����������
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */	 
void CAN_SetMsg(void)
{	  

}
/*************************END OF FILE******************************/
