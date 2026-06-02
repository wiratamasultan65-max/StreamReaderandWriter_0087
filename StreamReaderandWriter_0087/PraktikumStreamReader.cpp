#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <vector>
#include <stdexcept>
#include <limits>

using namespace std;

// Konstanta untuk nama file database
const string NAMA_FILE = "gudang.txt";

class TokoElektronik {
private:
    // Atribut private berupa array kapasitas 3 elemen untuk representasi etalase
    array<string, 3> etalase;

public:
    TokoElektronik() {
        etalase[0] = "Laptop Lenovo LOQ";
        etalase[1] = "Laptop HP Victus";
        etalase[2] = "Smartphone Poco F7 Pro";
    }
    string ambilProduk(size_t nomorRak) {
        try {
            // Wajib menggunakan fungsi .at() untuk mengakses indeks array
            return etalase.at(nomorRak);
        } catch (const out_of_range& e) {
            // Menangkap error bawaan out_of_range dan melempar (throw) pesan error kustom
            string pesanError = "Gagal Mengambil Barang : Rak nomor " + to_string(nomorRak) + " kosong atau tidak tersedia!";
            throw runtime_error(pesanError);
        }
    }
};

void initFile() {
    ifstream file(NAMA_FILE);
    if (!file) {
        ofstream newFile(NAMA_FILE);
        newFile.close();
    }
}

void readGudang() {
    ifstream file(NAMA_FILE);
    string line;
    int no = 1;
    bool isEmpty = true;
    
    cout << "\n=== DAFTAR BARANG DI GUDANG (.txt) ===" << endl;
    while (getline(file, line)) {
        cout << no++ << ". " << line << endl;
        isEmpty = false;
    }

    if (isEmpty) {
        cout << "(Data gudang kosong. Silakan gunakan menu Create.)" << endl;
    }
    cout << "======================================\n" << endl;
    file.close();
}

void createBarang() {
    ofstream file(NAMA_FILE, ios::app);
    string namaBarang;
    cout << "Masukkan nama barang elektronik baru: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, namaBarang);
    
    file << namaBarang << endl;
    cout << "[SUKSES] Barang berhasil ditambahkan ke gudang!\n";
    file.close();
}

void updateBarang() {
    vector<string> dataBarang;
    ifstream file(NAMA_FILE);
    string line;
    
    while (getline(file, line)) {
        dataBarang.push_back(line);
    }
    file.close();

    if (dataBarang.empty()) {
        cout << "[GAGAL] Gudang masih kosong, tidak ada yang bisa diubah!\n";
        return;
    }

    int index;
    cout << "Pilih nomor urut barang yang ingin diubah: ";
    cin >> index;

    if (index > 0 && index <= dataBarang.size()) {
        cout << "Masukkan nama barang pengganti: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, line);
        
        dataBarang[index - 1] = line;

        // Tulis ulang seluruh isi file (mode default/trunc)
        ofstream outFile(NAMA_FILE);
        for (const auto& barang : dataBarang) {
            outFile << barang << endl;
        }
        outFile.close();
        cout << "[SUKSES] Data barang berhasil diperbarui!\n";
    } else {
        cout << "[GAGAL] Nomor barang tidak valid!\n";
    }
}

void deleteBarang() {
    vector<string> dataBarang;
    ifstream file(NAMA_FILE);
    string line;
    
    while (getline(file, line)) {
        dataBarang.push_back(line);
    }
    file.close();

    if (dataBarang.empty()) {
        cout << "[GAGAL] Gudang masih kosong, tidak ada yang bisa dihapus!\n";
        return;
    }

    int index;
    cout << "Pilih nomor urut barang yang ingin dihapus: ";
    cin >> index;

    if (index > 0 && index <= dataBarang.size()) {
        dataBarang.erase(dataBarang.begin() + (index - 1));

        ofstream outFile(NAMA_FILE);
        for (const auto& barang : dataBarang) {
            outFile << barang << endl;
        }
        outFile.close();
        cout << "[SUKSES] Data barang berhasil dihapus dari gudang!\n";
    } else {
        cout << "[GAGAL] Nomor urut barang tidak valid!\n";
    }
}

int main() {
    initFile(); // Pastikan file tersedia agar tidak terjadi error stream
    int pilihan;
    TokoElektronik toko;

    do {
        // Otomatis dipanggil saat menu dibuka sesuai instruksi
        readGudang(); 

        cout << "=== MENU MANAJEMEN GIBRAN JAYA ===\n";
        cout << "1. Tambah Data Barang (Create)\n";
        cout << "2. Ubah Data Barang (Update)\n";
        cout << "3. Hapus Data Barang (Delete)\n";
        cout << "4. Simulasi Etalase (Exception Handling test)\n";
        cout << "0. Keluar Program\n";
        cout << "Masukkan Pilihan: ";

        // Pengamanan tipe data input untuk mencegah infinite loop jika dimasukkan huruf
        if (!(cin >> pilihan)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pilihan = -1; // Force invalid choice
        }

        switch (pilihan) {
            case 1:
                createBarang();
                break;
            case 2:
                updateBarang();
                break;
            case 3:
                deleteBarang();
                break;
            case 4:
                cout << "\n--- MEMULAI SIMULASI ETALASE ---\n";
                // Skenario 1: Ambil indeks ke-1 (Harus Sukses)
                cout << "-> Skenario 1: Mengambil rak indeks ke-1\n";
                try {
                    string produk = toko.ambilProduk(1);
                    cout << "   [BERHASIL] Item yang diambil: " << produk << "\n";
                } catch (const exception& e) {
                    cout << "   [ERROR] " << e.what() << "\n";
                }
                // Skenario 2: Ambil indeks ke-5 (Harus Gagal dan Menangkap Error Custom)
                cout << "\n-> Skenario 2: Mengambil rak indeks ke-5\n";
                try {
                    string produk = toko.ambilProduk(5);
                    cout << "   [BERHASIL] Item yang diambil: " << produk << "\n";
                } catch (const exception& e) {
                    cout << "   [EXCEPTION CATCHED] " << e.what() << "\n";
                }
                cout << "--------------------------------\n";
                break;
            case 0:
                cout << "\nMenutup program. Terima kasih!\n";
                break;
            default:
                cout << "\n[INFO] Pilihan menu tidak valid, silakan coba lagi.\n";
        }
        
        if (pilihan != 0) {
            cout << "\nTekan [ENTER] untuk kembali ke Menu Utama...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get(); 
        }
        
    } while (pilihan != 0);

    return 0;
}