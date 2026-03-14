#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;

// prentilan untuk membuat tabel biar rapi dan estetik di ambil dari kode/nilai extended ASCII characters
char tl = (char)218;  // ┌
char h = (char)196;   // ─
char tr = (char)191;  // ┐
char v = (char)179;   // │
char bl = (char)192;  // └
char br = (char)217;  // ┘
char pl = (char)195;  // ├
char pr = (char)180;  // ┤

// struct untuk menampung data film
struct Film {
    char name[100];
    double price;
    int year;
    float rating;
};

Film data[100];

string dbFilm = "netfilm_db.txt"; // deklarasi file database yang disimpan di variable dbFilm
char pilihMenu; // variabel untuk memilih menu

//mengecilkan huruf untuk pengecekan, agar bisa disearch pake lower maupun upper
string toLower(string teks) {
    transform(teks.begin(), teks.end(), teks.begin(), ::tolower);
    return teks;
}

// pemanggilan di atas agar bisa manggil satu sama lain menyesuaikan kondisi jumlah data
void binarySearch();
void linearSearch();

// fungsi menampilkan data Film yang diambil dari dbFilm
void tampilData() {
    FILE *file = fopen(dbFilm.c_str(), "r");
    if (!file) {
        cout << "ERROR: File tidak ditemukan!" << endl;
        return;
    }

    cout << "\n" << setfill(' ') << right << setw(72) << "N E T F L I X  C O N T E N T  K A T A L O G" << endl << endl;
    cout << tl << setfill(h) << setw(100) << "" << tr << endl;
    cout << setfill(' ');
    cout << left << v
         << setw(5)  << "NO"
         << setw(55) << "JUDUL FILM"
         << setw(20) << "LISENSI"
         << setw(10) << "TAHUN"
         << setw(10) << "RATING"
         << v << endl;
    cout << pl << setfill(h) << setw(100) << "" << pr << endl;

    Film row; // deklarasi row untuk menampung data sementara dari file
    int no = 1;
    while (fscanf(file, "%[^;];%lf;%d;%f\n", row.name, &row.price, &row.year, &row.rating) != EOF) {
        char id[6], harga[20];
        sprintf(id,    "[%d]",      no++);
        sprintf(harga, "Rp %.0lf",  row.price);

        cout << setfill(' ');
        cout << left << v
             << setw(5)  << id
             << setw(55) << row.name
             << setw(20) << harga
             << setw(10) << row.year
             << setw(10) << row.rating
             << v << endl;
    }

    cout << bl << setfill(h) << setw(100) << "" << br << endl << endl;
    fclose(file);
    cout << "Tekan Enter untuk melanjutkan..."; cin.get(); system("cls");
}

// menampilkan hasil sorting
void tampilDataSorting(Film arr[], int n) {
    cout << "\n" << setfill(' ') << right << setw(72) << "N E T F L I X  C O N T E N T  K A T A L O G" << endl << endl;
    cout << tl << setfill(h) << setw(100) << "" << tr << endl;
    cout << setfill(' ');
    cout << left << v
         << setw(5)  << "NO"
         << setw(55) << "JUDUL FILM"
         << setw(20) << "LISENSI"
         << setw(10) << "TAHUN"
         << setw(10) << "RATING"
         << v << endl;
    cout << pl << setfill(h) << setw(100) << "" << pr << endl;

    for (int i = 0; i < n; i++) {
        char id[6], harga[20];
        sprintf(id, "[%d]", i + 1);
        sprintf(harga, "Rp %.0lf", arr[i].price);

        cout << setfill(' ');
        cout << left << v
             << setw(5)  << id
             << setw(55) << arr[i].name
             << setw(20) << harga
             << setw(10) << arr[i].year
             << setw(10) << arr[i].rating
             << v << endl;
    }

    cout << bl << setfill(h) << setw(100) << "" << br << endl << endl;
}

