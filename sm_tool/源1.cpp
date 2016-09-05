#include<all.h>
#include <boost/progress.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include<boost/algorithm/string.hpp>
#include<mutex>
#include<random>
using namespace boost::interprocess;
struct ba
{
	ba() {}
	void start() { go(); }
	virtual void go() { cout << "ba go" << endl; }
	~ba() { cout << "~ba" << endl; }
};
struct sa :ba
{

	void go() { cout << "sa go" << endl; };
	~sa() { cout << "~sa" << endl; }
};

long long counts = 0;
const int n = 1000000000;
static named_mutex mtxb{ open_or_create, "cc5" };
static mutex mm;


void gg()
{
	const int sz = 1000 * 1000 * 100;

	shared_memory_object shm(open_or_create, "cs", read_write);
	shm.truncate(sz);
	char *p;
	{
		std::shared_ptr<mapped_region> region;
		region = make_shared<mapped_region>(shm, read_write);
		p = (static_cast<char*>(region->get_address()));
		cout << (int)p << endl;
	}
	shm.truncate(sz*2);
	{
		std::shared_ptr<mapped_region> region;
		region = make_shared<mapped_region>(shm, read_write);
		p = (static_cast<char*>(region->get_address()));
		cout << (int)p << endl;
	}
}

int main1()
{
	gg();
	return 0;
}