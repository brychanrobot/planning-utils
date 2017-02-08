
#include <cstdio>
#include <cstdlib>
#include <deque>
#include "display.hpp"
#include "geom/Coord.hpp"
#include "geom/Rect.hpp"
#include "geom/utils.hpp"
#include "libs/cxxopts.hpp"
#include "utils.hpp"

using namespace std;

const double OBSTACLE_PADDING = 5;

void display(deque<Coord>& path, std::shared_ptr<vector<shared_ptr<Rect>>> obstacleRects) {
	drawObstacles(obstacleRects, OBSTACLE_PADDING, HSL(275, 1.0, 0.5));

	drawPath(path, HSL(100, 1.0, 0.3), HSL(150, 1.0, 0.5));

	if (path.size() > 1) {
		drawPoint(path.front(), 20, HSL(25, 1.0, 0.5));
		drawPoint(path.back(), 20, HSL(50, 1.0, 0.5));
	}
}

Coord randomOpenAreaPoint(int width, int height, std::shared_ptr<vector<vector<bool>>> obstacleHash) {
	auto point = randomPoint(width, height);
	while ((*obstacleHash)[(int)point.y][(int)point.x]) {
		point = randomPoint(width, height);
	}

	return point;
}

int main(int argc, char* argv[]) {
	srand(time(0));

	int width = 700;
	int height = 700;
	bool isFullscreen = false;
	int monitorNum = 0;

	// clang-format off
	cxxopts::Options options("Example Planner", "A cool program for cool things");
	options.add_options()
		("f,fullscreen", "Enable Fullscreen", cxxopts::value(isFullscreen))
		("m,monitor", "Set Monitor Number", cxxopts::value(monitorNum));
	// clang-format on

	options.parse(argc, argv);

	auto window = initWindow(isFullscreen, monitorNum, width, height);

	auto obstacleRects = generateObstacleRects(width, height, 10, OBSTACLE_PADDING);
	auto obstacleHash = generateObstacleHash(width, height, obstacleRects);

	deque<Coord> path;

	auto displayCallback = [&path, obstacleRects]() { display(path, obstacleRects); };

	auto lastPointAdd = glfwGetTime();
	auto pointAddFrequency = 5.0;
	auto pointAddInterval = 1.0 / pointAddFrequency;

	auto remainderCallback = [&path, &lastPointAdd, &pointAddInterval, &width, &height, obstacleHash]() {
		auto currentTime = glfwGetTime();
		if (currentTime - lastPointAdd >= pointAddInterval) {
			lastPointAdd = currentTime;

			auto newPoint = randomOpenAreaPoint(width, height, obstacleHash);
			if (path.size() != 0) {
				while (lineIntersectsObstacles(newPoint, path.back(), obstacleHash, width, height)) {
					newPoint = randomOpenAreaPoint(width, height, obstacleHash);
				}
			}
			path.push_back(randomOpenAreaPoint(width, height, obstacleHash));
		}
	};

	displayLoop(window, 30.0, displayCallback, remainderCallback);
}
