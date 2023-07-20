
#include "DMA.h"

#include "stdio.h"
#include "string.h"

uint8_t USARTzTxBuffer[USARTzTxBufferSize];
uint8_t USARTzRxBuffer[USARTzTxBufferSize];
uint8_t USARTzRxBufferD[USARTzTxBufferSize];

send_data	com_x_send_data;//���ݷ���
rcv_data	com_x_rcv_data;//���ݽ���
 

//115200 8bit 1stop no parity
void com_x_usart_config(void)
{
//  USARTz configuration
/* Configure USARTz */
	USART_InitTypeDef USARTz_InitStructure;
  USARTz_InitStructure.USART_BaudRate = 115200;
  USARTz_InitStructure.USART_WordLength = USART_WordLength_8b;//
  USARTz_InitStructure.USART_StopBits = USART_StopBits_1;
  USARTz_InitStructure.USART_Parity = USART_Parity_No;
  USARTz_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USARTz_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USARTz, &USARTz_InitStructure);
	
	/* Enable USART3 Receive  interrupts */
  USART_ITConfig(USARTz, USART_IT_IDLE, ENABLE); //�������ڿ���IDLE�ж�
  /* Enable USARTz DMA TX request */
  USART_DMACmd(USARTz, USART_DMAReq_Tx, ENABLE);
	/* Enable USARTz DMA RX request */
	USART_DMACmd(USARTz, USART_DMAReq_Rx, ENABLE);
  /* Enable USARTz */
  USART_Cmd(USARTz, ENABLE);
  /* Enable USARTz DMA TX Channel */
	USART_DMACmd(USARTz, USART_DMAReq_Tx,ENABLE);// DMA_Cmd(USARTz_Tx_DMA_Channe, ENABLE);
 
	/* Enable USARTz DMA RX Channel */
	USART_DMACmd(USARTz, USART_DMAReq_Rx,ENABLE);//DMA_Cmd(USARTz_Rx_DMA_Channe, ENABLE);
	
}
void com_x_usart_gpio_config(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure USARTz Rx as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//
  GPIO_InitStructure.GPIO_Pin = USARTz_RxPin;
  GPIO_Init(USARTz_GPIO, &GPIO_InitStructure);  
  
  /* Configure USARTz Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//������ʽ���������
  GPIO_InitStructure.GPIO_Pin = USARTz_TxPin;
  GPIO_Init(USARTz_GPIO, &GPIO_InitStructure);
	
}

void com_x_uasrt_nvic_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

	/* Enable the DMA Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = (uint8_t)UASRTz_TX_DMA_IRQ;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* Enable the USARTz Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USARTz_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void com_x_usart_dma_config(void)
{
  DMA_InitTypeDef DMA_InitStructure;

/* USARTz_Tx_DMA_Channel  */
  DMA_Cmd(USARTz_Tx_DMA_Channe,DISABLE);//stop dma
	DMA_DeInit(USARTz_Tx_DMA_Channe);//�ָ�ȱʡ����
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USARTz_DR_Base;//���ڷ������ݼĴ���
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USARTzTxBuffer;//���ͻ����׵�ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//��������ΪĿ��
  DMA_InitStructure.DMA_BufferSize = USARTzTxBufferSize;//��Ҫ���͵��ֽ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�������ӵ���       
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ滺������ַ���ӵ���              
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݿ�� һ���ֽ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //�ڴ����ݿ��һ���ֽ�        
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           //���δ���ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //�����ȼ�                
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //�ر��ڴ浽�ڴ��DMAģʽ 
	DMA_Init(USARTz_Tx_DMA_Channe, &DMA_InitStructure);//д������	
	DMA_ClearFlag(USARTz_Tx_DMA_FLAG);    //���DMA���б�־                          
	//DMA_Cmd(USARTz_Tx_DMA_Channe, ENABLE); 
	DMA_ITConfig(USARTz_Tx_DMA_Channe, DMA_IT_TC, ENABLE);  //����DMA����ͨ���ж�  
	
/*USARTz_Rx_DMA_Channe*/
	DMA_Cmd(USARTz_Rx_DMA_Channe,DISABLE);//stop dma
	DMA_DeInit(USARTz_Rx_DMA_Channe);//�ָ�ȱʡ����
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USARTz_DR_Base;//���ô��ڽ������ݼĴ���
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USARTzRxBuffer;//���ջ����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//��������Ϊ����Դ
  DMA_InitStructure.DMA_BufferSize = USARTzTxBufferSize;//��Ҫ���յ��ֽ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�������ӵ���       
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ滺������ַ���ӵ���              
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݿ�� һ���ֽ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //�ڴ����ݿ��һ���ֽ�        
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           //���δ���ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //�����ȼ�                
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //�ر��ڴ浽�ڴ��DMAģʽ 
	DMA_Init(USARTz_Rx_DMA_Channe, &DMA_InitStructure);//д������	
	DMA_ClearFlag(USARTz_Rx_DMA_FLAG);    //���DMA���б�־                          
	DMA_Cmd(USARTz_Rx_DMA_Channe, ENABLE); //����DMA����ͨ��
	   
}
//************************************DMA����********************************
void DMA1_Channel4_IRQHandler(void)//�ж�����������startup_stm32f10x.s�ļ���
{
    if(DMA_GetITStatus(DMA1_FLAG_TC4))
    {
        com_x_usart_dma_tx_over();
    }
}
void com_x_usart_dma_tx_over(void)
{
    DMA_ClearFlag(USARTz_Tx_DMA_FLAG);    //���DMA���б�־    
    DMA_Cmd(USARTz_Tx_DMA_Channe, DISABLE);  //�ر�DMA����ͨ��
    
}
 
