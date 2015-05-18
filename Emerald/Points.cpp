//Points Demo
#if 1
#include "Emerald.h"

//----------------------------------------------------------------------------------------------------
void putpolywiths1(int* p, int n, EEPoints2D& points) {
	float alpha = EE_PI / 4, k = tan(alpha), db = 2;

	// get range of b
	// TODO: here magic
	float bmin = 1000, bmax = -1;
	auto getb = [](int x, int y, int k) {
		return y - k*x;
	};
	for (int i = 0; i < n; i += 1) {
		int b = getb(p[i * 2], p[i * 2 + 1], k);
		bmin = min(b, bmin);
		bmax = max(b, bmax);
	}
	struct Line{ int k, b; };
	struct Point{ int x, y; };
	struct Line2{ Point a, b; };
	auto getintersect = [](int x0, int y0, int x1, int y1, int b, float k, int& ix, int& iy) {
		if (abs(k*x0 + y1 - k*x1 + y0) <= 0.01) return 0;
		// k f
		float t = k*(x0 - x1) - y0 + y1;
		ix = (b*(x1 - x0) + x0*y1 - x1*y0) / t;
		iy = ((-b*y0) + b*y1 + k*x0*y1 - k*x1*y0) / t;
		if (x0>x1) std::swap(x0, x1);
		if (y0>y1) std::swap(y0, y1);
		if (ix < x0 || x1 < ix || iy < y0 || y1 < iy) return 0; // box detection
		return 1;
	};
	for (int b = bmin; b < bmax; b += db) {
		int x0(0), y0(0), x1(0), y1(0);
		int i;
		for (i = 0; i < n; ++i) {
			int t = i * 2;
			if (getintersect(p[t], p[t + 1], p[(t + 2) % (2 * n)], p[(t + 3) % (2 * n)], b, k, x0, y0)) 
				break;
		}
		for (++i; i < n; ++i) {
			int t = i * 2;
			if (getintersect(p[t], p[t + 1], p[(t + 2) % (2 * n)], p[(t + 3) % (2 * n)], b, k, x1, y1))
			{
				if (x0 == x1 && y0 == y1)
					continue;
				break;
			}
		}
		printf("%d %d %d %d\n", x0, y0, x1, y1);
		points.AddPoints(EEDDALine(FLOAT2(x0, y0), FLOAT2(x1, y1)));
	}
	int a = 2;
}

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	EEDesc desc;
	desc.applicationName = L"Emerald";	//窗口名称
	desc.isFullScreen = false;			//是否全屏
	desc.width = 800;					//窗口宽度
	desc.height = 450;					//窗口高度
	desc.isSSAA = false;				//是开启抗锯齿
	desc.isVsync = false;				//是否垂直同步
	EEInitialize(desc);

	std::vector<FLOAT2> pointsData;
	for (float t = 0; t < 1.0f; t += 0.001f)
		pointsData.push_back(EEBezier(FLOAT2(100.f, 100.f), FLOAT2(200.f, 200.f), FLOAT2(100.f, 300.f), FLOAT2(400.f, 400.f), t));

	EEPoints2D points(pointsData);
	points.SetColor(EEColor::RED); 
	points.AddPoints(EEDDALine(FLOAT2(500.f, 200.f), FLOAT2(600.f, 400.f)));
	points.AddPoints(EEBresenhamLine(FLOAT2(510.f, 200.f), FLOAT2(610.f, 400.f)));
	points.AddPoints(EEBresenhamArc(FLOAT2(510.f, 200.f), 100.f));
	points.AddPoints(EEPNArc(FLOAT2(100.f, 200.f), 100.f));
	int tmp[16] = { 100, 100, 100, 300, 300, 300, 300, 100 };
	putpolywiths1(tmp, 4, points);

	EELine2D line(FLOAT2(0.0f, 0.0f), FLOAT2(200.0f, 200.0f), EEColor::WHITE);

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);
		EEShowFPSInTitle(L"- -");

		points.Process();
		line.Process();
		//printf("%d ", EEGetFPS());
		EEEndScene();
	}

	EEShutdown();
	return 0;
}
#endif