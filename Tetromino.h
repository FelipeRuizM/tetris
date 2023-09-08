#pragma once
#include <vector>
#include "Point.h"
enum TetColor {
		RED,
		ORANGE,
		YELLOW,
		GREEN,
		BLUE_LIGHT,
		BLUE_DARK,
		PURPLE,
		colorCount
};

enum TetShape {
		S,
		Z,
		L,
		J,
		O,
		I,
		T,
		shapeCount
};

class Tetromino
{
friend class TestSuite;
friend class GridTetromino;

private:
		TetColor color;
		TetShape shape;
		std::vector<Point> blockLocs; // initialize it (if it doesn't work, blame diego)
public:
		Tetromino();
		TetColor getColor() const { return color; }
		TetShape getShape() const { return shape; }
		void setShape(TetShape shape);
		void rotateClockwise();	
		void printToConsole() const;
		bool pointFound(int x, int y) const;
		static TetShape getRandomShape();
		static TetColor getRandomColor();
};
