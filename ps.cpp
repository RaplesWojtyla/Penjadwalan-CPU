#include <bits/stdc++.h>

using namespace std;

struct Proses {
    string id;
    int arrival;
    int burst;
    int priority;
    int start_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
};

void PriorityScheduling(vector<Proses> data) {
    cout << "\n=== Priority Scheduling (Non-preemptive) ===\n";
    int waktu = 0, selesai = 0;
    vector<bool> done(data.size(), false);
    double total_wait = 0, total_response = 0;

    while (selesai < data.size()) {
        int idx = -1, best_priority = 1e9;
        for (int i = 0; i < data.size(); i++) {
            if (!done[i] && data[i].arrival <= waktu && data[i].priority < best_priority) {
                best_priority = data[i].priority;
                idx = i;
            }
        }

        if (idx == -1) {
            waktu++;
            continue;
        }

        auto& p = data[idx];
        p.start_time = waktu;
        p.completion_time = waktu + p.burst;
        p.waiting_time = p.start_time - p.arrival;
        p.response_time = p.waiting_time;
        p.turnaround_time = p.completion_time - p.arrival;

        waktu += p.burst;
        total_wait += p.waiting_time;
        total_response += p.response_time;
        done[idx] = true;
        selesai++;

        cout << p.id << ": Start=" << p.start_time << ", Finish=" << p.completion_time
             << ", Waiting=" << p.waiting_time << ", Response=" << p.response_time << '\n';
    }

    int total_time = waktu;
    cout << fixed << setprecision(2);
    cout << "Average Waiting Time   : " << total_wait / data.size() << endl;
    cout << "Average Response Time  : " << total_response / data.size() << endl;
    cout << "Throughput             : " << (double)data.size() / total_time << " proses/unit waktu\n";
}

vector<Proses> getSampleData() {
    return {
        {"P1", 0, 5, 2},  // {id, arrival, burst, priority}
        {"P2", 1, 3, 1},
        {"P3", 2, 8, 3},
        {"P4", 3, 6, 2}
    };
}

int main() {
    vector<Proses> data = getSampleData();
    PriorityScheduling(data);
    return 0;
}
