#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <string>
//#define testmode 0;
using namespace std;
enum ChessColor{
	blank, black, white
};
int valuegz[4][4] = {
	0, -5, -5, -5,
	5, -1, -2, -4,
	5, 2, 0, -3,
	5, 4, 3, 1
};
int bestmove[3], rootdepth = 10;
int playercolor, computercolor;
string p[15];
int xx = 13, yy = 9, xdx = 99, xdy = 99, a[4][4], hq[20][4][4], hq1 = 0, hqb = 0;
int ej, fj;
string getStyle(int i, int j)//获得棋盘中指定坐标交点位置的字符，通过制表符拼成棋盘
{
	if (a[i / 4][j / 4] == black&&i % 4 == 1 && j % 4 == 1)//1为黑子
		return "●";
	else if (a[i / 4][j / 4] == white && i % 4 == 1 && j % 4 == 1)//2为白子
		return "○";
	else if (i == 1 && j == 1)//以下为边缘棋盘样式
		return "┏";
	else if (i == 1 && j == 13)
		return "┓";
	else if (i == 13 && j == 13)
		return "┛";
	else if (i == 13 && j == 1)
		return "┗";
	else if (j == 1 && i % 4 == 1)
		return "┠";
	else if (j == 13 && i % 4 == 1)
		return "┨";
	else if (i == 1 && j % 4 == 1)
		return "┯";
	else if (i == 13 && j % 4 == 1)
		return "┷";
	else if ((i == 1 || i == 13) && j % 4 != 1 && j != 0 && j != 14)
		return "━";
	else if (i % 4 != 1 && (j == 1 || j == 13) && i != 0 && i != 14)
		return "┃";
	else if (i % 4 == 1 && j % 4 != 1 && j != 0 && j != 14)
		return "—";
	else if (i % 4 != 1 && j % 4 == 1 && i != 0 && i != 14)
		return "│";
	else if (i % 4 == 1 && j % 4 == 1)
		return "┼";//中间的空位
	else if (i == xx - 1)
	{
		if (xx != xdx)
		{
			if (j == yy - 1)return "┏";
			else if (j == yy + 1)return "┓";
		}
		else
		{
			if (j == yy - 1 || j == xdy - 1)return "┏";
			else if (j == yy + 1 || j == xdy + 1)return "┓";
		}
	}
	else if (i == xx + 1)
	{
		if (xx != xdx)
		{
			if (j == yy - 1)return "┗";
			else if (j == yy + 1)return "┛";
		}
		else
		{
			if (j == yy - 1 || j == xdy - 1)return "┗";
			else if (j == yy + 1 || j == xdy + 1)return "┛";
		}
	}
	else if (i == xdx - 1)
	{
		if (j == xdy - 1)return "┏";
		else if (j == xdy + 1)return "┓";
	}
	else if (i == xdx + 1)
	{
		if (j == xdy - 1)return "┗";
		else if (j == xdy + 1)return "┛";
	}
	return "  ";
}
int sc()//输出 
{
	int i, j;
	for (i = 0; i < 15; i++)p[i] = "";
	for (i = 0; i < 15; i++)
		for (j = 0; j < 15; j++)
			p[i] = p[i] + getStyle(i, j);
	system("cls");
	cout << endl << endl << endl << endl;
	for (i = 0; i < 15; i++)
		cout << "        " << p[i] << endl;
	return 0;
}
int szfz(int a2[4][4], int b[4][4])
{
	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			b[i][j] = a2[i][j];
	return 0;
}
int hqhs()//悔棋函数 
{
	if (hq1 == 0)hq1 = 20;
	hqb--;
	szfz(hq[hq1 - 1], a);
	sc();
	return 0;
}
int hqchu()
{
	if (hq1 == 20)hq1 = 0;
	szfz(a, hq[hq1++]);
	hqb++;
	return 0;
}
int lqz(int a2[4][4], int i, int j, int b, int k)
{
	int opp = black + white - k;
	if (b == 2)i++;
	else if (b == 8)i--;
	else if (b == 6)j++;
	else if (b == 4)j--;
	if (i == 0 && a2[1][j] == k&&a2[2][j] == opp&&a2[3][j] == blank)
	{
		a2[2][j] = blank; return 1;
	}
	else if (i == 3 && a2[2][j] == k&&a2[1][j] == opp&&a2[0][j] == blank)
	{
		a2[1][j] = blank; return 1;
	}
	else if (i == 1)
	{
		if (a2[0][j] == k&&a2[2][j] == opp&&a2[3][j] == blank)
		{
			a2[2][j] = blank; return 1;
		}
		else if (a2[2][j] == k)
		{
			if (a2[0][j] == opp&&a2[3][j] == blank)
			{
				a2[0][j] = blank; return 1;
			}
			else if (a2[3][j] == opp&&a2[0][j] == blank)
			{
				a2[3][j] = blank; return 1;
			}
		}
	}
	else if (i == 2)
	{
		if (a2[3][j] == k&&a2[1][j] == opp&&a2[0][j] == blank)
		{
			a2[1][j] = blank; return 1;
		}
		else if (a2[1][j] == k)
		{
			if (a2[3][j] == opp&&a2[0][j] == blank)
			{
				a2[3][j] = blank; return 1;
			}
			else if (a2[0][j] == opp&&a2[3][j] == blank)
			{
				a2[0][j] = blank; return 1;
			}
		}
	}
	return 0;
}
int hqz(int ab[4][4], int i, int j, int b, int k)
{
	int opp = black + white - k;
	if (b == 2)i++;
	else if (b == 8)i--;
	else if (b == 6)j++;
	else if (b == 4)j--;
	if (j == 0 && ab[i][1] == k&&ab[i][2] == opp&&ab[i][3] == blank)
	{
		ab[i][2] = blank; return 1;
	}
	else if (j == 3 && ab[i][2] == k&&ab[i][1] == opp&&ab[i][0] == blank)
	{
		ab[i][1] = blank; return 1;
	}
	else if (j == 1)
	{
		if (ab[i][0] == k&&ab[i][2] == opp&&ab[i][3] == blank)
		{
			ab[i][2] = blank; return 1;
		}
		else if (ab[i][2] == k)
		{
			if (ab[i][0] == opp&&ab[i][3] == blank)
			{
				ab[i][0] = blank; return 1;
			}
			else if (ab[i][3] == opp&&ab[i][0] == blank)
			{
				ab[i][3] = blank; return 1;
			}
		}
	}
	else if (j == 2)
	{
		if (ab[i][3] == k&&ab[i][1] == opp&&ab[i][0] == blank)
		{
			ab[i][1] = blank; return 1;
		}
		else if (ab[i][1] == k)
		{
			if (ab[i][3] == opp&&ab[i][0] == blank)
			{
				ab[i][3] = blank; return 1;
			}
			else if (ab[i][0] == opp&&ab[i][3] == blank)
			{
				ab[i][0] = blank; return 1;
			}
		}
	}
	return 0;
}
int zqz(int ab[4][4], int i, int j, int b, int k)
{
	hqz(ab, i, j, b, k);
	lqz(ab, i, j, b, k);
	return 0;
}
int chzs(int ab[4][4], int c)
{
	int i, j, b = 0;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (ab[i][j] == c)
				b++;
	return b;
}//存活子数
int yz(int ab[4][4], int i, int j, int b)
{
	if (b == 2)ab[i + 1][j] = ab[i][j];
	else if (b == 8)ab[i - 1][j] = ab[i][j];
	else if (b == 4)ab[i][j - 1] = ab[i][j];
	else if (b == 6)ab[i][j + 1] = ab[i][j];
	ab[i][j] = blank;
	return 0;
}//移子
int sfky(int ab[4][4], int i, int j, int b)
{
	int c = 1;
	if (b == 8 && (i == 0 || ab[i - 1][j] != blank))c = 0;
	else if (b == 2 && (i == 3 || ab[i + 1][j] != blank))c = 0;
	else if (b == 4 && (j == 0 || ab[i][j - 1] != blank))c = 0;
	else if (b == 6 && (j == 3 || ab[i][j + 1] != blank))c = 0;
	else if (ab[i][j] == blank)c = 0;
	else if (b != 2 && b != 8 && b != 4 && b != 6)c = 0;
	return c;
}//判断是否可移
//int wfs(int ab[4][4], int n, int len);
//int *dfs(int ab[4][4], int n, int g[4], int len)
//{
//	int e, f, i, j, b, d = -5, h;
//	g[0] = -2000000;
//	e = chzs(ab, 1);
//	f = chzs(ab, 9);
//	h = e - f;
//	if (e < 2) { g[0] = -4000 - n; return g; }
//	if (f < 2) { g[0] = 7200 + n; return g; }
//	if (h < ej)
//	{
//		g[0] = -100 * (h + 3) - n;
//		return g;
//	}
//	if (h > ej)
//	{
//		g[0] = 100 * (h + 3) + n;
//		return g;
//	}
//	if (n < 0)
//	{
//		g[0] = -(e + f);
//		return g;
//	}
//	for (i = 0; i < 4; i++)
//		for (j = 0; j < 4; j++)
//			if (ab[i][j] == 1)
//				for (b = 2; b < 9; b = b + 2)
//					if (sfky(ab, i, j, b))
//					{
//						int c[4][4];
//						szfz(ab, c);
//						yz(c, i, j, b);
//						zqz(c, i, j, b, 1);
//						d = wfs(c, n - 1, g[0]);
//						if (d > len){ g[0] = d; return g; }
//						if (g[0] < d)
//						{
//							g[0] = d;
//							g[1] = i; g[2] = j; g[3] = b;
//						}
//					}
//	return g;
//}
//int wfs(int ab[4][4], int n, int len)
//{
//	int e, f, c1 = 2000000, i, j, b, d;
//	e = chzs(ab, 1);
//	f = chzs(ab, 9);
//	if (e < 2) return -4000 - n;
//	if (f < 2) return 7200 + n;
//	//	if(n<0) return 0;
//	for (i = 0; i < 4; i++)
//		for (j = 0; j < 4; j++)
//			if (ab[i][j] == 9)
//				for (b = 2; b < 9; b = b + 2)
//					if (sfky(ab, i, j, b))
//					{
//						int c[4][4];
//						szfz(ab, c);
//						yz(c, i, j, b);
//						zqz(c, i, j, b, 9);
//						int *abab, bb[5];
//						abab = dfs(c, n - 1, bb, c1);
//						d = abab[0];
//						if (d < len)return d;
//						if (c1 > d)c1 = d;
//					}
//	return c1;
//}

