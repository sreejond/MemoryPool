/**
 *
 * \brief
 *
 * This is a Tester file to test ImageBlurUtil. It run two test cases on two different images and write the output image.
 */

#include <iostream>
#include <thread>
#include <unistd.h>

#include "MemoryPool.h"

using namespace std;

static const int numberOfObj = 10;

class DummyObj
{
public:
	int a;
	DummyObj()
	: a(-1)
	{
		cout << "Dummy object created: " << a << endl;
	}
	~DummyObj()
	{
		cout << "Dummy object destroyed: " << a << endl;
	}
};

void worker(MemoryPool<DummyObj>& mp, int threadId)
{
    vector<DummyObj*> objList(numberOfObj, nullptr);
    for (int i = 0; i < (int)objList.size(); i++)
    {
        objList[i] = mp.alloc();
        objList[i]->a = i + 1;
        usleep(1000);
        if (i % 2 == 1)
        {
            cout << "Thread ID: " << threadId << " obj" << i << ": "<< objList[i-1]->a << " " << " obj" << i + 1 << ": "<< objList[i]->a << endl;
            mp.free(objList[i-1]);
            mp.free(objList[i]);
        }
    }
}

int main()
{
	MemoryPool<DummyObj> mp(1);
	thread thread1(worker, ref(mp), 1);
	thread thread2(worker, ref(mp), 2);
	thread1.join();
	thread2.join();
	return 0;
}
