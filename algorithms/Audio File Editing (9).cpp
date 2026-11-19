/*
题目描述:
给定一个 WAV 文件 格式的语音数据，需要根据语音文件的 数据长度 mod 6 来自动执行一种特定的处理功能。
每个处理功能按照文件的样本数据长度（即采样点数的 mod 6 值）进行选择，功能具体如下：
len % 6	功能名称	说明
0	剪切	剪切区间：删除[1000, 3000] 的采样（含两端），其余前后拼接
1	中心削波	上阈值 = 800，下阈值 = –800，将位于该阈值范围间的值全部删除
2	强制静音	将区间 [2000, 4000] 的采样全部置为 0
3	片段求逆	将区间 [500, 2500] 的采样进行逆序，其余保持不变
4	截顶处理	上截顶 = 1000，下截顶 = –1000，超出部分截到阈值
5	加噪声	
为全段音频添加从 1000 开始的锯齿波，即从1000开始反复递减至0再递增回1000，数列示意如下：
1000, 999, 998, ..., 0, 1, 2, ..., 999, 1000, 999, ...
语音数据下标从0开始，所有区间均为闭区间。
所有操作都按固定的采样区间和规则进行。请根据输入的 WAV 文件，读取其数据，并根据文件的采样长度执行相应操作。

输入描述:
输入是一个 WAV 文件（input.wav），文件中包含 PCM 编码的音频数据（单声道）。
需要根据该文件的采样点总数 N 来判断使用哪种操作。

输出描述:
输出是一个处理后的 WAV 文件（output.wav），该文件也是 PCM 编码格式。

样例输入:
假设我们给定的 WAV 文件数据包含以下整数值（仅为示意，实际文件中是 PCM 编码的音频数据）：

文件：input.wav
采样数据： [1000, 1200, 1500, 800, -600, 700, 300, 100, 0, 50]

样例输出:
假设在处理后，生成了一个新的 WAV 文件 output.wav。

文件：output.wav
处理后的数据： [1000, 1000, 1000, 800, -600, 700, 300, 100, 0, 50]

注释:
len % 6 = 4 触发的是 截顶处理，则所有大于1000的数据将被截断至1000，小于-1000的数据将被截断至-1000。
wav文件格式可参照课程ppt L07-数据组织与处理（下）-V9 92-102页内容。
为保证生成的wav可读性，请注意文件头中的数据大小也需要修改。
文件头中size均以字节为单位，由于测试音频统一选用单声道16位编码，可默认采样点数量 = data_size / 2。
以下是一个典型的文件头结构：

struct WAVHeader {
char riff[4];        // "RIFF"
uint32_t size;       // file size - 8
char wave[4];        // "WAVE"
char fmt[4];         // "fmt "
uint32_t fmt_size;   // 16
uint16_t audio_fmt;  // 1 (PCM)
uint16_t channels;   // 1
uint32_t sample_rate;
uint32_t byte_rate;
uint16_t block_align;
uint16_t bits_per_sample; // 16
char data[4];        // "data"
uint32_t data_size;  // following data bytes
};

其中的uint32_t为4字节无符号整型，uint16_t为仅占用2字节的无符号整型，size单位均为字节，需要修改的项包括size及data_size，分别含义为去除riff、size外的块（riff块）的大小，及采样数据（data块）占据的大小。
riff块计算方式：hdr.size = 4 + (8 + hdr.fmt_size) + (8 + hdr.data_size)
hdr.data_size = 2 * num_samples
注意：本题较难，请尽早开始，有问题及时平台或微信询问。
*/

#include <iostream>
#include <fstream>
#include <cstdint>

// No need to pragma pack since it already contains types of 1 byte size
struct WAVHeader {
	char riff[4];        // "RIFF"
	uint32_t size;       // file size - 8
	char wave[4];        // "WAVE"
	char fmt[4];         // "fmt "
	uint32_t fmt_size;   // 16
	uint16_t audio_fmt;  // 1 (PCM)
	uint16_t channels;   // 1
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t block_align;
	uint16_t bits_per_sample; // 16
	char data[4];        // "data"
	uint32_t data_size;  // following data bytes
};

