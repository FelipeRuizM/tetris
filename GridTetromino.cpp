#include "GridTetromino.h"

GridTetromino::GridTetromino() {
		gridLoc.setXY(0, 0);
}

void GridTetromino::setGridLoc(int x, int y) {
		gridLoc.setXY(x, y);
}

void GridTetromino::setGridLoc(const Point& point) {
		gridLoc.setXY(point.getX(), point.getY());
}

void GridTetromino::move(int xOffset, int yOffset) {
		gridLoc.setXY(gridLoc.getX() + xOffset, gridLoc.getY() + yOffset);
}

std::vector<Point> GridTetromino::getBlockLocsMappedToGrid() const {
		std::vector<Point> blockLocsMappedToGrid;
		for (const Point& point : blockLocs) {
				blockLocsMappedToGrid.push_back(Point{ point.getX() + gridLoc.getX(), point.getY() + gridLoc.getY() });
		}
		return blockLocsMappedToGrid;
}