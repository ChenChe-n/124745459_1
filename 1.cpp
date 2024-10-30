#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>

std::string file_to_string(const std::string& filename) {
    auto _filename = filename;
    std::ifstream file(_filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << _filename << std::endl;
        return "";
    }

    std::ostringstream contents;
    contents << file.rdbuf(); // 将文件内容读入字符串流
    return contents.str(); // 返回字符串
}

void string_to_file(const std::string& filename, const std::string& content) {
    auto _filename = filename;
    std::ofstream file(_filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << _filename << std::endl;
        return;
    }

    file << content; // 写入内容
    file.close(); // 关闭文件
}

void 处理文本(std::string& 源文件, std::vector<unsigned long long>& 存储目标) {
    unsigned long long 当前位数 = 4;
    unsigned long long n = 0;
    char cr;
    while (!源文件.empty())
    {
        cr = 源文件.back();
        源文件.pop_back();
        switch (cr)
        {
        case '\n':
            if (当前位数 != 4)
            {
                n += 当前位数>>2;
                当前位数 = 4;
                存储目标.push_back(n);
                n = 0;
            }
            break;
        case '0':
            当前位数 += 4;
            break;
        case '1':
            n += (1ll << 当前位数);
            当前位数 += 4;
            break;
        case '2':
            n += (2ll << 当前位数);
            当前位数 += 4;
            break;
        case '3':
            n += (3ll << 当前位数);
            当前位数 += 4;
            break;
        case '4':
            n += (4ll << 当前位数);
            当前位数 += 4;
            break;
        case '5':
            n += (5ll << 当前位数);
            当前位数 += 4;
            break;
        case '6':
            n += (6ll << 当前位数);
            当前位数 += 4;
            break;
        case '7':
            n += (7ll << 当前位数);
            当前位数 += 4;
            break;
        case '8':
            n += (8ll << 当前位数);
            当前位数 += 4;
            break;
        case '9':
            n += (9ll << 当前位数);
            当前位数 += 4;
            break;
        default:
            break;
        }
    }
    //尾处理
    if (当前位数 != 4)
    {
        n += 当前位数 / 4;
        存储目标.push_back(n);
    }
}

void 输出字符串(std::vector<unsigned long long>& 数组, std::string& 目标字符串) {
    auto data = 目标字符串.data();
    for (auto i : 数组)
    {
        switch (i & 15)
        {
        case 12:
            目标字符串.push_back(((i >> 44) & 15) + 48);
        case 11:
            目标字符串.push_back(((i >> 40) & 15) + 48);
        case 10:
            目标字符串.push_back(((i >> 36) & 15) + 48);
        case 9:
            目标字符串.push_back(((i >> 32) & 15) + 48);
        case 8:
            目标字符串.push_back(((i >> 28) & 15) + 48);
        case 7:
            目标字符串.push_back(((i >> 24) & 15) + 48);
        case 6:
            目标字符串.push_back(((i >> 20) & 15) + 48);
        case 5:
            目标字符串.push_back(((i >> 16) & 15) + 48);
        case 4:
            目标字符串.push_back(((i >> 12) & 15) + 48);
        case 3:
            目标字符串.push_back(((i >> 8) & 15) + 48);
        case 2:
            目标字符串.push_back(((i >> 4) & 15) + 48);
        }
        目标字符串.push_back('\n');
    }
}
int main() {
    //读取文件
    std::vector<unsigned long long> 数组;
    std::string 文件名;
    std::cout << "输入文件名或完整路径：";
    std::cin >> 文件名;
    auto 开始时间 = std::chrono::high_resolution_clock::now();
    auto 源文件 = file_to_string(文件名);
    数组.reserve(源文件.size() / 4);

    //处理字符串
    处理文本(源文件, 数组);
    //std::cout << "文本处理完毕\n";

    // 升序排序
    std::sort(数组.begin(), 数组.end());
    ///std::cout << "数据排序完毕\n";

    //输出
    源文件.clear();
    输出字符串(数组, 源文件);
    //std::cout << "输出文本处理完毕\n";
    string_to_file("sort.txt", 源文件);
    // std::cout << "写入完毕\n";
    auto 结束时间 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> 消耗时间 = 结束时间-开始时间;
    std::cout<<"消耗用时:"<<消耗时间.count()<<"s\n";

    std::cout << "输入内容回车结束";
    std::cin >> 文件名;
    return 0;
}