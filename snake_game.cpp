#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

const int WIDTH = 800;
const int HEIGHT = 600;
const int SIZE = 20;

enum Direction { Up, Down, Left, Right };

struct SnakeSegment {
    int x, y;
    SnakeSegment(int x, int y) : x(x), y(y) {}
};

class SnakeGame {
private:
    sf::RenderWindow window;
    std::vector<SnakeSegment> snake;
    Direction dir;
    sf::Vector2i food;
    bool isGameOver;
    int score;

    void reset() {
        snake.clear();
        snake.push_back(SnakeSegment(WIDTH / 2, HEIGHT / 2));
        dir = Right;
        placeFood();
        isGameOver = false;
        score = 0;
    }

    void placeFood() {
        food.x = (rand() % (WIDTH / SIZE)) * SIZE;
        food.y = (rand() % (HEIGHT / SIZE)) * SIZE;
    }

    void handleInput() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && dir != Down) dir = Up;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && dir != Up) dir = Down;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && dir != Right) dir = Left;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && dir != Left) dir = Right;
    }

    void update() {
        if (isGameOver) return;

        // Move the snake
        SnakeSegment newHead = snake.front();
        switch (dir) {
            case Up: newHead.y -= SIZE; break;
            case Down: newHead.y += SIZE; break;
            case Left: newHead.x -= SIZE; break;
            case Right: newHead.x += SIZE; break;
        }

        // Check for collision with the walls
        if (newHead.x < 0 || newHead.x >= WIDTH || newHead.y < 0 || newHead.y >= HEIGHT) {
            isGameOver = true;
            return;
        }

        // Check for collision with itself
        for (const auto& segment : snake) {
            if (newHead.x == segment.x && newHead.y == segment.y) {
                isGameOver = true;
                return;
            }
        }

        snake.insert(snake.begin(), newHead);

        // Check if food is eaten
        if (newHead.x == food.x && newHead.y == food.y) {
            placeFood();
            score += 10;
        } else {
            snake.pop_back();
        }
    }

    void render() {
        window.clear();

        if (isGameOver) {
            // Display Game Over message
            sf::Font font;
            if (!font.loadFromFile("arial.ttf")) {
                // error handling
            }
            sf::Text gameOverText("Game Over! Press Enter to Restart", font, 30);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(WIDTH / 4, HEIGHT / 2);
            window.draw(gameOverText);
        } else {
            // Draw snake
            for (const auto& segment : snake) {
                sf::RectangleShape rectangle(sf::Vector2f(SIZE, SIZE));
                rectangle.setFillColor(sf::Color::Green);
                rectangle.setPosition(segment.x, segment.y);
                window.draw(rectangle);
            }

            // Draw food
            sf::RectangleShape rectangle(sf::Vector2f(SIZE, SIZE));
            rectangle.setFillColor(sf::Color::Red);
            rectangle.setPosition(food.x, food.y);
            window.draw(rectangle);
        }

        window.display();
    }

public:
    SnakeGame() : window(sf::VideoMode(WIDTH, HEIGHT), "Snake Game") {
        srand(static_cast<unsigned int>(time(0)));
        reset();
    }

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && isGameOver) {
                    reset();
                }
            }

            handleInput();
            update();
            render();
            sf::sleep(sf::milliseconds(100));
        }
    }
};

int main() {
    SnakeGame game;
    game.run();
    return 0;
}
