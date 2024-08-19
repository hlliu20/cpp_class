#include<iostream>
#include<fstream>
#include<cstring>
#include <iomanip>
#include <cstdint>
#include<vector>

using namespace std;

#define CHARS2U32(x) (x[0] | x[1] << 8 | x[2] << 16 | x[3] << 24)

// #define BASE 65521 /* largest prime smaller than 65536 */
//  /*
//  Update a running Adler-32 checksum with the bytes buf[0..len-1]
//  and return the updated checksum. The Adler-32 checksum should be
//  initialized to 1.
//  Usage example:
//  unsigned long adler = 1L;
//  while (read_buffer(buffer, length) != EOF) {
//  adler = update_adler32(adler, buffer, length);
//  }
//  if (adler != original_adler) error();
//  */
//  unsigned long update_adler32(unsigned long adler, unsigned char *buf, int len)
//  {
//     unsigned long s1 = adler & 0xffff;
//     unsigned long s2 = (adler >> 16) & 0xffff;
//     int n;
//     for (n = 0; n < len; n++) {
//         s1 = (s1 + buf[n]) % BASE;
//         s2 = (s2 + s1) % BASE;
//     }
//     return (s2 << 16) + s1;
//  }
//  /* Return the adler32 of the bytes buf[0..len-1] */
//  unsigned long adler32(unsigned char *buf, int len)
//  {
//     return update_adler32(1L, buf, len);
//  }

unsigned int nextU32(unsigned char * ch){
    unsigned int re=0;
    re |= (*((unsigned int *)(ch))) << 24;
    re |= (*((unsigned int *)(ch+1))) << 16;
    re |= (*((unsigned int *)(ch+2))) << 8;
    re |= (*((unsigned int *)(ch+3)));
    return re;
}

