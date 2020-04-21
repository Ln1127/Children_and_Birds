#include"acllib.h"
#include"my.h"
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include <conio.h>
#include <windows.h>  // 包含MessageBox 函数声明的头文件


#define Window_Width 1500    //图形化界面大小
#define Window_Height 985


#define Pen_Color RGB(0,0,0)   //画笔设置
#define Pen_Width 6

#define Box_Color RGB(120,102,225)
#define Box_Width 4

#define Score_Color RGB(0,0,0)
#define Score_Width 35

#define Right_A_Width 60

#define Q_num 166                   //题目个数
#define A_num 166                   //答案个数
#define Card_N 6                    //抽取问题个数


 //产生不同随机数个数，注意随机数的个数不能超过最大最小数之间的范围，不然会导致死循环

#define Select_Tag_count 5      //选择卡片时倒计时时间


struct mouse {//鼠标状态结构   
	int x;
	int y;
	int state;//鼠标状态
	int M_Chose;
}Mouse;

//------------------------------------------------//


//----------------函数定义----------------//
int* Produce_num(int max, int min, int n);//产生随机数，并返回存随机数的数组
void timeListener(int id);//计时器
void mouseListener(int x, int y, int button, int event);//鼠标输入
void Drawing(int x, int y, int button, int event);//绘画
void in_image(void);//加载图片
void order(void);//生成出牌顺序
int* random(int max, int min, int n, int q);//q在数组第一个，生成的数组有n个数
int MouseChose(void);
int where(void);
void show_q_a(void);//绘制画面------------------------------------------------------注意在实现中修改测试背景

int* question;//存放选中的题目
int q_count = -1;//问题计数
int* a, * a_sx;//存放抽中的答案和答案的顺序
int all_flag = 1;//标记是否进行下一题
int Right_a = 0;//答对题目数
int Pattern = -1;//模式选择  0-选择题   1-绘画
int A_flag = -1;//显示答案对错
int start_sound_flag = 0;


