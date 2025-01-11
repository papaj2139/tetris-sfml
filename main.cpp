#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iostream>


const int width = 10;   
const int height = 20;  
const int blockSize = 30;  

struct Tetromino {
    std::vector<std::array<int, 2>> shape;
    sf::Color color;
};

const std::vector<Tetromino> tetrominoes = {
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}}, sf::Color::Cyan},    // i 
    {{{0, 0}, {0, 1}, {1, 0}, {1, 1}}, sf::Color::Yellow},  // o
    {{{0, 0}, {1, 0}, {2, 0}, {1, 1}}, sf::Color::Magenta}, // t
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}, sf::Color::Green},   // s ( who the fuck named this it DOES NOT look like S lmao)
    {{{0, 1}, {1, 1}, {1, 0}, {2, 0}}, sf::Color::Red},     // z
    {{{0, 0}, {0, 1}, {1, 1}, {2, 1}}, sf::Color::Blue},    // l
    {{{0, 1}, {1, 1}, {2, 1}, {2, 0}}, sf::Color::White}    // j
};

class Tetris {
private:
    std::array<std::array<sf::Color, width>, height> grid;
    Tetromino current;
    Tetromino nextTetromino;
    int x, y;
    bool gameOver = false;
    int score = 0;
    sf::Font font;

public:
    Tetris() {
        for (auto& row : grid) {
            row.fill(sf::Color::Transparent);
        }
        
        // random block 
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        
        nextTetromino = tetrominoes[rand() % tetrominoes.size()];
        resetTetromino();
    }

    void resetTetromino() {
        current = nextTetromino;
        nextTetromino = tetrominoes[rand() % tetrominoes.size()];
        x = width / 2 - 1;
        y = 0;
        
        if (checkCollision(0, 0)) {
            gameOver = true;
        }
    }

    bool checkCollision(int dx, int dy) {
        for (const auto& block : current.shape) {
            int nx = x + block[0] + dx;
            int ny = y + block[1] + dy;

            if (nx < 0 || nx >= width || ny >= height) return true;
            if (ny >= 0 && grid[ny][nx] != sf::Color::Transparent) return true;
        }
        return false;
    }

    void mergeTetromino() {
        for (const auto& block : current.shape) {
            int nx = x + block[0];
            int ny = y + block[1];
            if (ny >= 0 && ny < height && nx >= 0 && nx < width) {
                grid[ny][nx] = current.color;
            }
        }
    }

    void clearLines() {
        for (int row = height - 1; row >= 0; --row) {
            bool full = true;
            for (int col = 0; col < width; ++col) {
                if (grid[row][col] == sf::Color::Transparent) {
                    full = false;
                    break;
                }
            }

            if (full) {
                score += 100;
                for (int r = row; r > 0; --r) {
                    grid[r] = grid[r - 1];
                }
                grid[0].fill(sf::Color::Transparent);
                row++; 
            }
        }
    }

    void rotateTetromino() {
        if (gameOver) return;
        
        auto oldShape = current.shape;
        for (auto& block : current.shape) {
            int temp = block[0];
            block[0] = -block[1];
            block[1] = temp;
        }
        
        if (checkCollision(0, 0)) {
            current.shape = oldShape;
        }
    }

    void move(int dx) {
        if (!gameOver && !checkCollision(dx, 0)) {
            x += dx;
        }
    }

    void drop() {
        if (gameOver) return;
        
        if (!checkCollision(0, 1)) {
            y++;
        } else {
            mergeTetromino();
            clearLines();
            resetTetromino();
        }
    }

    void hardDrop() {
        if (gameOver) return;
        
        while (!checkCollision(0, 1)) {
            y++;
        }
        mergeTetromino();
        clearLines();
        resetTetromino();
    }

    void draw(sf::RenderWindow& window) {
        for (int row = 0; row < height; ++row) {
            for (int col = 0; col < width; ++col) {
                sf::RectangleShape cell(sf::Vector2f(blockSize - 1, blockSize - 1));
                cell.setPosition(col * blockSize, row * blockSize);
                cell.setOutlineThickness(1);
                cell.setOutlineColor(sf::Color(50, 50, 50));
                
                if (grid[row][col] != sf::Color::Transparent) {
                    cell.setFillColor(grid[row][col]);
                } else {
                    cell.setFillColor(sf::Color(20, 20, 20));
                }
                
                window.draw(cell);
            }
        }


        if (!gameOver) {
            for (const auto& block : current.shape) {
                int nx = x + block[0];
                int ny = y + block[1];
                if (ny >= 0) {
                    sf::RectangleShape rect(sf::Vector2f(blockSize - 1, blockSize - 1));
                    rect.setPosition(nx * blockSize, ny * blockSize);
                    rect.setFillColor(current.color);
                    rect.setOutlineThickness(1);
                    rect.setOutlineColor(sf::Color::White);
                    window.draw(rect);
                }
            }
        }

        //all the fucking ui 
        if (font.getInfo().family != "") {
            sf::Text nextText;
            nextText.setFont(font);
            nextText.setString("Next Block:");
            nextText.setCharacterSize(20);
            nextText.setFillColor(sf::Color::White);
            nextText.setPosition(width * blockSize + 10, 50);
            window.draw(nextText);

            for (const auto& block : nextTetromino.shape) {
                sf::RectangleShape rect(sf::Vector2f(blockSize - 1, blockSize - 1));
                rect.setFillColor(nextTetromino.color);
                float previewX = (width + 1 + block[0]) * blockSize;
                float previewY = (3 + block[1]) * blockSize;
                rect.setPosition(previewX, previewY);
                rect.setOutlineThickness(1);
                rect.setOutlineColor(sf::Color::White);
                window.draw(rect);
            }
        }

        if (font.getInfo().family != "") {
            sf::Text scoreText;
            scoreText.setFont(font);
            scoreText.setString("Score: " + std::to_string(score));
            scoreText.setCharacterSize(20);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition(width * blockSize + 10, 10);
            window.draw(scoreText);
        }

        if (gameOver) {
            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("Game Over!");
            gameOverText.setCharacterSize(40);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(
                (width * blockSize - 200) / 2,
                height * blockSize / 2
            );
            window.draw(gameOverText);
        }
    }

    void loadFont() {
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "error loading font" << std::endl;
        }
    }

    bool isGameOver() const { return gameOver; }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(width * blockSize + 200, height * blockSize), "Tetris");
    window.setFramerateLimit(60);

    Tetris game;
    game.loadFont();

    sf::Clock clock;
    float dropTime = 0;
    const float dropInterval = 0.5f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && !game.isGameOver()) {
                switch (event.key.code) {
                    case sf::Keyboard::Left:
                        game.move(-1);
                        break;
                    case sf::Keyboard::Right:
                        game.move(1);
                        break;
                    case sf::Keyboard::Up:
                        game.rotateTetromino();
                        break;
                    case sf::Keyboard::Down:
                        game.drop();
                        break;
                    case sf::Keyboard::Space:
                        game.hardDrop();
                        break;
                    default:
                        break;
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();
        dropTime += deltaTime;
        
        if (dropTime >= dropInterval) {
            game.drop();
            dropTime = 0;
        }

        window.clear(sf::Color(40, 40, 40));
        game.draw(window);
        window.display();
    }

    return 0;
}