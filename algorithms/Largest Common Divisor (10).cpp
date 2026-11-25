/*
题目描述:
求两个整数的最大公约数是一个很有价值的问题，给定整数a和b，下面的方法可以较快速的求出a和b的最大公约数：

如果a是b的倍数，则a和b的最大公约数为b；
否则， a 和 b 的最大公约数，等于 b 和 a%b 的最大公约数（其中a%b表示a除b的余数）。

如，要求48和72的最大公约数，用(48,72)来表示，则可以按下面的过程来求：
(48,72) --- 48%72=48 --->(72,48) ---- 72%48=24 ----> (48,24) ---- 48是24的倍数 --> 最大公约数为24。

给出a和b，用递归的方式来求a和b的最大公约数。

输入描述:
输入的第一行包含两个整数a, b。(0 < a, b <= 10^9)

输出描述:
输出两个数的最大公约数。

样例输入:
48 72

样例输出:
24
*/

#include <iostream>

// lcd = Largest common divisor (not least common denominator, for once)
int lcd(int a, int b)
{
	// Algorithm given by instructions, very simple
	int mod = a % b;
	if (mod == 0)
	{
		return b;
	}
	return lcd(b, mod);
}

int main()
{
	// Recieve input
	int a, b;
	std::cin >> a >> b;
	
	// Calculate and print
	std::cout << lcd(a, b);
	
	return 0;
}
