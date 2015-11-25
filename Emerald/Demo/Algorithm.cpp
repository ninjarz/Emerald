// Demo
#if 1
#include "../Emerald.h"


//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	EEIntervalTree<int> tree;
	tree.Insert(1.f, 2.f, 1);
	tree.Insert(2.f, 3.f, 2);
	tree.Insert(3.f, 4.f, 3);
	tree.Insert(4.f, 5.f, 4);
	tree.Insert(5.f, 6.f, 5);

	std::vector<int> data;
	tree.FindData(3.f, 4.f, data);

	return 0;
}

#endif