// fungsi BUBBLE SORT berdasarkan abjad A-Z
void bubbleSort(Film arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(arr[j].name, arr[j + 1].name) > 0) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// fungsi QUICK SORT berdasarkan rating tertinggi
void quickSort(Film arr[], int left, int right) {
    if (left >= right) return;
    int i = left, j = right;
    Film pivot = arr[(left + right) / 2];

    while (i <= j) {
        while (arr[i].rating > pivot.rating) i++;
        while (arr[j].rating < pivot.rating) j--;
        if (i <= j) {
            swap(arr[i], arr[j]);
            i++; j--;
        }
    }

    if (left < j) quickSort(arr, left, j);
    if (i < right) quickSort(arr, i, right);
}

// fungsi pengurutan/sorting data
void sortingData() {
    FILE *file = fopen(dbFilm.c_str(), "r");
    if (!file) {
        cout << "ERROR: File tidak ditemukan!" << endl;
        return;
    }

    int n = 0;
    while (fscanf(file, "%[^;];%lf;%d;%f\n", data[n].name, &data[n].price, &data[n].year, &data[n].rating) != EOF) {
        n++;
    }
    fclose(file);

    if (pilihMenu == '2') {
        quickSort(data, 0, n - 1);
        tampilDataSorting(data, n);
        cout << "[!] Katalog berhasil diurutkan berdasarkan Rating Tertinggi.\n\n";
    } else if (pilihMenu == '3') {
        bubbleSort(data, n);
        tampilDataSorting(data, n);
        cout << "[!] Katalog berhasil diurutkan berdasarkan Abjad A-Z.\n\n";
    }

    cout << "Tekan Enter untuk melanjutkan..."; cin.get(); system("cls");
}

// pengecekan data terurut
bool sudahTerurut(Film arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (strcmp(arr[i].name, arr[i + 1].name) > 0) return false;
    }
    return true;
}

// fungsi pencarian LINEAR SEARCH
void linearSearch() {
    FILE *file = fopen(dbFilm.c_str(), "r");
    if (!file) {
        cout << "ERROR: File tidak ditemukan!" << endl;
        return;
    }

    int n = 0;
    Film row; // deklarasi row untuk menampung data sementara dari file
    while (fscanf(file, "%[^;];%lf;%d;%f\n", row.name, &row.price, &row.year, &row.rating) != EOF) {
        n++;
    }
    rewind(file); // mengatur ulang/mengembalikan pointer  pointer file ke awal

    // validasi jumlah data apakah >= 30 jika iya sarankan untuk ke binary search
    if (n >= 30) {
        fclose(file);
        char konfirm;
        cout << "[!] Data >= 30 film, Linear Search kurang efisien.\n";
        cout << "[!] Apakah ingin dialihkan ke Binary Search? (y/n): "; cin >> konfirm;
        cin.ignore();

        if (konfirm == 'y' || konfirm == 'Y') {
            binarySearch();
        } else {
            cout << "[!] Pencarian dibatalkan.\n\n";
            cout << "Tekan Enter untuk melanjutkan..."; cin.get(); system("cls");
        }
    } else {
        char keyword[100];
        cout << "\n      L I N E A R  S E A R C H     \n\n";
        cout << "Masukkan Keyword Judul: "; cin.getline(keyword, 100);

        cout << "\n" << setfill(' ') << right << setw(72) << "N E T F L I X  C O N T E N T  K A T A L O G" << endl << endl;
        cout << tl << setfill(h) << setw(100) << "" << tr << endl;
        cout << setfill(' ');
        cout << left << v << setw(5) << "NO" << setw(55) << "JUDUL FILM" << setw(20) << "LISENSI" << setw(10) << "TAHUN" << setw(10) << "RATING" << v << endl;
        cout << pl << setfill(h) << setw(100) << "" << pr << endl;

        int no = 1, ketemu = 0;
        while (fscanf(file, "%[^;];%lf;%d;%f\n", row.name, &row.price, &row.year, &row.rating) != EOF) {
            // konversi keduanya ke lowercase sebelum dibandingkan
            string namaLower = toLower(string(row.name));
            string keyLower  = toLower(string(keyword));

            // pengecekan apakah keyword == substring dari nama film
            if (strstr(namaLower.c_str(), keyLower.c_str()) != NULL) {
                char id[6], harga[20];
                sprintf(id, "[%d]", no++);
                sprintf(harga, "Rp %.0lf", row.price);
                cout << setfill(' ');
                cout << left << v << setw(5) << id << setw(55) << row.name << setw(20) << harga << setw(10) << row.year << setw(10) << row.rating << v << endl;
                ketemu++;
            }
        }

        cout << bl << setfill(h) << setw(100) << "" << br << endl << endl;
        if (ketemu == 0) {
            cout << "[!] Film dengan keyword \"" << keyword << "\" tidak ditemukan.\n\n";
        } else {
            cout << "[!] Ditemukan " << ketemu << " film dengan keyword " << keyword << ".\n\n";
        }

        fclose(file);
        cout << "Tekan Enter untuk melanjutkan..."; cin.get(); system("cls");
    }
}

