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
#include "Flight_Control_Fault.h"

Flight_Control_Fault_State  State_Breakdown;


#define DIVIDER_RES_MAX  100//分压电阻1
#define DIVIDER_RES_MIN  10//分压电阻2
#define DIVIDER_RES_SCALE (DIVIDER_RES_MAX+DIVIDER_RES_MIN)/DIVIDER_RES_MIN

float Battery_Valtage=0;
void Battery_Fault_Check(void)
{
  static uint16_t battery_fault_cnt=0;
  Battery_Valtage=(DIVIDER_RES_SCALE*ADC_Value[1]*3.3f)/4096;
  if(Battery_Valtage<Safe_Vbat/1000.0f)
  {
    battery_fault_cnt++;
    if(battery_fault_cnt>=100)//持续100*5ms=500ms
    {
      State_Breakdown.Low_Voltage_Fault_Flag=true;
    }
  }
  else
  {
    battery_fault_cnt/=2;
    if(battery_fault_cnt==0)  State_Breakdown.Low_Voltage_Fault_Flag=false; 
  }
}

void Baro_Fault_Check(float baropress)
{
  static uint16_t baro_fault_cnt=0;
  static float last_baropress=0;
  static uint16_t baro_gap_cnt=0;
  baro_gap_cnt++;
  if(baro_gap_cnt>=40)//每200ms检测一次，因为气压计更新周期大于5ms
  {
    baro_gap_cnt=0;
    if(last_baropress==baropress)
    {
      baro_fault_cnt++;
      if(baro_fault_cnt>5)  State_Breakdown.Baro_Fault_Flag=true;   
    }
    else
    {
      baro_fault_cnt/=2;
      if(baro_fault_cnt==0)  State_Breakdown.Baro_Fault_Flag=false; 
    }
    last_baropress=baropress; 
  }
}

void Accel_Fault_Check(Vector3f accel)
{
  static uint16_t accel_fault_cnt=0;
  static Vector3f last_accel={0};
  if(last_accel.x==accel.x
     &&last_accel.y==accel.y
       &&last_accel.z==accel.z)
  {
    accel_fault_cnt++;
    if(accel_fault_cnt>20)  State_Breakdown.Accel_Fault_Flag=true;   
  }
  else
  {
    accel_fault_cnt/=2;
    if(accel_fault_cnt==0)  State_Breakdown.Accel_Fault_Flag=false; 
  }
  last_accel=accel; 
}


void Gyro_Fault_Check(Vector3f gyro)
{
  static uint16_t gyro_fault_cnt=0;
  static Vector3f last_gyro={0};
  if(last_gyro.x==gyro.x
     &&last_gyro.y==gyro.y
       &&last_gyro.z==gyro.z)
  {
    gyro_fault_cnt++;
    if(gyro_fault_cnt>20)  State_Breakdown.Gyro_Fault_Flag=true;   
  }
  else
  {
    gyro_fault_cnt/=2;
    if(gyro_fault_cnt==0)  State_Breakdown.Gyro_Fault_Flag=false; 
  }
  last_gyro=gyro; 
}


void Compass_Fault_Check(Vector3f compass)
{
  static uint16_t compass_fault_cnt=0;
  static Vector3f last_compass={0};
  static uint16_t compass_gap_cnt=0;
  compass_gap_cnt++;
  if(compass_gap_cnt>=40)//每200ms检测一次，因为磁力计更新周期大于5ms
  {
    compass_gap_cnt=0;
    if(last_compass.x==compass.x
       &&last_compass.y==compass.y
         &&last_compass.z==compass.z)
    {
      compass_fault_cnt++;
      if(compass_fault_cnt>10)  State_Breakdown.Compass_Fault_Flag=true;   
    }
    else
    {
      compass_fault_cnt/=2;
      if(compass_fault_cnt==0)  State_Breakdown.Compass_Fault_Flag=false; 
    }
    last_compass=compass;
  }
}



void Ultrasonic_Fault_Check(float ultrasonic)
{
  static uint16_t ultrasonic_fault_cnt=0;
  static float last_ultrasonic;
  if(last_ultrasonic==ultrasonic)
  {
    ultrasonic_fault_cnt++;
    if(ultrasonic_fault_cnt>20)  State_Breakdown.Ultrasonic_Fault_Flag=true;   
  }
  else
  {
    ultrasonic_fault_cnt/=2;
    if(ultrasonic_fault_cnt==0)  State_Breakdown.Ultrasonic_Fault_Flag=false; 
  }
  last_ultrasonic=ultrasonic; 
}



void Opticalflow_Fault_Check(int16_t opticalflow_x,int16_t opticalflow_y)
{
  static uint16_t opticalflow_fault_cnt=0;
  static uint16_t opticalflow_fault_zero_cnt=0; 
  static int16_t last_opticalflow_x,last_opticalflow_y;
  if(last_opticalflow_x==opticalflow_x
     &&last_opticalflow_y==opticalflow_y
       &&last_opticalflow_x!=0
         &&last_opticalflow_y!=0)
  {
    opticalflow_fault_cnt++;
    if(opticalflow_fault_cnt>20)  State_Breakdown.Opticalflow_Fault_Flag=true;   
  }
  else if(last_opticalflow_x==opticalflow_x
          &&last_opticalflow_y==opticalflow_y
            &&last_opticalflow_x==0
              &&last_opticalflow_y==0)
  {
    opticalflow_fault_zero_cnt++;
    if(opticalflow_fault_zero_cnt>100)  State_Breakdown.Opticalflow_Fault_Flag=true;   
  }
  else
  {
    opticalflow_fault_cnt/=2;
    opticalflow_fault_zero_cnt/=2;
    if(opticalflow_fault_cnt==0)  State_Breakdown.Opticalflow_Fault_Flag=false; 
  }
  last_opticalflow_x=opticalflow_x;
  last_opticalflow_y=opticalflow_y;
}


Vector3f Accel,Gyro,Compass;
void Flight_Control_Fault_ALL(void)
{
  Battery_Fault_Check();
  Baro_Fault_Check(Altitude_Estimate);
  Accel_Fault_Check(Accel);
  Gyro_Fault_Check(Gyro);
  Compass_Fault_Check(Compass);
}


