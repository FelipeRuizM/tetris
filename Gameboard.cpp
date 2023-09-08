#include "Gameboard.h"

Gameboard::Gameboard() {
		empty();
}

void Gameboard::empty() {
		for (int y = 0; y < MAX_Y; y++) {
				for (int x = 0; x < MAX_X; x++) {
						setContent(x, y, EMPTY_BLOCK);
				}
		}
}

void Gameboard::printToConsole() const {
		for (int y = 0; y < MAX_Y; y++) {
				for (int x = 0; x < MAX_X; x++) {
						if (grid[y][x] == EMPTY_BLOCK) {
								std::cout << std::setw(2) << '.';
						}
						else {
								std::cout << std::setw(2) << grid[y][x];
						}
				}
				std::cout << std::endl;
		}
		std::cout << std::endl;
}

int Gameboard::getContent(const Point& point) const {
		return getContent(point.getX(), point.getY());
}

int Gameboard::getContent(const int x, const int y) const {
		assert(isValidPoint(x, y));
		return grid[y][x];
}

void Gameboard::setContent(const std::vector<Point>& points, const int content) {
		for (const Point& point : points) {
				setContent(point, content);
		}
}

void Gameboard::setContent(const Point& point, const int content) {
		setContent(point.getX(), point.getY(), content);
}

void Gameboard::setContent(const int x, const int y, const int content) {
		if (isValidPoint(x, y)) {
				grid[y][x] = content;
		}
}

bool Gameboard::areAllLocsEmpty(const std::vector<Point>& points) const {
		for (const Point& point : points) {
				if (isValidPoint(point)) {
						if (getContent(point) != EMPTY_BLOCK) {
								return false;
						}
				}
		}
		return true;
}

int Gameboard::removeCompletedRows() {
		std::vector<int> completedRows = getCompletedRowIndices();
		removeRows(completedRows);
		return completedRows.size();
}

bool Gameboard::isValidPoint(const Point& point) const {
		return isValidPoint(point.getX(), point.getY());
}

bool Gameboard::isValidPoint(const int x, const int y) const {
		return x < MAX_X && y < MAX_Y && x >= 0 && y >= 0;
}

bool Gameboard::isRowCompleted(const int y) const {
		for (int x = 0; x < MAX_X; x++) {
				if (getContent(x, y) == EMPTY_BLOCK) {
						return false;
				}
		}
		return true;
}

void Gameboard::fillRow(const int y, const int content) {
		for (int x = 0; x < MAX_X; x++) {
				setContent(x, y, content);
		}
}

std::vector<int> Gameboard::getCompletedRowIndices() const {
		std::vector<int> completedRows{};
		for (int y = 0; y < MAX_Y; y++) {
				if (isRowCompleted(y)) {
						completedRows.push_back(y);
				}
		}
		return completedRows;
}

void Gameboard::copyRowIntoRow(const int sourceY, const int targetY) {
		for (int x = 0; x < MAX_X; x++) {
				setContent(x, targetY, grid[sourceY][x]);
		}
}

void Gameboard::removeRow(const int row) {
		if (!isValidPoint(0, row)) {
				return;
		}
		for (int y = row - 1; y >= 0; y--) {
				copyRowIntoRow(y, y + 1);
		}
		fillRow(0, EMPTY_BLOCK);
}

void Gameboard::removeRows(const std::vector<int>& rows) {
		for (int row : rows) {
				removeRow(row);
		}
}