//-----------------------------------------//注意修改参数  54->Q_num
ACL_Image q_image[Q_num];
/*
char* q_card_num[Q_num] = { "image/1.jpg","image/2.jpg","image/3.jpg","image/4.jpg","image/5.jpg","image/6.jpg",
"image/7.jpg","image/8.jpg", "image/9.jpg","image/10.jpg","image/11.jpg","image/12.jpg","image/13.jpg",
"image/14.jpg","image/15.jpg","image/16.jpg","image/17.jpg","image/18.jpg", "image/19.jpg","image/20.jpg",
"image/21.jpg","image/22.jpg","image/23.jpg","image/24.jpg","image/25.jpg","image/26.jpg","image/27.jpg",
"image/28.jpg", "image/29.jpg","image/30.jpg", "image/31.jpg","image/32.jpg","image/33.jpg","image/34.jpg",
"image/35.jpg","image/36.jpg","image/37.jpg","image/38.jpg", "image/39.jpg","image/40.jpg","image/41.jpg",
"image/42.jpg","image/43.jpg","image/44.jpg","image/45.jpg","image/46.jpg","image/47.jpg","image/48.jpg",
"image/49.jpg","image/50.jpg","image/51.jpg","image/52.jpg","image/53.jpg","image/54.jpg" };*/
char* q_card_num[Q_num] = { "image/Q_0.jpg","image/Q_1.jpg","image/Q_2.jpg","image/Q_3.jpg","image/Q_4.jpg",
"image/Q_5.jpg","image/Q_6.jpg","image/Q_7.jpg","image/Q_8.jpg","image/Q_9.jpg","image/Q_10.jpg","image/Q_11.jpg",
"image/Q_12.jpg","image/Q_13.jpg","image/Q_14.jpg","image/Q_15.jpg","image/Q_16.jpg","image/Q_17.jpg",
"image/Q_18.jpg","image/Q_19.jpg","image/Q_20.jpg","image/Q_21.jpg","image/Q_22.jpg","image/Q_23.jpg",
"image/Q_24.jpg","image/Q_25.jpg","image/Q_26.jpg","image/Q_27.jpg","image/Q_28.jpg","image/Q_29.jpg",
"image/Q_30.jpg","image/Q_31.jpg","image/Q_32.jpg","image/Q_33.jpg","image/Q_34.jpg","image/Q_35.jpg",
"image/Q_36.jpg","image/Q_37.jpg","image/Q_38.jpg","image/Q_39.jpg","image/Q_40.jpg","image/Q_41.jpg",
"image/Q_42.jpg","image/Q_43.jpg","image/Q_44.jpg","image/Q_45.jpg","image/Q_46.jpg","image/Q_47.jpg",
"image/Q_48.jpg","image/Q_49.jpg","image/Q_50.jpg","image/Q_51.jpg","image/Q_52.jpg","image/Q_53.jpg",
"image/Q_54.jpg","image/Q_55.jpg","image/Q_56.jpg","image/Q_57.jpg","image/Q_58.jpg","image/Q_59.jpg",
"image/Q_60.jpg","image/Q_61.jpg","image/Q_62.jpg","image/Q_63.jpg","image/Q_64.jpg","image/Q_65.jpg",
"image/Q_66.jpg","image/Q_67.jpg","image/Q_68.jpg","image/Q_69.jpg", "image/Q_70.jpg","image/Q_71.jpg",
"image/Q_72.jpg","image/Q_73.jpg","image/Q_74.jpg","image/Q_75.jpg","image/Q_76.jpg","image/Q_77.jpg",
"image/Q_78.jpg","image/Q_79.jpg", "image/Q_80.jpg","image/Q_81.jpg","image/Q_82.jpg","image/Q_83.jpg",
"image/Q_84.jpg","image/Q_85.jpg","image/Q_86.jpg","image/Q_87.jpg","image/Q_88.jpg","image/Q_89.jpg",
"image/Q_90.jpg","image/Q_91.jpg","image/Q_92.jpg","image/Q_93.jpg","image/Q_94.jpg","image/Q_95.jpg",
"image/Q_96.jpg","image/Q_97.jpg","image/Q_98.jpg","image/Q_99.jpg", "image/Q_100.jpg","image/Q_101.jpg",
"image/Q_102.jpg","image/Q_103.jpg","image/Q_104.jpg","image/Q_105.jpg","image/Q_106.jpg","image/Q_107.jpg",
"image/Q_108.jpg","image/Q_109.jpg","image/Q_110.jpg","image/Q_111.jpg","image/Q_112.jpg","image/Q_113.jpg",
"image/Q_114.jpg","image/Q_115.jpg","image/Q_116.jpg","image/Q_117.jpg","image/Q_118.jpg","image/Q_119.jpg",
"image/Q_120.jpg","image/Q_121.jpg","image/Q_122.jpg","image/Q_123.jpg","image/Q_124.jpg","image/Q_125.jpg",
"image/Q_126.jpg","image/Q_127.jpg","image/Q_128.jpg","image/Q_129.jpg","image/Q_130.jpg","image/Q_131.jpg",
"image/Q_132.jpg","image/Q_133.jpg","image/Q_134.jpg","image/Q_135.jpg","image/Q_136.jpg","image/Q_137.jpg",
"image/Q_138.jpg","image/Q_139.jpg","image/Q_140.jpg","image/Q_141.jpg","image/Q_142.jpg","image/Q_143.jpg",
"image/Q_144.jpg","image/Q_145.jpg","image/Q_146.jpg","image/Q_147.jpg","image/Q_148.jpg","image/Q_149.jpg",
"image/Q_150.jpg","image/Q_151.jpg","image/Q_152.jpg","image/Q_153.jpg","image/Q_154.jpg","image/Q_155.jpg",
"image/Q_156.jpg","image/Q_157.jpg","image/Q_158.jpg","image/Q_159.jpg","image/Q_160.jpg","image/Q_161.jpg",
"image/Q_162.jpg","image/Q_163.jpg","image/Q_164.jpg","image/Q_165.jpg" };

