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

// sistem
void saveGudang();
void loadGudang();
void resetGudang();

// Menu Utama
void menuGudang();
void menuKasir();

// Sub-Menu Gudang
void menuManajemen();
void menuLaporanBarang();
void menuSorting(); // <--- MENU BARU

// Fungsi Operasional Gudang
void tambahBarang();
void restokBarang();
void hapusBarang();
void cariBarang();
void tampilGudang();

// Fungsi Sorting
void urutkanStok();  
void urutkanExpired();

// Laporan
void laporanStokMenipis();
void laporanExp();

// Kasir
void kasir();
void laporanPenjualan();
void catatLaporanPenjualan(char *nama, int qty, int modal, int hargaJual);
void cetakStruk(ItemBeli list[], int n, int total);

// Utils
void getTanggalHariIni(char *buffer);

void getTanggalHariIni(char *buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

void saveGudang() {
    FILE *f = fopen("gudang.txt", "w");
    fprintf(f, "%d\n", jumlahBarang);
    for(int i=0; i<jumlahBarang; i++){
        fprintf(f, "%s|%d|%d|%d|%s\n",
            gudang[i].nama,
            gudang[i].stok,
            gudang[i].modal,
            gudang[i].harga,
            gudang[i].expired
        );
    }
    fclose(f);
}


void loadGudang() {
    FILE *f = fopen("gudang.txt", "r");
    if (!f) return; 
    fscanf(f, "%d\n", &jumlahBarang);
    for(int i=0; i<jumlahBarang; i++){
        fscanf(f, "%[^|]|%d|%d|%d|%[^\n]\n",
            gudang[i].nama,
            &gudang[i].stok,
            &gudang[i].modal,
            &gudang[i].harga,
            gudang[i].expired
        );
    }
    fclose(f);
}

void resetGudang(){
    char yakin;
    printf("\n!!! PERINGATAN: HAPUS SEMUA DATA !!!\n");
    printf("Yakin? (y/n): ");
    scanf(" %c", &yakin);
    if (yakin == 'y' || yakin == 'Y') {
        jumlahBarang = 0;
        saveGudang();
        remove("riwayat_penjualan.txt");
        printf("Gudang berhasil di-reset.\n");
    }
}

// Menu Utama
void menuGudang(){
        int p;
    do {
        printf("\n=== MENU GUDANG ===\n");
        printf("1. Manajemen Barang\n");
        printf("2. Laporan Barang\n");
        printf("3. Tampilkan Semua Barang\n");
        printf("4. Cari Barang\n"); 
        printf("5. Reset Gudang\n");
        printf("0. Kembali\n");
        printf("Pilih: "); scanf("%d", &p);
        switch(p) {
            case 1: menuManajemen(); break;
            case 2: menuLaporanBarang(); break;
            case 3: tampilGudang(); break;
            case 4: cariBarang(); break; 
            case 5: resetGudang(); break;
        }
    } while (p != 0);
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

void menuSorting(){
    int p;
    do{
        printf("\n--- MENU SORTING ---\n");
        printf("1. Sorting Menurut Stok\n");
        printf("2. Sorting Menurut Expired\n");
        printf("0. Kembali\n");
        printf("pilih: ");
        scanf("%d", &p);
        switch(p) {
            case 1: urutkanStok();
                printf("\nBerhasil diurutkan berdasarkan STOK (Sedikit -> Banyak).\n");
                tampilGudang();
            break;
            case 2: urutkanExpired(); 
                printf("\nBerhasil diurutkan berdasarkan EXPIRED (Terdekat -> Terlama).\n");
                tampilGudang();
            break;
        }

    } while(p != 0);
}

void cariBarang(){
    char nama[50];
    printf("\n--- PENCARIAN BARANG ---\n");
    printf("Masukkan Nama Barang: ");
    scanf(" %[^\n]", nama);

    int ditemukan = 0;

    printf("\nHasil Pencarian:\n");
    // Header Tabel Kecil
    printf("%-15s | %-5s | %-8s | %-8s | %-12s\n", "Nama", "Stok", "Modal", "Jual", "Expired");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < jumlahBarang; i++) {
        if (strcmp(gudang[i].nama, nama) == 0) {
            printf("%-15s | %-5d | %-8d | %-8d | %-12s\n", 
                   gudang[i].nama, 
                   gudang[i].stok, 
                   gudang[i].modal, 
                   gudang[i].harga,
                   gudang[i].expired);
            ditemukan = 1;
            break;
        }
    }
    printf("-------------------------------------------------------------\n");

    if (!ditemukan) {
        printf("Barang '%s' tidak ditemukan di gudang.\n", nama);
    }
}
void restokBarang() {
    char nama[50];
    int tambahStok;
    printf("\n--- RESTOK BARANG ---\n");
    tampilGudang();
    printf("Nama barang: "); scanf(" %[^\n]", nama);

    for (int i = 0; i < jumlahBarang; i++) {
        if (strcmp(gudang[i].nama, nama) == 0) {
            printf("Stok saat ini: %d. Tambah: ", gudang[i].stok);
            scanf("%d", &tambahStok);
            gudang[i].stok += tambahStok;
            saveGudang();
            printf("Stok berhasil ditambah.\n");
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

void tampilGudang() {
    if (jumlahBarang == 0) {
        printf("\nGudang Kosong.\n"); return;
    }
    printf("\n%-3s | %-15s | %-5s | %-8s | %-8s | %-12s\n", 
           "No", "Nama", "Stok", "Modal", "Jual", "Expired");
    printf("-------------------------------------------------------------------\n");
    
    for (int i = 0; i < jumlahBarang; i++) {
        printf("%-3d | %-15s | %-5d | %-8d | %-8d | %-12s\n",
               i + 1, 
               gudang[i].nama, 
               gudang[i].stok, 
               gudang[i].modal, 
               gudang[i].harga,
               gudang[i].expired);
    }
    printf("-------------------------------------------------------------------\n");
}

void tambahBarang() {
    char nama[50], expired[20];
    int stok, modal, harga;

    printf("\n--- TAMBAH BARANG BARU ---\n");
    printf("Nama barang : "); scanf(" %[^\n]", nama);

    for (int i = 0; i < jumlahBarang; i++) {
        if (strcmp(gudang[i].nama, nama) == 0) {
            printf("ERROR: Barang sudah ada! Gunakan menu 'Restok Barang'.\n"); return;
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


// SORTING

void urutkanStok() {
    if (jumlahBarang < 2) {
        printf("Data kurang untuk diurutkan.\n");
        return;
    }

    // Bubble Sort Ascending (Kecil -> Besar)
    for (int i = 0; i < jumlahBarang - 1; i++) {
        for (int j = 0; j < jumlahBarang - i - 1; j++) {
            if (gudang[j].stok > gudang[j + 1].stok) {
                Barang temp = gudang[j];
                gudang[j] = gudang[j + 1];
                gudang[j + 1] = temp;
            }
        }
    }
    saveGudang();
}

void urutkanExpired() {
    if (jumlahBarang < 2) {
        printf("Data kurang untuk diurutkan.\n");
        return;
    }

    // Bubble Sort Ascending (Tanggal Dulu -> Tanggal Nanti)
    // Menggunakan strcmp: "2024-01-01" < "2024-05-05"
    for (int i = 0; i < jumlahBarang - 1; i++) {
        for (int j = 0; j < jumlahBarang - i - 1; j++) {
            if (strcmp(gudang[j].expired, gudang[j + 1].expired) > 0) {
                Barang temp = gudang[j];
                gudang[j] = gudang[j + 1];
                gudang[j + 1] = temp;
            }
        }
    }
    saveGudang(); 
}

// Sub-Menu Laporan
void menuLaporanBarang(){
    int p;
    do {
        printf("\n--- LAPORAN BARANG ---\n");
        printf("1. Laporan Stok Menipis\n");
        printf("2. Laporan Expired\n");
        printf("3. Sorting Barang\n");
        printf("0. Kembali\n");
        printf("Pilih: "); scanf("%d", &p);
        switch(p) {
            case 1: laporanStokMenipis(); break;
            case 2: laporanExp(); break;
            case 3: menuSorting(); break; 
        }
    } while (p != 0);
}
void laporanStokMenipis(){
    printf("\n--- LAPORAN STOK MENIPIS (< 5) ---\n");
    int count = 0;
    printf("%-20s | %-6s\n", "Nama Barang", "Sisa");
    for (int i = 0; i < jumlahBarang; i++) {
        if (gudang[i].stok < 5) {
            printf("%-20s | %-6d\n", gudang[i].nama, gudang[i].stok);
            count++;
        }
    }
    if (count == 0) printf("Aman. Tidak ada stok menipis.\n");
}
void laporanExp(){
    char today[20];
    getTanggalHariIni(today);
    urutkanExpired(); 

    printf("\n--- STATUS EXPIRED (Hari ini: %s) ---\n", today);
    printf("%-20s | %-12s | %s\n", "Nama Barang", "Expired", "Status");
    printf("----------------------------------------------------\n");
    
    for (int i = 0; i < jumlahBarang; i++) {
        char status[20] = "Aman";
        if (strcmp(gudang[i].expired, today) < 0) strcpy(status, "BASI!");
        else if (strncmp(gudang[i].expired, today, 7) == 0) strcpy(status, "Bulan Ini!");
        printf("%-20s | %-12s | %s\n", gudang[i].nama, gudang[i].expired, status);
    }
}

// Kasir
void kasir(){
    int jumlahItem = 0; ItemBeli list[50]; int totalBayar = 0;
    printf("\n=== TRANSAKSI KASIR ===\n");
    tampilGudang();
    while (1) {
        char nama[50]; int qty;
        printf("\nNama barang (ketik 'selesai'): "); scanf(" %[^\n]", nama);
        if (strcmp(nama, "selesai") == 0) break;
        int idx = -1;
        for (int i = 0; i < jumlahBarang; i++) {
            if (strcmp(gudang[i].nama, nama) == 0) { 
                idx = i; 
                break; }
        }
        if (idx == -1) { 
            printf("Barang tidak ada.\n"); 
            continue; }
        
        printf("Jumlah beli: "); 
        scanf("%d", &qty);
        
        if (qty > gudang[idx].stok) { 
            printf("Stok kurang!\n"); 
            continue; }

        gudang[idx].stok -= qty;
        strcpy(list[jumlahItem].nama, nama);
        list[jumlahItem].jumlah = qty;
        list[jumlahItem].subtotal = gudang[idx].harga * qty;
        catatLaporanPenjualan(nama, qty, gudang[idx].modal, gudang[idx].harga);
        totalBayar += list[jumlahItem].subtotal;
        jumlahItem++;
        saveGudang();
        printf(">> %s berhasil diinput.\n", nama);
    }
    if (jumlahItem > 0) {
        cetakStruk(list, jumlahItem, totalBayar);
        printf("Total: Rp %d\n", totalBayar);
    }
}

void catatLaporanPenjualan(char *nama, int qty, int modal, int hargaJual) {
    FILE *f = fopen("riwayat_penjualan.txt", "a");
    if (!f) return;
    int profit = (hargaJual - modal) * qty;
    int omset = hargaJual * qty;
    fprintf(f, "%s|%d|%d|%d\n", nama, qty, omset, profit);
    fclose(f);
}


void laporanPenjualan(){
    FILE *f = fopen("riwayat_penjualan.txt", "r");
    if (!f) { printf("\nBelum ada data penjualan.\n"); return; }
    char line[100], nama[50];
    int qty, omset, profit;
    long totalProfit = 0, totalOmset = 0;

    printf("\n=== LAPORAN PENJUALAN ===\n");
    printf("%-15s | %-4s | %-10s | %-10s\n", "Barang", "Qty", "Omset", "Profit");
    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "%[^|]|%d|%d|%d", nama, &qty, &omset, &profit);
        printf("%-15s | %-4d | Rp%-8d | Rp%-8d\n", nama, qty, omset, profit);
        totalOmset += omset; totalProfit += profit;
    }
    printf("-------------------------------------------------\n");
    printf("TOTAL OMSET : Rp %ld\n", totalOmset);
    printf("TOTAL PROFIT: Rp %ld\n", totalProfit);
    fclose(f);
}


void cetakStruk(ItemBeli list[], int n, int total){
    FILE *f = fopen("struk_terakhir.txt", "w");
    fprintf(f, "======= STRUK BELANJA =======\n");
    for (int i = 0; i < n; i++) fprintf(f, "%s x%d = %d\n", list[i].nama, list[i].jumlah, list[i].subtotal);
    fprintf(f, "\nTOTAL = %d\n", total);
    fclose(f);
    printf("Struk dicetak ke 'struk_terakhir.txt'\n");
}


int main() {
    loadGudang();
    int p;
    do {
        printf("\n======= SISTEM MANAJEMEN GUDANG TOKO KLONTONG =======\n");
        printf("==================== MENU UTAMA =======================\n");
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