int main()
{
	// I'll probably start specifying ios::binary from now on since this time it's required
	std::ifstream in("input.wav", std::ios::binary);
	if (!in.is_open())
	{
		return 1;
	}
	
	// Get header data
	WAVHeader header;
	if (!in.read(reinterpret_cast<char*>(&header), sizeof(WAVHeader))) // Read fail?
	{
		return 1;
	}
	
	// Because wav files (audio files in general) are a headache to work with, we will be making a lot of assumptions based on the instructions
	// For example, we assume that all samples have 16 bits (a short)
	// So we read the data here using uint16_t
	uint32_t arr_size = header.data_size / 2; // 2 bytes (16 bits) per sample
	int16_t* data = new int16_t[arr_size];
	if (!in.read(reinterpret_cast<char*>(data), header.data_size)) // Read fail?
	{
		return 1;
	}
	
	// Close input
	in.close();
	
	switch (arr_size % 6) // Specified type
	{
	case 0:
	{
		// Cut off [1000, 3000] (inclusive)
		uint32_t chunk1 = std::min((uint32_t)1000, arr_size); // Chunk before 1000 (0-999)
		uint32_t chunk2 = 0; // Chunk after 3000 (3001-end)
		if (arr_size > 3001)
		{
			chunk2 += arr_size - 3001;
		}
		
		arr_size = chunk1 + chunk2; // Total size of new array
		int16_t* data2 = new int16_t[arr_size];
		for (uint32_t i = 0; i < chunk1; ++i) // Read first chunk
		{
			data2[i] = data[i];
		}
		for (uint32_t i = 0; i < chunk2; ++i) // Read second chunk (skipped if there is no second chunk)
		{
			data2[chunk1 + i] = data[3001 + i];
		}
		
		// Replace data
		delete[] data;
		data = data2;
		break;
	}
	case 1:
	{
		// Cut all samples that are within -800 to 800
		int16_t* data2 = new int16_t[arr_size]; // New array (use old size to not have to calculate new size)
		uint32_t j = 0;
		for (uint32_t i = 0; i < arr_size; ++i)
		{
			int16_t d = data[i];
			if (d < -800 || d > 800) // Within bounds?
			{
				data2[j] = d; // Put into new data
				++j;
			}
		}
		
		// Replace data
		delete[] data;
		data = data2;
		arr_size = j; // Set new array size
		break;
	}
	case 2:
	{
		// Silence within [2000, 4000] (inclusive)
		for (uint32_t i = 2000; i < std::min((uint32_t)4001, arr_size); ++i)
		{
			data[i] = 0; // Silence
		}
		break;
	}
	case 3:
	{
		// Flip data in between 500 and 2500
		if (arr_size <= 500) // Too short to flip?
		{
			break;
		}
		uint32_t i = 500; // Starting sample
		uint32_t j = std::min((uint32_t)2500, arr_size - 1); // Ending sample
		while (i < j)
		{
			// Flip until all flipped
			int16_t t = data[i];
			data[i] = data[j];
			data[j] = t;
			++i;
			--j;
		}
		break;
	}
	case 4:
	{
		// Cap samples at a maximum of 1000 and a minimum of -1000
		for (uint32_t i = 0; i < arr_size; ++i)
		{
			if (data[i] > 1000) // Maximum
			{
				data[i] = 1000;
			}
			else if(data[i] < -1000) // Minimum
			{
				data[i] = -1000;
			}
		}
		break;
	}
	case 5:
	{
		// Add value of 1000, 999, 998, ... , 2, 1, 0, 1, 2, ... , 998, 999, 1000, 999, ... starting from 0
		int c = 1000; // Starting add
		int d = -1; // Step by
		for (uint32_t i = 0; i < arr_size; ++i)
		{
			data[i] += c; // Add value
			c += d;
			if (c == 0 || c == 1000) // Flip when the bounds are reached
			{
				d = -d;
			}
		}
		break;
	}
	}
	
	// New header to output (we could technically modify the old header but nah)
	WAVHeader out_header = header;
	out_header.data_size = arr_size * 2; // New data size in case array size changed
	out_header.size = 4 + (8 + out_header.fmt_size) + (8 + out_header.data_size); // New total size in case data size changed
	
	// Open output file
	std::ofstream out("output.wav", std::ios::binary);
	if (!out)
	{
		return 1;
	}
	
	// Write header
	if (!out.write(reinterpret_cast<const char*>(&out_header), sizeof(out_header)))
	{
		return 1;
	}
	
	// Write data
	if (!out.write(reinterpret_cast<const char*>(data), out_header.data_size))
	{
		return 1;
	}
	
	// Clean out data
	delete[] data;
	
	// Close output
	out.close();
	
	return 0;
}
