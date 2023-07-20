#ifndef _DMA_H
#define	_DMA_H

#include "stm32f10x.h"
#define		DINT()		__disable_irq()
#define		EINT()		__enable_irq()
#define USARTzTxBufferSize   64
#define USARTz										USART1
#define USARTz_GPIO								GPIOA
#define USARTz_CLK								RCC_APB2Periph_USART1
#define USARTz_GPIO_CLK						RCC_APB2Periph_GPIOA
#define USARTz_RxPin							GPIO_Pin_10
#define USARTz_TxPin							GPIO_Pin_9
#define USARTz_IRQn								USART1_IRQn
#define USARTz_DR_Base						(uint32_t)(&USART1->DR)
//#define USARTz_DR_Base						USART3_BASE+0x04
#define USARTz_Tx_DMA_Channe			DMA1_Channel4
#define USARTz_Tx_DMA_FLAG				DMA1_FLAG_GL4 //DMA1_FLAG_TC2|DMA1_FLAG_TE2
#define UASRTz_TX_DMA_IRQ					DMA1_Channel4_IRQn

#define USARTz_Rx_DMA_Channe			DMA1_Channel5
#define USARTz_Rx_DMA_FLAG				DMA1_FLAG_GL5//DMA1_FLAG_TC3 |DMA1_FLAG_TE3
#define UASRTz_RX_DMA_IRQ					DMA1_Channel5_IRQn  


//��������HEX���ٻ�ȡ
typedef	union{
		float fv;
		uint8_t cv[4];
}float_union;

//�������ݽṹ
typedef	struct{

		float_union		linear_vx;//���ٶ�x
		float_union		linear_vy;//���ٶ�y
		float_union		angular_v;//���ٶ�
		
}rcv_data;

//�������ݽṹ
typedef	struct{
		
		float_union	x_pos;//x��������
		float_union	y_pos;//y��������
		float_union	x_v;//x�����ٶ�
		float_union	y_v;//y�����ٶ�
		float_union	angular_v;//���ٶ�
		float_union	pose_angular;//�Ƕ�
	
}send_data;

void com_x_usart_config(void);
void com_x_usart_gpio_config(void);
void com_x_uasrt_nvic_config(void);
void com_x_usart_dma_config(void);
void com_x_usart_dma_tx_over(void);
void com_x_usart_dma_start_tx(uint8_t size);
void com_x_usart_dma_read(void);//DMA ���������� �ڴ��ڵĿ����ж��е��ã�������д�ú������ŵ������ط�����
void com_x_usart_rcc_config(void);
//****************�ⲿ���ú���************************
void com_x_usart_Init(void);//��ʼ��
int8_t get_data_analyze(uint8_t	*pdata);//�������ݷ���
void data_pack(void);//���ݴ��������

#endif
