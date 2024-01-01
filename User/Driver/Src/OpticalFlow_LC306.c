/* Copyright (c)  2019-2025 Wuhan Nameless Innovation Technology Co.,Ltd. All rights reserved.*/
/*----------------------------------------------------------------------------------------------------------------------/
                开源并不等于免费，先驱者的历史已经证明，在国内这个环境下，毫无收益的开源，单靠坊间个人爱好者，自发地参与项
								目完善的方式行不通，好的开源项目需要请专职人员做好售后服务、手把手教学、统计用户反馈需求、在实践中完成对产
								品的一次次完善与迭代升级。经过综合考虑，无名飞控开源代码中，程序仅保留公司正版激活功能代码，版本激活无实际
								功能，属于公司产品常规出厂操作，不影响客户学习，其余代码全部开放给客户学习，客户移植和二次开发代码请保留代
								码版权。
-----------------------------------------------------------------------------------------------------------------------/
*               本程序只供购买者学习使用，版权著作权属于无名科创团队，无名科创团队将飞控程序源码提供给购买者，
*               购买者要为无名科创团队提供保护，未经作者许可，不得将源代码提供给他人，不得将源代码放到网上供他人免费下载， 
*               更不能以此销售牟利，如发现上述行为，无名科创团队将诉之以法律解决！！！
-----------------------------------------------------------------------------------------------------------------------
*                                                 为什么选择无名创新？
*                                         感动人心价格厚道，最靠谱的开源飞控；
*                                         国内业界良心之作，最精致的售后服务；
*                                         追求极致用户体验，高效进阶学习之路；
*                                         萌新不再孤单求索，合理把握开源尺度；
*                                         响应国家扶贫号召，促进教育体制公平；
*                                         新时代奋斗最出彩，建人类命运共同体。 
-----------------------------------------------------------------------------------------------------------------------
*               生命不息、奋斗不止；前人栽树，后人乘凉！！！
*               开源不易，且学且珍惜，祝早日逆袭、进阶成功！！！
*               学习优秀者，简历可推荐到DJI、ZEROTECH、XAG、AEE、GDU、AUTEL、EWATT、HIGH GREAT等公司就业
*               求职简历请发送：15671678205@163.com，需备注求职意向单位、岗位、待遇等
*               无名科创开源飞控QQ群：2号群465082224、1号群540707961（人员已满）
*               CSDN博客：http://blog.csdn.net/u011992534
*               优酷ID：NamelessCotrun无名小哥
*               B站教学视频：https://space.bilibili.com/67803559/#/video
*               客户使用心得、改进意见征集贴：http://www.openedv.com/forum.php?mod=viewthread&tid=234214&extra=page=1
*               淘宝店铺：https://shop348646912.taobao.com/?spm=2013.1.1000126.2.5ce78a88ht1sO2
*               百度贴吧:无名科创开源飞控
*               公司官网:www.nameless.tech
*               无名创新国内首款TI开源飞控设计初衷、知乎专栏:https://zhuanlan.zhihu.com/p/54471146
*               修改日期:2019/11/27                    
*               版本：慧飞者PRO——WisdomPilot_Pro_V1.0.0
*               版权所有，盗版必究。
*               Copyright(C) 2019-2025 武汉无名创新科技有限公司 
*               All rights reserved
-----------------------------------------------------------------------------------------------------------------------
*               重要提示：
*               正常淘宝咸鱼转手的飞控、赠送朋友、传给学弟的都可以进售后群学习交流，
*               不得直接在网上销售无名创新资料，无名创新代码有声明版权，他人不得将
*               资料代码传网上供他人下载，不得以谋利为目的销售资料代码，发现有此操
*               作者，公司会提前告知，请1天内及时处理，否则你的学校、单位、姓名、电
*               话、地址信息会被贴出在公司官网、官方微信公众平台、官方技术博客、知乎
*               专栏以及淘宝店铺首页予以公示公告，此种所作所为，会成为个人污点，影响
*               升学、找工作、社会声誉、很快就很在无人机界出名，后果很严重。
*               因此行为给公司造成重大损失者，会以法律途径解决，感谢您的合作，谢谢！！！
----------------------------------------------------------------------------------------------------------------------*/
#include "Headfile.h"
#include "OpticalFlow_LC306.h"


