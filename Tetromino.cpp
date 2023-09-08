#include "Tetromino.h"

Tetromino::Tetromino() {
		setShape(TetShape::O);
}

void Tetromino::setShape(TetShape shape) {
		this->shape = shape;
		switch (shape) {
				case TetShape::O: {
						color = TetColor::YELLOW;
						blockLocs = { Point {0, 1},  Point {1, 1}, Point {0, 0}, Point {1, 0} };
						break;
				}
				case TetShape::I: {
						color = TetColor::BLUE_LIGHT;
						blockLocs = { Point (0, 2),  Point (0, 1), Point (0, 0), Point (0, -1) };
						break;
				}
				case TetShape::S: {
						color = TetColor::RED;
						blockLocs = { Point {0, 1},  Point {1, 1}, Point {-1, 0}, Point {0, 0} };
						break;
				}
				case TetShape::Z: {
						color = TetColor::GREEN;
						blockLocs = { Point {-1, 1},  Point {0, 1}, Point {0, 0}, Point {1, 0} };
						break;
				}
				case TetShape::L: {
						color = TetColor::ORANGE;
						blockLocs = { Point {0, 1},  Point {0, 0}, Point {0, -1}, Point {1, -1} };
						break;
				}
				case TetShape::J: {
						color = TetColor::BLUE_DARK;
						blockLocs = { Point {0, 1},  Point {0, 0}, Point {-1, -1}, Point {0, -1} };
						break;
				}
				case TetShape::T: {
						color = TetColor::PURPLE;
						blockLocs = { Point {-1, 0},  Point {0, 0}, Point {1, 0}, Point {0, -1} };
						break;
				}
		}
}

// print a grid to display the current shape
// to do this: print out a “grid” of text to represent a co-ordinate
// system. Start at top left [-3,3] go to bottom right [3,-3]
// (use nested for loops)
// for each [x,y] point, loop through the blockLocs and if the point exists
// in the list, print an 'x' instead of a '.'. You should end up with something
// like this: (results will vary depending on shape and rotation, eg: this
// one shows a T shape rotated clockwise once)
// .......
// .......
// ...x...
// ..xx...
// ...x...
// .......

void Tetromino::printToConsole() const {
		for (int y = 3; y > -3; y--) {
				std::cout << std::endl;
				for (int x = -3; x < 3; x++) {
						if (pointFound(x, y)) {
								std::cout << "X";
						}
						else {
								std::cout << ".";
						}
				}
		}
}

// rotate the shape 90 degrees around [0,0] (clockwise)
// to do this:
// - iterate through blockLocs
// - rotate each Point 90 degrees clockwise around [0,0]
// hint: as you rotate the point [1,2] clockwise around [0,0], note
// how the x,y values change. Once you find the pattern you can use
// a couple existing methods in the Point class to accomplish this.
// make it so that the TetShape::O doesn’t rotate
void Tetromino::rotateClockwise() {
		if (shape != TetShape::O) {
				for (int i = 0; i < blockLocs.size(); i++) {
						blockLocs[i].setX(blockLocs[i].getX() * -1);
						blockLocs[i].swapXY();
				}
		}
}

bool Tetromino::pointFound(int x, int y) const {
		for (int i = 0; i < blockLocs.size(); i++) {
				if (blockLocs[i].getX() == x) {
						if (blockLocs[i].getY() == y) {
								return true;
						}
				}
		}
		return false;
}

TetShape Tetromino::getRandomShape() {
		return static_cast<TetShape>(rand() % static_cast<int>(TetShape::shapeCount));
}

TetColor Tetromino::getRandomColor() {
		return static_cast<TetColor>(rand() % static_cast<int>(TetColor::colorCount));
}