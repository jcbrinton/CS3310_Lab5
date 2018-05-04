/*
Joseph Brinton
Group 9 Lab #5
CS 3310 Section 001
*/

#define _SCL_SECURE_NO_WARNINGS 

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <dos.h>
#include <iomanip>
#include <vector>
#include <algorithm>      
#include <random>
#include <unordered_map>
#include <queue>

using namespace std;
using std::setw;

struct node {
	int level;
	int profit;
	int weight;
	double bound;
	std::vector<int>  items;
	bool operator<(const node& rhs) const
	{
		return bound < rhs.bound;
	}
};

struct loot {
	int weight;
	int profit;
	double ratio;
};

int basic_operations = 0;
std::vector<int>  k, z;
std::vector<loot>  l;

int W = 0, knapsack = 0, n;

//randomly generates weights and values for each item
void lootGen() {
	l.resize(n);

	for (int i = 0; i < n; i++) {

		l[i].profit = (rand() % 20) * 10;
		l[i].weight = rand() % 20 + 1;
		l[i].ratio = l[i].profit / l[i].weight;
	}

}

double bound(node x) {
	int j, k, total;
	double result;
	basic_operations++;
	if (x.weight >= W)
		return 0;
	else {
		result = x.profit;
		j = x.level + 1;
		total = x.weight;
		while (j < n && total + l[j].weight <= W) {
			
			total = total + l[j].weight;
			result = result + l[j].profit;
			j++;
		}
		k = j;
		if (k < n)
			result = result + (W - total)*l[k].ratio;
		return result;
	}

}
std::queue<node> r;
void knapsack2() {
	node u, v;
	std::queue<node> q;
	v.level = 0 ;
	v.profit = 0;
	v.weight = 0;
	v.bound = 0;
	v.items.clear();
	u.level = 0;
	u.profit = 0;
	u.weight = 0;
	u.bound = 0;
	u.items.clear();
	int max = 0;
	q.push(v);

	while (!q.empty()) {
		v=q.front();
		q.pop();
		u.level = v.level + 1;
		u.weight = v.weight + l[u.level-1].weight;
		u.profit = v.profit + l[u.level-1].profit;
		v.items.push_back(u.level - 1);
		u.items = v.items;
		if (u.weight <= W && u.profit > max) {
			max = u.profit;
			k = v.items;
		}
		if (bound(u) > max)
			q.push(u);
		u.items.pop_back();
		
		u.weight = v.weight;
		u.profit = v.profit;
		if (bound(u) > max) 
			q.push(u);

	}
}

void knapsack3() {
	node u, v;
	std::priority_queue<node> pq;
	v.level = 0;
	v.profit = 0;
	v.weight = 0;
	v.bound = 0;
	v.items.clear();
	u.level = 0;
	u.profit = 0;
	u.weight = 0;
	u.bound = 0;
	u.items.clear();
	int max = 0;
	v.bound = bound(v);
	pq.push(v);

	while (!pq.empty()) {
		v =pq.top();
		pq.pop();
		if (v.bound > max) {

			u.level = v.level + 1;
			u.weight = v.weight + l[u.level - 1].weight;
			u.profit = v.profit + l[u.level - 1].profit;
			v.items.push_back(u.level - 1);
			u.items = v.items;
			
			
			if (u.weight <= W && u.profit > max) {
				max = u.profit;
				k = u.items;
			}
			u.bound = bound(u);

			if (u.bound > max)
				pq.push(u);
			u.items.pop_back();
			
			u.weight = v.weight;
			u.profit = v.profit;
			u.bound = bound(u);
			if (u.bound > max)
				pq.push(u);
			v.items.clear();
		}

	}
}

bool compareStruct(const loot &a, const loot &b) {
	return a.ratio < b.ratio;
}

//main function
int main()
{
	while (true) {

		int count = 0;
		cout << "Enter the number of items n: ";
		cin >> n;
		while (!cin || n<=0) {
			cin.clear();
			cin.ignore();
			cout << "Input invalid. Enter the number of items n: ";
			cin >> n;
		}
		cout << "Enter the size of the knapsack W: ";
		cin >> W;
		while (!cin || W<0) {
			cin.clear();
			cin.ignore();
			cout << "Input invalid. Enter the size of the knapsack W: ";
			cin >> W;
		}

		count = n;

		lootGen();
		sort(l.begin(), l.end(), compareStruct);
		reverse(l.begin(), l.end());
		cout << "Loot:  " << "  ";
		for (int k = 0; k < n; k++)
			cout << setw(4) << std::left << k << " ";
		cout << endl;
		cout << "price:  " << " ";
		for (int k = 0; k < n; k++)
		cout << setw(4) << std::left << l[k].profit << " " ;
		cout << endl;
		cout << "weight: " << " ";
		for (int k = 0; k < n; k++)
		cout << setw(4) << std::left << l[k].weight << " ";
		cout << endl;
		cout << "ratio:  " << " ";
		for (int k = 0; k < n; k++)
		cout << setw(4) << std::left << l[k].ratio << " ";
		cout << endl;
		cout << setw(11) << std::left << "" << setw(11) << std::left << "Number" << setw(11) << std::left << "Max" << setw(11) << std::left << "Time" << setw(11) << std::left << "Basic" << endl;
		cout << setw(11) << std::left << "Algorithm" << setw(11) << std::left << "Items (n)" << setw(11) << std::left << "Weight (W) " << setw(11) << std::left << "Elapsed" << setw(11) << std::left << "Operations" << endl;

		clock_t start;
		clock_t end;
		clock_t elapsed_clock;
		clock_t elapsed_time;

		start = clock();

		knapsack2();

		end = clock();
		elapsed_clock = end - start;
		elapsed_time = ((end - start) / (CLK_TCK));

		cout << setw(11) << "Breadth" << setw(11) << count << setw(11) << W << setw(11) << elapsed_clock << setw(11) << basic_operations << endl;

		
		z = k;
		//reset global variables
	
		basic_operations = 0;
		knapsack = 0;
		k.clear();
		start = 0;
		end = 0;
		elapsed_clock = 0;
		elapsed_time = 0;

		start = clock();

		knapsack3();

		end = clock();
		elapsed_clock = end - start;
		elapsed_time = ((end - start) / (CLK_TCK));

		cout << setw(11) << "Best-first" << setw(11) << count << setw(11) << W << setw(11) << elapsed_clock << setw(11) << basic_operations << endl;
		cout << "Breadth-first knapsack:  " << " ";
		for (size_t j = 0; j < z.size(); j++)
			cout << setw(4) << std::left << z[j] << " ";
		cout << endl;
		cout << "Best-first knapsack:  " << " ";
		for (size_t j = 0; j < k.size(); j++)
			cout << setw(4) << std::left << k[j] << " ";
		cout << endl;
		
		//reset stats
		basic_operations = 0;
		knapsack = 0;
		k.clear();
		l.clear();
		z.clear();
		W = 0;
		n = 0;

	}

}