#define OPTICALFLOW_UART UART4
#define SENSOR_IIC_ADDR 0xdc
const static u8 tab_focus[4] = {0x96,0x26,0xbc,0x50};		
const static u8 Sensor_cfg[]={
  //地址, 数据
  0x12, 0x80, 
  0x11, 0x30, 
  0x1b, 0x06, 
  0x6b, 0x43, 
  0x12, 0x20, 
  0x3a, 0x00, 
  0x15, 0x02, 
  0x62, 0x81, 
  0x08, 0xa0, 
  0x06, 0x68, 
  0x2b, 0x20, 
  0x92, 0x25, 
  0x27, 0x97, 
  0x17, 0x01, 
  0x18, 0x79, 
  0x19, 0x00, 
  0x1a, 0xa0, 
  0x03, 0x00, 
  0x13, 0x00, 
  0x01, 0x13, 
  0x02, 0x20, 
  0x87, 0x16, 
  0x8c, 0x01, 
  0x8d, 0xcc, 
  0x13, 0x07, 
  0x33, 0x10, 
  0x34, 0x1d, 
  0x35, 0x46, 
  0x36, 0x40, 
  0x37, 0xa4, 
  0x38, 0x7c, 
  0x65, 0x46, 
  0x66, 0x46, 
  0x6e, 0x20, 
  0x9b, 0xa4, 
  0x9c, 0x7c, 
  0xbc, 0x0c, 
  0xbd, 0xa4, 
  0xbe, 0x7c, 
  0x20, 0x09, 
  0x09, 0x03, 
  0x72, 0x2f, 
  0x73, 0x2f, 
  0x74, 0xa7, 
  0x75, 0x12, 
  0x79, 0x8d, 
  0x7a, 0x00, 
  0x7e, 0xfa, 
  0x70, 0x0f, 
  0x7c, 0x84, 
  0x7d, 0xba, 
  0x5b, 0xc2, 
  0x76, 0x90, 
  0x7b, 0x55, 
  0x71, 0x46, 
  0x77, 0xdd, 
  0x13, 0x0f, 
  0x8a, 0x10, 
  0x8b, 0x20, 
  0x8e, 0x21, 
  0x8f, 0x40, 
  0x94, 0x41, 
  0x95, 0x7e, 
  0x96, 0x7f, 
  0x97, 0xf3, 
  0x13, 0x07, 
  0x24, 0x58, 
  0x97, 0x48, 
  0x25, 0x08, 
  0x94, 0xb5, 
  0x95, 0xc0, 
  0x80, 0xf4, 
  0x81, 0xe0, 
  0x82, 0x1b, 
  0x83, 0x37, 
  0x84, 0x39, 
  0x85, 0x58, 
  0x86, 0xff, 
  0x89, 0x15, 
  0x8a, 0xb8, 
  0x8b, 0x99, 
  0x39, 0x98, 
  0x3f, 0x98, 
  0x90, 0xa0, 
  0x91, 0xe0, 
  0x40, 0x20, 
  0x41, 0x28, 
  0x42, 0x26, 
  0x43, 0x25, 
  0x44, 0x1f, 
  0x45, 0x1a, 
  0x46, 0x16, 
  0x47, 0x12, 
  0x48, 0x0f, 
  0x49, 0x0d, 
  0x4b, 0x0b, 
  0x4c, 0x0a, 
  0x4e, 0x08, 
  0x4f, 0x06, 
  0x50, 0x06, 
  0x5a, 0x56, 
  0x51, 0x1b, 
  0x52, 0x04, 
  0x53, 0x4a, 
  0x54, 0x26, 
  0x57, 0x75, 
  0x58, 0x2b, 
  0x5a, 0xd6, 
  0x51, 0x28, 
  0x52, 0x1e, 
  0x53, 0x9e, 
  0x54, 0x70, 
  0x57, 0x50, 
  0x58, 0x07, 
  0x5c, 0x28, 
  0xb0, 0xe0, 
  0xb1, 0xc0, 
  0xb2, 0xb0, 
  0xb3, 0x4f, 
  0xb4, 0x63, 
  0xb4, 0xe3, 
  0xb1, 0xf0, 
  0xb2, 0xa0, 
  0x55, 0x00, 
  0x56, 0x40, 
  0x96, 0x50, 
  0x9a, 0x30, 
  0x6a, 0x81, 
  0x23, 0x33, 
  0xa0, 0xd0, 
  0xa1, 0x31, 
  0xa6, 0x04, 
  0xa2, 0x0f, 
  0xa3, 0x2b, 
  0xa4, 0x0f, 
  0xa5, 0x2b, 
  0xa7, 0x9a, 
  0xa8, 0x1c, 
  0xa9, 0x11, 
  0xaa, 0x16, 
  0xab, 0x16, 
  0xac, 0x3c, 
  0xad, 0xf0, 
  0xae, 0x57, 
  0xc6, 0xaa, 
  0xd2, 0x78, 
  0xd0, 0xb4, 
  0xd1, 0x00, 
  0xc8, 0x10, 
  0xc9, 0x12, 
  0xd3, 0x09, 
  0xd4, 0x2a, 
  0xee, 0x4c, 
  0x7e, 0xfa, 
  0x74, 0xa7, 
  0x78, 0x4e, 
  0x60, 0xe7, 
  0x61, 0xc8, 
  0x6d, 0x70, 
  0x1e, 0x39, 
  0x98, 0x1a
};

