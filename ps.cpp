/*
██████╗  █████╗ ███████╗██╗  ██╗ █████╗ ██╗     
██╔══██╗██╔══██╗██╔════╝██║ ██╔╝██╔══██╗██║     
██████╔╝███████║███████╗█████╔╝ ███████║██║     
██╔═══╝ ██╔══██║╚════██║██╔═██╗ ██╔══██║██║     
██║     ██║  ██║███████║██║  ██╗██║  ██║███████╗
╚═╝     ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝
*/

#include <bits/stdc++.h>
#define FOR(i, a, b) for (int i = a; i < b; ++i)
#define all(x) x.begin(), x.end()

using namespace std;

// Fungsi cetak fleksibel (kompatibel dengan C++11 ke atas)
template<typename... Args>
void print(const Args&... args) {
    bool f = true;
    auto pws = [&](const auto& arg) {
        if (!f) cout << ' ';
        f = false;
        cout << arg;
    };
#if __cplusplus >= 201703L
    (pws(args), ...);
#else
    int dummy[] = {0, (pws(args), 0)...};
    static_cast<void>(dummy);
#endif
    cout << '\n';
}

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;

    int startTime = 0;
    int completionTime = 0;
    int waitingTime = 0;
    int turnaroundTime = 0;
    int responseTime = -1;

    Process(int id, int at, int bt, int pr)
        : id(id), arrivalTime(at), burstTime(bt), priority(pr) {}
};

class PriorityScheduler {
private:
    vector<Process> processes;
    int lastCompletionTime;

    static bool compareByArrival(const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    }

public:
    PriorityScheduler(const vector<Process>& proc) {
        processes = proc;
    }

    void run() {
        int waktu = 0, selesai = 0;
        int n = processes.size();
        vector<bool> done(n, false);

        while (selesai < n) {
            int idx = -1;
            int bestPriority = INT_MAX;

            FOR(i, 0, n) {
                if (!done[i] && processes[i].arrivalTime <= waktu && processes[i].priority < bestPriority) {
                    bestPriority = processes[i].priority;
                    idx = i;
                }
            }

            if (idx == -1) {
                ++waktu;
                continue;
            }

            Process& p = processes[idx];
            p.startTime = waktu;
            p.completionTime = waktu + p.burstTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.waitingTime = p.startTime - p.arrivalTime;
            p.responseTime = p.waitingTime;

            waktu += p.burstTime;
            done[idx] = true;
            selesai++;
        }

        lastCompletionTime = waktu;
    }

    void printResult() {
        sort(all(processes), compareByArrival);

        float totalWait = 0, totalTurnaround = 0, totalResponse = 0;
        int n = processes.size();

        print("\n--- Hasil Penjadwalan Priority (Non-preemptive) ---");
        print("--------------------------------------------------------------------------------------");
        print("| PID | Arrival | Burst | Priority | Completion | Waiting | Turnaround | Response |");
        print("--------------------------------------------------------------------------------------");

        for (const Process& p : processes) {
            totalWait += p.waitingTime;
            totalTurnaround += p.turnaroundTime;
            totalResponse += p.responseTime;

            cout << "| " << setw(3) << p.id
                 << " | " << setw(7) << p.arrivalTime
                 << " | " << setw(5) << p.burstTime
                 << " | " << setw(8) << p.priority
                 << " | " << setw(10) << p.completionTime
                 << " | " << setw(7) << p.waitingTime
                 << " | " << setw(10) << p.turnaroundTime
                 << " | " << setw(8) << p.responseTime << " |\n";
        }

        print("--------------------------------------------------------------------------------------");

        cout << fixed << setprecision(2);
        print("Rata-rata Waktu Tunggu (Average Waiting Time): ", totalWait / n);
        print("Rata-rata Waktu Putar (Average Turnaround Time): ", totalTurnaround / n);
        print("Rata-rata Waktu Respons (Average Response Time): ", totalResponse / n);

        float totalTime = lastCompletionTime - processes[0].arrivalTime;
        if (totalTime > 0)
            print("Throughput: ", (float)n / totalTime, " proses/unit waktu");
        else
            print("Throughput: N/A");
    }
};

int main() {
    vector<Process> processes;
    int n;

    cout << "Masukkan jumlah proses: ";
    cin >> n;

    FOR(i, 0, n) {
        int at, bt, pr;
        print("Proses", i + 1, ":");
        cout << "\tWaktu Kedatangan (Arrival Time): ";
        cin >> at;

        cout << "\tWaktu Eksekusi (Burst Time): ";
        cin >> bt;

        cout << "\tPrioritas (semakin kecil = prioritas lebih tinggi): ";
        cin >> pr;

        processes.emplace_back(i + 1, at, bt, pr);
    }

    PriorityScheduler scheduler(processes);
    scheduler.run();
    scheduler.printResult();

    return 0;
}
