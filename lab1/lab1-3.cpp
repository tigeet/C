
#include<iostream>
#include<fstream>
using namespace std;



int A[1000][1000];
int main() {

    ifstream fn;
    fn.open("turtle.in");
    ofstream ft;
    ft.open("turtle.out");

    int w, h;
    fn >> h >> w;


    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++){
            fn >> A[i][j];
        }
    }



    for (int i = h - 2; i >=0; i--) {
        A[i][0] = A[i + 1][0] + A[i][0];
    }

    for (int j = 1; j < w; j++) {
        A[h - 1][j] = A[h - 1][j-1] + A[h-1][j];
    }


    for (int j = 1; j < w; j++) {
        for (int i = h - 2; i >=0; i--) {
            A[i][j] = A[i][j] + max(A[i + 1][j], A[i][j - 1]);
        }
    }

    ft << A[0][w - 1];
    ft.close();
    return 0;
}