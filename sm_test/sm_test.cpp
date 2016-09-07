#include"sm_hash_map.h"
#include<string>
#include<iostream>
#include<vector>
#include<thread>
#include<mutex>

#include<boost/progress.hpp>
using namespace std;
static mutex mu;
const unsigned int siz = 1000*1000;
int kk = 0;
unsigned int key_size = 8;
unsigned int value_size = 8;
double factor = 0.75;
vector<string> v;
int nn = 5;
SM_HANDLE handle;

void s(SM_HANDLE handle)
{
	if (!handle)
		return;
	boost::progress_timer t;
	for (int i = 0; i < nn; ++i)
		for (auto cc : v)
			sm_set_str(handle, cc.c_str(), cc.c_str());
	lock_guard<mutex> lg(mu);
	cout << endl<<"server" << endl;
	/*cout << "avrage depth " << sm_avg_depth(handle) << endl;
	cout << "sm_size " << sm_size(handle) << endl;
	cout << "sm_memory_use " << sm_memory_use(handle) << endl;
	cout << "sm_total_memory " << sm_total_memory(handle) << endl;*/
}

void g(SM_HANDLE handle)
{
	if (!handle)
		return;
	boost::progress_timer t;

	int k = 0;
	int count = 0;
	for (int i = 0; i < nn; ++i)
		for (auto cc : v)
		{
			unsigned int len = 128;
			char out[128];
			if (sm_get_str(handle, cc.c_str(), out, len) == 0)
				if (cc != out)
				{
					++k;
					if (++count < 9)
					{
						lock_guard<mutex> lg(mu);
						cout << cc << " "<<out<<endl;
					}
				}
		}
	lock_guard<mutex> lg(mu);
	cout << endl << "client" << endl;
	cout << "error number: " << k << endl;
	cout << "sm_avr_depth " << sm_avg_depth(handle) << endl;
	cout << "sm_size " << sm_size(handle) << endl;
	cout << "sm_memory_use " << sm_memory_use(handle) << endl;
	cout << "sm_total_memory " << sm_total_memory(handle) << endl;
	cout << "sm_key_len " << sm_key_len(handle) << endl;
	cout << "sm_value_len " << sm_value_len(handle) << endl;

}

void del(SM_HANDLE handle)
{
	if (!handle)
		return;
	boost::progress_timer t;
	for (int i = 0; i < nn; ++i)
		for (auto cc : v)
		{
			sm_remove(handle, cc.c_str());
		}
	lock_guard<mutex> lg(mu);
	cout << endl << "del" << endl;
}

int main(int argc, char* argv[])
{

	for (unsigned int i = 0; i < siz; ++i)
		v.push_back(to_string(i));
	vector<thread> v;
	const char *ss[] = { "ab","bb","gc","fv","ee","af","ag","ah" };
	const char *xa = "in";
	if (argc > 1)
		xa = argv[1];
	int n = 1;

	for (int i = 0; i < n; ++i)
	{
		handle = sm_server_init(ss[i], key_size, value_size, siz, factor);
		v.emplace_back(bind(s, handle));
		//this_thread::sleep_for(100ms);
		v.emplace_back(bind(del, handle));
		v.emplace_back(bind(g, handle));
	}
	for (int i = 0; i < v.size(); ++i)
		v[i].join();
	return 0;
}