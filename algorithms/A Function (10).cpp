/*
题目描述:
定义递归函数f(x):

当x=1时，f(x)=1。

当x>1且为奇数时f(x)=f((x-1)/2)+f((x+1)/2)

当x为偶数时f(x)=f(x/2)

请编写程序，完成对f(x)的计算

输入描述:
第一行一个正整数T，表示数据组数。（T<=100）
之后T行，每行一个正整数x。（x<=10^9）

输出描述:
对于每组数据，输出一行答案。

样例输入:
3
1
3
10

样例输出:
1
2
3
*/

#include <iostream>

// The f(x) function as described by the instructions
int theFunction(int x)
{
	if (x == 1)
	{
		return 1;
	}
	
	// Even numbers come first
	if (x % 2 == 0)
	{
		return theFunction(x / 2);
	}
	
	if (x > 1)
	{
		return theFunction((x - 1) / 2) + theFunction((x + 1) / 2);
	}
	
	return x;
}

int main()
{
	// Recieve input
	int n;
	std::cin >> n;
	
	// Recieve array inputs
	int* nums = new int[n];
	for (int i = 0; i < n; ++i)
	{
		std::cin >> nums[i];
	}
	
	// Calculate for every number (and print)
	for (int i = 0; i < n; ++i)
	{
		std::cout << theFunction(nums[i]) << "\n";
	}
	
	// Cleanup
	delete[] nums;
	
	return 0;
}
