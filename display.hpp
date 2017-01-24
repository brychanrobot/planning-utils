#include <functional>
#include <memory>
#include <set>
#include <vector>
#include <cmath>
#include <GLFW/glfw3.h>
#include "libs/color.hpp"
#include "geom/Coord.hpp"
#include "geom/Rect.hpp"
#include "Node.hpp"

static void onError(int error, const char* description) { fputs(description, stderr); }

void initDisplay(int width, int height) {
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1.f, 1.f);
	glScalef(1, -1, 1);
	glTranslatef(0, -height, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	// glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

static void onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
	}
}

inline void setColor(HSL& hsl, double alpha = 1) {
	auto rgb = HSLToRGB(hsl);
	glColor4d(rgb.R, rgb.G, rgb.B, alpha);
}

inline void drawHollowCircle(Coord point, double radius, HSL hsl, double lineThickness = 2) {
	glEnable(GL_LINE_SMOOTH);

	int numTriangles = 4 * radius;
	double stepAngle = 2 * M_PI / numTriangles;
	setColor(hsl);

	glLineWidth(lineThickness);
	glBegin(GL_LINE_LOOP);

	for (int v = 0; v < numTriangles; v++) {
		auto angle = v * stepAngle;
		auto x = cos(angle) * radius + point.x;
		auto y = sin(angle) * radius + point.y;
		glVertex2d(x, y);
	}

	glEnd();

	glDisable(GL_LINE_SMOOTH);
}

inline void drawSolidCircle(Coord point, double radius, HSL hsl, double alpha = 1) {
	glEnable(GL_POLYGON_SMOOTH);

	int numTriangles = 4 * radius;
	double stepAngle = 2 * M_PI / numTriangles;
	setColor(hsl, alpha);

	glBegin(GL_TRIANGLE_FAN);

	glVertex2d(point.x, point.y);
	for (int v = 0; v < numTriangles + 1; v++) {
		auto angle = v * stepAngle;
		auto x = cos(angle) * radius + point.x;
		auto y = sin(angle) * radius + point.y;
		glVertex2d(x, y);
	}

	glEnd();

	glDisable(GL_POLYGON_SMOOTH);
}

inline void drawPoint(Coord point, double radius, HSL hsl) {
	/*glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPointSize(radius);
	setColor(hsl);

	glBegin(GL_POINTS);
	glVertex2d(point.x, point.y);
	glEnd();
	*/
	drawSolidCircle(point, radius, hsl);
	// drawHollowCircle(point, radius, hsl);
}

inline void drawLine(Coord start, Coord end, int linewidth, HSL hsl) {
	setColor(hsl);
	glLineWidth(linewidth);
	glBegin(GL_LINES);
	glVertex2d(start.x, start.y);
	glVertex2d(end.x, end.y);
	glEnd();
}

inline void drawTree(const std::shared_ptr<Node>& root, HSL hsl) {
	for (auto child : root->children) {
		drawLine(root->coord, child->coord, 1, hsl);
		drawTree(child, hsl);
	}
}

inline void drawGraphRecursive(const std::shared_ptr<Node>& node, std::set<std::shared_ptr<Node>>& visited) {
	visited.insert(node);
	for (auto child : node->children) {
		HSL* hsl;
		int linewidth = 3;
		switch (child->status) {
			case Status::Unvisited:
				hsl = new HSL(100, 1, 0.5);
				linewidth = 1;
				break;
			case Status::Open:
				hsl = new HSL(50, 1, 0.5);
				break;
			case Status::Closed:
				hsl = new HSL(250, 1, 0.5);
				break;
			default:
				hsl = new HSL(360, 1, 0.5);
		}

		drawLine(node->coord, child->coord, linewidth, *hsl);
		if (visited.find(child) == visited.end()) {
			drawGraphRecursive(child, visited);
		}
	}
}

