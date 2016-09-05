#include"sm_hash_map.h"
#include<string>
#include<iostream>
#include<vector>
#include<random>
#include<boost/progress.hpp>
#include<boost/algorithm/string.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
using namespace std;
using namespace boost::algorithm;
using namespace boost::interprocess;

static SM_HANDLE handle;
static void info()
{
	cout << "Tool for shared memory hashmap\n"
		"Copyright SZKINGDOM Inc. All Rights Reserved\n"
		"Author: leipeng@szkingdom.com\n\n";
}
static void help()
{
	cout << "usage:\n"
		"    use [namespace]\n"
		"    show: list all namespace\n"
		"    get key\n"
		"    set key value\n"
		"    ls [number]\n"
		"    info\n"
		"    quit\n" 
		"    help\n"<< endl;;
}
static string trimm(string s)
{
	trim(s);
	return s;
}
int main()
{
	info();
	help();
	string input;
	while (std::getline(cin, input))
	{
		if (trimm(input) == "show")
		{
			try {
				shared_memory_object smo(open_only, "sm_namespaces", read_only);
				mapped_region rg(smo, read_only);
				char *q = static_cast<char*>(rg.get_address());
				while (*q)
				{
					cout << q << endl;
					q += 128;
				}
			}
			catch (const exception&)
			{
				cout << "no namespace exsits" << endl;
			}

			continue;
		}

		if (starts_with(input, "use"))
		{
			if (handle)
				sm_release(handle);
			handle = sm_client_init(trimm(input.substr(3)).c_str());

			if (!handle)
			{
				cout << "failed: namespace " << trimm(input.substr(3)) << " not exist or tool version not match" << endl;
			}
			continue;
		}

		if (input == "h" || input == "help")
		{
			help();
		}

		else if (input == "q" || input == "quit")
		{
			break;
		}

		else if (!handle)
		{
			cout << "use namespace first!" << endl;
			continue;
		}

		else if (input == "info")
		{
			cout <<"version "<<sm_version()
				<< "\nsize " << sm_size(handle)
				<< "\naverage depth " << sm_avg_depth(handle)
				<< "\nmemory used " << sm_memory_use(handle)
				<< "\nmemory allocated " << sm_total_memory(handle)
				<< "\nmax key length " << sm_key_len(handle)
				<< "\nmax value length " << sm_value_len(handle) << "\n" << endl;
		}

		else if (starts_with(input, "ls"))
		{
			size_t n= trimm(input.substr(2)).empty()?20: stoi(trimm(input.substr(2)));
			if (n > sm_size(handle))
				n = sm_size(handle);

			auto kvi_size = sm_key_len(handle) + sm_value_len(handle) + 2*sizeof size_t;
			std::random_device rd;
			std::default_random_engine engine(rd());
			std::uniform_int_distribution<> distribution(0, sm_size(handle) - n);
			auto p = sm_bucket_head(handle) + distribution(engine)*kvi_size;
			printf("%-32s%s\n", "key", "value");
			printf("---------------------------------------------\n");
			for (size_t i = 0; i < n;)
			{
				if (*p)
				{
					printf("%-32s%s\n", p, p + sm_key_len(handle));
					++i;
				}
				p += kvi_size;
			}
			cout << endl;
		}

		else if (starts_with(input, "get"))
		{
			string key = trimm(input.substr(3));
			char value[4096];
			size_t len = 4096;
			int error = sm_get_str(handle, key.c_str(), value, len);
			if (error)
				cout << "sm_get error: " << error << endl;
			else
				cout << value << endl;
		}

		else if (starts_with(input, "set"))
		{
			string kv = trimm(input.substr(3));
			auto finder = kv.find_first_of(" ");
			if (finder == string::npos)
			{
				cout << "failed: set formmat error" << endl;
				continue;
			}
			string key = kv.substr(0, finder);
			string value = trimm(kv.substr(finder));
			int error = sm_set_str(handle, key.c_str(), value.c_str());
			if (error)
				cout << "sm_set error: " << error << endl;
		}

		else if (starts_with(input, "rm"))
		{
			string key = trimm(input.substr(2));
			int error = sm_remove(handle, key.c_str());
			if(error)
				cout << "sm_remove error: " << error << endl;
		}

		else
			cout << "command not found" << endl;
	}
	return 0;
}