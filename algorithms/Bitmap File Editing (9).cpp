/*
题目描述
给定一个 BMP文件 格式的图像数据，需要根据图像文件的 高度（像素）来自动选择一种特定的几何图形，按照下表中的“说明”部分的要求，将原图像中指定区域中的像素值保持不变，而在该区域以外的图像像素值则全部改为0（对应的是黑色）。

高度（像素）	几何图形	说明（以下x,y表示坐标原点在左下角时像素点的坐标值）
100	内切圆
以 (w/2，h/2) 为圆心，半径 r=min(w，h)/2 的圆内区域（含圆上）
200	内切圆取反	以 (w/2，h/2) 为圆心，半径 r=min(w，h)/2 的圆外区域
300	第一象限的长方形	w/2≤x≤w，h/2≤y≤h 的长方形区域
400
中心位置的长方形	w/4≤x≤3w/4，h/4≤y≤3h/4 的长方形区域
500	从左下到右上宽度为固定值的一条斜线	距离对角线距离 d≤D/2 的区域（斜线宽度 D=min(w，h)/10）
图片宽 w 像素（在本题中，所有BMP文件宽度 w 均为500），高 h 像素（h=100，200，300，400，500）

注意：
1、输入文件名为 image.bmp，输出文件名为 output.bmp。
2、为了简单起见，本题 所有测试图片只包含BMP文件头、BMP信息头 和 位图数据（详情见注释部分），颜色用24位二进制（3个字节）来存储，图片宽度均为 500像素。
3、位图数据区起始地址的偏移值 是 54字节（BMP文件头 14 字节，BMP信息头 40 字节）。位图数据区是一行接一行的像素点颜色值，全零的3个字节表示黑色。位图数据的BMPInfoHeader.biHeight > 0，其存放方式是：第0行为图片底边，第h-1行为图片顶边。
4、点 (x0，y0) 到直线 y=kx 的距离 d 的计算公式如下：
[distance.png]

输入描述:
一个BMP图片文件 image.bmp（示例BMP图片，请点击下载）
（1）样例1：输入图像（宽500，高100）
[image1.bmp]
（2）样例2：输入图像（宽500，高200）
[image2.bmp]
（3）样例3：输入图像（宽500，高300）
[image3.bmp]
（4）样例4：输入图像（宽500，高400）
[image4.bmp]
（5）样例5：输入图像（宽500，高500）
[image5.bmp]

输出描述
一个修改后的BMP图片文件 output.bmp
（1）样例1：输出图像（宽500，高100）
处理方式：因为高100，所以进行 内切圆 处理
[output1.bmp]
（2）样例2：输出图像（宽500，高200）
处理方式：因为高200，所以进行 内切圆取反 处理
[output2.bmp]
（3）样例3：输出图像（宽500，高300）
处理方式：因为高300，所以进行 第一象限的长方形 处理
[output3.bmp]
（4）样例4：输出图像（宽500，高400）
处理方式：因为高400，所以进行 中心位置的长方形 处理
[output4.bmp]
（5）样例5：输出图像（宽500，高500）
处理方式：因为高500，所以进行 从左下到右上宽度为固定值的一条斜线 处理
[output5.bmp]

样例输入:
一个BMP图片文件 image.bmp

样例输出:
一个修改后的BMP图片文件 output.bmp

注释:
本题中，所有BMP文件的数据按照从文件头开始的先后顺序分为  3 个部分：
BMP文件头（BMP file header）：提供文件的格式、大小等信息
BMP信息头（BMP information header）：提供图像数据的宽度、高度、每像素位数等信息
位图数据（BMP data）：图像数据

BMP图片的文件头和信息头结构体：

#include <cstdint>  // int32_t, uint32_t, uint16_t

#pragma pack(push, 1)    // 本行不可删改！
typedef struct {
uint16_t signature;      // "BM" (0x4D42)
uint32_t file_size;      // 文件总大小
uint16_t reserved1;      // 保留
uint16_t reserved2;      // 保留
uint32_t data_offset;    // 图像数据起始偏移
} BMPFileHeader;

typedef struct {
uint32_t biSize;         // 本结构大小(40字节)
int32_t biWidth;         // 图像宽度（像素）
int32_t biHeight;        // 图像高度（像素）
uint16_t biPlanes;       // 颜色平面数（必须为1）
uint16_t biBitCount;     // 每像素位数（在本题中，统一为24，即3个字节存储一个像素点）
uint32_t biCompression;  // 压缩类型
uint32_t biSizeImage;    // 图像数据大小（字节）
int32_t biXPelsPerMeter; // 水平分辨率（像素/米）
int32_t biYPelsPerMeter; // 垂直分辨率（像素/米）
uint32_t biClrUsed;      // 实际使用的颜色数
uint32_t biClrImportant; // 重要颜色数
} BMPInfoHeader;
#pragma pack(pop)         // 本行不可删改！
*/

#include <iostream>
#include <fstream>
#include <cstdint>  // int32_t, uint32_t, uint16_t

// This time, we will use the given code

// It's funny that one of the older directives is the most compatible directive
// #pragma pack()
// It's as if the coding community cannot figure out what they want

// By packing, we guarentee that the compiler doesn't put any padding into the data, so we can read file data into the struct as a whole
// Otherwise, the padding would expand the data size and reading data directly into the struct would result in misaligned information
// However, the downsides also mean that this struct is not very portable, but that's not too big of a concern here
#pragma pack(push, 1)
typedef struct {
	uint16_t signature;
	uint32_t file_size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t data_offset;
} BMPFileHeader;

// Note: typedef struct is essentially useless in C++, but it's a very good quality-of-life feature in C
typedef struct {
	uint32_t biSize;
	int32_t biWidth;
	int32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t biXPelsPerMeter;
	int32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} BMPInfoHeader;
