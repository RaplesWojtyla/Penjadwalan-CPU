#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

struct Proses {
    int id;
    int burstTime;
    int waitingTime;
    int turnaroundTime;
};

bool bandingkanBurstTime(const Proses& a, const Proses& b) {
    return a.burstTime < b.burstTime;
}

void hitungWaktu(std::vector<Proses>& prosesList) {
    prosesList[0].waitingTime = 0;
    prosesList[0].turnaroundTime = prosesList[0].burstTime;
    for (size_t i = 1; i < prosesList.size(); ++i) {
        prosesList[i].waitingTime = prosesList[i - 1].waitingTime + prosesList[i - 1].burstTime;
        prosesList[i].turnaroundTime = prosesList[i].burstTime + prosesList[i].waitingTime;
    }
}

int main() {
    
    int jumlahProses;
    std::cout << "Masukkan jumlah proses: ";
    std::cin >> jumlahProses;

    // Membuat vector kosong untuk menampung proses yang diinput
    std::vector<Proses> proses; 
    
    // Loop untuk meminta input Burst Time untuk setiap proses
    for (int i = 0; i < jumlahProses; ++i) {
        int burstTimeInput;
        std::cout << "Masukkan Burst Time untuk Proses " << i + 1 << ": ";
        std::cin >> burstTimeInput;
        // Menambahkan proses baru ke dalam vector
        // ID diisi otomatis, WT dan TAT diinisialisasi 0
        proses.push_back({i + 1, burstTimeInput, 0, 0}); 
    }
    
    std::cout << "\n--- Menghitung Penjadwalan SJF ---\n\n";

    // --- AKHIR BAGIAN MODIFIKASI ---


    // Sisa dari kode tidak perlu diubah, karena akan bekerja
    // dengan data yang sudah diisi oleh pengguna ke dalam vector 'proses'.

    // 1. Mengurutkan proses berdasarkan Burst Time (prinsip utama SJF)
    std::sort(proses.begin(), proses.end(), bandingkanBurstTime);

    // 2. Menghitung Waiting Time dan Turnaround Time untuk urutan SJF
    hitungWaktu(proses);

    // 3. Menampilkan hasil
    std::cout << std::left << std::setw(10) << "Proses"
              << std::left << std::setw(15) << "Burst Time"
              << std::left << std::setw(15) << "Waiting Time"
              << std::left << std::setw(15) << "Turnaround Time" << std::endl;

    float totalWaitingTime = 0;
    float totalTurnaroundTime = 0;

    for (const auto& p : proses) {
        std::cout << std::left << std::setw(10) << ("P" + std::to_string(p.id))
                  << std::left << std::setw(15) << p.burstTime
                  << std::left << std::setw(15) << p.waitingTime
                  << std::left << std::setw(15) << p.turnaroundTime << std::endl;
        totalWaitingTime += p.waitingTime;
        totalTurnaroundTime += p.turnaroundTime;
    }

    float rataRataWaitingTime = totalWaitingTime / proses.size();
    float rataRataTurnaroundTime = totalTurnaroundTime / proses.size();

    std::cout << std::endl;
    std::cout << "Rata-rata Waiting Time = " << rataRataWaitingTime << std::endl;
    std::cout << "Rata-rata Turnaround Time = " << rataRataTurnaroundTime << std::endl;

    return 0;
}