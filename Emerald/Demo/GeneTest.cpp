// Demo
#if 0
#include "../Emerald.h"


//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	EEGeneController geneController(
		[](std::vector<boost::any> _meanings) -> float
	{
		if (!_meanings.size())
			return 0.f;

		bool isInitialized = false;
		bool isFuncLoaded = false;
		bool isNumLoaded = false;
		int left = 0;
		int right = 0;
		std::function<int(int, int)> operateFunc;
		for (unsigned int i = 0; i < _meanings.size(); ++i)
		{
			if (_meanings[i].type() == typeid(int))
			{
				if (isNumLoaded)
				{
					right *= 10;
					right += boost::any_cast<int>(_meanings[i]);
				}
				else
				{
					right = boost::any_cast<int>(_meanings[i]);
					isNumLoaded = true;
				}
			}
			else if (_meanings[i].type() == typeid(std::function<int(int, int)>))
			{
				if (!isInitialized)
				{
					left = right;
					isInitialized = true;
				}
				else if (isNumLoaded && isFuncLoaded)
				{
					std::cout << left << " op " << right;
					left = operateFunc(left, right);
					std::cout << " = " << left << std::endl;
				}

				operateFunc = boost::any_cast<std::function<int(int, int)>>(_meanings[i]);
				isNumLoaded = false;
				isFuncLoaded = true;
			}
		}
		if (isNumLoaded && isFuncLoaded)
		{
			std::cout << left << " op " << right;
			left = operateFunc(left, right);
			std::cout << " = " << left << std::endl;
		}

		float result = 1.f / (0.05f * abs(left - 100) + 1); // Option: logistic regression function
		std::cout << "Fitness:" << result << std::endl;
		return result;
	});

	geneController.AddTranslation("0000", 0);
	geneController.AddTranslation("0001", 1);
	geneController.AddTranslation("0010", 2);
	geneController.AddTranslation("0011", 3);
	geneController.AddTranslation("0100", 4);
	geneController.AddTranslation("0101", 5);
	geneController.AddTranslation("0110", 6);
	geneController.AddTranslation("0111", 7);
	geneController.AddTranslation("1000", 8);
	geneController.AddTranslation("1001", 9);
	geneController.AddTranslation("00", std::function<int(int, int)>([](int _a, int _b) -> int { return _a + _b; }));
	geneController.AddTranslation("01", std::function<int(int, int)>([](int _a, int _b) -> int { return _a - _b; }));
	geneController.AddTranslation("10", std::function<int(int, int)>([](int _a, int _b) -> int { return _a * _b; }));
	geneController.AddSample({ "0000", "00", "0000", "00", "0000", "00", "0000" });
	geneController.AddSample({ "0000", "01", "0000", "01", "0000", "01", "0000" });
	geneController.AddSample({ "0000", "10", "0000", "10", "0000", "10", "0000" });
	while (geneController.Epoch());

	return 0;
}

#endif