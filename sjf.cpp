/*
███████╗███████╗██████╗ ██████╗ ██╗    ██╗ █████╗ ███╗   ██╗
██╔════╝██╔════╝██╔══██╗██╔══██╗╚██╗ ██╔╝██╔══██╗████╗  ██║
█████╗  █████╗  ██████╔╝██║  ██║ ╚████╔╝ ███████║██╔██╗ ██║
██╔══╝  ██╔══╝  ██╔══██╗██║  ██║  ╚██╔╝  ██╔══██║██║╚██╗██║
██║     ███████╗██║  ██║██████╔╝   ██║   ██║  ██║██║ ╚████║
╚═╝     ╚══════╝╚═╝  ╚═╝╚═════╝    ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝
*/

#include <bits/stdc++.h>
#define FOR(a, b, c) for (int a = b; a < c; ++a)
#define all(x) x.begin(), x.end()
#define pb push_back
using namespace std;

template<typename ...Args>
void print(const Args &...args){
    bool f=true;
    auto pws=[&](const auto &arg){
        if(!f) cout << ' ';
        f=false;
        cout << arg;
    };
    (pws(args),...);
}

template<typename T>
void num_input(T &num){
    auto c = getchar();
    num = 0;
    while (c >= '0' and c <= '9'){
        num = num * 10 + c - '0';
        c = getchar();
    }
}

struct Process
{
    int id;
    int arrivalTime;
    int burstTime;

    int completionTime;
    int waitingTime;
    int turnaroundTime;
    int responseTime;
    bool isCompleted;

    Process(int id, int at, int bt) {
        this->id = id;
        this->arrivalTime = at;
        this->burstTime = bt;

        this->completionTime = 0;
        this->waitingTime = 0;
        this->turnaroundTime = 0;
        this->responseTime = -1;
        this->isCompleted = false;
    }

    int getStartTime() const {
        return completionTime - burstTime;
    }
};

class SJFScheduler
{
private:
    vector<Process> processes;
    int lastCompletionTime;

    static bool compareByArrivalTime(const Process &a, const Process &b)
    {
        return a.arrivalTime < b.arrivalTime;
    }

public:
    SJFScheduler(vector<Process> initProcesses)
    {
        this->processes = initProcesses;
    }

    void run()
    {
        if (processes.empty())
        {
            print("Tidak ada proses untuk dijadwalkan.\n");
            return;
        }

        int n = processes.size();
        int cp = 0; // completed process count
        int ct = 0; // current time

        sort(processes.begin(), processes.end(), compareByArrivalTime);

        while (cp < n)
        {
            // Cari proses dengan arrivalTime <= ct & belum selesai, dengan burstTime terkecil
            int idx = -1;
            int minBT = INT_MAX;

            for (int i = 0; i < n; ++i)
            {
                if (processes[i].arrivalTime <= ct && !processes[i].isCompleted)
                {
                    if (processes[i].burstTime < minBT)
                    {
                        minBT = processes[i].burstTime;
                        idx = i;
                    }
                    // Optional: tie breaker by arrivalTime
                    else if (processes[i].burstTime == minBT && processes[i].arrivalTime < processes[idx].arrivalTime)
                    {
                        idx = i;
                    }
                }
            }

            if (idx == -1)
            {
                // Tidak ada proses yang siap → maju waktu
                ct++;
                continue;
            }

            // Jalankan proses idx
            if (processes[idx].responseTime == -1)
                processes[idx].responseTime = ct - processes[idx].arrivalTime;

            ct += processes[idx].burstTime;

            processes[idx].completionTime = ct;
            processes[idx].turnaroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
            processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
            processes[idx].isCompleted = true;

            cp++;
        }

        this->lastCompletionTime = ct;
    }

    void printResult()
    {
        // Urutkan proses berdasarkan waktu mulai eksekusi (startTime)
        sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
            return a.id < b.id;
        });

        float totalWaitingTime = 0;
        float totalTurnaroundTime = 0;
        float totalResponseTime = 0;
        int n = (int) processes.size();

        print("\n--- Hasil Penjadwalan SJF Non-Preemptive ---\n");
        print("------------------------------------------------------------------------------------------------\n");
        print("| PID | Arrival | Burst | Completion | Waiting | Turnaround | Response |\n");
        print("------------------------------------------------------------------------------------------------\n");

        for (const Process &p: processes)
        {
            totalWaitingTime += p.waitingTime;
            totalResponseTime += p.responseTime;
            totalTurnaroundTime += p.turnaroundTime;

            std::cout << "| " << setw(3) << p.id
                      << " | " << setw(7) << p.arrivalTime
                      << " | " << setw(5) << p.burstTime
                      << " | " << setw(10) << p.completionTime
                      << " | " << setw(7) << p.waitingTime
                      << " | " << setw(10) << p.turnaroundTime
                      << " | " << setw(8) << p.responseTime << " |\n";
        }

        print("------------------------------------------------------------------------------------------------\n");

        cout << fixed << setprecision(2);
        print("Rata-rata Waktu Tunggu (Average Waiting Time):", totalWaitingTime / n, '\n');
        print("Rata-rata Waktu Putar (Average Turnaround Time):", totalTurnaroundTime / n, '\n');
        print("Rata-rata Waktu Respons (Average Response Time):", totalResponseTime / n, '\n');

        float totalTime = lastCompletionTime - processes[0].arrivalTime;
        if (totalTime > 0) print("Throughput:", static_cast<float>(n) / totalTime, "proses/unit waktu\n");
        else print("Throughput: N/A\n");
    }
};

int main()
{
    vector<Process> processes;
    int n;

    print("Masukkan jumlah proses: ");
    num_input(n);

    FOR(i, 0, n)
    {
        int arrival, burst;

        print("Proses", i + 1, ":\n");
        print("    Waktu Kedatangan (Arrival Time): ");
        num_input(arrival);

        print("    Waktu Eksekusi (Burst Time): ");
        num_input(burst);

        processes.emplace_back(i + 1, arrival, burst);
    }

    SJFScheduler scheduler(processes);

    scheduler.run();

    scheduler.printResult();

    return 0;
}