// fungsi pencarian BINARY SEARCH
void binarySearch() {
    int n = 0;
    FILE *file = fopen(dbFilm.c_str(), "r");
    if (!file) {
        cout << "ERROR: File tidak ditemukan!" << endl;
        return;
    }

    while (fscanf(file, "%[^;];%lf;%d;%f\n", data[n].name, &data[n].price, &data[n].year, &data[n].rating) != EOF) {
        n++;
    }
    fclose(file);

    // validasi jumlah data apakah < 30 jika iya sarankan untuk ke linear search
    if (n < 30) {
        char konfirm;
        cout << "[!] Data < 30 film, Binary Search tidak cocok.\n";
        cout << "[!] Apakah ingin dialihkan ke Linear Search? (y/n): "; cin >> konfirm;
        cin.ignore();

        if (konfirm == 'y' || konfirm == 'Y') {
            linearSearch();
        } else {
            cout << "[!] Pencarian dibatalkan.\n\n";
            cout << "Tekan Enter untuk melanjutkan..."; cin.get(); system("cls");
        }
    } else {
        // validasi data sudah terurut belum
        if (!sudahTerurut(data, n)) {
            char konfirm;
            cout << "[!] Data belum terurut! Apakah Anda ingin melakukan pengurutan? (y/n): "; cin >> konfirm;
            cin.ignore();
            if (konfirm == 'y' || konfirm == 'Y') {
                bubbleSort(data, n);
                cout << "[!] Data berhasil diurutkan.\n\n";
            } else {
                cout << "[!] Binary Search dibatalkan.\n\n";
                cout << "Tekan Enter untuk melanjutkan..."; cin.get(); system("cls");
                return;
            }
        }

        char keyword[100];
        cout << "\n      B I N A R Y  S E A R C H     \n\n";
        cout << "Masukkan Judul (Exact): "; cin.getline(keyword, 100);

        // konversi ke lowercase sebelum dibandingkan
        string keyLowerStr = toLower(string(keyword));

        int lo = 0, hi = n - 1, ketemu = -1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            string namaLowerStr = toLower(string(data[mid].name));

            int cmp = strcmp(namaLowerStr.c_str(), keyLowerStr.c_str());
            if (cmp == 0) {
                ketemu = mid;
                break;
            } else if (cmp < 0) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        cout << "\n" << setfill(' ') << right << setw(72) << "N E T F L I X  C O N T E N T  K A T A L O G" << endl << endl;
        cout << tl << setfill(h) << setw(100) << "" << tr << endl;
        cout << setfill(' ');
        cout << left << v << setw(5) << "NO" << setw(55) << "JUDUL FILM" << setw(20) << "LISENSI" << setw(10) << "TAHUN" << setw(10) << "RATING" << v << endl;
        cout << pl << setfill(h) << setw(100) << "" << pr << endl;

        if (ketemu != -1) {
            char id[6], harga[20];
            sprintf(id, "[1]");
            sprintf(harga, "Rp %.0lf", data[ketemu].price);
            cout << setfill(' ');
            cout << left << v << setw(5) << id << setw(55) << data[ketemu].name << setw(20) << harga << setw(10) << data[ketemu].year << setw(10) << data[ketemu].rating << v << endl;
        }

        cout << bl << setfill(h) << setw(100) << "" << br << endl << endl;
        if (ketemu == -1) {
            cout << "[!] Film \"" << keyword << "\" tidak ditemukan.\n\n";
        } else {
            cout << "[ FOUND ] " << data[ketemu].name << " ditemukan.\n\n";
        }

        cout << "Tekan Enter untuk melanjutkan..."; cin.get(); system("cls");
    }
}

