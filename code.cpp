//******头文件******
#include <iostream>
#include<bits/stdc++.h>
#include<string.h>
#include<windows.h>
#include<fstream>
using namespace std;



// 停车场模拟系统
// 本停车场可停大、中、小型三种型号的车辆
// 收费规则：按分钟收费。大【0.2】 中【0.1】 小【0.05】
// 本停车场停放的车辆最多为MAXSIZE
// 停车场的车位数限制，但便道可无限停车
// 车牌号规定位汉字+大写字母+5个阿拉伯数字
//系统语句颜色：3 湖蓝色 加亮
//提示语句：4 红色 加亮
//结果语句：9 淡浅绿色 加亮
//输入语句：7 白色 加亮



//******宏定义，规定停车场的车位数量******
#define MAXSIZE 6	//停车场最多可停留汽车辆数



//******全局变量*****
//下面是每分钟停车费用，我们按分钟收费,分大中小三种车型
const float price_1 = 0.2;	//大
const float price_2 = 0.1;	//中
const float price_3 = 0.05;	//小
string Car_Type;	//汽车的型号
int i;	//汽车在便道中的位置
int number=0; //计数器：记录停车时的总输入量
int number_SeqList=0; //计数器：记录当前停车场已有的车辆数
int number_LQueue=0;  //计数器：记录当前便道上已有的停车数



//*******定义结构体存储信息******
//时间
typedef struct time {
	int h, m;	//小时 分钟
}Time;

//汽车
typedef struct car {
	Time t1, t2;	//到达时间和离开时间
	string num;	//车牌号
	int p1, p2;	//汽车在停车场或者便道停留位置
	string type;    //汽车的型号
}Car;

//停车场
typedef struct {
	Car data[MAXSIZE];
	int top;
}SeqList;

// 停车场车位
typedef struct{
    int space;
    int flag;
}Car_space;
Car_space Car_Space[MAXSIZE];

//便道
typedef struct qnode {
	Car data;
	struct qnode* next;
}QNode;
typedef struct lqueue {
	QNode* front, * rear;
}LQueue;



//******函数声明******
//初始化操作
SeqList* Init_SeqList();	//初始化停车场
LQueue* Init_LQueue();	//初始化便道
void Init_Car_Space();  //初始化停车场车位

//停车问题函数
void Car_In(SeqList* p, LQueue* q);	//汽车到达
void Push_SeqList(SeqList* s, Car x);	//汽车进入停车场
void In_LQueue(LQueue* q, Car x);	//停车场满的时候进入便道
void Traverse_LQueue(LQueue* q);    //便道第一辆车进入停车场后，对后面车辆前移

//取车问题函数
void Car_Out(SeqList* p, LQueue* q);	//汽车离去
Car Pop_SeqList(SeqList* s);	//汽车离开停车场
Car Out_LQueue(LQueue* q);	//停车场有车离开有空位时便道上的车辆按照到达顺序依次离开便道并进入停车场
Car Traverse_SeqList(SeqList *q,int top,string num);     ////栈的遍历用来寻找出栈的车辆

//查询有关函数
void Display(SeqList* p);	//显示停车场内车辆信息

//中间衔接函数
void Car_Condition(SeqList* p, LQueue* q);	//获取车辆的型号和进入/离开停车场的信息
bool Str_strcmp(string str1,string str2);  //字符串比较函数

//检验操作函数
bool Car_if(SeqList* p, Car c);	//检验停车场内是否有该车辆
bool checkNum(string s);        //检查车辆的车牌号格式是否正确
bool checkTime(int h,int m);    //检查车辆的时间是否正确
bool Check_TimeOut(int h1,int h2,int m1,int m2);        //检查汽车离开出栈的时间是否合法

//文件操作函数
void LQueue_Input(string Car_Type, Car car);	//向便道文件写入车辆数据
void LQueue_Delete(Car car);	//删除便道文件中离开便道的车辆数据
void SeqList_Input(string Car_Type, Car car);	//向停车场写入车辆数据
void SeqList_Delete(Car car);	//删除停车场车辆数据

