
#include <cstdio>
#include <cstdlib>
#include <deque>
#include "libs/cxxopts.hpp"
#include "geom/Coord.hpp"
#include "geom/Rect.hpp"
#include "geom/utils.hpp"
#include "display.hpp"

using namespace std;

const double OBSTACLE_PADDING = 5;

void display(vector<Coord>& path, vector<shared_ptr<Rect>>* obstacleRects) {
	drawObstacles(obstacleRects, OBSTACLE_PADDING, HSL(275, 1.0, 0.5));

	drawPath(path, HSL(100, 1.0, 0.3), HSL(150, 1.0, 0.5));

	drawPoint(path.front(), 20, HSL(25, 1.0, 0.5));
	drawPoint(path.back(), 20, HSL(50, 1.0, 0.5));
}

int main(int argc, char* argv[]) {
	srand(time(0));

	int width = 700;
	int height = 700;
	bool isFullscreen = false;

	// clang-format off
	cxxopts::Options options("OnlineRRT*", "A cool program for cool things");
	options.add_options()
		("f,fullscreen", "Enable Fullscreen", cxxopts::value(isFullscreen))
		("m,monitor", "Set Monitor Number", cxxopts::value(monitorNum))
	    // clang-format on

	    options.parse(argc, argv);

	auto window = initWindow(isFullscreen, monitorNum, width, height);

	vector<shared_ptr<Rect>> obstacleRects;
	generateObstacleRects(width, height, 10, obstacleRects, OBSTACLE_PADDING);

	vector<vector<bool>> obstacleHash(height, vector<bool>(width, false));
	generateObstacleHash(obstacleRects, obstacleHash);

	deque<Coord> path;

	auto displayCallback = [&path, obstacleRects]() { display(path, obstacleRects); };

	auto lastPointAdd = glfwGetTime();
	auto pointAddFrequency = 5.0;
	auto pointAddInterval = 1.0 / pointAddFrequency;

	auto remainderCallback = [&path, &pointAddInterval]() {
		auto currentTime = glfwGetTime();
		if (currentTime - lastPointAdd >= pointAddInterval) {
			lastPointAdd = currentTime;

			path.push_back(randomPoint(width, height));
		}
	};

	displayLoop(window, 30.0, displayCallback, remainderCallback);
}
