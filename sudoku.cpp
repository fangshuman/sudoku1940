// sudoku.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <fstream>
#include <iostream>
using namespace std;

void dfs(int x, int y);
int check(int x, int y, int a);
void createBoard(long n);
void createLine(int k);
void swapLine(int line1, int line2);
void output();

int num[9][9];
int line[9];
int orl[9] = { 5,1,2,3,4,6,7,8,9 };
bool flag = false;
ofstream Output;


int main(int argc, char* argv[])
{
	ifstream Input;
	int i, j;
	Output.open("./BIN/sudoku.txt");
	if (argv[1][0] == '-'&&argv[1][1] == 'c')
	{
		long n;
		n = atoi(argv[2]); //把char转为int
		if (n == 0)
			cout << "wrong input" << endl;
		else
			createBoard(n);
	}
	else if (argv[1][0] == '-'&&argv[1][1] == 's')
	{
		Input.open(argv[2]);
		i = 0;
		j = 0;
		while (!Input.eof())
		{
			Input >> num[i][j];
			j++;
			if (j == 9)
			{
				j = 0;
				i++;
			}
			if (i == 9)
			{
				dfs(0, 0);
				flag = false;
				memset(num, 0, sizeof(num));
				i = 0;
			}
		}
	}
	else
		cout << "wrong input" << endl;
	Output.close();
	return 0;
}


//判断填入的数字是否会有冲突
int check(int x, int y, int a)	
{
	int i, j;
	for (i = 0; i < 9; i++)	
	{
		if (num[x][i] == a && i != y)	//搜索横排
			return false;
		else if (num[i][y] == a && i != x)	//搜索竖排
			return false;
	}
	int start_x = x / 3 * 3;
	int start_y = y / 3 * 3;
	int end_x = x / 3 * 3 + 2;
	int end_y = y / 3 * 3 + 2;
	for (i = start_x; i <= end_x; i++)	//搜索九宫格
		for (j = start_y; j <= end_y; j++)
			if (num[i][j] == a && i != x && j != y)
				return false;
	return true;
}


//dfs搜索填写数字
void dfs(int x, int y)	
{
	int i;
	if (y == 9)
	{
		x++;
		y = 0;
	}
	if (x == 9 && y == 0)	//边界
	{
		output();
		flag = true; //如果输出过就不要再继续输出其他解
	}
	if (!num[x][y])
	{
		for (i = 1; i <= 9; i++)
		{
			num[x][y] = i;
			if (check(x, y, i))
				dfs(x, y + 1);
			if (flag)
				return;
			num[x][y] = 0;	//还原为空格
		}
	}
	else
		dfs(x, y + 1);
}


//产生行
void createLine(int k)
{
	int i;
	for (i = 0; i < 9; i++)
		num[k][i] = line[i]; //设置第一行
	for (i = 0; i < 6; i++)	 //设置第二行
		num[k + 1][i] = line[i + 3];
	for (i = 0; i < 3; i++)
		num[k + 1][i + 6] = line[i];
	for (i = 0; i < 3; i++)
		num[k + 2][i] = line[i + 6]; //设置第三行
	for (i = 0; i < 6; i++)
		num[k + 2][i + 3] = line[i];
}


//产生数独终局
void createBoard(long n)
{
	int i, j, a;
	memset(num, 0, sizeof(0));
	for (a = 0; a < 40320; a++)
	{
		for (i = 0; i < 9; i++)	//跳转createLine准备设置前三行
			line[i] = orl[i];
		createLine(0);

		for (i = 0; i < 8; i++)	//跳转createLine准备设置中三行
			line[i] = orl[i + 1];
		line[8] = orl[0];
		createLine(3);

		for (i = 0; i < 7; i++) //跳转createLine准备设置后三行
			line[i] = orl[i + 2];
		line[7] = orl[0];
		line[8] = orl[1];
		createLine(6);

		//完成初始图，4-6行，7-9行可进行交换形成更多新终局
		for (i = 0; i < 2; i++)
		{
			for (j = 0; j < 3; j++)
			{
				swapLine(i * 3 + 3 + 0, i * 3 + 3 + 1);
				output();
				if (--n == 0)
					break;
				swapLine(i * 3 + 3 + 1, i * 3 + 3 + 2);
				output();
				if (--n == 0)
					break;
			}
			if (n == 0)
				break;
		}

		if (n == 0)
			break;
		next_permutation(orl + 1, orl + 9);	//重置第一行，共有8!种重置方法
	}
}


//交换行
void swapLine(int line1, int line2)
{
	int tmp[9];
	int i;
	for (i = 0; i < 9; i++)
		tmp[i] = num[line1][i];
	for (i = 0; i < 9; i++)
		num[line1][i] = num[line2][i];
	for (i = 0; i < 9; i++)
		num[line2][i] = tmp[i];
	return;
}

//输出函数
void output()
{
	int i, j;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 8; j++)
			Output << num[i][j] << " ";
		Output << num[i][8] << endl;
	}
	Output << endl;
}