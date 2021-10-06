#include<iostream>
#include<fstream>
#include<string.h>
using namespace std;

int N;
char na[100000][11] = {""};
char nb[100000][11] = {""};

char ca[100000][11] = {""};
char cb[100000][11] = {""};


void merge(int start, int end) {
    int mid = (start + end) / 2;

    if (start >= end) 
        return ;
    merge(start, mid);
    merge(mid + 1, end);
    int p1n,p2n, pbn; pbn = p1n = start;  p2n = mid + 1;
    int p1c, p2c, pbc; pbc = p1c = start;  p2c = mid + 1;


    while ((p1c <= mid) && (p2c  <= end)) {
        if (strcmp(ca[p1c], ca[p2c]) <= 0) {
            strcpy(cb[pbc++], ca[p1c++]);
            strcpy(nb[pbn++], na[p1n++]);
        } else {
            strcpy(cb[pbc++], ca[p2c++]);
            strcpy(nb[pbn++], na[p2n++]);
        }
    }

    while (p1c <= mid) {
        strcpy(cb[pbc++], ca[p1c++]);
        strcpy(nb[pbn++], na[p1n++]);
    }

    while (p2c <= end) {
        strcpy(cb[pbc++], ca[p2c++]);
        strcpy(nb[pbn++], na[p2n++]);
    }

    for (int i = start; i <= end; ++i) {
        strcpy(ca[i],cb[i]);
        strcpy(na[i],nb[i]);
    }
}


int main() {
    ifstream fin; fin.open("race.in");
    ofstream fout; fout.open("race.out");
    fin >> N;

    for (int i = 0; i < N; ++i) {
        fin >> ca[i] >> na[i];
    }

    merge(0, N - 1);

    char t[11] = "";
    for (int i = 0; i < N; ++i) {
        if (strcmp(ca[i], t)) 
            fout << "=== " << ca[i] << " ===" << "\n";
        strcpy(t, ca[i]);
        fout << na[i] << "\n";
    }
}