#include<stdio.h>
#include<conio.h>
#include<graphics.h>//Ҫ��װ�ģ�ͼ�ο�ͷ�ļ�easyx
#include<mmsystem.h>//win32 ��ý���豸�ӿ�
//΢�Ź��ںţ�C����Plus
#pragma comment(lib,"winmm.lib")
#define WIN_WIDTH 591
#define WIN_HEIGHT 864     
#define BULLET_NUM 15//����ӵ�����
#define ENEMYPLANE_NUM 10

struct Image
{
	IMAGE bakcGround;//����ͼƬ
	IMAGE player[4];
	IMAGE bullet[2];
	IMAGE enemy[4];
}image;
//�ṹ�����ֵܣ��ṹ�壬ö�٣�����
enum TYPE
{
	SMALL,
	BIG,
};
struct Plane //�ң��л����ӵ�
{
	int x;
	int y;
	bool flag;//�Ƿ���  true flase
	union
	{
		int type;//�л���������
		int score;//��Ҷ�������
	}un;
	int hp;//Ѫ��
}player, bullet[BULLET_NUM], enemy[ENEMYPLANE_NUM];
DWORD t1, t2, t3, t4;
int a[3] = { 1,2,3 };//�����ĸ�����
DWORD WINAPI PlayGun(LPVOID lparamter);
//��ģ�鴦��ר�ŵ����飬ר�ŵĺ�����
//ȷ���ɻ�������
void EnemyHP(int i)
{
	if (rand() % 10 == 0) //ʮ��֮һ�ĸ��ʲ���һ����ɻ�
	{
		enemy[i].un.type = BIG;
		enemy[i].hp = 3;
	}
	else
	{
		enemy[i].un.type = SMALL;
		enemy[i].hp = 1;
	}
}
//����ͼƬ
void GameInit()
{
	//�������������
	srand(GetTickCount());
	t1 = t2 = t3 = t4 = GetTickCount();//��ȡϵͳ��������ǰ�������ĺ�����
	//��������
	mciSendString("open ./images/game_music.mp3 alias BGM", 0, 0, 0);//���ý���豸�ӿڷ����ַ��� media device interface 
	mciSendString("play BGM repeat", 0, 0, 0);
	//���ر���ͼƬ
	loadimage(&image.bakcGround, "./images/background.jpg");//��ͼƬ���ز�����
	loadimage(&image.player[0], "./images/planeNormal_1.jpg");
	loadimage(&image.player[1], "./images/planeNormal_2.jpg");
	loadimage(&image.player[2], "./images/boom_1.jpg");
	loadimage(&image.player[3], "./images/boom_2.jpg");
	loadimage(&image.bullet[0], "./images/bullet1.jpg");
	loadimage(&image.bullet[1], "./images/bullet2.jpg");
	loadimage(&image.enemy[0], "./images/enemy_1.jpg");
	loadimage(&image.enemy[1], "./images/enemy_2.jpg");
	loadimage(&image.enemy[2], "./images/enemyPlane1.jpg");
	loadimage(&image.enemy[3], "./images/enemyPlane2.jpg");

	//��ʼ���������
	player.x = WIN_WIDTH / 2;
	player.y = WIN_HEIGHT - 120;
	player.flag = true;
	player.hp = 999;
	player.un.score = 0;
	//��ʼ���ӵ�����
	for (int i = 0; i < BULLET_NUM; i++)
	{
		bullet[i].flag = false;
	}
	//��ʼ���л�
	for (int i = 0; i < ENEMYPLANE_NUM; i++)
	{
		enemy[i].flag = false;
		EnemyHP(i);
	}
}
//��Ϸ�������
void Gamedraw()
{
	//���Ʊ���
	putimage(0, 0, &image.bakcGround);
	//�������
	if (player.flag == true)
	{
		putimage(player.x, player.y, &image.player[0], NOTSRCERASE);
		putimage(player.x, player.y, &image.player[1], SRCINVERT);
	}
	else//�����������Ϸ����
	{
		settextcolor(RED);
		settextstyle(50, 0, "����");
		char arr[] = "��Ϸ����~";
		int tsize = WIN_WIDTH / 2-textwidth(arr)/2;
		outtextxy(tsize, WIN_HEIGHT / 3,arr);
		putimage(player.x, player.y, &image.player[2], NOTSRCERASE);
		putimage(player.x, player.y, &image.player[3], SRCINVERT);
	}

	//�����ӵ�
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].flag == true)
		{
			putimage(bullet[i].x, bullet[i].y, &image.bullet[0], NOTSRCERASE);
			putimage(bullet[i].x, bullet[i].y, &image.bullet[1], SRCINVERT);
		}
	}
	for (int i = 0; i < ENEMYPLANE_NUM; i++)
	{
		if (enemy[i].flag)
		{
			if (enemy[i].un.type == TYPE::SMALL)
			{
				putimage(enemy[i].x, enemy[i].y, &image.enemy[0], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &image.enemy[1], SRCINVERT);
			}
			else if (enemy[i].un.type == TYPE::BIG)
			{

				putimage(enemy[i].x, enemy[i].y, &image.enemy[2], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &image.enemy[3], SRCINVERT);
			}
		}
	}
	//��ʾ����
	char score[10];
	settextcolor(BLACK);
	settextstyle(20, 0, "����");
	sprintf(score, "������%d", player.un.score);
	setbkmode(TRANSPARENT);
	outtextxy(10, 10, score);
}
//����һ���ӵ�
void CreateBullet()
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].flag == false)
		{
			bullet[i].flag = true;
			bullet[i].x = player.x + 45;
			bullet[i].y = player.y;
			break;
		}
	}
}
//�ӵ��ƶ�
void BulletMove(int speed)
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].flag == true)
		{
			bullet[i].y -= speed;
			if (bullet[i].y <= 0)
			{
				bullet[i].flag = false;
			}
		}
	}
}
//�����л�
void CreateEnemy()
{
	for (int i = 0; i < ENEMYPLANE_NUM; i++)
	{
		if (enemy[i].flag == 0)
		{
			enemy[i].flag = true;
			enemy[i].x = rand() % (WIN_WIDTH - 100);
			enemy[i].y = 0;
			EnemyHP(i);
			break;
		}
	}
}
//�л��ƶ�
void EnemyMove(int speed)
{
	for (int i = 0; i < ENEMYPLANE_NUM; i++)
	{
		if (enemy[i].flag)
		{
			enemy[i].y += speed;
			if (enemy[i].y >= WIN_HEIGHT)
			{
				enemy[i].flag = false;
			}
		}
	}
}
//��Ϸ����
void GameControl(int speed)
{
	//_getch();GetAnsyncKeyState();
	if (GetAsyncKeyState(VK_UP) && player.y >= 0)
	{
		player.y -= speed;
	}
	if (GetAsyncKeyState(VK_DOWN) && player.y + 120 <= WIN_HEIGHT)
	{
		player.y += speed;
	}
	if (GetAsyncKeyState(VK_LEFT) && player.x + 50 >= 0)
	{
		player.x -= speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) && player.x + 55 <= WIN_WIDTH)
	{
		player.x += speed;
	}
	//�����ӵ�
	if (GetAsyncKeyState(VK_SPACE) && t2 - t1 > 200)
	{
		CreateBullet();
		HANDLE threadID = CreateThread(NULL, 0, PlayGun, a, 0, 0);
		CloseHandle(threadID);
		t1 = t2;
	}
	t2 = GetTickCount();
	if (t4 - t3 > rand() % 500 + 500)
	{
		CreateEnemy();
		t3 = t4;
	}
	t4 = GetTickCount();
	EnemyMove(1);
	BulletMove(1);
}
void PlayEnemyP()
{
	//1�������л�����  �ж��Ƿ���
	for (int i = 0; i < ENEMYPLANE_NUM; i++)
	{
		if (enemy[i].flag == 0)
		{
			continue;
		}
		//����л��Ŀ�Ⱥ͸߶�
		int width = 0, height = 0;
		if (enemy[i].un.type == BIG)
		{
			width = 100;
			height = 130;
		}
		else if (enemy[i].un.type == TYPE::SMALL)
		{
			width = 50;
			height = 30;
		}
		//�˳���д��ǰ����Ϊ���ã����������������ʾ���ó���������һ�Σ�
		if (player.flag == false)
		{
			MessageBox(GetHWnd(), "������`", "Over", MB_OKCANCEL);
			exit(666);
		}
		//��Һ͵л��Ƿ���ײ
		if (player.x + 50 >= enemy[i].x && player.x + 50 <= enemy[i].x + width
			&& player.y > enemy[i].y && player.y < enemy[i].y + height)
		{
			player.flag = false;
		}
		//2�������ӵ�����
		for (int k = 0; k < BULLET_NUM; k++)
		{
			if (bullet[k].flag == 0)
			{
				continue;
			}

			//3���ж��ӵ��Ƿ���ел�
			if (bullet[k].x > enemy[i].x && bullet[k].x<enemy[i].x + width
				&& bullet[k].y>enemy[i].y && bullet[k].y < enemy[i].y + height)
			{
				//4,�ﵽ�ˣ����Ըı䣬Ѫ��--�����״̬����ҷ���++��
				enemy[i].hp--;
				bullet[k].flag = 0;
				player.un.score++;
				if (player.un.score > 0 && player.un.score % 5 == 0 && player.un.score % 2 != 0)
				{

					HANDLE threadID = CreateThread(NULL, 0, PlayGun, a + 1, 0, 0);
					CloseHandle(threadID);
				}
				if (player.un.score % 10 == 0)
				{
					HANDLE threadID = CreateThread(NULL, 0, PlayGun, a + 2, 0, 0);
					CloseHandle(threadID);
				}
			}
			//5�����Ѫ��Ϊ0���л�����
			if (enemy[i].hp <= 0)
			{
				enemy[i].flag = 0;
			}
		}
	}
}
int main()
{
	initgraph(WIN_WIDTH, WIN_HEIGHT, SHOWCONSOLE);//����һ��ͼ�δ���
	GameInit();
	BeginBatchDraw();//����˫����ͼ�������ڴ����滭�ã�����ʵ���㿴
	while (1)
	{
		Gamedraw();
		FlushBatchDraw();//���뿴��
		GameControl(1);
		PlayEnemyP();
	}
	EndBatchDraw();//����
	return 0;
}
//��������
DWORD WINAPI PlayGun(LPVOID lparamter)
{
	int* choose = (int*)lparamter;
	printf("%d  \n", *choose);
	if (*choose == 1)
	{
		mciSendString("close f_gun", NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
		mciSendString("open ./images/f_gun.mp3 alias f_gun", NULL, 0, NULL);//�򿪱�������
		mciSendString("play f_gun", NULL, 0, NULL);  // ѭ������
	}
	else if (*choose == 2)
	{
		mciSendString("close 5music", NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
		mciSendString("open ./images/5.mp3 alias 5music", NULL, 0, NULL); // ������
		mciSendString("play 5music", NULL, 0, NULL); // ������һ��
	}
	else if (*choose == 3)
	{
		mciSendString("close 10music", NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
		mciSendString("open ./images/10.mp3 alias 10music", NULL, 0, NULL); // ������
		mciSendString("play 10music", NULL, 0, NULL); // ������һ��
	}

	return 0;
}

