#include"sm_hash_map.h"
#include<string>
#include<iostream>
#include<vector>
#include<thread>
#include<mutex>
#include<boost/progress.hpp>
using namespace std;
static mutex mu;
static void g()
{
	vector<string> v;
	for (int i = 0; i < 50000; ++i)
		v.push_back(to_string(i));

	SM_HANDLE handle = sm_client_init("smd1");
	if (!handle)
		return;
	boost::progress_timer t;

	int k = 0;
	for (int i = 0; i < 1; ++i)
		for (auto cc : v)
		{
			size_t len = 128;
			char out[128];
			sm_get(handle, cc.c_str(), out, len);
			if (cc != out)
				++k;
		}
	lock_guard<mutex> lg(mu);
	cout << "error number: " << k << endl;
	cout << "sm_avr_depth " << sm_avg_depth(handle) << endl;
	cout << "sm_size " << sm_size(handle) << endl;
	cout << "sm_memory_use " << sm_memory_use(handle) << endl;
	cout << "sm_total_memory " << sm_total_memory(handle) << endl;
}
int mainc()
{
	vector<thread> v;
	int n = 4;
	for (int i = 0; i < n; ++i)
		v.emplace_back(g);
	for (int i = 0; i < n; ++i)
		v[i].join();
	return 0;
}