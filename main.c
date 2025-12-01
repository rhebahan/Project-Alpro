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
        int p;
    do {
        printf("\n=== MENU GUDANG ===\n");
        printf("1. Manajemen Barang\n");
        printf("2. Laporan Barang\n");
        printf("3. Tampilkan Semua Barang\n");
        printf("4. Cari Barang\n"); // <--- MENU BARU DITAMBAHKAN
        printf("5. Reset Gudang\n");
        printf("0. Kembali\n");
        printf("Pilih: "); scanf("%d", &p);
        switch(p) {
            case 1: menuManajemen(); break;
            case 2: menuLaporanBarang(); break;
            case 3: tampilGudang(); break;
            case 4: cariBarang(); break; // <--- PANGGIL FUNGSI
            case 5: resetGudang(); break;
        }
    } while (p != 0);
}


void cariBarang(){

}
void menuKasir(){
int p;
    do {
        printf("\n=== MENU KASIR ===\n");
        printf("1. Transaksi Penjualan\n");
        printf("2. Laporan Penjualan & Keuntungan\n");
        printf("0. Kembali\n");
        printf("Pilih: "); scanf("%d", &p);
        switch(p) {
            case 1: kasir(); break;
            case 2: laporanPenjualan(); break;
        }
    } while (p != 0);
}

// Sub-Menu Gudang
void menuManajemen(){
    int p;
    do {
        printf("\n--- MANAJEMEN BARANG ---\n");
        printf("1. Tambah Barang Baru\n");
        printf("2. Restok Barang Lama\n");
        printf("3. Hapus Barang\n");
        printf("0. Kembali\n");
        printf("Pilih: "); scanf("%d", &p);
        switch(p) {
            case 1: tambahBarang(); break;
            case 2: restokBarang(); break;
            case 3: hapusBarang(); break;
        }
    } while (p != 0);
}
void tambahBarang(){
char nama[50], expired[20];
    int stok, modal, harga;

    printf("\n--- TAMBAH BARANG BARU ---\n");
    printf("Nama barang : ");
    scanf(" %[^\n]", nama);

    for (int i = 0; i < jumlahBarang; i++) {
        if (strcmp(gudang[i].nama, nama) == 0) {
            printf("ERROR: Barang sudah ada! Gunakan menu 'Restok Barang'.\n");
            return;
        }
    }

    printf("Stok Awal   : "); scanf("%d", &stok);
    printf("Harga BELI  : "); scanf("%d", &modal);
    printf("Harga JUAL  : "); scanf("%d", &harga);
    printf("Expired (YYYY-MM-DD): "); scanf(" %[^\n]", expired);

    strcpy(gudang[jumlahBarang].nama, nama);
    gudang[jumlahBarang].stok = stok;
    gudang[jumlahBarang].modal = modal;
    gudang[jumlahBarang].harga = harga;
    strcpy(gudang[jumlahBarang].expired, expired);
    
    jumlahBarang++;
    saveGudang();
    printf("Barang baru berhasil ditambahkan!\n");
}

void restokBarang() {
    char nama[50];
    int tambahStok;

    printf("\n--- RESTOK BARANG ---\n");
    tampilGudang();
    printf("Nama barang yang mau direstok: ");
    scanf(" %[^\n]", nama);

    for (int i = 0; i < jumlahBarang; i++) {
        if (strcmp(gudang[i].nama, nama) == 0) {
            printf("Stok saat ini: %d\n", gudang[i].stok);
            printf("Jumlah tambah stok: ");
            scanf("%d", &tambahStok);

            gudang[i].stok += tambahStok;
            saveGudang();
            printf("Berhasil! Stok %s sekarang menjadi %d.\n", nama, gudang[i].stok);
            return;
        }
    }
    printf("Barang tidak ditemukan.\n");
}

void hapusBarang(){
    char nama[50];
    printf("\n--- HAPUS BARANG ---\n");
    printf("Nama barang yang dihapus: ");
    scanf(" %[^\n]", nama);

    for (int i = 0; i < jumlahBarang; i++) {
        if (strcmp(gudang[i].nama, nama) == 0) {
            for (int j = i; j < jumlahBarang - 1; j++)
                gudang[j] = gudang[j + 1];

            jumlahBarang--;
            saveGudang();
            printf("Barang berhasil dihapus!\n");
            return;
        }
    }
    printf("Barang tidak ditemukan.\n");
}

// Sub-Menu Laporan
void menuLaporanBarang(){
    int p;
    do {
        printf("\n--- LAPORAN GUDANG ---\n");
        printf("1. Laporan Stok Menipis\n");
        printf("2. Laporan Expired\n");
        printf("0. Kembali\n");
        printf("Pilih: "); scanf("%d", &p);
        switch(p) {
            case 1: laporanStokMenipis(); break;
            case 2: laporanExp(); break;
        }
    } while (p != 0);
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
    loadGudang();
    int p;
    do {
        printf("\n======= MENU UTAMA =======\n");
        printf("1. Gudang\n");
        printf("2. Kasir\n");
        printf("0. Keluar\n");
        printf("Pilih: "); scanf("%d", &p);
        switch(p) {
            case 1: menuGudang(); break;
            case 2: menuKasir(); break;
        }
    } while (p != 0);
    return 0;

}