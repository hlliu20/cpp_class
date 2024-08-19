#include<iostream>
#include<fstream>
#include"qoi_cpp.hpp"

using namespace std;

int main(int argc, const char * argv[]){
    unsigned char * pixels;
    qoi_desc * desc;
    pixels = qoi_read(argv[1], desc);
    if(pixels == nullptr){
        cerr<< "file:" << argv[1] <<" open falid" << endl;;
        exit(-1);
    }
    for(int i=1;i<=20;++i){
        if(desc->channels==3){
            printf("(%3d,%3d,%3d),",pixels[i*3],pixels[i*3+1],pixels[i*3+2]);
            if(i%5==0) putchar('\n');
        } else if(desc->channels==4){
            printf("(%3d,%3d,%3d,%3d),",pixels[i*4],pixels[i*4+1],pixels[i*4+2],pixels[i*4+3]);
            if(i%5==0) putchar('\n');
        } else {
            cout << "?";
            if(i%5==0) cout << endl;
        }
    }

    if(argc >= 3){
        int flag = qoi_write(argv[2], pixels, desc);
        if(flag >= 0) cout << "写入成功" << endl;
    }

    delete[] pixels;
    return 0;
}