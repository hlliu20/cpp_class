#include<stdio.h>
#include<stdlib.h>
#define QOI_IMPLEMENTATION
#include"qoi.h"

int main(){
    void * p;
    unsigned char * pixel;
    qoi_desc *desc;
    p = qoi_read("testcard.qoi", desc, 4);
    if(p != nullptr){
        pixel = (unsigned char *)p;
        for(int i=1;i<=20;++i){
            // cout << "(" << pixel[i*3] << "," << pixel[i*3+1];
            // cout << "," << pixel[i*3+2] << "," << pixel[i*3+3];
            // cout << "),";
            // if(i%5==0) cout << endl;
            printf("(%3d,%3d,%3d,%3d),",pixel[i*3],pixel[i*3+1],pixel[i*3+2],pixel[i*3+3]);
            if(i%5==0) putchar('\n');
        }
    } else {
        // cout << "wrong" << endl;
        printf("wrong!\n");
    }
    return 0;
}