//页面可视化操作函数
int color(int m);   //修改字体颜色
void menu_load();		//程序刚开始运行时的加载过程
void menu();	//系统主菜单页面
void load_1();	//加载百分比可视化
void load_2();	//系统退出百分比可视化
void load_3();	//进入系统加载界面
void load_4();	//进入停车场加载界面




//******主函数******
int main()
{
	//首先清空上次停车场停入的汽车和便道中的汽车
	FILE *file;
	file = fopen("SeqList.txt","w");
	fclose(file);
	file = fopen("LQueue.txt","w");
	fclose(file);
	file = fopen("temp.txt","w");
	fclose(file);
	Init_Car_Space();   //初始化停车场车位
	//开始加载并进入停车管理系统系统
    menu_load();
	system("pause");
	return 0;
}




//******全部函数******
//初始化停车场
SeqList* Init_SeqList()
{
	SeqList* s;
	s = new SeqList;
	s->top = -1;
	return s;
}

//初始化停车场车位
void Init_Car_Space()
{
    for(int j=0;j<MAXSIZE;j++)
    {
        Car_Space[j].space = j+1;
        Car_Space[j].flag = 1;
    }
}

//初始化便道
LQueue* Init_LQueue()
{
	LQueue* q;
	QNode* p;
	q = new lqueue;
	p = new qnode;
	p->next = NULL;
	q->front = p;
	q->rear = p;
	return q;
}

//向便道文件写入车辆数据
void LQueue_Input(string Car_Type, Car car)
{
	FILE* fp;
	if ((fp = fopen("LQueue.txt", "at+")) == NULL)
	{
	    color(4);
		cout<<"不能打开文件"<<endl;
		return;
	}
	string st1 = "车牌号: "+ car.num + "; 型号: "+car.type + "; ";
	int h = car.t1.h;
	int m = car.t1.m;
	int locate = car.p2;
    char st2[st1.length()];
    for(int j=0;j<st1.length();j++)
    {
        st2[j] = st1[j];
    }
    fprintf(fp,"%s便道车位00%d; 停入便道时间: %d时%d分。\n",st2,locate,h,m);
    //fputs(st2,fp);
	fclose(fp);
}

//删除便道文件中离开便道的车辆数据
void LQueue_Delete(Car car)
{
    FILE *fp1,*fp2;
    char strLine[1024];
    if ((fp1 = fopen("LQueue.txt", "rt")) == NULL)
	{
	    color(4);
		cout << "不能打开文件" << endl;
		return;
	}
	if ((fp2 = fopen("temp.txt", "wt")) == NULL)
	{
	    color(4);
		cout << "不能打开文件" << endl;
		return;
	}

    //将车牌号字符串转换为字符数组
    string st1 = car.num;
    char st2[st1.length()];
    for(int j=0;j<st1.length();j++)
    {
        st2[j] = st1[j];
    }

	while (fscanf(fp1,"%[^\n]",strLine)!=EOF)			//循环读取每一行，直到文件尾
	{
		fgetc(fp1);
        if(strstr(strLine,st2)==NULL)
        {
            fprintf(fp2,"%s\n",strLine);
        }
	}
	fclose(fp1);
	fclose(fp2);

	if ((fp1 = fopen("temp.txt", "rt")) == NULL)
	{
	    color(4);
		cout << "不能打开文件" << endl;
		return;
	}
	if ((fp2 = fopen("LQueue.txt", "wt")) == NULL)
	{
	    color(4);
		cout << "不能打开文件" << endl;
		return;
	}
	while (fscanf(fp1,"%[^\n]",strLine)!=EOF)	//循环读取每一行，直到文件尾
	{
		fgetc(fp1);
        fprintf(fp2,"%s\n",strLine);
	}
    fclose(fp1);
	fclose(fp2);
}

