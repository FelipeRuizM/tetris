#include "TetrisGame.h"

TetrisGame::TetrisGame(sf::RenderWindow& window, sf::Sprite& blockSprite, const Point& gameboardOffset, const Point& nextShapeOffset)
		: window(window), blockSprite(blockSprite), gameboardOffset(gameboardOffset), nextShapeOffset(nextShapeOffset)
{
		determineSecondsPerTick();
		//music.openFromFile("music/vinivici.ogg");
		/*if (!music.openFromFile("music/vinivici.ogg"))
		{
				assert(false && "Missing music");
		};*/
		//music.setLoop(true);
		//music.play();
		if (!scoreFont.loadFromFile("fonts/RedOctober.ttf"))
		{
				assert(false && "Missing font: RedOctober.ttf");
		};
		scoreText.setFont(scoreFont);
		scoreText.setCharacterSize(18);
		scoreText.setFillColor(sf::Color::White);
		scoreText.setPosition(425, 325);

		reset();
}


void TetrisGame::onKeyPressed(const sf::Event& event) {
		if (restartScreen) {
				if (event.key.code == sf::Keyboard::R) {
						if (restartScreen) {
								restartScreen = false;
								paused = false;
						}
				}
				return;
		}
		else if (paused) {
				if (event.key.code == sf::Keyboard::Escape) {
						paused = false;
				}
				return;
		}
		if (event.key.code == sf::Keyboard::Up) {
				attemptRotate(currentShape);
		}
		else if (event.key.code == sf::Keyboard::Left) {
				attemptMove(currentShape, -1, 0);
		}
		else if (event.key.code == sf::Keyboard::Down) {
				if (!attemptMove(currentShape, 0, 1)) {
						lock(currentShape);
				}
		}
		else if (event.key.code == sf::Keyboard::Right) {
				attemptMove(currentShape, 1, 0);
		}
		else if (event.key.code == sf::Keyboard::Space) {
				drop(currentShape);
				lock(currentShape);
		} else if (event.key.code == sf::Keyboard::Escape) {
				paused = true;
		}
}

void TetrisGame::togglePause() {
		if (paused) {
				paused = false;
		}
		else {
				paused = true;
		}
}

// called every game loop to handle ticks & tetromino placement (locking)
// - param 1: float secondsSinceLastLoop
// return: nothing
void TetrisGame::processGameLoop(float secondsSinceLastLoop) {
		secondsSinceLastTick += secondsSinceLastLoop;
		if (secondsSinceLastTick > secondsPerTick) {
				secondsSinceLastTick -= secondsPerTick;
				tick();
		}
		if (shapePlacedSinceLastGameLoop) {
				shapePlacedSinceLastGameLoop = false;
				if (spawnNextShape()) {
						pickNextShape();
						int removedRows = board.removeCompletedRows();
						if (removedRows != 0) {
								totalRemovedRows += removedRows;
								score = getScore(removedRows);
								updateLevel();
								updateScoreDisplay();
								determineSecondsPerTick();
						}
				}
				else {
						recordScore();
						scoreText.setString("Your Score: " + std::to_string(score) + "\nHigh Score: XXXXX\nPress Space to play again");
						restartScreen = true;
						paused = true;
						draw();
						reset();
				}
		}
}

void TetrisGame::tick() {
		if (!paused) {
				if (!attemptMove(currentShape, 0, 1)) {
						lock(currentShape);
				}
		}
}

void TetrisGame::reset() {
		score = 0;
		totalRemovedRows = 0;
		updateScoreDisplay();
		secondsPerTick = MAX_SECONDS_PER_TICK;
		board.empty();
		pickNextShape();
		spawnNextShape();
		pickNextShape();
		//music.stop();
		//music.play();
}

void TetrisGame::pickNextShape() {
		nextShape.setShape(Tetromino::getRandomShape());
}

bool TetrisGame::spawnNextShape() {
		nextShape.setGridLoc(board.getSpawnLoc());
		if (isPositionLegal(nextShape)) {
				nextShape.setGridLoc(0, 0);
				currentShape = nextShape;
				currentShape.setGridLoc(board.getSpawnLoc());
				return true;
		}
		return false;
}

