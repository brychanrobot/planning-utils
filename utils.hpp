#pragma once

#include <cstdlib>
#include <cmath>
// #include <math.h>
#include "geom/Rect.hpp"
#include "geom/utils.hpp"

inline double randDouble(double bottom, double top) {
	double randNum = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
	return bottom + randNum * (top - bottom);
}

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

// algorithm from http://playtechs.blogspot.com/2007/03/raytracing-on-grid.html
inline bool lineIntersectsObstacles(int x0, int y0, int x1, int y1, std::vector<std::vector<bool>>* obstacleHash, int width, int height) {
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int x = x0;
	int y = y0;
	int n = 1 + dx + dy;
	int x_inc = (x1 > x0) ? 1 : -1;
	int y_inc = (y1 > y0) ? 1 : -1;
	int error = dx - dy;
	dx *= 2;
	dy *= 2;

	for (; n > 0; --n) {
		if (x > 0 && x < width && y > 0 && y < height && (*obstacleHash)[y][x]) {
			return true;
		}

		if (error > 0) {
			x += x_inc;
			error -= dy;
		} else {
			y += y_inc;
			error += dx;
		}
	}

	return false;
}

inline bool lineIntersectsObstacles(Coord p1, Coord p2, std::vector<std::vector<bool>>* obstacleHash, int width, int height) {
	return lineIntersectsObstacles(p1.x, p1.y, p2.x, p2.y, obstacleHash, width, height);
}