//向停车场写入车辆数据
void SeqList_Input(string Car_Type, Car car)
{
	FILE* fp;
	if ((fp = fopen("SeqList.txt", "at+")) == NULL)
	{
	    color(4);
		cout << "不能打开文件" << endl;
		return;
	}
	string st1 = "车牌号: "+ car.num + "; 型号: "+car.type + "; ";
	int h = car.t1.h;
	int m = car.t1.m;
	int locate = car.p1;
    char st2[st1.length()];
    for(int j=0;j<st1.length();j++)
    {
        st2[j] = st1[j];
    }
    fprintf(fp,"%s停车场车位00%d; 停车时间: %d时%d分。\n",st2,locate,h,m);
	//fputs(st2, fp);
	fclose(fp);
}

//删除停车场车辆数据
void SeqList_Delete(Car car)
{
    FILE *fp1,*fp2;
    char strLine[1024];
    if ((fp1 = fopen("SeqList.txt", "rt")) == NULL)
	{
	    color(4);
		cout << "不能打开文件" << endl;
		return;
	}
	if ((fp2 = fopen("temp.txt", "wt")) == NULL)
	{
	    color(4);
		cout << "不能打开文件" << endl;
		return;
	}

    //将车牌号字符串转换为字符数组
    string st1 = car.num;
    char st2[st1.length()];
    for(int j=0;j<st1.length();j++)
    {
        st2[j] = st1[j];
    }

	while (fscanf(fp1,"%[^\n]",strLine)!=EOF)   //循环读取每一行，直到文件尾
	{
		fgetc(fp1);					    //将fp所指向的文件一行内容读到strLine缓冲区
        if(strstr(strLine,st2)==NULL)
        {
            fprintf(fp2,"%s\n",strLine);
        }
	}
	fclose(fp1);
	fclose(fp2);

	if ((fp1 = fopen("temp.txt", "rt")) == NULL)
	{
	    color(4);
		cout << "不能打开文件" << endl;
		return;
	}
	if ((fp2 = fopen("SeqList.txt", "wt")) == NULL)
	{
	    color(4);
		cout << "不能打开文件" << endl;
		return;
	}
	while (fscanf(fp1,"%[^\n]",strLine)!=EOF)	//循环读取每一行，直到文件尾
	{
		fgetc(fp1);				    //将fp所指向的文件一行内容读到strLine缓冲区
        fprintf(fp2,"%s\n",strLine);
	}
    fclose(fp1);
	fclose(fp2);
}


//汽车进入停车场
void Push_SeqList(SeqList* s, Car x)
{
	s->top++;
	s->data[s->top] = x;
}

//汽车离开停车场
Car Pop_SeqList(SeqList* s)
{
	Car x = s->data[s->top];
	s->top--;
	return x;
}

//停车场满的时候进入便道
void In_LQueue(LQueue* q, Car x)
{
	QNode* p;
	p = new qnode;
	p->data = x;
	p->next = NULL;
	q->rear->next = p;
	q->rear = p;
}

//便道第一辆车进入停车场后，对后面车辆前移
void Traverse_LQueue(LQueue* q)
{
    while(q->front!=q->rear)
    {
        q->front = q->front->next;
        q->front->data.p2--;
    }
}

//停车场有车离开有空位时便道上的车辆按照到达顺序依次离开便道并进入停车场
Car Out_LQueue(LQueue* q)
{
	Car x;
	QNode* p;
	p = q->front->next;
	q->front->next = p->next;
	x = p->data;
	delete p;
	if (q->front->next == NULL)	//只有一个元素时，出队后队空，此时还要修改队尾指针
	{
		q->front->next = NULL;
		q->rear = q->front;
	}
	return x;
}

//检验停车场内是否有该车辆
bool Car_if(SeqList* p, Car c)
{
	int t=p->top;
	while (!Str_strcmp(c.num,p->data[t].num))
	{
        t--;
		if(t==-1) break;
	}
	if (t == -1) return 0;
    else return 1;
}

