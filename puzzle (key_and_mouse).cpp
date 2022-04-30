#include<graphics.h>//图形库
#include<time.h>	//随机数

#define N  3		//地图是N*N格式
#define puzzle_length  150  //拼图块边长

PIMAGE img, imgs[N * N];   //img 1张原图    imgs分割图片
int aim_x = N - 1, aim_y = N - 1;		//假定空白处为目标  参数为目标所在位置坐标
int map[N][N] = { 0 };	//地图初始化为0

//游戏数据初始化
void GameInit();

//游戏渲染
void DrawMap();

//玩家操作
void mouseplay();
void keyplay();

//判断输赢
void Judg();

//返回 map 数据的逆序数
int inverseNumber();

int main()
{
	initgraph(N * puzzle_length, N * puzzle_length, 0);	//初始化窗口大小
	setbkcolor(WHITE);

	img = newimage();
	for (int m = 0; m < N*N; m++)
	{
		imgs[m] = newimage();
	}

	//加载资源图片    1张图片
	getimage(img, "tly1.jpg");

	//设置最后一张图片为空白图片，作为目标图片，空白图只是100*100
	getimage(imgs[(N*N)-1], "空白.jpg");

	srand((unsigned int)time(NULL));//设置随机种子

	GameInit();						//游戏初始化
	DrawMap();						//游戏渲染
	while (1)
	{
		//玩家操作
		for (; is_run(); delay_fps(60))
		{
			mouseplay();
			keyplay();
		}

		//判断输赢
		Judg();	
	}

	delimage(img);
	for (int m = 0; m < N*N; m++)
	{
		delimage(imgs[m]);
	}

	closegraph();					//关闭窗口
	return 0;
}


void GameInit()
{
	//设置绘图目标为 img 对象	对拼图图片进行切分
	for (int y = 0, n = 0 ; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			if (n == (N*N)-1)
				break;

			//逐个获取 puzzle_length * puzzle_length 像素图片, 存储在imgs里面
			getimage(imgs[n++], img , x * puzzle_length , y * puzzle_length , puzzle_length , puzzle_length);
		}
	}

		int index;      //a数组的下标
		int size = (N * N) - 1;  //a数组的元素个数
		int a[(N * N) - 1] ;
		for(int count =0; count < (N * N) - 1; count++)
		{
			a[count] = count;
		}

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				//右下角空白可交换图片不动
				if (i == N-1 && j == N-1) 
					continue;

				//从(N * N) - 1个数字中随机取一个
				index = rand() % size;

				//将随机抽取d额数字逐个放入地图中
				map[i][j] = a[index];

				//将抽取的位置后面的数字往前挪一位
				for (int k = index; k < size; k++)
				{
					a[k] = a[k + 1];
				}

				//控制数组长度 抽取了一张图片长度减一
				size--;
			}
		}
		map[N - 1][N - 1] = (N * N) - 1;
}

void DrawMap()
{
	cleardevice();
	for (int x = 0; x < N; x++)
	{
		for (int y = 0; y < N; y++)
		{
			putimage(x * puzzle_length , y * puzzle_length , imgs[map[x][y]]);
		}
	}
}

void mouseplay()
{
	int x, y;		               //鼠标点击的位置坐标(x,y)
	mouse_msg M_msg = mouse_msg(); //鼠标消息,创建鼠标消息结构体变量并初始化  
	//如果有鼠标消息
	while (mousemsg())
	{
		//读取鼠标消息
		M_msg = getmouse();
	}

	if (M_msg.is_left() && M_msg.is_up())//当鼠标消息是左键按下时
	{

		//获取鼠标按下所在点x轴坐标
		x = M_msg.x / puzzle_length;
		if (M_msg.x == N * puzzle_length)
		{
			x = N - 1;
		}
		//获取鼠标按下所在点y轴坐标
		y = M_msg.y / puzzle_length;//col--x
		if (M_msg.y == N * puzzle_length)
		{
			y = N - 1;
		}

		//判难鼠标点击位置和目标是否相邻，相邻交换数据
		if ((x == aim_x && y == aim_y + 1) ||
			(x == aim_x && y == aim_y - 1) ||
			(x == aim_x + 1 && y == aim_y) ||
			(x == aim_x - 1 && y == aim_y))
		{
			//鼠标点击图片和空白目标图片交换
			map[aim_x][aim_y] = map[x][y];
			map[x][y] = (N * N) - 1;
			aim_x = x;
			aim_y = y;

			DrawMap();
		}
	}
}

void keyplay()
{
	key_msg  K_msg = { 0 };//键盘消息,创建键盘消息结构体变量，并初始化
	//如果有键盘消息
	while (kbmsg())
	{
		//读取键盘信息
		K_msg = getkey();
	}

	//按下‘上’键
	if (K_msg.key == key_up && K_msg.msg == key_msg_up)
	{
		//空白目标图片和其上图片交换
		if (aim_y != 0)
		{
			map[aim_x][aim_y] = map[aim_x][aim_y - 1];
			map[aim_x][aim_y - 1] = (N * N) - 1;
			aim_y = aim_y - 1;

			K_msg = { 0 };
			DrawMap();
		}
	}
	//按下‘下’键
	if (K_msg.key == key_down && K_msg.msg == key_msg_up)
	{
		//空白目标图片和其上图片交换
		if (aim_y != 2)
		{
			map[aim_x][aim_y] = map[aim_x][aim_y + 1];
			map[aim_x][aim_y + 1] = (N * N) - 1;
			aim_y = aim_y + 1;

			K_msg = { 0 };
			DrawMap();
		}
	}
	//按下‘左’键
	if (K_msg.key == key_left && K_msg.msg == key_msg_up)
	{
		//空白目标图片和其上图片交换
		if (aim_x != 0)
		{
			map[aim_x][aim_y] = map[aim_x - 1][aim_y];
			map[aim_x - 1][aim_y] = (N * N) - 1;
			aim_x = aim_x - 1;

			K_msg = { 0 };
			DrawMap();
		}
	}
	//按下‘右’键
	if (K_msg.key == key_right && K_msg.msg == key_msg_up)
	{
		//空白目标图片和其上图片交换
		if (aim_x != 2)
		{
			map[aim_x][aim_y] = map[aim_x + 1][aim_y];
			map[aim_x + 1][aim_y] = (N * N) - 1;
			aim_x = aim_x + 1;

			K_msg = { 0 };
			DrawMap();
		}
	}
}

void Judg()
{
	//判断当每张图片是否在对应位置
	if (inverseNumber() == 0)
	{
		//挑战成功之后将全图贴上
		putimage(0, 0, img);

		//按任意键退出游戏
		getch();
		exit(0);
	}
}

int inverseNumber()
{
	int sum = 0, k = 0;		//sum为逆序数
	int arr[N * N];
	//将map里面的数据导入到一维数组arr里面去
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			arr[k++] = map[i][j];
		}
	}
	//求逆序数
	for (int i = 1; i < N * N; i++)		//从第二个数起有逆序数
	{
		for (int j = 0; j < i; j++)		//循环遍历 第i个数 前面的数
		{
			if (arr[i] < arr[j])
			{
				sum++;
			}
		}
	}
	return sum;	//返回逆序数
}
