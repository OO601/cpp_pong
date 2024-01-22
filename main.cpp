#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <string>

bool areVectorsEqual(const sf::Vector2f& v1, const sf::Vector2f& v2) {
    return v1.x == v2.x && v1.y == v2.y;
}

int main() {

    // WINDOW
    uint width = 1600;
    uint height = 900;

    sf::RenderWindow window(sf::VideoMode(width, height), "pong");

    // PLAYER 1
    sf::Vector2f posL(100, 375);
    sf::RectangleShape playerL(sf::Vector2f(30, 150));
    playerL.setFillColor(sf::Color::White);
    playerL.setPosition(posL);
    float speedL = 750.f;
    uint scoreL = 0;

    // PLAYER 2
    sf::Vector2f posR(1470, 375);
    sf::RectangleShape playerR(sf::Vector2f(30, 150));
    playerR.setFillColor(sf::Color::White);
    playerR.setPosition(posR);
    float speedR = 750.f;
    uint scoreR = 0;

    // RANDOM
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> distributionY(0, 870);
    std::uniform_int_distribution<int> distributionS(1, 2);

    float x = distributionS(gen) / 10.f;
    float y = distributionS(gen) / 10.f; 

    // BALL
    sf::Vector2f posb(785, distributionY(gen));
    sf::RectangleShape ball(sf::Vector2f(30, 30));
    ball.setFillColor(sf::Color::White);
    ball.setPosition(posb);
    sf::Vector2f speedB(0.1f, 0.1f);

    // CLOCK    
    sf::Clock clock;

    // FONT
    sf::Font font;
    if (!font.loadFromFile("lcd.ttf")) {
        std::cout << "err";
    }

    // TEXT
    sf::Text text;
    text.setFont(font);
    text.setString(scoreL + " : " + scoreR);
    text.setCharacterSize(50);
    text.setFillColor(sf::Color(60, 60, 60));
    text.setPosition(sf::Vector2f(750, 400));

    // GAME LOOP
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Time deltaTime = clock.restart();

        // MOVEMENT
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            posL.y -= speedL * deltaTime.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            posL.y += speedL * deltaTime.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            posR.y -= speedR * deltaTime.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            posR.y += speedR * deltaTime.asSeconds();

        // BALL MOVEMENT
        posb.x += speedB.x;
        posb.y += speedB.y;

        // BOUNDRY
        if (posL.y < 0)
            posL.y = 0;
        if (posL.y > 750)
            posL.y = 750;
        if (posR.y < 0)
            posR.y = 0;
        if (posR.y > 750)
            posR.y = 750;
        
        // BALL PHYSIC
        if (posb.x < 0){
            posb.x = 785;
            float a = distributionS(gen);
            speedB = sf::Vector2f(0.1f, 0.1f);
            if (a == 1)
                speedB.x *= -1.f;
            scoreR += 1;
        }
        if (posb.y < 0){
            posb.y = 0; 
            speedB.y *= -1.f;
            }
        if (posb.x > 1570){
            posb.x = 785;
            float a = distributionS(gen);
            speedB = sf::Vector2f(0.1f, 0.1f);
            if (a == 1)
                speedB.x *= -1.f;
            scoreL += 1;
        }
        if (posb.y > 870){
            posb.y = 870; 
            speedB.y *= -1.f;
            }
        
        // BALL BOUNCE FROM PLAYERS
        sf::FloatRect pll = playerL.getGlobalBounds();
        sf::FloatRect plr = playerR.getGlobalBounds();
        sf::FloatRect bal = ball.getGlobalBounds();
        
        if (pll.intersects(bal)){
            speedB.x *= -1.f;
            posb.x = 130.f;
            if (speedB.x > 0) {
                speedB.x += 0.02f;
                if (speedB.y > 0) {
                    speedB.y += 0.02f;
                }
            }
            if (speedB.x < 0) {
                speedB.x -= 0.02f;
                if (speedB.y < 0) {
                    speedB.y -= 0.02f;
                }
            }
        }
        if (plr.intersects(bal)){
            speedB.x *= -1.f;
            posb.x = 1440.f;
            if (speedB.x > 0) {
                speedB.x += 0.02f;
                if (speedB.y > 0) {
                    speedB.y += 0.02f;
                }
            }
            if (speedB.x < 0) {
                speedB.x -= 0.02f;
                if (speedB.y < 0) {
                    speedB.y -= 0.02f;
                }
            }
        }

        // SCORE
        text.setString(std::to_string(scoreL) + " : " + std::to_string(scoreR));

        // POSITION
        playerL.setPosition(posL);

        playerR.setPosition(posR);
        ball.setPosition(posb);

        // DRAWING
        window.clear(sf::Color::Black);

        window.draw(playerL);
        window.draw(playerR);
        window.draw(text);
        window.draw(ball);

        window.display();
    }

    return 0;
}