//汽车到达
void Car_In(SeqList* p, LQueue* q)
{
    Car c;
    string num; //临时储存车牌号
    color(4);
    cout << "请输入汽车的型号[small/mid/large]:\n";
    color(7);
	cin >> Car_Type;
	c.type = Car_Type;
	color(4);
	cout << "请输入汽车车牌号:\n";
	color(7);
	cin >> num;
	while(!checkNum(num))  //检验车牌号的格式,如果输入错误则需要重新输入
    {
        color(4);
        cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
        cout << "| 注意：车牌号格式为普通车牌号格式【第一位为省份，第二位为发证机关A~H|J~N|P~Y，第三~七位为号码0~9|A~H|J~N|P~Z】 |" << endl;
        cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
        cout << "请重新输入汽车车牌号:\n";
        cin >> num;
    }
    c.num = num;
    if(p->top!=-1)
    {
    	while(Car_if(p,c))     //判断车牌号是否重复
        {
            color(4);
            cout<<"你输入的车牌号已经在车库当中了，请检查你的车牌号是否正确：\n";
            color(7);
            cin >> c.num;
        }
    }
    color(4);
	cout << "请输入汽车的到达时间（格式：小时+空格+分钟）:\n";
	color(7);
	cin >> c.t1.h >> c.t1.m;
	while(!checkTime(c.t1.h,c.t1.m))    //判断输入的到达时间是否符合要求
    {
        color(4);
        cout << "---------------------------------------------------------" << endl;
        cout << "| 注意：我们的时间要求是24小时的标准时间,注意中间的空格 |" << endl;
        cout << "---------------------------------------------------------" << endl;
        cout << "请重新输入汽车的到达时间（格式：小时+空格+分钟）:\n";
        color(7);
        cin >> c.t1.h >> c.t1.m;
    }
	number++;   //到此输入的信息+1
	if (p->top == MAXSIZE - 1)  //当停车场满时
	{
		//进入便道
		c.p2 = ++i;
		color(4);
		cout << "生意火爆，停车场已满，进入便道等待，位置为：00" << c.p2 << endl;
		number_LQueue++;    //便道上等待的车辆+1
		In_LQueue(q, c);
		LQueue_Input(Car_Type,c);	//向便道文件写入车辆数据
		Sleep(2000);
        system("cls");
	}
	else {
		//进入停车场
        int flag = 1;   //标记输入车位是否错误，输入正确置为0
        int arr[MAXSIZE];   //临时记录停车场的空位
        color(4);
        cout << "停车场未满，正在进入停车场~" <<endl;
        cout << "当前空闲车位有:";
        for(int j=0;j<MAXSIZE;j++)
        {
            if(Car_Space[j].flag == 1)
            {
                color(9);
                cout << " "<< Car_Space[j].space;     //输出停车场的空位
                arr[j] = Car_Space[j].space;    //将空位信息存到arr数组中，方便后续检查
            }
        }
        color(4);
        cout<<endl;
		cout << "请选择车位~  "<<endl;
		color(7);
		cin >> c.p1;
		for(int j=0;j<MAXSIZE;j++)
        {
            if(c.p1==arr[j] && c.p1!=0)  flag = 0;   //判断输入车位是否空闲或者存在，符合要求，flag置为0
        }
        while(flag)
        {
            color(4);
            cout << "输入有误，请重新选择车位~  "<<endl;
            color(7);
            cin >> c.p1;
            for(int j=0;j<MAXSIZE;j++)
            {
                if(c.p1==arr[j] && c.p1!=0)  flag = 0;   //判断输入车位是否空闲或者存在，符合要求，flag置为0
            }
        }
        color(4);
		cout << "正在停车，位置为：00" << c.p1 << endl; //输入正确进入停车场
		color(3);
		load_4();   //停车加载动画
		Car_Space[c.p1-1].flag = 0; //此位置已停车，flag置为0
		number_SeqList++;   //停车场已经停的车辆+1
		Push_SeqList(p, c);
		SeqList_Input(Car_Type,c);	//向停车场文件写入车辆数据
	}
	Car_Condition(p,q); //再进行下一次的输入判断
}


