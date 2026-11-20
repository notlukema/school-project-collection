/*
题目描述:
有三个向量（可视作三根杆），分别命名为 A、B、C。
初始时，向量 A 中包含 N 个互不相同的正整数，并且已经按 从大到小（最左是最大的，最右是最小的） 排好序；
向量 B 和 C 均为空；
每个向量的尾部（右侧）视为“顶部”，可以从尾部弹出，也可以向尾部压入。

随后输入若干动作。每个动作由两个不同的字符构成，例如：
AB：表示将 A 的 顶部（尾部） 元素移动到 B 的顶部（尾部）。
CA：表示将 C 的顶部元素移动到 A 的顶部。
更一般地，动作 XY 表示：从向量 X 的尾部取出一个数字，并将其压入向量 Y 的尾部。

动作必须满足 两个条件同时成立才合法：
源向量 X 非空（否则非法动作）。
不能把大数字压在小数字上。若 Y 非空，则要求：移动的数字 < Y 的尾部数字，否则该动作非法。

非法动作出现时：
不执行该动作；
立即输出 ERROR，程序结束；
之后不再输出任何状态。

程序在满足下列条件之一时终止：
出现非法动作；
某一步执行完后，A 和 B 都为空（全部数字在 C 中）；
输入动作读完。

输入描述:
第一行一个整数 N ，表示有 N 个不同的正整数。
第二行共 N 个整数，互不相同并且从大到小排好序，为A的初始内容。
第三行一个整数 M ，表示接下来有 M 个操作。
接着 M 行，每行为两个不同的字符，代表一个动作。

输出描述:
输出若干组状态。
一个状态包括三行，每行依次是 A、B、C 的内容，相邻两个状态之间不需要额外空行。
输出一个向量的内容时，从左到右依次输出该向量包含的数字（从大到小），每个数字之间用空格分开；
如果一个向量为空，则输出空行。
首先输出动作操作前的状态，然后依次输出每一步动作后的状态。
如果在某步结束后 A、B 都为空，则输出该状态组后结束；
若遇到非法动作，则不需要输出该动作后的状态，而是直接输出一行 ERROR 并且结束输出。

样例输入:
3
5 3 1
4
AC
AB
CB
CA

样例输出:
5 3 1


5 3

1
5
3
1
5
3 1

ERROR
*/

// This is a very simple program compared to file editing (I hate editing those files)

#include <iostream>
#include <vector>
#include <string>

// Move number (disc) between towers with error checking
bool move(char from, char to, std::vector<int>** tower_set)
{
	std::vector<int>& i = *tower_set[from - 'A']; // A = 0, B = 1, C = 2
	std::vector<int>& j = *tower_set[to - 'A']; // We assume the characters are always in bounds
	
	if (i.empty()) // Moving nonexistent number?
	{
		return false;
	}
	
	int v = i.back(); // Get last item (top of the tower)
	i.pop_back(); // Pop
	
	if (!j.empty() && v > j.back()) // Pushing a larger number? (Not allowed)
	{
		return false;
	}
	
	// Passed checks and move number
	j.push_back(v);
	return true;
}

// Print current tower states
void print(std::vector<int>** tower_set)
{
	// For every tower in the set
	for (int i = 0; i < 3; i++)
	{
		// Print every number (disc)
		for (int v : *tower_set[i])
		{
			std::cout << v << " ";
		}
		std::cout << "\n";
	}
}

int main()
{
	// Recieve input
	int n;
	std::cin >> n;
	
	// Initialize three towers
	std::vector<int> a;
	std::vector<int> b;
	std::vector<int> c;
	
	// Used later for easier ABC-to-tower mapping
	std::vector<int>* tower_set[3] = {&a, &b, &c};
	
	for (int i = 0; i < n; ++i) // Grab numbers on starting tower
	{
		int v;
		std::cin >> v;
		a.push_back(v);
	}
	
	int m;
	std::cin >> m; // Number of commands
	
	// First print
	print(tower_set);
	
	// Receive commands
	std::string* commands = new std::string[m];
	for (int i = 0; i < m; ++i)
	{
		std::cin >> commands[i];
	}
	
	// Parse
	for (int i = 0; i < m; ++i)
	{
		// Initial check
		std::string& str = commands[i];
		if (str.size() < 2)
		{
			std::cout << "ERROR";
			return 1;
		}
		
		// Attempt to move
		if (!move(str.at(0), str.at(1), tower_set))
		{
			std::cout << "ERROR";
			return 1;
		}
		
		// Print
		print(tower_set);
		
		// Check if tower puzzle is solved (ignore later commands)
		if (a.empty() && b.empty())
		{
			break;
		}
	}
	
	// Clean data
	delete[] commands;
	
	return 0;
}