ACL_Image a_image[A_num];
char* a_card_num[A_num] = {"image/A_0.jpg","image/A_1.jpg","image/A_2.jpg","image/A_3.jpg","image/A_4.jpg",
"image/A_5.jpg","image/A_6.jpg","image/A_7.jpg","image/A_8.jpg","image/A_9.jpg","image/A_10.jpg","image/A_11.jpg",
"image/A_12.jpg","image/A_13.jpg","image/A_14.jpg","image/A_15.jpg","image/A_16.jpg","image/A_17.jpg",
"image/A_18.jpg","image/A_19.jpg","image/A_20.jpg","image/A_21.jpg","image/A_22.jpg","image/A_23.jpg",
"image/A_24.jpg","image/A_25.jpg","image/A_26.jpg","image/A_27.jpg","image/A_28.jpg","image/A_29.jpg",
"image/A_30.jpg","image/A_31.jpg","image/A_32.jpg","image/A_33.jpg","image/A_34.jpg","image/A_35.jpg",
"image/A_36.jpg","image/A_37.jpg","image/A_38.jpg","image/A_39.jpg","image/A_40.jpg","image/A_41.jpg",
"image/A_42.jpg","image/A_43.jpg","image/A_44.jpg","image/A_45.jpg","image/A_46.jpg","image/A_47.jpg",
"image/A_48.jpg","image/A_49.jpg","image/A_50.jpg","image/A_51.jpg","image/A_52.jpg","image/A_53.jpg",
"image/A_54.jpg","image/A_55.jpg","image/A_56.jpg","image/A_57.jpg","image/A_58.jpg","image/A_59.jpg",
"image/A_60.jpg","image/A_61.jpg","image/A_62.jpg","image/A_63.jpg","image/A_64.jpg","image/A_65.jpg",
"image/A_66.jpg","image/A_67.jpg","image/A_68.jpg","image/A_69.jpg", "image/A_70.jpg","image/A_71.jpg",
"image/A_72.jpg","image/A_73.jpg","image/A_74.jpg","image/A_75.jpg","image/A_76.jpg","image/A_77.jpg",
"image/A_78.jpg","image/A_79.jpg", "image/A_80.jpg","image/A_81.jpg","image/A_82.jpg","image/A_83.jpg",
"image/A_84.jpg","image/A_85.jpg","image/A_86.jpg","image/A_87.jpg","image/A_88.jpg","image/A_89.jpg",
"image/A_90.jpg","image/A_91.jpg","image/A_92.jpg","image/A_93.jpg","image/A_94.jpg","image/A_95.jpg",
"image/A_96.jpg","image/A_97.jpg","image/A_98.jpg","image/A_99.jpg", "image/A_100.jpg","image/A_101.jpg",
"image/A_102.jpg","image/A_103.jpg","image/A_104.jpg","image/A_105.jpg","image/A_106.jpg","image/A_107.jpg",
"image/A_108.jpg","image/A_109.jpg","image/A_110.jpg","image/A_111.jpg","image/A_112.jpg","image/A_113.jpg",
"image/A_114.jpg","image/A_115.jpg","image/A_116.jpg","image/A_117.jpg","image/A_118.jpg","image/A_119.jpg",
"image/A_120.jpg","image/A_121.jpg","image/A_122.jpg","image/A_123.jpg","image/A_124.jpg","image/A_125.jpg",
"image/A_126.jpg","image/A_127.jpg","image/A_128.jpg","image/A_129.jpg","image/A_130.jpg","image/A_131.jpg",
"image/A_132.jpg","image/A_133.jpg","image/A_134.jpg","image/A_135.jpg","image/A_136.jpg" ,"image/A_137.jpg",
"image/A_138.jpg","image/A_139.jpg","image/A_140.jpg","image/A_141.jpg","image/A_142.jpg","image/A_143.jpg",
"image/A_144.jpg","image/A_145.jpg","image/A_146.jpg","image/A_147.jpg","image/A_148.jpg","image/A_149.jpg",
"image/A_150.jpg","image/A_151.jpg","image/A_152.jpg","image/A_153.jpg","image/A_154.jpg","image/A_155.jpg",
"image/A_156.jpg","image/A_157.jpg","image/A_158.jpg","image/A_159.jpg","image/A_160.jpg","image/A_161.jpg",
"image/A_162.jpg","image/A_163.jpg","image/A_164.jpg","image/A_165.jpg" };