//检查车辆的车牌号是否正确
bool checkNum(string S)
{
    char s[S.length()];
    for(int j=0;j<S.length();j++)
    {
        s[j] = S[j];
    }
    int n = S.length(); //车牌的长度
    char *ch; //用来放车牌号的汉字
    strncpy(ch,s,2);    //截取字符串的前两个字符，汉字占两个字节
    if(n!=8) return false;
    if((strcmp(ch,"京")!=0)&&(strcmp(ch,"津")!=0)&&(strcmp(ch,"晋")!=0)&&(strcmp(ch,"冀")!=0)&&(strcmp(ch,"蒙")!=0)&&(strcmp(ch,"辽")!=0)&&
       (strcmp(ch,"吉")!=0)&&(strcmp(ch,"黑")!=0)&&(strcmp(ch,"沪")!=0)&&(strcmp(ch,"苏")!=0)&&(strcmp(ch,"浙")!=0)&&(strcmp(ch,"皖")!=0)&&
       (strcmp(ch,"闽")!=0)&&(strcmp(ch,"赣")!=0)&&(strcmp(ch,"鲁")!=0)&&(strcmp(ch,"豫")!=0)&&(strcmp(ch,"鄂")!=0)&&(strcmp(ch,"湘")!=0)&&
       (strcmp(ch,"粤")!=0)&&(strcmp(ch,"桂")!=0)&&(strcmp(ch,"琼")!=0)&&(strcmp(ch,"渝")!=0)&&(strcmp(ch,"川")!=0)&&(strcmp(ch,"贵")!=0)&&
       (strcmp(ch,"云")!=0)&&(strcmp(ch,"藏")!=0)&&(strcmp(ch,"陕")!=0)&&(strcmp(ch,"甘")!=0)&&(strcmp(ch,"青")!=0)&&(strcmp(ch,"宁")!=0)&&
       (strcmp(ch,"新")!=0)) return false;
    if(s[2]<'A' || s[2]>'Z' || s[2]=='I' || s[2]=='O') return false;
    if(!(s[3]>='0' && s[3]<='9') && !(s[3]>='A' && s[3]<='H') && !(s[3]>='J' && s[3]<='N') && !(s[3]>='P' && s[3]<='Z')) return false;
    if(!(s[4]>='0' && s[4]<='9') && !(s[4]>='A' && s[4]<='H') && !(s[4]>='J' && s[4]<='N') && !(s[4]>='P' && s[4]<='Z')) return false;
    if(!(s[5]>='0' && s[5]<='9') && !(s[5]>='A' && s[5]<='H') && !(s[5]>='J' && s[5]<='N') && !(s[5]>='P' && s[5]<='Z')) return false;
    if(!(s[6]>='0' && s[6]<='9') && !(s[6]>='A' && s[6]<='H') && !(s[6]>='J' && s[6]<='N') && !(s[6]>='P' && s[6]<='Z')) return false;
    return true;
}

//检查车辆的时间是否正确
bool checkTime(int h,int m)
{
    if(0<=h&&h<24 && 0<=m&&m<60)
        return true;
    else return false;
}

