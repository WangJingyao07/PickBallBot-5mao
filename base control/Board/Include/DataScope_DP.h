/**************************************************************************
���ߣ�ƽ��С��֮�� 
�Ա����̣�http://shop114407458.taobao.com/
**************************************************************************/

#ifndef __DATA_PRTOCOL_H
#define __DATA_PRTOCOL_H
 
 
extern unsigned char DataScope_OutPut_Buffer[42];	   //������֡���ݻ�����


void DataScope_Get_Channel_Data(float Data,unsigned char Channel);    // дͨ�������� ������֡���ݻ�����

unsigned char DataScope_Data_Generate(unsigned char Channel_Number);  // ����֡�������ɺ��� 

void DataScope(int Channel1,int Channel2,int Channel3,int Channel4,int Channel5,int Channel6,int Channel7,int Channel8);
 
#endif 