//-----------------画图---------------------------------------------

ACL_Image White_Background;//白板
char* W_B = "image/white_background.jpg";
ACL_Image eraser;//橡皮
char* Eraser = "image/eraser.jpg";
ACL_Image pen_image;//笔
char* Pen_image = "image/pen.jpg";

//------------text--------------------------------------------------change_button
ACL_Image Text_Background;//测试背景
char* text_background = "image/Text_Background.jpg";

ACL_Image Painting_Background;//绘画背景
char* painting_background = "image/Painting_background.jpg";

ACL_Image Basic_Background;//基本背景
char* basic_background = "image/basic_background.jpg";

ACL_Image Hello_Background;//hello-背景
char* hello_background = "image/hello.jpg";

ACL_Image Change_Button;//hello-背景
char* change_button = "image/change_button.jpg";

ACL_Sound Start_Sound;//开启声音
char* start_sound = "sound/start.mp3";

ACL_Sound Right_Sound;
char* right_sound = "sound/good.mp3";

ACL_Sound Painting_Sound;
char* painting_sound = "sound/painting .mp3";

int Setup(void)
{
	//------启动终端窗口-------//
	//可以用来跟踪程序的运行情况
	//initConsole();
	//--------------------------//
	initWindow("Children & Birds", DEFAULT, DEFAULT, Window_Width, Window_Height);//初始化程序窗口，width:1500 * height:927

	loadSound(start_sound, &Start_Sound);//加载启动声音
	loadSound(right_sound, &Right_Sound);//加载答对声音
	
	order();//生成顺序
	//in_image();//加载图片


	registerMouseEvent(mouseListener);//与鼠标的结构结合更新当前鼠标所在位置
	//registerMouseEvent(Drawing);//绘画
	registerTimerEvent(timeListener);
	startTimer(0, 200);//绘制选择题画面计算器
	startTimer(1, 500);
	startTimer(2, 20);//启动声音、画面
	startTimer(3, 7500);
	
	return 0;
}

/*------以下为函数的实现------*//////////////////////////////////////////////////////////////////////////////////////////////

void order(void)
{
	question = Produce_num(Q_num - 1, 0, Card_N);//随机生成出牌顺序
	//out_order = Produce_num(Card_N - 1, 0, Card_N);
}

int where(void)
{
	if (Mouse.y > 550 && Mouse.y < 800)
	{
		if (Mouse.x > 300 && Mouse.x < 550)
		{
			return 0;
		}
		else if (Mouse.x > 600 && Mouse.x < 850)
		{
			return 1;
		}
		else if (Mouse.x > 900 && Mouse.x < 1150)
		{
			return 2;
		}
		else if (Mouse.x > 1200 && Mouse.x < 1450)
		{
			return 3;
		}
	}
	if (Mouse.x > 20 && Mouse.x < 180 && Mouse.y > 700 && Mouse.y < 800)
	{
		return 4;//切换到绘画
	}
}