//汽车离去
void Car_Out(SeqList* p, LQueue* q)
{
	Car c, x1, first, x2 , car;
	int flag = 0;   //标记作用：用来标记便道中是否有车【0表示没有，1表示有】
	string num;    //临时存储汽车车牌号
	if (p->top == -1)
	{
		//停车场无车
		color(4);
		cout << "当前停车场无车~" << endl;
		return;
	}
	if (p->top == MAXSIZE - 1)
	{
		//当停车库满了的时候判断便道是否有车
		if (q->front == q->rear) return;
		else {
			first = Out_LQueue(q);	//便道内第一辆车
			flag = 1;
		}
	}
	SeqList* temp = Init_SeqList();	//临时栈，保留有车离开让车时从停车场退出的车辆
	color(4);
	cout << "请输入汽车车牌号:\n";
	color(7);
	cin >> num;
	while(!checkNum(num))  //检验车牌号的格式,如果输入错误则需要重新输入
    {
        color(4);
        cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
        cout << "| 注意：车牌号格式为普通车牌号格式【第一位为省份，第二位为发证机关A~H|J~N|P~Y，第三~七位为号码0~9|A~H|J~N|P~Z】 |" << endl;
        cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
        cout << "请重新输入汽车车牌号:\n";
        color(7);
        cin >> num;
    }
    c.num = num;
	//判断停车场内是否有该车辆
	if(!Car_if(p,c))
	{
	    color(4);
		cout << "该停车场内没有该车辆~" << endl;
		Car_Condition(p,q); //再进行下一次的输入判断
		return;
	}
	cout << "请输入汽车的离开时间（格式：小时+空格+分钟）:\n";
	color(7);
	cin >> c.t2.h >> c.t2.m;
	while(!checkTime(c.t2.h,c.t2.m))    //判断输入的离开时间是否符合要求
    {
        color(4);
        cout << "---------------------------------------------------------" << endl;
        cout << "| 注意：我们的时间要求是24小时的标准时间,注意中间的空格 |" << endl;
        cout << "---------------------------------------------------------" << endl;
        cout << "请重新输入汽车的到达时间（格式：小时+空格+分钟）:\n";
        color(7);
        cin >> c.t2.h >> c.t2.m;
    }
    int location = p->top;
    car = Traverse_SeqList(p,location,num);     //得到目标车量
    while(!Check_TimeOut(car.t1.h,c.t2.h,car.t1.m,c.t2.m))     //判断目标车辆的离开时间是否合法
    {
        color(4);
        //不合法需要重新输入
        cout << "时间不合法，请重新输入:\n";
        color(7);
        cin >> c.t2.h >> c.t2.m;
    }
    number++;   //车辆离开信息输入成功，输入数据+1
	while (c.num != p->data[p->top].num)
	{
		x1 = Pop_SeqList(p);
		color(9);
		cout << "车牌号为" << x1.num << "的车让路" << endl;
		Push_SeqList(temp, x1);
	}
	x1 = Pop_SeqList(p);	//目标车辆,即准备开出的车辆
	x1.t2.h = c.t2.h, x1.t2.m = c.t2.m;
	int t = (x1.t2.h - x1.t1.h) * 60 + x1.t2.m - x1.t1.m; //总停留分钟
	int t1, t2;
	if (x1.t2.m >= x1.t1.m)
	{
		t2 = x1.t2.m - x1.t1.m;
		t1 = x1.t2.h - x1.t1.h;
	}
	else {
		t2 = x1.t2.m + 60 - x1.t1.m;
		t1 = x1.t2.h - x1.t1.h - 1;
	}
	color(9);
	cout << "车牌号为" << x1.num << "的车开出" << endl;
	Car_Space[x1.p1-1].flag = 1;    //车辆离开，停车场的车位空出，flag置为1。
	number_SeqList--;   //停车场的车辆数目减少
	SeqList_Delete(x1);	//停车场文件删除此车辆
	//刚才让出的车辆按照原来的顺序依次开入停车场
	while (temp->top != -1)
	{
		x2 = Pop_SeqList(temp);
		//x.p1 = p->top + 2;
		color(9);
		cout << "车牌号为" << x2.num << "的车开入" << endl;
		Push_SeqList(p, x2);
	}
	//若便道有车则让便道上第一辆车进入停车场
	if (flag)
	{
		first.p1 = x1.p1;    //将离开的车位给便道上开入的汽车
		Car_Space[x1.p1-1].flag = 0;    //便道的上车开入，停车场的车位被占用，flag置为0
		first.t1.h = x1.t2.h;    //更新便道上开入的汽车的开始停车的时间【便道上停车的时间不收费】
		first.t1.m = x1.t2.m;
		Push_SeqList(p, first);
		color(9);
		cout << "车牌号为" << first.num << "的车开入" << endl;
		i--;    //对便道上的车位标签-1
		Traverse_LQueue(q);     //便道第一辆车进入停车场后，对后面车辆前移
		number_SeqList++;       //便道上的车开入停车场，停车场的车辆数目+1
		number_LQueue--;        //便道上的车开入停车场，便道上的车辆数目-1
		LQueue_Delete(first);	//删除便道文件的第一辆车
	}
	//计算车费
	Car_Type = x1.type;
	color(9);
	if (!Car_Type.compare("large"))
	{
		cout << "您的车辆型号符合大车型，我们对你进行0.2￥/分钟的收费标准进行收费"<<endl;
		cout << "停留时间：" << t1 << "小时" << t2 << "分钟" << "    收费：" << t * price_1 << "元\n" << "欢迎下次光临~" << endl;
	}
	else if (!Car_Type.compare("mid"))
	{
		cout << "您的车辆型号符合中车型，我们对你进行0.1￥/分钟的收费标准进行收费"<<endl;
		cout << "停留时间：" << t1 << "小时" << t2 << "分钟" << "    收费：" << t * price_2 << "元\n" << "欢迎下次光临~" << endl;
	}
	else if (!Car_Type.compare("small"))
	{
		cout << "您的车辆型号符合大车型，我们对你进行0.05￥/分钟的收费标准进行收费"<<endl;
		cout << "停留时间：" << t1 << "小时" << t2 << "分钟" << "    收费：" << t * price_3 << "元\n" << "欢迎下次光临~" << endl;
	}
	else
	{
		cout << "您的车型不符合我们的规范我们将对你按0.5￥/分钟的收费标准进行收费" << endl;
		cout << "停留时间：" << t1 << "小时" << t2 << "分钟" << "    收费：" << t * 0.5 << "元\n" << "建议换个停车场！" << endl;
	}
	Sleep(500);
	Car_Condition(p,q); //再进行下一次的输入判断
}

