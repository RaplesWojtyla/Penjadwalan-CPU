#include <bits/stdc++.h>
using namespace std;

void hitungWaitingTime(const vector<int>& burstTime, vector<int>& waitingTime) {
    waitingTime[0] = 0;
    for (int i = 1; i < (int)burstTime.size(); i++)
        waitingTime[i] = waitingTime[i-1] + burstTime[i-1];
}

void hitungTurnaroundTime(const vector<int>& burstTime, 
                          const vector<int>& waitingTime,
                          vector<int>& turnaroundTime) {
    for (int i = 0; i < (int)burstTime.size(); i++)
        turnaroundTime[i] = burstTime[i] + waitingTime[i];
}

void sjfScheduling(vector<int> burstTime) {
    int n = burstTime.size();
    vector<int> waitingTime(n), turnaroundTime(n);

    // Urutkan burst time (proses SJF)
    vector<pair<int, int>> proses; // pair<burstTime, id>
    for (int i = 0; i < n; i++)
        proses.push_back({burstTime[i], i+1});

    sort(proses.begin(), proses.end());

    // Ambil burst time terurut
    for (int i = 0; i < n; i++)
        burstTime[i] = proses[i].first;

    hitungWaitingTime(burstTime, waitingTime);
    hitungTurnaroundTime(burstTime, waitingTime, turnaroundTime);

    cout << "\n--- Menghitung Penjadwalan SJF ---\n\n";
    cout << "Proses\tBurst Time\tWaiting Time\tTurnaround Time\n";

    double totalWT = 0, totalTAT = 0;
    for (int i = 0; i < n; i++) {
        cout << "P" << proses[i].second << "\t"
             << burstTime[i]          << "\t\t"
             << waitingTime[i]        << "\t\t"
             << turnaroundTime[i]     << "\n";
        totalWT  += waitingTime[i];
        totalTAT += turnaroundTime[i];
    }
    cout << fixed << setprecision(5)
         << "\nRata-rata Waiting Time    = " << (totalWT/n)  
         << "\nRata-rata Turnaround Time = " << (totalTAT/n) 
         << "\n";
}

int main() {
    int n;
    cout << "Masukkan jumlah proses: ";
    cin  >> n;

    vector<int> burstTime(n);
    for (int i = 0; i < n; i++) {
        cout << "Masukkan Burst Time untuk Proses " << (i+1) << ": ";
        cin  >> burstTime[i];
    }

    sjfScheduling(burstTime);
    return 0;
}