void show_q_a(void)
{
	A_flag = -1;
	if (all_flag)
	{
		printf("%d\n", question[q_count]);
		q_count++;
		loadImage(q_card_num[question[q_count]], &q_image[question[q_count]]);
		a = random(A_num, 0, 4, question[q_count]);//抽中的答案
		a_sx = Produce_num(3, 0, 4);
		for (int i = 0; i < 4; i++)//输出答案照片
		{
			loadImage(a_card_num[a[a_sx[i]]], &a_image[a[a_sx[i]]]);
		}
		all_flag = 0;
	}
	
	int num = question[q_count];
	printf("%d\n", Right_a);
	
	beginPaint();
	clearDevice();
	//putImageScale(&Text_Background, 0, 0, Window_Width, Window_Height);
	putImageScale(&Basic_Background, 0, 0, Window_Width, Window_Height);
	//Basic_Background
	putImageScale(&q_image[num], 280, 100, 1100, 250);//320  <-40
	putImageScale(&Change_Button, 20, 700, 160, 100);

	int x, y;
	x = 260, y = 550;
	
	for (int i = 0; i < 4; i++)//输出答案照片
	{
		loadImage(a_card_num[a[a_sx[i]]], &a_image[a[a_sx[i]]]);
		putImageScale(&a_image[a[a_sx[i]]], x, y, 250, 250);
		x += 300;
	}

	if (where() == 0)
	{
		setPenColor(Box_Color);
		setPenWidth(Box_Width);
		line(258, 548, 512, 548);
		line(512, 548, 512, 802);
		line(512, 802, 258, 802);
		line(258, 802, 258, 548);
	}
	else if (where() == 1)
	{
		setPenColor(Box_Color);
		setPenWidth(Box_Width);
		line(558, 548, 812, 548);
		line(812, 548, 812, 802);
		line(812, 802, 558, 802);
		line(558, 802, 558, 548);
	}
	else if (where() == 2)
	{
		setPenColor(Box_Color);
		setPenWidth(Box_Width);
		line(858, 548, 1112, 548);
		line(1112, 548, 1112, 802);
		line(1112, 802, 858, 802);
		line(858, 802, 858, 548);
	}
	else if(where() == 3)
	{
		setPenColor(Box_Color);
		setPenWidth(Box_Width);
		line(1158, 548, 1412, 548);
		line(1412, 548, 1412, 802);
		line(1412, 802, 1158, 802);
		line(1158, 802, 1158, 548);
	}
	
	setTextColor(Score_Color);
	setTextSize(Score_Width);
	paintText(10, 100, "答对题目数");
	char a[100];
	itoa(Right_a, a, 10);
	setTextColor(Pen_Color);
	setTextSize(Right_A_Width);
	paintText(85, 150, a);
	A_flag = MouseChose();
	

	endPaint();
}

void Show_r(void)
{
	
	if (A_flag == 1)
	{
		beginPaint();
		playSound(Right_Sound, 0);
		endPaint();
		MessageBoxTimeout(NULL,
			_T("恭喜你，答对了 ^_^"),
			_T("Children & Birds"),
			MB_OK,
			//MB_ICONINFORMATION,
			GetSystemDefaultLangID(),
			1800);
		/*
		int nSelect = MessageBox(
			NULL, 
			TEXT("恭喜你，答对了！"), 
			TEXT("Children & Birds"), 
			MB_OK
		);*/
		if (q_count + 1 < Card_N)
		{
			//q_count++;
			all_flag = 1;
		}
		else
		{
			int nSelect = MessageBox(
				NULL, 
				TEXT("恭喜你  ^_^  完成了所有题目！\n画一幅画休息一下吧！"),
				TEXT("Children & Birds"), 
				MB_ICONINFORMATION | MB_OKCANCEL);
			if (nSelect == IDOK) 
			{
				Pattern = 1;//切换到绘画
				printf("你点击了“确定”按钮 \n");
			}
			else 
			{
				int nSelect = MessageBox(
					NULL,
					TEXT("Bye！"),
					TEXT("Children & Birds"),
					MB_ICONINFORMATION | MB_OK
				);
				if (nSelect == IDOK)
				{
					exit(0);
				}
				printf("你点击了“取消”按钮 \n");
			}
			
		}
	}
	else if (A_flag == 0)
	{
		MessageBoxTimeout(NULL,
			_T("选错了哦！\n再试一次吧！"),
			_T("Children & Birds"),
			MB_ICONSTOP | MB_OK,
			//MB_ICONINFORMATION,
			GetSystemDefaultLangID(),
			900);
		/*
		int nSelect = MessageBox(
			NULL,
			TEXT("选错了哦！\n再试一次吧！"),
			TEXT("Children & Birds"),
			MB_ICONSTOP | MB_OK);*/
	}
	beginPaint();
	char a[100];
	itoa(Right_a, a, 10);
	setTextColor(Pen_Color);
	setTextSize(Right_A_Width);
	paintText(85, 150, a);
	endPaint();
}