uint32_t mntohl(uint32_t x){
    x = (x << 16) | (x >> 16);
    x = ((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8);
    return x;
}

/// @brief crc32校验
/// @param data unsigned char * 指向数据起始位置的指针
/// @param dataLen int 数据的字节数
/// @param ori_crc unsigned char * 给出的crc
/// @param crcLen int 默认crc长度为4字节
/// @return bool 给出的crc和算出的crc是否一致
bool crc32(unsigned char * data, int dataLen, unsigned char * ori_crc, int crcLen=4){
    unsigned long crc_table[256];
    unsigned long c=0,o=0;
    int n, k;
    for (n = 0; n < 256; n++) {
        c = (unsigned long) n;
        for (k = 0; k < 8; k++) {
            if (c & 1) {
                c = 0xedb88320L ^ (c >> 1);
            } else {
                c = c >> 1;
            }
        }
        crc_table[n] = c;
    }
    c = 0xffffffffL;
    for (n = 0; n < dataLen; n++) {
        c = crc_table[(c ^ data[n]) & 0xff] ^ (c >> 8);
    }
    c = c ^ 0xffffffffL;
    // std::cout << "crc=0x" << std::setfill('0') << std::setw(8) << std::hex << c << std::endl;
    o |= ((unsigned long)(*(ori_crc)) & 0xff) << 24;
    o |= ((unsigned long)(*(ori_crc+1)) & 0xff) << 16;
    o |= ((unsigned long)(*(ori_crc+2)) & 0xff) << 8;
    o |= (unsigned long)(*(ori_crc+3)) & 0xff;
    return c == o;
}
/// @brief chunk类
class chunk{
public:
    char cName[5];
    unsigned int start;
    unsigned int len;
    /// @brief 构造函数
    /// @param name char * chunk块的“符号”
    /// @param s unsigned int 块起始位置
    /// @param l unsigned int 块内容的长度
    chunk(char * name,unsigned int s, unsigned int l){
        cName[0]=name[0]; cName[1]=name[1];
        cName[2]=name[2]; cName[3]=name[3];
        cName[4]='\0';
        start = s; len = l;
    }
};

struct IHDR{
    uint32_t width;
    uint32_t height;
    uint8_t bitDepth;
    uint8_t colorType;
    uint8_t compressionMethod;
    uint8_t filterMethod;
    uint8_t interlaceMethod;
};


/// @brief png的文件类
class PNGFILE{
public:
    unsigned char * byte;
    bool isReadPngFile = false;
    unsigned int fileLen=0;
    vector<chunk> v;
    IHDR ihdr;
    unsigned long IDATlen=0;
    bool needMergeIDAT = false;
    unsigned char * IDATs;
    /// @brief 构造函数
    /// @param filePath 文件的路径/文件名 
    PNGFILE(string filePath){
        std::ifstream fs;
        fs.open(filePath, ios_base::binary|ios::ate);
        if(!fs)
        {
            std::cerr << "无法打开文件" << std::endl;
            isReadPngFile = false;
            return;
        }
        fileLen = fs.tellg();
        byte = new unsigned char[fileLen+1];
        fs.seekg(0, ios::beg);
        fs.read((char *)byte, fileLen);
        fs.close();
        isReadPngFile = isPngFile();
        if(isReadPngFile){
            analyze();
        }
    }
    void mergeIDAT(){
        IDATs = new unsigned char[IDATlen+1];
        unsigned int i=0;
        for(chunk c:v){
            if(c.cName[0]=='I' && c.cName[1]=='D' && c.cName[2]=='A' && c.cName[3]=='T'){
                memcpy(IDATs+i,byte+c.start+4+4,c.len);
                i += c.len;
            }
        }
    }
    void showIDATs(int len=-1){
        unsigned int nn = 0;
        nn = *((unsigned int *)(IDATs)) & 0xff;
        if(len==-1){
            for(int i=1;i<=160 && i<=IDATlen;++i){
                nn = *((unsigned int *)(IDATs+i-1)) & 0xff;
                std::cout << std::setfill('0') << std::setw(2) << std::hex << nn << " ";
                if(i%16==0) std::cout << std::endl;
            }
        } else {
            for(int i=1;i<=len && i<=IDATlen;++i){
                nn = *((unsigned int *)(IDATs+i-1)) & 0xff;
                std::cout << std::setfill('0') << std::setw(2) << std::hex << nn << " ";
                if(i%16==0) std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }
    /// @brief 将文件中从某字节开始的向后四字节转为unsigned long
    /// @param n 开始字节到文件开头的长度
    /// @return unsigned int
    unsigned int nextU32(unsigned int n){
        unsigned int re=0;
        re |= uIntAt(n) << 24;
        re |= uIntAt(n+1) << 16;
        re |= uIntAt(n+2) << 8;
        re |= uIntAt(n+3);
        return re;
    }
    /// @brief 对PNG文件分析，将各个chunk找出来并保存到vector中
    void analyze(){
        int i=8;
        unsigned int len;
        std::vector<int> vi;
        while(i<fileLen){
            len = nextU32(i);
            v.push_back(chunk((char *)(byte+i+4),i,len));
            if(nextU32(i+4) == 0x49484452){
                memcpy(&ihdr, (const void *)(byte+i+8), len);
                ihdr.width = mntohl(ihdr.width);
                ihdr.height = mntohl(ihdr.height);
            }
            else if(nextU32(i+4) == 0x49444154) {
                vi.push_back(i+8);
                IDATlen += len;
                IDATs = byte + 8 + i; 
            }
            else if(nextU32(i+4) == 0x49454E44) break;
            i += 4+4+len+4;
        }
        if(vi.size()>1) mergeIDAT();
    }
    void showIHDR(){
        std::cout << "width=" << (int)ihdr.width << "\nheight=" << (int)ihdr.height;
        std::cout << "\nbit depyh=" << (int)ihdr.bitDepth << "\ncolor type=";
        std::cout << (int)ihdr.colorType << "\ncompression method=" << (int)ihdr.compressionMethod;
        std::cout << "\nfilter method=" << (int)ihdr.filterMethod << "\n interlace method=";
        std::cout << (int)ihdr.interlaceMethod << std::endl;
    }
    /// @brief 简单输出一下都有什么chunk和每个chunk的起始位置及内容的长度
    void showChunks(){
        cout << "chunk:  start   :len\n";
        for(chunk c:v){
            cout << c.cName << " :" << setw(10) << c.start << ":" << c.len << endl;
        }
    }
    /// @brief 返回指向文件第n字节保存位置的指针
    /// @param n unsigned int 到文件开头的字节数
    /// @return unsigned char *
    unsigned char * ptrAt(unsigned int n){
        if(n < fileLen)return byte+n;
        else return nullptr;
    }
    /// @brief 返回文件第n字节转成的unsigned int
    /// @param n unsigned int 到文件开头的字节数
    /// @return unsigned int
    unsigned int uIntAt(unsigned int n){
        if(n < fileLen)return (unsigned int)(*(byte+n)) & 0xff;
        else return 0;
    }
    /// @brief 输出文件第n字节的16进制表示
    /// @param n unsigned int 到文件开头的字节数
    void printHexAt(unsigned int n){
        unsigned int nn = (unsigned int)(*(byte+n)) & 0xff;
        std::cout << std::setfill('0') << std::setw(2) << std::hex << nn;
    }
    /// @brief 判断这个文件是不是PNG文件
    /// @return bool 是否为PNG文件
    bool isPngFile(){
        return (uIntAt(0)==0x89)&(uIntAt(1)==0x50)&(uIntAt(2)==0x4e)&(uIntAt(3)==0x47)&(uIntAt(4)==0x0d)&(uIntAt(5)==0x0a)&(uIntAt(6)==0x1a)&(uIntAt(7)==0x0a);
    }
    /// @brief 如果为png文件，返回图片的宽
    /// @return unsigned int
    unsigned int getPngwidth(){
        unsigned int n=0;
        n |= uIntAt(19);
        n |= uIntAt(18)<<8;
        n |= uIntAt(17)<<16;
        n |= uIntAt(16)<<24;
        return n;
    }
    /// @brief 如果为png文件，返回图片的高
    /// @return unsigned int
    unsigned int getPngHeight(){
        unsigned int n=0;
        n |= uIntAt(23);
        n |= uIntAt(22)<<8;
        n |= uIntAt(21)<<16;
        n |= uIntAt(20)<<24;
        return n;
    }
    /// @brief 检测Png文件的IHDR块的crc对不对
    /// @return bool
    bool IHDRcrc(){return crc32(byte+12,17,byte+29);}
    ~PNGFILE(){
        delete [] byte;
        if(needMergeIDAT) delete[] IDATs;
    }
};


int main(int argc, const char * argv[]){
    PNGFILE mf(argv[1]);
    if(mf.isReadPngFile){
        cout << argv[1] << " is a png file\n";
        mf.showIHDR();
        mf.showChunks();
        mf.showIDATs();
    } else {
        cout << "file:" << argv[1] << " open failed!\n";
    }
    return 0;
}