#pragma pack(pop)

// Classic rgb pixel, no need to pack since all data sizes are the same (no padding to begin with)
struct Pixel
{
	uint8_t b, g, r, a;
	
	// Default to fully opaque in case we only have rgb and not a in the file
	Pixel() : b(0), g(0), r(0), a(255)
	{}
};

// No outsourced functions today (funny scenario)

int main()
{
	// Fast load header data
	std::ifstream input("image.bmp");
	if (!input.is_open())
	{
		return 1;
	}
	
	// Read right into the header structs
	BMPFileHeader file_header{};
	BMPInfoHeader info_header{};
	if (!input.read(reinterpret_cast<char*>(&file_header), sizeof(BMPFileHeader))) // Read fail?
	{
		return 1;
	}
	
	if (file_header.signature != 0x4D42) // Not "BM"
	{
		return 1;
	}
	
	if (!input.read(reinterpret_cast<char*>(&info_header), sizeof(BMPInfoHeader))) // Read fail?
	{
		return 1;
	}
	
	// A couple checks
	// (File formats nowadays have soooo many weird details that they've accumulated over the years of change)
	if (info_header.biSize < 40)
	{
		return 1;
	}
	if (info_header.biCompression != 0)
	{
		return 1;
	}
	if (info_header.biBitCount != 24 && info_header.biBitCount != 32)
	{
		return 1;
	}
	
	// Quickly construct data for reading pixels
	int32_t width  = info_header.biWidth;
	int32_t height = std::abs(info_header.biHeight);
	bool flipped = info_header.biHeight < 0; // Funny case where the data is flipped upside down
	
	int bpp = info_header.biBitCount / 8; // Bits -> bytes
	int row_size = width * bpp;
	int padding = (4 - (row_size % 4)) % 4; // How many bytes of padding (useless bytes) there is per row
	
	// Pixel array
	uint64_t arr_size = width * height;
	Pixel* pixels = new Pixel[arr_size]; // Row-major
	
	// Set starting read position based on data offset
	if (!input.seekg(file_header.data_offset, std::ios::beg))
	{
		return 1;
	}
	
	// Retrieve pixels
	for (int y = 0; y < height; ++y) {
		// Starting pixel in the pixel array (plus considerations for flipped or not)
		uint64_t i = flipped ? ((uint64_t)(height - y - 1) * width) : ((uint64_t)y * width);
		
		// Read the row
		for (int x = 0; x < width; ++x) {
			// Here, the pixel is aligned to read directly, and if we only read 3 bits then the pixel's a is pre-set to 255
			if (!input.read(reinterpret_cast<char*>(&pixels[i + x]), bpp)) // Read fail?
			{
				return 1;
			}
		}
		
		// Padding
		if (padding > 0)
		{
			if (!input.ignore(padding)) // Read fail?
			{
				return 1;
			}
		}
	}
	
	// Output data
	
	// Define the condition for blacking out a pixel
	bool (*black_out)(int w, int h, int x, int y) = (height < 200 ? [](int w, int h, int x, int y) {
		// Condition for a height of 100
		// Outside circle in middle
		int r = std::min(w, h) / 2;
		int dx = x - w / 2;
		int dy = y - h / 2;
		return dx * dx + dy * dy > r * r;
		
	} : (height < 300 ? [](int w, int h, int x, int y) {
		// Condition for a height of 200
		// Inside circle in middle
		int r = std::min(w, h) / 2;
		int dx = x - w / 2;
		int dy = y - h / 2;
		return dx * dx + dy * dy <= r * r;
		
	} : (height < 400 ? [](int w, int h, int x, int y) {
		// Condition for a height of 300
		// Not in top right area
		return x < w / 2 || y < h / 2;
		
	} : (height < 500 ? [](int w, int h, int x, int y) {
		// Condition for a height of 400
		// Outside inner rectangle
		return x < w / 4 || x > w * 3 / 4 || y < h / 4 || y > h * 3 / 4;
		
	} : [](int w, int h, int x, int y) {
		// Condition for a height of 500
		// Outside a diagonal line
		int D = std::min(w, h) / 10;
		
		// sqrt(2)
		return std::abs(x - y) / 1.41421356237 > D / 2;
		
	}))));
	
	// Open output file
	std::ofstream output("output.bmp");
	
	// We just spit out a lot of previously read data
	if (!output.write(reinterpret_cast<char*>(&file_header), sizeof(BMPFileHeader))) // Write fail?
	{
		return 1;
	}
	
	if (flipped) // Unflip if flipped
	{
		info_header.biWidth = -info_header.biWidth;
	}
	
	// Spit out info header
	if (!output.write(reinterpret_cast<char*>(&info_header), sizeof(BMPInfoHeader))) // Write fail?
	{
		return 1;
	}
	
	// Zeros for padding, which is at most 3
	const char zeros[3] = {};
	
	// Black pixel
	Pixel black;
	
	for (int y = 0; y < height; ++y)
	{
		// Starting pixel in the array
		// Our data is already unflipped so no need to consider that
		uint64_t i = (uint64_t)y * width;
		
		// Write out row
		for (int x = 0; x < width; ++x)
		{
			// Pixel data already aligned so spit out directly
			// We also determine here whether to black out the pixel
			if (!output.write(reinterpret_cast<char*>(&(black_out(width, height, x, y) ? black : pixels[i + x])), bpp)) // Write fail?
			{
				return 1;
			}
		}
		
		// The padding is the same since we didn't change any of the settings
		if (padding > 0)
		{
			if (!output.write(zeros, padding)) // Write fail?
			{
				return 1;
			}
		}
	}
	
	// Clean data
	delete[] pixels;
	
	return 0;
}