void in_image(void)
{
	/*
	for (int i = 0; i < Card_N; i++)//加载问题图片
	{
		loadImage(q_card_num[question[i]], &q_image[question[i]]);
	}
/*
	for (int i = 0; i < A_num; i++)//加载答案图片
	{
		loadImage(q_card_num[i], &q_image[i]);
	}

	for (int i = 0; i < A_num; i++)//加载答案图片
	{
		loadImage(a_card_num[i], &a_image[i]);
	}*/
	
	loadImage(W_B, &White_Background);//加载白色背景
	loadImage(Eraser, &eraser);//加载橡皮擦图标
	loadImage(Pen_image, &pen_image);//加载笔的图标
	//loadImage(text_background, &Text_Background);//加载测试背景
	//loadImage(painting_background, &Painting_Background);
	loadImage(basic_background, &Basic_Background);
	loadImage(change_button, &Change_Button);
//	loadImage(hello_background, &Hello_Background);//加载开始背景
}

//----------------产生随机数，并返回存随机数的数组----------------//
int* Produce_num(int max, int min, int n)
{
	srand(time(NULL));
	int RandNum;
	int* card = (int*)malloc(n * sizeof(int));//card存生成的数组
	int i, flag = 0, t = 0;
	while (1)
	{
		flag = 0;//标记
		if (t == n)
			break;

		RandNum = (rand() % (max + 1 - min)) + min;
		for (i = 0; i < t; i++)//查重
		{
			if (card[i] == RandNum)
				flag = 1;
		}
		if (flag != 1)
		{
			card[t] = RandNum;
			printf("      第%2d个的ID是: %d\n", t + 1, card[t]);//输出分牌情况
			t++;
		}
	}
	return card;
}

void timeListener(int id)
{
	if (id == 0 && Pattern == 0)
	{
		if (Pattern != 0)
		{
			cancelTimer(0);
			//Show_r();
		}
		else
		{
			printf("---------\nq_count:%d\n--------\n", q_count);
			show_q_a();
			Show_r();
		}
	}
	if (id == 1)
	{
		if (Pattern == 1)
		{
			loadSound(painting_sound, &Painting_Sound);
			beginPaint();
			clearDevice();
			playSound(Painting_Sound, 1);
			//&Painting_Background, 0, 0, Window_Width, Window_Height);
			endPaint();
			registerMouseEvent(Drawing);//绘画
			cancelTimer(1);
			int nSelect = MessageBox(
				NULL,
				TEXT("欢迎进入绘画界面！\n\n下面是一些使用技巧：\n1、按住左键即可绘制\n2、按住右键即可擦除\n3、双击左键可以重新开始\n\n相信你已经迫不及待了，那就点击确定开始吧开始绘画吧 ^_^"),
				TEXT("Children & Birds"),
				MB_OK);
		}
	}
	if (id == 2)
	{
		if (start_sound_flag == 0)
		{
			start_sound_flag = 1;
			loadImage(hello_background, &Hello_Background);//加载开始背景
			beginPaint();
			putImageScale(&Hello_Background, 0, 0, Window_Width, Window_Height);
			playSound(Start_Sound, 0);
			endPaint();
			in_image();//加载图片
		}
		else
			cancelTimer(2);
	}
	if (id == 3)
	{
		Pattern = 0;
		cancelTimer(3);
	}
}

