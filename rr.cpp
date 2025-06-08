/*

$$\      $$\                 $$\               $$\           
$$ | $\  $$ |                $$ |              $$ |          
$$ |$$$\ $$ | $$$$$$\  $$\ $$$$$$\   $$\   $$\ $$ | $$$$$$\  
$$ $$ $$\$$ |$$  __$$\ \__|\_$$  _|  $$ |  $$ |$$ | \____$$\ 
$$$$  _$$$$ |$$ /  $$ |$$\   $$ |    $$ |  $$ |$$ | $$$$$$$ |
$$$  / \$$$ |$$ |  $$ |$$ |  $$ |$$\ $$ |  $$ |$$ |$$  __$$ |
$$  /   \$$ |\$$$$$$  |$$ |  \$$$$  |\$$$$$$$ |$$ |\$$$$$$$ |
\__/     \__| \______/ $$ |   \____/  \____$$ |\__| \_______|
                 $$\   $$ |          $$\   $$ |              
                 \$$$$$$  |          \$$$$$$  |              
                  \______/            \______/               
*/

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

template<typename ...Args>void print(const Args &...args){bool f=true;auto pws=[&](const auto &arg){if(!f)cout<<' ';f=false;cout<<arg;};(pws(args),...);}
template<typename T>void num_input(T &num){auto c = getchar();num = 0;while (c >= '0' and c <= '9'){num = num * 10 + c - '0';c = getchar();}}


struct Process 
{
	int id;
	int arrivalTime;
	int burstTime;
	int priority;

	int remainingBurstTime;
	int startTime;
	int completionTime;
	int waitingTime;
	int turnaroundTime;
	int responseTime;

	Process(int id, int at, int bt, int p = 0) {
		this->id = id;
		this->arrivalTime = at;
		this->burstTime = bt;
		this->remainingBurstTime = bt;
		this->priority = p;

		this->startTime = 0;
		this->completionTime = 0;
		this->waitingTime = 0;
		this->turnaroundTime = 0;
		this->responseTime = -1;
	}
};

class RoundRobinScheduler
{
	private:
		vector<Process> processes;
		int quantum;
		int lastCompletionTime;

		static bool compareByArrivalTime(const Process &a, const Process &b)
		{
			return a.arrivalTime < b.arrivalTime;
		}

		static bool compareByPId(const Process &a, const Process &b)
		{
			return a.id < b.id;
		}

		void printReadyQueue(queue<int> q)
		{
			if (q.empty())
			{
				print("[Kosong]");
				return;
			}

			print("[");
			while(!q.empty())
			{
				print("P" + to_string(processes[q.front()].id));
				q.pop();

				if (!q.empty()) print(", ");
			}
			print("]");
		}
	
	public:
		RoundRobinScheduler(vector<Process> initProcesses, int q)
		{
			this->processes = initProcesses;
			this->quantum = q;
		}

		void run()
		{
			if (processes.empty())
			{
				print("Tidak ada proses untuk dijadwalkan.\n");
				return;
			}

			int n = processes.size();
			queue<int> ready;

			sort(all(processes), compareByArrivalTime);

			int ct = 0;
			int cp = 0; 
			int currProcessIdx = 0;

			
			print("\n--- Jejak Eksekusi Round Robin ---\n");
			print("------------------------------------------------------------------------------------------\n");
			cout << left << setw(8) << "Waktu" << "| " << setw(10) << "Proses" << "| " << setw(40) << "Kejadian" << "| " << "Isi Ready Queue\n";
			print("------------------------------------------------------------------------------------------\n");


			while (cp < n) 
			{
				while (currProcessIdx < n && processes[currProcessIdx].arrivalTime <= ct)
				{
					ready.push(currProcessIdx);
					++currProcessIdx;
				}

				if (ready.empty())
				{
					if (currProcessIdx < n) {
						cout << left << setw(8) << ct << "| " << setw(10) << "-" << "| " << setw(40) << "CPU Idle..." << "| ";
						printReadyQueue(ready);
						print('\n');
						ct = processes[currProcessIdx].arrivalTime;
					}
					else break;

					continue;
				}

				int pId = ready.front();
				ready.pop();

				
				string event = (processes[pId].responseTime == -1) ? "Memulai eksekusi" : "Melanjutkan eksekusi";
				cout << left << setw(8) << ct << "| " << setw(10) << ("P" + to_string(processes[pId].id)) << "| " << setw(40) << event << "| ";
				printReadyQueue(ready);
				print('\n');

				if (processes[pId].responseTime == -1) processes[pId].responseTime = ct - processes[pId].arrivalTime;

				int timeSlice = min(processes[pId].remainingBurstTime, quantum);

				ct += timeSlice;
				processes[pId].remainingBurstTime -= timeSlice;

				while (currProcessIdx < n && processes[currProcessIdx].arrivalTime <= ct) 
				{
					ready.push(currProcessIdx);
					++currProcessIdx;
				}

				if (processes[pId].remainingBurstTime == 0)
				{
					cp++; 
					processes[pId].completionTime = ct;
					processes[pId].turnaroundTime = processes[pId].completionTime - processes[pId].arrivalTime;
					processes[pId].waitingTime = processes[pId].turnaroundTime - processes[pId].burstTime;

					cout << left << setw(8) << ct << "| " << setw(10) << ("P" + to_string(processes[pId].id)) << "| " << setw(40) << "Proses Selesai" << "| ";
					printReadyQueue(ready);
					print('\n');
				}
				else 
				{
					ready.push(pId); 
					
					cout << left << setw(8) << ct << "| " << setw(10) << ("P" + to_string(processes[pId].id)) << "| " << setw(40) << "Quantum habis, kembali ke antrian" << "| ";
					printReadyQueue(ready);
					print('\n');
				}
			}
			print("------------------------------------------------------------------------------------------\n");

			this->lastCompletionTime = ct;
		}

		void printResult()
		{
			sort(processes.begin(), processes.end(), compareByPId);
			float totalWaitingTime = 0;
			float totalTurnaroundTime = 0;
			float totalResponseTime = 0;
			int n = (int) processes.size();

			print("\n--- Hasil Penjadwalan Round Robin (Quantum =", quantum, ") ---\n");
			print("------------------------------------------------------------------------------------------------\n");
			print("| PID | Arrival | Burst | Priority | Completion | Waiting | Turnaround | Response |\n");
			print("------------------------------------------------------------------------------------------------\n");

			for (const Process &p: processes)
			{
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
			print("Rata-rata Waktu Putar (Average Turnaround Time):",totalTurnaroundTime / n, '\n');
			print("Rata-rata Waktu Respons (Average Response Time): ", totalResponseTime / n, '\n');

			float totalTime = lastCompletionTime - processes[0].arrivalTime;

			if (totalTime > 0) print("Throughput:", static_cast<float>(n) / totalTime, "proses/unit waktu\n");
			else print("Throughput: N/A\n");
		}
};


int main()
{
	vector<Process> processes;
    int n, quantum;

    print("Masukkan jumlah proses: ");
    num_input(n);
    
    print("Masukkan Quantum Waktu (Time Quantum): ");
    num_input(quantum);

    FOR(i, 0, n)
	{
        int arrival, burst;

        print("Proses", i + 1, ":\n");
        print("	Waktu Kedatangan (Arrival Time): ");
        num_input(arrival);

        print("	Waktu Eksekusi (Burst Time): ");
        num_input(burst);

        processes.emplace_back(i + 1, arrival, burst);
    }

    RoundRobinScheduler scheduler(processes, quantum);

	scheduler.run();

	scheduler.printResult();

    return 0;
}
