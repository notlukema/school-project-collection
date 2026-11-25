/*
题目描述:
有一个数列，f(n) = f(n-1) + f(n//2) + f(n//3)，其中n//m表示整除。 如 3//2=1, 4//2=2, 5//2=2。
当n<=0时，有f(n) = 0。
现在告诉你f(1)=x，请计算f(n)的值.

请使用递归完成计算。

输入描述:
输入两个整数x、n。
0 < x，n <= 20

输出描述:
输出f(n)的值。

样例输入:
1 6

样例输出:
16

注释:
样例含义

f(1) = 1
f(2) = f(1) + f(1) + f(0) = 1+1+0 = 2
f(3) = f(2) + f(1) + f(1) = 2+1+1 = 4
f(4) = f(3) + f(2) + f(1) = 4+2+1 = 7
f(5) = f(4) + f(2) + f(1) = 7+2+1 = 10
f(6) = f(5) + f(3) + f(2) = 9+4+2 = 16
*/

#include <iostream>

// Dynamically set f(1) value (when x = 1, f(x) = x1)
int x1 = 0;

// The f(x) function as described by the instructions
int theFunction(int n)
{
	if (n <= 0)
	{
		return 0;
	}
	if (n == 1)
	{
		return x1; // Dynamic return value
	}
	
	return theFunction(n - 1) + theFunction(n / 2) + theFunction(n / 3);
}

int main()
{
	// Recieve inputs
	int x, n;
	std::cin >> x >> n;
	x1 = x; // Set f(1) equivalent value
	
	// Calculate and print
	std::cout << theFunction(n);
	
	return 0;
}