/*
void drawGraphIterative(set<Node*>& visited, unordered_map<Node*, std::vector<Node*>> visibilityGraph) {
    for (auto kv_pair : visibilityGraph) {
        auto parent = kv_pair.first;

        if (visited.find(parent) != visited.end()) {
            continue;
        }

        for (auto child : kv_pair.second) {
            HSL* hsl;
            int linewidth = 3;
            switch (child->status) {
                case Status::Unvisited:
                    hsl = new HSL(100, 1, 0.5);
                    linewidth = 1;
                    break;
                case Status::Open:
                    hsl = new HSL(50, 1, 0.5);
                    linewidth = 1;
                    break;
                case Status::Closed:
                    hsl = new HSL(250, 1, 0.5);
                    break;
                default:
                    hsl = new HSL(360, 1, 0.5);
            }

            if (child->status == Status::Closed) {
                drawLine(parent->coord, child->coord, *hsl, linewidth);
            }
        }
    }
}

void drawGraph(Node* root, unordered_map<Node*, std::vector<Node*>> visibilityGraph) {
    set<Node*> visited;
    // drawGraphRecursive(root, visited);
    drawGraphIterative(visited, visibilityGraph);
}
*/

inline void drawPath(std::deque<Coord>& path, HSL lineHsl, HSL pointHsl) {
	for (auto point : path) {
		drawPoint(point, 2, pointHsl);
	}

	glEnable(GL_LINE_SMOOTH);
	glLineWidth(3);
	// glColor3d(0.0, 1.0, 0.2);
	setColor(lineHsl);

	glBegin(GL_LINE_STRIP);
	for (auto point : path) {
		glVertex2d(point.x, point.y);
	}
	glEnd();

	glDisable(GL_LINE_SMOOTH);
}

inline void drawObstacles(std::vector<std::shared_ptr<Rect>>* obstacleRects, const double OBSTACLE_PADDING, HSL hsl) {
	// glColor3d(0.0, 1.0, 0.5);
	setColor(hsl);
	glBegin(GL_QUADS);
	for (auto obstacle : *obstacleRects) {
		glVertex2d(obstacle->topLeft.x + OBSTACLE_PADDING, obstacle->topLeft.y + OBSTACLE_PADDING);
		glVertex2d(obstacle->bottomRight.x - OBSTACLE_PADDING, obstacle->topLeft.y + OBSTACLE_PADDING);
		glVertex2d(obstacle->bottomRight.x - OBSTACLE_PADDING, obstacle->bottomRight.y - OBSTACLE_PADDING);
		glVertex2d(obstacle->topLeft.x + OBSTACLE_PADDING, obstacle->bottomRight.y - OBSTACLE_PADDING);
	}
	glEnd();
}

inline GLFWwindow* initWindow(bool isFullscreen, int monitorNum, int& width, int& height) {
	GLFWwindow* window;
	glfwSetErrorCallback(onError);
	if (!glfwInit()) exit(EXIT_FAILURE);

	GLFWmonitor* monitor = NULL;
	// if (options["fullscreen"].as<bool>()) {

	if (isFullscreen) {
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		monitor = monitors[monitorNum];
		auto vidMode = glfwGetVideoMode(monitor);
		width = vidMode->width;
		height = vidMode->height;
	}

	window = glfwCreateWindow(width, height, "Simple example", monitor, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, onKey);

	initDisplay(width, height);

	return window;
}

inline void close(GLFWwindow* window) {
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

inline void displayLoop(GLFWwindow* window, double frameRate, std::function<void()> display, std::function<void()> remainder) {
	auto lastFrame = glfwGetTime();
	auto frameInterval = 1.0 / frameRate;

	while (!glfwWindowShouldClose(window)) {
		auto currentTime = glfwGetTime();
		if (currentTime - lastFrame >= frameInterval) {
			lastFrame = currentTime;

			glClear(GL_COLOR_BUFFER_BIT);

			display();

			glfwSwapBuffers(window);
			glfwPollEvents();
			// printf("drawing\n");
		} else {
			remainder();
		}
	}

	close(window);
}