static void SensorConfig_UartSend(u8 dat)
{
  while(USART_GetFlagStatus(OPTICALFLOW_UART, USART_FLAG_TXE)==RESET);
  USART_SendData(OPTICALFLOW_UART, dat);	
}

uint16_t LC306_Ready_Cnt=0;
/**************************************************/		
uint8_t Config_Init_Uart()
{
  u16 i;
  u16 len ;
  u8 recv[3];
  int recv_cnt;		
  delay_ms(100);		
  len = sizeof(Sensor_cfg);	
  SensorConfig_UartSend(0xAA);//0xAA指令
  SensorConfig_UartSend(0xAB);//0xAB指令		 
  SensorConfig_UartSend(tab_focus[0]);		
  SensorConfig_UartSend(tab_focus[1]);
  SensorConfig_UartSend(tab_focus[2]);
  SensorConfig_UartSend(tab_focus[3]);
  SensorConfig_UartSend(tab_focus[0]^tab_focus[1]^tab_focus[2]^tab_focus[3]);			 		 
  recv_cnt = 0;
  LC306_Ready_Cnt=65535;
  while(recv_cnt<3)  //如接收不到模块返回的三个数，可延时10ms从0xAA指令开始重新配置
  {
    while(USART_GetFlagStatus(OPTICALFLOW_UART, USART_FLAG_RXNE)==RESET)
    {
      LC306_Ready_Cnt--;
      if(LC306_Ready_Cnt==0) return 0;
    };
    recv[recv_cnt++] = USART_ReceiveData(OPTICALFLOW_UART);							
  }			
  if(((recv[0]^recv[1]) == recv[2]) & (recv[1] == 0x00)) ;
  //printf("AB Command configuration successconfig succefful\n");		
  
  for(i=0;i<len;i+=2)//0xBB�  噶�
  {
    SensorConfig_UartSend(0xBB);		 
    SensorConfig_UartSend(SENSOR_IIC_ADDR);		
    SensorConfig_UartSend(Sensor_cfg[i]);
    SensorConfig_UartSend(Sensor_cfg[i+1]);
    SensorConfig_UartSend(SENSOR_IIC_ADDR^Sensor_cfg[i]^Sensor_cfg[i+1]);	 
    recv_cnt = 0;
    while(recv_cnt<3)  //如接收不到模块返回的三个数，可延时1ms重新发送0xBB指令
    {
      while(USART_GetFlagStatus(OPTICALFLOW_UART, USART_FLAG_RXNE)==RESET){};
      recv[recv_cnt++] = USART_ReceiveData(OPTICALFLOW_UART);							
    }			
    if(((recv[0]^recv[1]) == recv[2]) & (recv[1] == 0x00));
    //printf("BB Command configuration successconfig succefful\n");								 
  }	 		 
  SensorConfig_UartSend(0xDD);//0xDD		
  //printf("Configuration success\n");
  return 1;
}			 