void mouseListener(int x, int y, int button, int event)//鼠标输入
{
	Mouse.x = x;
	Mouse.y = y;
	if (button == 1 && event == 2)
	{
		Mouse.state = 1;//左键按下后抬起
	}
	
	/*
	button
		NO_BUTTON = 0,
		LEFT_BUTTON=1,
		MIDDLE_BUTTON=2,
		RIGHT_BUTTON=3
	event
		BUTTON_DOWN,=0
		BUTTON_DOUBLECLICK,=1
		BUTTON_UP,=2
		ROLL_UP,=3
		ROLL_DOWN,=4
		MOUSEMOVE=5
	*/
}

int MouseChose(void)
{// 0-选择错误，1-选择正确，2-切换到绘画  -1-未选择
	if (Mouse.state == 1)//鼠标按下后抬起
	{
		int chose = where();
		if (Pattern == 0)//选择题模式
		{
			if (chose == 4)
			{
				Pattern = 1;
				Mouse.state = 0;
				Mouse.M_Chose = 2;
				return 2;
			}
			if (chose == 0 || chose == 1 || chose == 2 || chose == 3)
			{
				if (a[a_sx[chose]] == question[q_count])
				{
					Right_a++;
					Mouse.state = 0;
					return 1;//答对了
				}
				else
				{
					Mouse.state = 0;
					return 0;//答错了
				}
					
			}
		}
	}
	else
		return -1;
}

int* random(int max, int min, int n, int q)
{
	srand(time(NULL));
	int RandNum;
	int* card = (int*)malloc(n * sizeof(int));//card存生成的数组
	card[0] = q;
	int i, flag = 0, t = 1;
	while (1)
	{
		flag = 0;//标记
		if (t == n)
			break;

		RandNum = (rand() % (max + 1 - min)) + min;
		for (i = 0; i < t; i++)//查重
		{
			if (card[i] == RandNum)
				flag = 1;
		}
		if (flag != 1)
		{
			card[t] = RandNum;
			t++;
		}
	}
	return card;
}

void Drawing(int x, int y, int button, int event)
{
	static int ox=0, oy=0;
	static int flag = 0;
	if (button == 1 && event == 0)
	{
		flag = 1;//画笔
		beginPaint();
		putImageScale(&pen_image, Window_Width / 2, 0, 35, 60);
		endPaint();
	}
	if (event == 2 && flag != 0)
	{
		flag = 0;//不绘图
		beginPaint();
		putImageScale(&White_Background, Window_Width / 2, 0, 35, 60);
		endPaint();
	}
	if (button == 3 && event == 0)
	{
		flag = 2;//橡皮擦
		beginPaint();
		putImageScale(& eraser, Window_Width/2, 0, 35, 60);
		endPaint();
	}
	if (event == 1)
	{
		beginPaint();
		//putImageScale(&Painting_Background, 0, 0, Window_Width, Window_Height);
		putImageScale(&White_Background, 0, 0, Window_Width, Window_Height);
		endPaint();
	}
	
	printf("x:%04d | y:%04d | button:%d | event:%d | flag:%d\n", x, y, button, event, flag);
	beginPaint();
	if (flag == 1)
	{
		setPenColor(Pen_Color);
		setPenWidth(Pen_Width);
		line(ox, oy, x, y);
	}
	if (flag == 2)//橡皮擦
	{
		setPenColor(WHITE);
		setPenWidth(Pen_Width*2);
		line(ox, oy, x, y);
	}
	endPaint();
	ox = x; oy = y;
	
	/*
	button
		NO_BUTTON = 0,
		LEFT_BUTTON=1,
		MIDDLE_BUTTON=2,
		RIGHT_BUTTON=3
	event
		BUTTON_DOWN,=0
		BUTTON_DOUBLECLICK,=1
		BUTTON_UP,=2
		ROLL_UP,=3
		ROLL_DOWN,=4
		MOUSEMOVE=5
	*/
}
/*
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	int nSel = 0;
	switch (msg) {
	case WM_CLOSE:
		nSel = MessageBox(hWnd, 
			TEXT("确定要退出吗"),
			TEXT("Children & Birds"), 
			MB_YESNO | MB_ICONQUESTION);
		if (nSel == IDYES) DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}*/
////2020-03-30 Children & Birds4.0 