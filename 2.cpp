#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <execution>
std::string file_to_string(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string contents(size, '\0');
    if (!file.read(&contents[0], size)) {
        std::cerr << "Failed to read file: " << filename << std::endl;
        return "";
    }

    return contents;
}

void string_to_file(const std::string& filename, const std::string& content) {
    std::ofstream file(filename, std::ios::binary | std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    file.rdbuf()->pubsetbuf(0, 4096);
    file.write(content.data(), content.size());
}


void 处理文本(std::string& 源文件, std::vector<unsigned long long>& 存储目标) {
    unsigned long long 当前位数 = 0;
    unsigned long long n = 0;
    char 当前字符;
    //将源文件转化为字节流，4bit一位十进制数字
    for (size_t i = 0; i < 源文件.size(); i++)
    {
        当前字符 = 源文件[i];
        switch (当前字符)
        {
        case '\n':
            n = (n << 4) ^ (当前位数);
            存储目标.push_back(n);
            n = 0;
            当前位数 = 0;
            break;
        case '0':
            n = (n << 4);
            ++当前位数;
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            n = (n << 4) ^ (当前字符 - 48);
            ++当前位数;
            break;
        default:
            break;
        }
    }
    if (当前位数 != 0)
    {
        n = (n << 4) ^ (当前位数);
        存储目标.push_back(n);
    }
}

void 输出字符串(std::vector<unsigned long long>& 数组, std::string& 目标字符串) {
    auto data = 目标字符串.data();
    for (auto i : 数组)
    {
        switch (i & 15)
        {
        case 10:
            目标字符串.push_back(((i >> 40) & 15) + 48);
        case 9:
            目标字符串.push_back(((i >> 36) & 15) + 48);
        case 8:
            目标字符串.push_back(((i >> 32) & 15) + 48);
        case 7:
            目标字符串.push_back(((i >> 28) & 15) + 48);
        case 6:
            目标字符串.push_back(((i >> 24) & 15) + 48);
        case 5:
            目标字符串.push_back(((i >> 20) & 15) + 48);
        case 4:
            目标字符串.push_back(((i >> 16) & 15) + 48);
        case 3:
            目标字符串.push_back(((i >> 12) & 15) + 48);
        case 2:
            目标字符串.push_back(((i >> 8) & 15) + 48);
        case 1:
            目标字符串.push_back(((i >> 4) & 15) + 48);
            目标字符串.push_back('\n');
        default:
            break;
        }
    }
}

#define 是否显示信息 true
int main() {
    std::vector<unsigned long long> 数组;
    std::string 文件名;
    std::cout << "输入文件名或完整路径：";
    std::cin >> 文件名;
    auto 开始时间 = std::chrono::high_resolution_clock::now();
    auto 开始时间2 = std::chrono::high_resolution_clock::now();


    //读取文件
    auto 源文件 = file_to_string(文件名);
#if 是否显示信息
    std::cout << "文本读取完毕\n";
    std::cout << "消耗用时:" << (std::chrono::high_resolution_clock::now() - 开始时间2) << "\n";
    开始时间2 = std::chrono::high_resolution_clock::now();
#endif
    数组.reserve(源文件.size() / 4);


    //输入格式化
    处理文本(源文件, 数组);
#if 是否显示信息
    std::cout << "文本处理完毕\n";
    std::cout << "消耗用时:" << (std::chrono::high_resolution_clock::now()- 开始时间2) << "\n";
    开始时间2 = std::chrono::high_resolution_clock::now();
#endif


    // 升序排序 多线程
    std::sort(std::execution::par, 数组.begin(), 数组.end());
#if 是否显示信息
    std::cout << "数据排序完毕\n";
    std::cout << "消耗用时:" << (std::chrono::high_resolution_clock::now() - 开始时间2) << "\n";
    开始时间2 = std::chrono::high_resolution_clock::now();
#endif


    //输出格式化
    源文件.clear();
    输出字符串(数组, 源文件);
#if 是否显示信息
    std::cout << "输出文本处理完毕\n";
    std::cout << "消耗用时:" << (std::chrono::high_resolution_clock::now() - 开始时间2) << "\n";
    开始时间2 = std::chrono::high_resolution_clock::now();
#endif

    //写入文件
    string_to_file("sort.txt", 源文件);
#if 是否显示信息
    std::cout << "写入完毕\n";
    std::cout << "消耗用时:" << (std::chrono::high_resolution_clock::now() - 开始时间2) << "\n";
    开始时间2 = std::chrono::high_resolution_clock::now();
#endif
    auto 结束时间 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> 消耗时间 = 结束时间 - 开始时间;
    std::cout << "消耗用时:" << 消耗时间.count() << "s\n";

    std::cout << "输入内容回车结束";
    std::cin >> 文件名;
    return 0;
}