#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100

// ==========================
// STRUCT
// ==========================
typedef struct {
    char nama[50];
    int stok;
    int modal; // Harga Beli
    int harga; // Harga Jual
    char expired[20]; // Format: YYYY-MM-DD
} Barang;

typedef struct {
    char nama[50];
    int jumlah;
    int subtotal;
    int keuntungan;
} ItemBeli;

// ==========================
// GLOBAL
// ==========================
Barang gudang[MAX];
int jumlahBarang = 0;

// ==========================
// PROTOTYPE
// ==========================
void saveGudang();
void loadGudang();
void resetGudang();

// Menu Utama
void menuGudang();
void menuKasir();

// Sub-Menu Gudang
void menuManajemen();
void tambahBarang();
void restokBarang();
void hapusBarang();

// Sub-Menu Laporan
void menuLaporanBarang();
void laporanStokMenipis();
void laporanExp();

// Helper Gudang
void tampilGudang();
void cariBarang();
void sortingExpired();
void getTanggalHariIni(char *buffer);

// Kasir & Laporan Penjualan
void kasir();
void laporanPenjualan();
void catatLaporanPenjualan(char *nama, int qty, int modal, int hargaJual);
void cetakStruk(ItemBeli list[], int n, int total);


void getTanggalHariIni(char *buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

void saveGudang() {
    FILE *f = fopen("gudang.dat", "wb");
    fwrite(&jumlahBarang, sizeof(int), 1, f);
    fwrite(gudang, sizeof(Barang), jumlahBarang, f);
    fclose(f);
}

void loadGudang() {
    FILE *f = fopen("gudang.dat", "rb");
    fread(&jumlahBarang, sizeof(int), 1, f);
    fread(gudang, sizeof(Barang), jumlahBarang, f);
    fclose(f);
}

void resetGudang(){

}

// Menu Utama
void menuGudang(){

}

void cariBarang(){

}
void menuKasir(){

}

// Sub-Menu Gudang
void menuManajemen(){

}
void tambahBarang(){

}
void restokBarang(){

}
void hapusBarang(){

}

// Sub-Menu Laporan
void menuLaporanBarang(){

}
void laporanStokMenipis(){

}
void laporanExp(){

}

// Kasir
void kasir(){

}
void laporanPenjualan(){

}
void cetakStruk(ItemBeli list[], int n, int total){

}



int main() {
    printf("coba dulu, bisa gak");
    printf("");
    return 0;

}