#ifdef testmode
	int deb = 0;
#endif
int alphabeta(int depth, int alpha, int beta, int ab[4][4], int curcolor)
{
	int e, f, best=-1000,value, i, j, b, d;
	int opp = black + white - curcolor;
	e = chzs(ab, curcolor);
	f = chzs(ab, opp);
	if (e < 2||f < 2||depth <= 0) 
		return valuegz[e-1][f-1];
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (ab[i][j] == curcolor)
				for (b = 2; b < 9; b = b + 2)
					if (sfky(ab, i, j, b))
					{
						int c[4][4];
						szfz(ab, c);
						yz(c, i, j, b);
						zqz(c, i, j, b, curcolor);
#ifdef testmode
						for (int i2 = 0; i2 < 4; i2++)
						{
							for (int i3 = 0; i3 < 4; i3++)
							{
								cout << c[i2][i3] << " ";
							}
							cout << endl;
						}
						cout << deb++ << endl;
#endif
						value = -alphabeta(depth - 1, -beta, -alpha, c, opp);
#ifdef testmode							
						cout << depth << " " << value << " " << best << " " << alpha << " " << beta << endl;
						cout << --deb << endl;
#endif
						if (value > best)
						{
							best = value;
							if (depth == rootdepth)
							{
								bestmove[0] = i;
								bestmove[1] = j;
								bestmove[2] = b;
							}
							if (best > alpha)
							{
								alpha = best;
							}
						}
						if (best >= beta)
						{
							return best;
						}
						
					}
	return best;
}
int mtdf(int depth, int alpha, int beta, int ab[4][4], int curcolor)
{
	int e, f, best = -1000, value, i, j, b, d;
	int low=-5, high=5, mid=0;
	int opp = black + white - curcolor;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (ab[i][j] == curcolor)
				for (b = 2; b < 9; b = b + 2)
					if (sfky(ab, i, j, b))
					{
						int c[4][4];
						szfz(ab, c);
						yz(c, i, j, b);
						zqz(c, i, j, b, curcolor);
#ifdef testmode
						for (int i2 = 0; i2 < 4; i2++)
						{
							for (int i3 = 0; i3 < 4; i3++)
							{
								cout << c[i2][i3] << " ";
							}
							cout << endl;
						}
						cout << deb++ << endl;
#endif
						low = alpha;
						high = beta;
						while (low<high)
						{
							mid = (low + high) / 2;
							value = -alphabeta(depth-1, -mid, -mid+1, c,opp);
							if (value > mid)
								low = mid + 1;
							else if (value < mid)
								high = mid - 1;
							else 
								break;
						}
						value =mid;
#ifdef testmode
						cout << depth << " " << value << " " << best << " " << alpha << " " << beta <<"mtdf"<< endl;
						cout << --deb << endl;
#endif
						if (value > best)
						{
							best = value;
							if (best > alpha)
							{
								alpha = best;
							}
							bestmove[0] = i;
							bestmove[1] = j;
							bestmove[2] = b;
						}
						if (best >= beta)
						{
							return best;
						}
					}
	return best;
}
int dny(int n,int curcolor)
{
	int i;
	int opp = black + white - curcolor;
	alphabeta(rootdepth, -5, 5, a, curcolor);
	yz(a, bestmove[0], bestmove[1], bestmove[2]);
	zqz(a, bestmove[0], bestmove[1], bestmove[2], curcolor);
	return 0;
}
int wjy(int &i, int &j, int &b,int curcolor)
{
	int c = 0, d = 0, e, f, ch;
	do
	{
		while (1)
		{
		v1:ch = getch(); e = xx / 4; f = yy / 4;
			if (ch == 27)
			{
				if (hqb < 2)goto v1;
				hq1--;
				hqhs();
				goto v1;
			}
			else if (ch == 0x20 && d == 0 && a[e][f] != blank){ i = e; j = f; d = 1; xdx = xx; xdy = yy; }
			else if (ch == 0x20 && d == 1)
			{
				if (i == e)
				{
					if (j == f + 1){ b = 4; break; }
					else if (j == f - 1){ b = 6; break; }
					else if (j == f){ xdx = 99; xdy = 99; d = 0; goto v1; }
					else goto v1;
				}
				else if (j == f)
				{
					if (i == e + 1){ b = 8; break; }
					else if (i == e - 1){ b = 2; break; }
					else goto v1;
				}
				else goto v1;
			}
			else if (ch == 0xE0)//如果按下的是方向键，会填充两次输入，第一次为0xE0表示按下的是控制键
			{
				ch = getch();//获得第二次输入信息
				switch (ch)//判断方向键方向并移动光标位置
				{
				case 0x4B://
					yy = yy - 4;
					if (yy < 0)yy = 13;
					break;
				case 0x48:
					xx = xx - 4;
					if (xx < 0)xx = 13;
					break;
				case 0x4D:
					yy = yy + 4;
					if (yy > 16)yy = 1;
					break;
				case 0x50:
					xx = xx + 4;
					if (xx > 16)xx = 1;
					break;
				}
				sc();
			}
		}
		c = sfky(a, i, j, b);
	} while (c != 1);
	xdx = 99; xdy = 99;
	yz(a, i, j, b);
	zqz(a, i, j, b, curcolor);
	return 0;
}
int main()
{
	int j, n, e = 0, f = 0, ss, b, i;
	system("title 四棋 ——最爱吃兽奶制作");//设置标题
//	system("mode con cols=45 lines=24");//设置窗口大小
	system("color b0");//设置颜色
	while (1)
	{	
//		cout << "请输入难度0~9，0~2新手，3~4高手，5~9大神" << endl;
//		cin >> rootdepth;
		cout << "1  黑，2  白" << endl;
		cin >> ss;
		if (ss == 2)
		{
			a[0][1] = blank;
			a[1][1] = black;
			playercolor = white;
			computercolor = black;
		}
		else
		{
			playercolor = black;
			computercolor = white;
		}

		for (i = 0; i < 4; i++)
			a[1][i] = blank;
		for (i = 0; i < 4; i++)
			a[2][i] = blank;
		for (i = 0; i < 4; i++)
			a[0][i] = computercolor;
		for (i = 0; i < 4; i++)
			a[3][i] = playercolor;
		if (ss == 2)
		{
			a[0][1] = blank;
			a[1][1] = black;
		}
		sc();
		while (1)
		{
			hqchu();
			wjy(i, j, b,playercolor);
			zqz(a, i, j, b, playercolor);
			dny(rootdepth,computercolor);
			sc();
			e = chzs(a, computercolor);
			f = chzs(a, playercolor);
			if (e < 2)
			{
				cout << "你赢了" << endl;
				cout << "重新请按回车键" << endl;
				e = getch();
				if (e != 13)return 0;
				system("cls");
				break;
			}
			else if (f < 2)
			{
				cout << "你输了" << endl;
				cout << "重新请按回车键" << endl;
				f = getch();
				if (f != 13)return 0;
				system("cls");
				break;
			}
		}
	}
}