void com_x_usart_dma_start_tx(uint8_t size)
{
    USARTz_Tx_DMA_Channe->CNDTR = (uint16_t)size; //���¸�ֵ ָ�����ͻ��泤��
    DMA_Cmd(USARTz_Tx_DMA_Channe, ENABLE);  //����DMA����      
}
//**************************************************************************

void USART1_IRQHandler(void)//�ж�����������startup_stm32f10x.s��
{
    
	if(USART_GetITStatus(USARTz, USART_IT_IDLE) != RESET)  
    {
        com_x_usart_dma_read();
				USART_ReceiveData( USARTz );
    }
}
void com_x_usart_dma_read(void)
{
    uint8_t rxcounter;
		uint8_t i;
		DMA_Cmd(USARTz_Rx_DMA_Channe, DISABLE);    //�ر�DMA��ֹ����   
    DMA_ClearFlag( USARTz_Rx_DMA_FLAG );   //�����־λ       
    rxcounter= USARTzTxBufferSize - DMA_GetCurrDataCounter(USARTz_Rx_DMA_Channe);//��ȡ���յ����ֽ��� 
    USARTz_Rx_DMA_Channe->CNDTR = USARTzTxBufferSize; //���¸�ֵ����ֵ   
    memset(USARTzRxBufferD,0,sizeof(USARTzRxBufferD));
		printf("Received data:");
	  for(i=0;i<rxcounter;i++){
			USARTzRxBufferD[i] = USARTzRxBuffer[i];//��ȡ���յ������ݣ���������RxBufferD��
			
			printf("0x%02x",USARTzRxBufferD[i]);
		}
		printf("\n\r");
		//�����յ������ݰ�
		//DINT();
		if(get_data_analyze(USARTzRxBufferD) != 0)
			printf("data analyze error\n\r");
		//EINT();
		for(i=0;i<rxcounter;i++)
			USARTzRxBuffer[i] = 0;//clear Rx buffer
		DMA_Cmd(USARTz_Rx_DMA_Channe, ENABLE);  //DMA���� �ȴ���һ֡����     
    
   
   
}
void com_x_usart_rcc_config(void)
{
/* DMA clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
  RCC_APB2PeriphClockCmd( USARTz_GPIO_CLK  |RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(USARTz_CLK,ENABLE);

}
void com_x_usart_Init(void)
{
	
	com_x_usart_rcc_config();
	com_x_uasrt_nvic_config();
	com_x_usart_gpio_config();
	com_x_usart_dma_config();
	com_x_usart_config();
}
extern float linear_x,linear_y,angular_z,position_x,position_y,position_z;
//����
void data_pack(void)
{
	
	com_x_send_data.x_pos.fv += linear_x;//x����
	com_x_send_data.y_pos.fv += linear_y;//y����
	com_x_send_data.x_v.fv	= linear_x;//x�����ٶ�
	com_x_send_data.y_v.fv = linear_y;//y �����ٶ�
	com_x_send_data.angular_v.fv = angular_z;//���ٶ� ��z��
	com_x_send_data.pose_angular.fv += angular_z;//yawƫ���� 
	
	USARTzTxBuffer[0] = 0xaa;
	USARTzTxBuffer[1] = 0xaa;
	
	USARTzTxBuffer[2] = com_x_send_data.x_pos.cv[0];
	USARTzTxBuffer[3] = com_x_send_data.x_pos.cv[1];
	USARTzTxBuffer[4] = com_x_send_data.x_pos.cv[2];
	USARTzTxBuffer[5] = com_x_send_data.x_pos.cv[3];
	
	USARTzTxBuffer[6] = com_x_send_data.y_pos.cv[0];
	USARTzTxBuffer[7] = com_x_send_data.y_pos.cv[1];
	USARTzTxBuffer[8] = com_x_send_data.y_pos.cv[2];
	USARTzTxBuffer[9] = com_x_send_data.y_pos.cv[3];
	
	USARTzTxBuffer[10] = com_x_send_data.x_v.cv[0];
	USARTzTxBuffer[11] = com_x_send_data.x_v.cv[1];
	USARTzTxBuffer[12] = com_x_send_data.x_v.cv[2];
	USARTzTxBuffer[13] = com_x_send_data.x_v.cv[3];
	
	USARTzTxBuffer[14] = com_x_send_data.y_v.cv[0];
	USARTzTxBuffer[15] = com_x_send_data.y_v.cv[1];
	USARTzTxBuffer[16] = com_x_send_data.y_v.cv[2];
	USARTzTxBuffer[17] = com_x_send_data.y_v.cv[3];
	
	USARTzTxBuffer[18] = com_x_send_data.angular_v.cv[0];
	USARTzTxBuffer[19] = com_x_send_data.angular_v.cv[1];
	USARTzTxBuffer[20] = com_x_send_data.angular_v.cv[2];
	USARTzTxBuffer[21] = com_x_send_data.angular_v.cv[3];
	
	USARTzTxBuffer[22] = com_x_send_data.pose_angular.cv[0];
	USARTzTxBuffer[23] = com_x_send_data.pose_angular.cv[1];
	USARTzTxBuffer[24] = com_x_send_data.pose_angular.cv[2];
	USARTzTxBuffer[25] = com_x_send_data.pose_angular.cv[3];
	
	USARTzTxBuffer[26] = USARTzTxBuffer[2]^USARTzTxBuffer[3]^USARTzTxBuffer[4]^USARTzTxBuffer[5]^USARTzTxBuffer[6]^
												USARTzTxBuffer[7]^USARTzTxBuffer[8]^USARTzTxBuffer[9]^USARTzTxBuffer[10]^USARTzTxBuffer[11]^
												USARTzTxBuffer[12]^USARTzTxBuffer[13]^USARTzTxBuffer[14]^USARTzTxBuffer[15]^USARTzTxBuffer[16]^
												USARTzTxBuffer[17]^USARTzTxBuffer[18]^USARTzTxBuffer[19]^USARTzTxBuffer[20]^USARTzTxBuffer[21]^
												USARTzTxBuffer[22]^USARTzTxBuffer[23]^USARTzTxBuffer[24]^USARTzTxBuffer[25];
	
	
	com_x_usart_dma_start_tx(27);	//���ݰ�����
}
//���ݽ��շ���
int8_t get_data_analyze(uint8_t	*pdata)
{
	
	int8_t ret=0;
	int8_t	crc = 0;
	int8_t  p_crc = 0;
	if((*(pdata + 0) == 0xff) && (*(pdata + 1) == 0xff)){
		crc = (*(pdata + 2))^(*(pdata + 3))^(*(pdata + 4))^(*(pdata + 5))^(*(pdata + 6))^(*(pdata + 7))^(*(pdata + 8))^(*(pdata + 9))^(*(pdata + 10))^(*(pdata + 11))^(*(pdata + 12))^(*(pdata + 13));
		p_crc = (int8_t)(*(pdata + 14));//����������ת��������������
	}
	else{
		ret = -1;
		return ret;
	}
	if(p_crc != crc ){//У��ͷ�������
		ret = -1;
		return ret;
	}
	//���ݰ�������ȷ����ȡ����
	memset(&com_x_rcv_data,0,sizeof(com_x_rcv_data));
	com_x_rcv_data.linear_vx.cv[0] = *(pdata + 2);
	com_x_rcv_data.linear_vx.cv[1] = *(pdata + 3);
	com_x_rcv_data.linear_vx.cv[2] = *(pdata + 4);
	com_x_rcv_data.linear_vx.cv[3] = *(pdata + 5);
	
	com_x_rcv_data.linear_vy.cv[0] = *(pdata + 6);
	com_x_rcv_data.linear_vy.cv[1] = *(pdata + 7);
	com_x_rcv_data.linear_vy.cv[2] = *(pdata + 8);
	com_x_rcv_data.linear_vy.cv[3] = *(pdata + 9);
	
	com_x_rcv_data.angular_v.cv[0] = *(pdata + 10);
	com_x_rcv_data.angular_v.cv[1] = *(pdata + 11);
	com_x_rcv_data.angular_v.cv[2] = *(pdata + 12);
	com_x_rcv_data.angular_v.cv[3] = *(pdata + 13);
	
	return ret;
	
}
