#include <bits/stdc++.h>
#define FOR(a, b, c) for (int a = b; a < c; ++a)
#define mFOR(a, b, c) for (int a = b; a > c; --a)
#define pii pair<int, int>
#define all(x) x.begin(), x.end()
#define pb push_back
#define sit set<int>::iterator
using i64 = long long;
using ull = unsigned long long;
using namespace std;

template<typename ...Args> void print(const Args &...args) {
	bool f = true;
	auto pws = [&](const auto &arg) {
		if (!f) cout << ' ';
		f = false;
		cout << arg;
	};
	(pws(args), ...);
}

template<typename T> void num_input(T &num) {
	auto c = getchar(); num = 0;
	while (c >= '0' and c <= '9') {
		num = num * 10 + c - '0';
		c = getchar();
	}
}

struct Process {
	int id;
	int arrivalTime;
	int burstTime;
	int priority;

	int startTime;
	int completionTime;
	int waitingTime;
	int turnaroundTime;
	int responseTime;

	Process(int id, int at, int bt, int p = 0) {
		this->id = id;
		this->arrivalTime = at;
		this->burstTime = bt;
		this->priority = p;

		this->startTime = 0;
		this->completionTime = 0;
		this->waitingTime = 0;
		this->turnaroundTime = 0;
		this->responseTime = -1;
	}
};

class FCFSScheduler {
	private:
		vector<Process> processes;

		static bool compareByArrivalTime(const Process &a, const Process &b) {
			return a.arrivalTime < b.arrivalTime;
		}

	public:
		FCFSScheduler(vector<Process> initProcesses) {
			this->processes = initProcesses;
		}

		void run() {
			if (processes.empty()) {
				print("Tidak ada proses untuk dijadwalkan.\n");
				return;
			}

			int n = processes.size();
			sort(all(processes), compareByArrivalTime);

			int ct = 0;

			for (Process &p : processes) {
				if (ct < p.arrivalTime) {
					ct = p.arrivalTime;
				}

				p.startTime = ct;
				p.responseTime = ct - p.arrivalTime;
				ct += p.burstTime;

				p.completionTime = ct;
				p.turnaroundTime = p.completionTime - p.arrivalTime;
				p.waitingTime = p.turnaroundTime - p.burstTime;
			}
		}

		void printResult() {
			sort(processes.begin(), processes.end(), compareByArrivalTime);
			float totalWaitingTime = 0;
			float totalTurnaroundTime = 0;
			float totalResponseTime = 0;
			int n = (int) processes.size();

			print("\n--- Hasil Penjadwalan FCFS ---\n");
			print("------------------------------------------------------------------------------------------------\n");
			print("| PID | Arrival | Burst | Priority | Completion | Waiting | Turnaround | Response |\n");
			print("------------------------------------------------------------------------------------------------\n");

			for (const Process &p : processes) {
				totalWaitingTime += p.waitingTime;
				totalResponseTime += p.responseTime;
				totalTurnaroundTime += p.turnaroundTime;

				std::cout << "| " << setw(3) << p.id
						<< " | " << setw(7) << p.arrivalTime
						<< " | " << setw(5) << p.burstTime
						<< " | " << setw(8) << p.priority
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

			float totalTime = processes[n - 1].completionTime - processes[0].arrivalTime;
			if (totalTime > 0) print("Throughput:", static_cast<float>(n) / totalTime, "proses/unit waktu\n");
			else print("Throughput: N/A\n");
		}
};

int main() {
	vector<Process> processes;
	int n;

	print("Masukkan jumlah proses: ");
	num_input(n);

	FOR(i, 0, n) {
		int arrival, burst;

		print("Proses", i + 1, ":\n");
		print("	Waktu Kedatangan (Arrival Time): ");
		num_input(arrival);

		print("	Waktu Eksekusi (Burst Time): ");
		num_input(burst);

		processes.emplace_back(i + 1, arrival, burst);
	}

	FCFSScheduler scheduler(processes);

	scheduler.run();
	scheduler.printResult();

	return 0;
}
