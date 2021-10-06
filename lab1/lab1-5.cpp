
#include<iostream>
#include<fstream>
using namespace std;


float A[10000];
int B[10000];

int main() {
    ifstream fn;
    fn.open("sortland.in");
    ofstream ft;
    ft.open("sortland.out");

    int n; 
    fn >> n;

    fn >> A[0];
    for (int i = 0; i < n; i++) {
        B[i] = i + 1;
    }
    for (int i = 1; i < n; i++) {
        fn >> A[i];
        for (int j = i; (A[j] < A[j-1]) & (j > 0); j--) {
            swap(A[j], A[j - 1]);
            swap(B[j], B[j - 1]);
        }
    }
    
    
    ft << B[0] << " " << B[n / 2] << " " << B[n - 1];
    ft.close();
    ft << n;
    return 0;
}