//显示停车场内车辆信息
void Display(SeqList* p)
{
	Car x;
	SeqList* temp = Init_SeqList();
	if (p->top == -1)
	{
	    color(4);
		cout << "当前停车场无车，生意惨淡TvT~" << endl;
		Sleep(2000);
        system("cls");
		return;
	}
	else
	{
		while (p->top != -1)
		{
			x = Pop_SeqList(p);
			Push_SeqList(temp, x);
		}
		color(4);
		cout << "当前停放的车辆信息:" << endl;
		while (temp->top != -1)
		{
			x = Pop_SeqList(temp);
			color(9);
			cout << "停车位置为：00" << x.p1 << "  车牌号码:" << x.num << "  停车时间：" << x.t1.h << "点" << x.t1.m << "分" << endl;
			Push_SeqList(p, x);
		}
	}
	Sleep(4000);
	system("cls");
}

//获取车辆进入/离开停车场的信息
void Car_Condition(SeqList* p, LQueue* q)
{
	string x;
	color(9);
	cout << "当前已经输入"<<number<<"组数据" << "这是第"<<number+1<<"次输入信息"<< endl;
	cout << "当前停车场已停有"<<number_SeqList<<"辆车" << endl;
	cout << "当前便道上已有"<<number_LQueue<<"辆车在等待" << endl;
	color(4);
	cout << "请输入车辆进入或离开的信息[A表示进入停车场/D表示离开停车场/E表示终止输入]:\n";\
	color(7);
	cin >> x;
	if(!x.compare("A"))
	{
		Car_In(p, q);
	}
	else if(!x.compare("D"))
	{
		Car_Out(p, q);

	}
	else if(!x.compare("E"))
    {
        color(4);
        cout << "输入终止" <<endl;
        Sleep(1000);
        system("cls");
        return;
    }
    else
    {
        color(4);
        cout << "输入信息有误，请重新输入！！！"<<endl;
        Sleep(300);
        Car_Condition(p,q);     //递归调用Car_Condition函数，重新输入信息
    }
}

//修改字体颜色
int color(int m)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),m|8);
    return 0;
}

//程序刚开始运行时的加载过程
void menu_load()
{
    color(3);
    cout << "\n\n\n\t\t\t\t================欢迎使用停车场===================" << endl;
    cout<<"\t\t\t\t*\t\t\t\t\t\t"<<"*"<<endl;
    cout<<"\t\t\t\t*\t  当前正在加载，请耐心等待\t\t"<<"*"<<endl;
    cout<<"\t\t\t\t*\t\t\t\t\t\t"<<"*"<<endl;
    cout << "\t\t\t\t=================================================" << endl<<endl;
    Sleep(500);
    load_3();
    cout<<endl;
    Sleep(1000);
    load_1();
    Sleep(500);
    menu();
}

