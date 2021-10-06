#include<iostream>
#include<fstream>
#include<string.h>
using namespace std;

int n, k, A, B, C;
int a[30000001];

void qsort(int left, int right) {
    //cout << left << " " << right << "\n";
    int key = a[(left + right) / 2];
    int i = left;
    int j = right;


    while (i < j) {
        for (int m = 0; m < n; m++) 
            cout << a[m] << " ";
        cout << "\n";    
        while (a[i] < key){
            ++i;
        }
        while (a[j] > key){
            --j;
        }
        if (i <= j) {
            swap(a[i++], a[j--]);
        }  
    }

    //if ((left < j) && (left <= k) && (k <= j)) qsort(left, j);
    //if ((i < right) && (k <= right) && (k >= i)) qsort(i, right);
    if (left < j)  qsort(left, j);
    if (i < right) qsort(i, right);
}



int main() {
    ifstream fin; fin.open("i.txt");
    ofstream fout; fout.open("o.txt");
    fin >> n >> k;
    fin >> A >> B >> C >> a[0] >> a[1];

    for (int i = 2; i < n; ++i) {
        a[i] = A*a[i - 2] + B*a[i - 1] + C;
    }

    // for (int i = 0; i < n; ++i) {
    //     cout << a[i] << " "; 
    // }
    // cout << "\n";
    qsort(0, n - 1);
    cout << a[k - 1];
    return 0;
}
