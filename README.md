# -STM32F4-
基于STM32F4的非标车驱动器，传感器代码

`这两个库是非常基础的底层配置代码不用细看，只需要根据此readme文档提供的用法学会稍加修改在自己的程序中调用就可以了，有兴趣的同学可以移步底层配置类的技术贴或者学着使用CubeMX（强推）`

# 1.adc.c
非标车需要用大量距离传感器来实现自主上台自主寻敌自主攻击，并且会用几个灰度传感器检测台边判断自身在台上位置，adc库就是实现读取这些传感器值的功能。此adc库使用dma方式读取每个传感器的adc值，因为dma方式更快更节约算力。
本代码可管理14个adc接口，具体传感器的接线可查阅目录下的引脚表。下面介绍大家可能的对于本代码的打开方式：

0. 如果你不打算改我的配置代码，请看完引脚图知道各传感器的分配及排布以后以后直接移步第四步（强推）
1. 首先大家需要根据机械设计图确定传感器的数量，然后根据芯片手册（一并在目录里）设计自己的引脚表，接着根据自己要用到的引脚修改下列代码：
```C
//对应相应C文件中的41~51行代码
//我的设计由3个GPIO组成，所以有三块代码，大家自己设计的时候可以根据需求修改，不过最偷懒的方法就是直接用我的~~
//	初始化引脚		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;//PA0-7 通道0-7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 GPIOA
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化 GPIOB

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化 GPIOC
```
主要要做的是把GPIO_InitStructure.GPIO_Pin改成自己需要的pin接口，没啥好说的，我觉得我的代码没啥问题，大家可以直接在我的上面删删改改~~

2.对应的，如果你改了上面的GPIO口，别忘了改这里的时钟初始化代码，尤其是 AHB几 要确认好，我们组（其实就是我）就是因为忘了改这里的代码，几行代码三天没跑起来。。。
```C
//对应C文件中14-19行
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA,GPIOB,GPIOC，GPIOE时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//dma clock enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
```

3. ADC1够用！ADC1够用！ADC1够用！尽量别用其他的ADC2或者ADC3这类妖艳贱货，不然dma和时钟那边还得改，多麻烦~

4. 使用：
大家可以在main文件中通过下列代码调用对应adc
```C
Get_Adc(ADC_Channel_1);//对adcChannel_1读取一次数据
Get_Adc_Average(ADC_Channel_12, N);对adcChannel_12连续读取N次数据取平均，考虑到采样时间与采样精度之间的trade off，一般N取3~5即可

//当然你也可以把这些读取adc的函数写在define里，例如：
#define adc5 Get_Adc_Average(ADC_Channel_10, 3)//前左
#define adc6 Get_Adc_Average(ADC_Channel_12, 3)//前右
#define adc7 Get_Adc(ADC_Channel_6)//左侧前     2
#define adc8 Get_Adc_Average(ADC_Channel_9, 3)//左侧中
#define adc9 Get_Adc_Average(ADC_Channel_8, 3)//左侧后
#define adc10 Get_Adc(ADC_Channel_1)//右侧前    3
#define adc11 Get_Adc_Average(ADC_Channel_5, 3)//右侧中
#define adc12 Get_Adc_Average(ADC_Channel_4, 3)//右侧后
#define adc13 Get_Adc(ADC_Channel_11)//后左
#define adc2 Get_Adc(ADC_Channel_13)//后右
#define adc14 Get_Adc_Average(ADC_Channel_0, 3)//上
//这样想调用的时候直接输入adc代号即可，
//例如我想判断adc2距离是否大于1500
//按原方式：if(Get_Adc(ADC_Channel_13) > 1500)
//define后：if(adc2 > 1500)
//利人利己，利国利民，make life easier.
```
5. Last but not least,  **记得他娘的初始化!** 在main函数中加入初始化函数：
```C
Adc_Init();
```

# 2.pwm.c，motor.c
众所周知，直流电机的驱动方式主要是控制器生成占空比不同的pwm波，此文件的作用就是为左右两个驱动器(每个驱动器控制两个电机)提供pwm控制信号，并封装好供main函数直接调用驱动小车进行各种机动。
这里底层不推荐大家改，大家看完引脚表后直接调用即可。
调用方式：
```C
//首先在开始的时候初始化各函数
MotorInit();
motor_Init(1000-1,21-1,0);//84000000/1000/21=4kHZ

//调用时，0~1000对应马力，0为静止，1000为最大马力，注意最大不要超过950马力，不然驱动可能会出问题
fwd(500, 500);/*fwd = forward 左右轮均以一半马力前进*/
bkw(500, 500);/*bkw = backward 左右轮均以一半马力倒退*/
turn(300, -300);/*以300马力右转*/
turn(-300, 300);/*以300马力左转*/
stop();/*急停*/
```
