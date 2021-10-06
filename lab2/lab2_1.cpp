#include<iostream>
#include<fstream>
using namespace std;

int a[300001];
int b[300001];
int n;


void merge(int start, int end) {
    int mid = (start + end) / 2;

    if (start >= end) 
        return ;
    merge(start, mid);
    merge(mid + 1, end);
    int p1,p2, pb;
    pb = p1 = start;  p2 = mid + 1;



    while ((p1 <= mid) && (p2  <= end)) {
        if (a[p1] <= a[p2]) {
            b[pb++] =a[p1++];
        } else if (a[p1] > a[p2]) {
            b[pb++] = a[p2++];
        }
    }

    while (p1 <= mid) {
        b[pb++] = a[p1++];
    }

    while (p2 <= end) {
        b[pb++] = a[p2++];
    }

    for (int i = start; i <= end; ++i) {
        a[i] = b[i];
    }
}


int main() {
    ifstream fin; fin.open("sort.in");
    ofstream fout; fout.open("sort.out");

    fin >> n;

    for (int i = 0; i < n; ++i) 
        fin >> a[i]; 

    merge(0, n - 1);


    fout << a[0];
    for (int i = 1; i < n; ++i) {
        fout << " " << a[i];
    }

    fout.close();
    return 0;
}