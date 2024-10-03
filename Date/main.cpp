#include <iostream>
#include"Date.h"

using namespace std;

int main(){
    cout << "hello world!" << endl;
    Date d1,d2(2024,10,3);
    cout << d1 << endl << d2 << endl;
    cout << (d2-d1) << endl;
    Date d3(d1),d4;
    d4 = d2;
    cout << d3 << endl << d4 << endl;
    d3++;--d4;
    cout << d3 << endl << d4 << endl;
    d3 += 1000;d4 -= 1000;
    cout << d3 << endl << d4 << endl;
    cin >> d2;
    cout << d2 << endl;
    return 0;
}