bool TetrisGame::attemptRotate(GridTetromino& shape) {
		GridTetromino temp = shape;
		temp.rotateClockwise();
		if (isPositionLegal(temp)) {
				shape.rotateClockwise();
				return true;
		}
		return false;
}

bool TetrisGame::attemptMove(GridTetromino& shape, int x, int y) {
		GridTetromino temp = shape;
		temp.move(x, y);
		if (isPositionLegal(temp)) {
				shape.move(x, y);
				return true;
		}
		return false;
}

void TetrisGame::drop(GridTetromino& shape) {
		while (attemptMove(shape, 0, 1)) {};
}

void TetrisGame::lock(const GridTetromino& shape) {
		board.setContent(shape.getBlockLocsMappedToGrid(), static_cast<int>(shape.getColor()));
		shapePlacedSinceLastGameLoop = true;
}

void TetrisGame::draw() const {
		if (!restartScreen) {
				drawTetromino(currentShape, gameboardOffset);
				drawTetromino(nextShape, nextShapeOffset);
				drawGameboard();
				window.draw(scoreText);
		}
}

// Graphics methods ==============================================

void TetrisGame::drawBlock(const Point& topLeft, int xOffset, int yOffset, TetColor color) const {
		blockSprite.setTextureRect(sf::IntRect(BLOCK_WIDTH*static_cast<int>(color), 0, BLOCK_WIDTH, BLOCK_HEIGHT));
		blockSprite.setPosition(topLeft.getX() + xOffset * BLOCK_WIDTH, topLeft.getY() + yOffset * BLOCK_HEIGHT);
		window.draw(blockSprite);
}

void TetrisGame::drawGameboard() const {
		for (int row = 0; row < Gameboard::MAX_Y; row++) {
				for (int col = 0; col < Gameboard::MAX_X; col++) {
						if (board.getContent(col, row) != Gameboard::EMPTY_BLOCK) { 
								drawBlock(gameboardOffset, col, row, static_cast<TetColor>(board.getContent(col, row)));
						}
				}
		}
}

void TetrisGame::drawTetromino(const GridTetromino& tetromino, const Point& topLeft) const {
		for (const Point& point : tetromino.getBlockLocsMappedToGrid()) {
				drawBlock(topLeft, point.getX(), point.getY(), tetromino.getColor());
		}
}

void TetrisGame::updateLevel() {
		level = totalRemovedRows / CHANGE_LEVEL_EVERY_ROW;
		if (level > 10) {
				level = 10;
		}
}

void TetrisGame::updateScoreDisplay() {
		updateLevel();
		scoreText.setString("score: " + std::to_string(score) + "\nlevel: " + std::to_string(level));
}

int TetrisGame::getScore(int completedRowsNum) const {
		int points = 0;
		switch (completedRowsNum) {
		case 1: points = 40; break;
		case 2: points = 100; break;
		case 3: points = 300; break;
		case 4: points = 1200; break;
		}
		//std::cout << "level + points * (level + 1)" << std::endl;
		//std::cout << level  << " + " << points << " * (" << (level + 1) << ")" << std::endl;
		return score + points * (level + 1);
}

// State & gameplay/logic methods ================================

bool TetrisGame::isPositionLegal(const GridTetromino& shape) const {
		return isWithinBorders(shape) && board.areAllLocsEmpty(shape.getBlockLocsMappedToGrid());
}

bool TetrisGame::isWithinBorders(const GridTetromino& shape) const {
		for (const Point& point : shape.getBlockLocsMappedToGrid()) {
				if (!(point.getX() < Gameboard::MAX_X && point.getY() < Gameboard::MAX_Y && point.getX() >= 0)) {
						return false;
				}
		}
		return true;
}

void TetrisGame::determineSecondsPerTick() {
		secondsPerTick = MAX_SECONDS_PER_TICK - decreaseTickBy * level;
		//std::cout << "secondsPerTick => " << secondsPerTick << std::endl;
}

void TetrisGame::recordScore() {
		std::ofstream fin;
		fin.open(scores);
		if (!fin) {
				std::cout << "Error: could not open file:\"" << scores << "\"" << std::endl;
				exit(1);
		}
		if (score != 0) {
				std::cout << "Score: " << score << ", Level: " << level << std::endl;
				fin << "Score: " << score << ", Level: " << level << std::endl;
				fin.close();
		}
}