//系统主菜单页面
void menu()
{
    int flag = 1;
    SeqList* p = Init_SeqList();
	LQueue* q = Init_LQueue();
    while (flag)
	{
        color(3);
		cout << "\n\n\n\t\t\t=========================欢迎使用停车场===========================" << endl;
		cout << "\t\t\t||\t\t\t\t\t\t\t\t||" << endl;
		cout << "\t\t\t||\t\t   本停车场最多可停放" << MAXSIZE << "辆汽车" << "\t\t\t||" << endl;
		cout << "\t\t\t||\t\t\t\t\t\t\t\t||" << endl;
		cout << "\t\t\t||   本停车场可停大[large]、中[mid]、小[small]三种型号的车辆" << "    ||" << endl;
		cout << "\t\t\t||\t\t\t\t\t\t\t\t||" << endl;
		cout << "\t\t\t||  收费标准：大[0.2￥/分钟]，大[0.1￥/分钟]，小[0.05￥/分钟]" << "   ||" << endl;
		cout << "\t\t\t||\t\t\t\t\t\t\t\t||" << endl;
		cout << "\t\t\t||\t   如果您不幸不符合我们的要求，按每分钟0.5￥收费"<< "\t||" << endl;
		cout << "\t\t\t||\t\t\t\t\t\t\t\t||" << endl;
		cout << "\t\t\t==================================================================" << endl;
		cout << "\t\t\t    **********1.输入汽车信息   2.查看车库  3.退出系统**********" << endl;
        color(4);
        cout << "请进行选择[输入序号即可]:\n";
		char i;
		color(7);
		cin >> i;
		while (getchar() != '\n');	//清除缓冲区
		switch (i)
		{
            case '1':number=0;Car_Condition(p, q); break;
            case '2':Display(p); break;
            case '3':flag = 0; color(3);load_2(); break;
            default:cout << "输入错误啦~请重新输入数字1--3~" << endl; Sleep(1000); system("cls"); break;
		}
	}
}

//加载百分比可视化
void load_1()
{
    std::cout << "\n请稍后...... ";
    for (int i = 0; i <= 100; i++)  // 打印百分比
    {
        std::cout.width(3);  // i的输出为3位宽
        cout << i << "%";
        Sleep(15);
        std::cout << "\b\b\b\b";  //回删三个字符，使数字在原地变化
    }
    std::cout << "\t\t\t\t\t\t\t\t" << "\n\n";
    cout << "加载完成！";
    Sleep(1000);
    system("cls");
}

//系统退出百分比可视化
void load_2()
{
    std::cout << "\n请稍后...... ";
    for (int i = 0; i <= 100; i++)  // 打印百分比
    {
        std::cout.width(3);  // i的输出为3位宽
        cout << i << "%";
        Sleep(15);
        std::cout << "\b\b\b\b";  //回删三个字符，使数字在原地变化
    }
    std::cout << "\t\t\t\t\t\t\t\t" << "\n\n";
    cout << "系统退出成功！";
    Sleep(1000);
    system("cls");
}

//进入系统加载界面
void load_3()
{
    string reading="正在为您进入系统~~";
    for(int i=0; i<reading.length(); i++)
    {
        cout<<reading[i];
        Sleep(10);
    }
}

//停车加载百分比可视化
void load_4()
{
    std::cout << "\n请稍后...... ";
    for (int i = 0; i <= 100; i++)  // 打印百分比
    {
        std::cout.width(3);  // i的输出为3位宽
        cout << i << "%";
        Sleep(15);
        std::cout << "\b\b\b\b";  //回删三个字符，使数字在原地变化
    }
    std::cout << "\t\t\t\t\t\t\t\t" << "\n\n";
    cout << "停车成功！";
    Sleep(2000);
    system("cls");
}

//检查汽车离开出栈的时间是否合法
bool Check_TimeOut(int h1,int h2,int m1,int m2)
{
    if(((h2*60+m2)-(h1*60+m1))>0) return true;
    else return false;
}

//栈的遍历用来寻找出栈的车辆
Car Traverse_SeqList(SeqList *p,int top,string num)
{
    while(num!=p->data[top].num)
    {
        top--;
    }
    return p->data[top];
}

//字符串比较函数
bool Str_strcmp(string str1,string str2)
{
    if(!str1.compare(str2)) return 1;
    else return 0;
}