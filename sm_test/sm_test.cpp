#include"sm_hash_map.h"
#include<string>
#include<iostream>
#include<vector>
#include<boost/progress.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include<thread>
#include<mutex>
using namespace std;
using namespace boost::interprocess;
static mutex mu;
const size_t siz =100*1;
int kk=0;
size_t key_size = 32;
size_t value_size = 64;
double factor = 0.75;
vector<string> v;

void sg(const char* name)
{
	SM_HANDLE handle = sm_server_init(name, key_size, value_size, siz, factor);
	if (!handle)
		return;
	boost::progress_timer t;
	for (int i = 0; i < 1; ++i)
		for (auto cc : v)
			sm_set_str(handle, cc.c_str(), cc.c_str());
	lock_guard<mutex> lg(mu);
	cout << endl<<"server" << endl;
	cout << "avrage depth " << sm_avg_depth(handle) << endl;
	cout << "sm_size " << sm_size(handle) << endl;
	cout << "sm_memory_use " << sm_memory_use(handle) << endl;
	cout << "sm_total_memory " << sm_total_memory(handle) << endl;
}

void g(const char* name)
{
	SM_HANDLE handle = sm_server_init(name, key_size, value_size, siz, factor);
	if (!handle)
		return;
	boost::progress_timer t;

	int k = 0;
	for (int i = 0; i < 1; ++i)
		for (auto cc : v)
		{
			size_t len = 128;
			char out[128];
			if (sm_get_str(handle, cc.c_str(), out, len) == 0)
			if (cc != out)
				++k;
		}
	lock_guard<mutex> lg(mu);
	cout <<endl<< "client" << endl;
	cout << "error number: " << k << endl;
	cout << "sm_avr_depth " << sm_avg_depth(handle) << endl;
	cout << "sm_size " << sm_size(handle) << endl;
	cout << "sm_memory_use " << sm_memory_use(handle) << endl;
	cout << "sm_total_memory " << sm_total_memory(handle) << endl;
	cout << "sm_key_len" << sm_key_len(handle) << endl;
	cout << "sm_value_len" << sm_value_len(handle) << endl;

}

void del(const char* name)
{
	SM_HANDLE handle = sm_server_init(name, key_size, value_size, siz, factor);
	if (!handle)
		return;
	boost::progress_timer t;
	for (int i = 0; i < 1; ++i)
		for (auto cc : v)
		{
			sm_remove(handle, cc.c_str());
		}
}
void x(const string& s)
{
	cout << s << endl;
}
int main(int argc, char* argv[])
{
	for(size_t i = 0; i < siz; ++i)
		v.push_back(to_string(i));
	cout << "ya" << endl;
	vector<thread> v;
	const char *ss[] = {"ya","ih","ik","ad","ee","af","ag","ah"};
	const char *xa = "in";
	if (argc > 1)
		xa = argv[1];
	int n = 1;
	for (int i = 0; i < n; ++i)
		v.emplace_back(bind(sg, ss[i]));
	//for (int i = 0; i < n; ++i)
		//v.emplace_back(bind(del, ss[i]));
	for (int i = 0; i < v.size(); ++i)
		v[i].join();
	return 0;
}