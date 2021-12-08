#include "Settings.h"

int width = 1178;
int height = 674;
double movespeed = 0.35;
double backstrength = 600;
int bulletspeed = 6;
int gap_time = 300;
double g = 1000;
double gx = 800;
char player1[128] = "���1";
char player2[128] = "���2";
int rolelives = 5;
char char_rolelives[10];

int initialhp = 20;

double bornx[2] = { 350, 700 };
double borny[2] = { -100, -100 };

void showsettings() {
  drawAlpha(&gameimage.settings, 0, 0);
  myfont.lfHeight = 36;
  myfont.lfWeight = FW_BOLD;
  settextstyle(&myfont);
  outtextxy(69, 279, player1);
  outtextxy(659, 279, player2);
  _stprintf_s(char_rolelives, _T("%d"), rolelives);
  outtextxy(71, 528, char_rolelives);
}

void changesetting(int& par) {
  char s[10];
  InputBox(s, 5, _T("��������ֵ"));
  
  if (atoi(s) < 1) {
    MessageBox(GetHWnd(), "��Ч��������!", _T("��ʾ"), MB_OK | MB_ICONWARNING);
  }
  else {
    par = atoi(s);
    writesettings();
  }
  showsettings();
}

void changesetting(char* par) {
  char s[20];
  InputBox(s, 10, _T("���������û���"));
  strcpy(par, s);
  writesettings();
  showsettings();
}