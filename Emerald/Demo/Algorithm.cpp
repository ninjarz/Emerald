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

	EERedBlackTree<int> rbTree;
	rbTree.Insert(6.f, 1);
	rbTree.Insert(3.f, 1);
	rbTree.Insert(5.f, 1);
	rbTree.Insert(2.f, 1);
	rbTree.Insert(20.f, 1);
	rbTree.Insert(1.f, 1);
	rbTree.Delete(5.f, 1);
	std::vector<int> rbData;
	rbTree.GetData(rbData);
	for (auto& rbTmp : rbTree)
	{
		rbData.push_back(rbTmp);
	}

	EERouletteWheel<int> rouletteWheel;
	rouletteWheel.Insert(50, 1);
	rouletteWheel.Insert(30, 2);
	rouletteWheel.Insert(10, 3);
	int rwResult = 0;
	rouletteWheel.Select(-30, rwResult);
	rouletteWheel.Delete(10, 3);
	
	EEGeneController geneController;
	while (geneController.Epoch());

	return 0;
}

#endif