// fungsi tambah film baru 
void tambahData(int jmlData) {
    FILE *file = fopen(dbFilm.c_str(), "a");
    if (!file) {
        cout << "ERROR: File tidak bisa dibuka!\n";
        return;
    }

    Film row;
    cin.ignore();

    for (int i = 0; i < jmlData; i++) {
        cout << "\nData ke-" << i + 1 << " : \n";
        cout << "Judul Film : "; cin.getline(row.name, 100);
        cout << "Harga      : "; cin >> row.price;
        cout << "Tahun      : "; cin >> row.year;
        cout << "Rating     : "; cin >> row.rating;
        cin.ignore();
        fprintf(file, "%s;%.0lf;%d;%.1f\n", row.name, row.price, row.year, row.rating);
    }

    fclose(file);
    cout << "[ SUCCESS ] Data berhasil disinkronkan ke netfilm_db.txt\n"
         << "[ SUCCESS ] Film ditambahkan.\n\n";
    cout << "Tekan Enter untuk melanjutkan..."; cin.get(); system("cls");
}

int main() {
    do {
        cout << tl << setfill(h) << setw(40) << "" << tr << endl;
        cout << v << " " << tl << setfill(h) << setw(36) << "" << tr << " " << v << endl;
        cout << v << " " << v << "     NETFILM SYSTEM - DASHBOARD     " << v << " " << v << endl;
        cout << v << " " << bl << setfill(h) << setw(36) << "" << br << " " << v << endl;
        cout << v << " " << tl << setfill(h) << setw(36) << "" << tr << " " << v << endl;

        cout << v << " " << v << " [1] Lihat Katalog Film             " << v << " " << v << endl;
        cout << v << " " << v << " [2] Urutkan Rating (Quick Sort)    " << v << " " << v << endl;
        cout << v << " " << v << " [3] Urutkan Abjad (Bubble Sort)    " << v << " " << v << endl;
        cout << v << " " << v << " [4] Cari Film (Linear Search)      " << v << " " << v << endl;
        cout << v << " " << v << " [5] Cari Film (Binary Search)      " << v << " " << v << endl;
        cout << v << " " << v << " [6] Tambah Film Baru               " << v << " " << v << endl;
        cout << v << " " << v << " [0] Keluar & Simpan                " << v << " " << v << endl;

        cout << v << " " << bl << setfill(h) << setw(36) << "" << br << " " << v << endl;
        cout << bl << setfill(h) << setw(40) << "" << br << endl;

        cout << "Pilih Menu > "; cin >> pilihMenu;
        
        switch (pilihMenu) {
            case '1': cin.ignore(); tampilData(); break; // menu tampil data
            case '2': cin.ignore(); sortingData(); break; // menu pengurutan rating
            case '3': cin.ignore(); sortingData(); break; // menu pengurutan abjad
            case '4': cin.ignore(); linearSearch(); break; // menu pencarian linear
            case '5': cin.ignore(); binarySearch(); break; // menu pencarian binary
            case '6': // menu tambah data
                int jmlData;
                cout << "\nINPUT DATA\n\n"
                     << "Masukkan jumlah film baru yang akan ditambahkan: "; cin >> jmlData;
                tambahData(jmlData);
                break;
            case '0': // menu keluar dan simpan
                cout << "[ SUCCESS ] Data berhasil disinkronkan ke netfilm_db.txt\n\n"
                     << "Sistem Offline. Terimakasih!" << endl;
                return 0;
            default: // apabila salah input menu
                system("cls");
                cout << "[!] Menu tidak tersedia.\n\n";
                cin.ignore();
                break;
        }
    } while (pilihMenu != '0');
}