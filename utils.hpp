#pragma once

#include <cstdlib>
#include <cmath>
// #include <math.h>
#include "geom/Rect.hpp"
#include "geom/utils.hpp"

inline bool hasIntersection(Rect& rect, std::vector<std::shared_ptr<Rect>>& obstacles) {
	for (const auto& obstacle : obstacles) {
		if (obstacle->intersects(rect)) {
			return true;
		}
	}

	return false;
}

inline void generateObstacleRects(int width, int height, int count, std::vector<std::shared_ptr<Rect>>& obstacles, double padding = 5) {
	for (int x = 0; x < count; x++) {
		std::shared_ptr<Rect> rect;
		while (true) {
			auto topLeft = randomPoint(width, height);
			auto bottomRight = randomPoint(width, height);

			// delete rect;
			rect = std::make_shared<Rect>(topLeft, bottomRight);
			if (rect->width() > padding * 2 && rect->height() > padding * 2 && rect->width() < 0.5 * width && rect->height() < 0.5 * height &&
			    !hasIntersection(*rect, obstacles)) {
				break;
			}
		}

		obstacles.push_back(rect);
	}
}

inline void generateObstacleHash(std::vector<std::shared_ptr<Rect>>& obstacleRects, std::vector<std::vector<bool>>& obstacleHash) {
	for (const auto& obstacle : obstacleRects) {
		for (int r = obstacle->topLeft.y; r < obstacle->bottomRight.y; r++) {
			for (int c = obstacle->topLeft.x; c < obstacle->bottomRight.x; c++) {
				obstacleHash[r][c] = true;
			}
		}
	}
}

inline bool lineIntersectsObstacles(Coord p1, Coord p2, std::vector<std::vector<bool>>* obstacleHash, int width, int height) {
	auto dx = p2.x - p1.x;
	auto dy = p2.y - p1.y;

	if (p1.x < 0 || p1.y < 0 || p2.x < 0 || p2.y < 0) {
		return true;
	}

	/*
	auto m = 20000.0;  // a big number for vertical slope

	if (abs(dx) > 0.0001) {
	    m = dy / dx;
	}
	*/
	auto m = clamp(dy / dx, -20000, 20000);

	// printf("m: %f\n", m);

	auto b = -m * p1.x + p1.y;

	if (abs(m) != 20000) {
		auto minX = std::min(p1.x, p2.x);
		auto maxX = std::max(p1.x, p2.x);

		for (int ix = minX; ix <= maxX; ix++) {
			auto y = m * ix + b;
			// printf("[%.2f, %.2f]:[%.2f, %.2f] %.3f, (%d, %f)\n", p1.x, p1.y, p2.x, p2.y, m, ix, y);
			// printf("h: [%.2f, %.2f]:[%.2f, %.2f] --------dx: %f m: %f, b: %.2f, (%d, %.2f)\n", p1.x, p1.y, p2.x, p2.y, dx, m, b, ix, y);
			// printf("%.2d, %.2d\n", (int)y, ix);

			// printf("%.2f, %.2f : %d\n", p1.x, p2.x, ix);
			if (y > 0 && y < height && (*obstacleHash)[(int)y][ix]) {
				// printf("returning true\n");
				return true;
			}
		}
	}

	if (abs(m) != 0) {
		auto minY = std::min(p1.y, p2.y);
		auto maxY = std::max(p1.y, p2.y);

		for (int iy = minY; iy < maxY; iy++) {
			auto x = (iy - b) / m;
			// printf("v: [%.2f, %.2f]:[%.2f, %.2f] --------dx: %f m: %f, b: %.2f, (%.2f, %d)\n", p1.x, p1.y, p2.x, p2.y, dx, m, b, x, iy);
			if (x > 0 && x < width && (*obstacleHash)[iy][(int)x]) {
				// printf("returning true\n");
				return true;
			}
		}
	}

	return false;
}
