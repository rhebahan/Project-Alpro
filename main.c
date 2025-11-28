#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// struct untuk gudang

typedef struct{
    char nama[50];
    int stok;
    int harga;
    char expired[20];
} barang;

// global function

barang gudang[MAX];
int jumlahBarang = 0;

// pakai function prototype (isinya di bawah)

void saveGudang();
void loadGudang();
void menuGudang();

// fitur untuk pilihan gudang

void tambahBarang();
void hapusBarang();
void tampilBarang();
void cariBarang();
void resetGudang();

void sortingStok();
void sortingExpired();

void saveGudang() {
    FILE *f = fopen("gudang.dat", "wb");
    fwrite(&jumlahBarang, sizeof(int), 1, f);
    fwrite(gudang, sizeof(barang), jumlahBarang, f);
    fclose(f);
}

void loadGudang() {
    FILE *f = fopen("gudang.dat", "rb");
    fread(&jumlahBarang, sizeof(int), 1, f);
    fread(gudang, sizeof(barang), jumlahBarang, f);
    fclose(f);
}







int main() {
    printf("coba dulu, bisa gak");
    printf("");
    return 0;
}