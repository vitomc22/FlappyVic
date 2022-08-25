#include "flappy.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

FlappyBird::FlappyBird() {
  window = std::make_shared<sf::RenderWindow>(
      sf::VideoMode(1000, 600), "Flappy Bird (remix)",
      sf::Style::Titlebar | sf::Style::Close);
  window->setPosition(sf::Vector2i(0, 0));
  window->setFramerateLimit(60);

  gravity = frame = {0.f};
  space = {250.f};
  count = {0};

  bg.loadFromFile("./resources/background.png");
  flappy.loadFromFile("./resources/flappy.png");
  pipe.loadFromFile("./resources/pipe.png");

  background = std::make_shared<sf::Sprite>();
  bird = std::make_shared<sf::Sprite>();
  pipeBottom = std::make_shared<sf::Sprite>();
  pipeTop = std::make_shared<sf::Sprite>();

  background->setTexture(bg);
  bird->setTexture(flappy);
  pipeBottom->setTexture(pipe);
  pipeTop->setTexture(pipe);

  bird->setPosition(500.f - flappy.getSize().x / 2.f,
                    300.f - flappy.getSize().y / 2.f);

  bird->setScale(2.f, 2.f);
  bird->setTextureRect(sf::IntRect(0, 0, 34, 24));
  pipeBottom->setScale(1.5f, 1.5f);
  pipeTop->setScale(1.5f, -1.5f);

  gameOver = add = {false};
  score = {0};
  // mensagem de game over
  font.loadFromFile("./resources/flappybird.ttf");
  txt_game_over.setFont(font);
  txt_game_over.setString("Press SPACE to restart");
  txt_game_over.setPosition(200, 300);
  txt_game_over.setCharacterSize(50);
  txt_game_over.setOutlineThickness(3);
  // score na tela
  txt_score.setFont(font);
  txt_score.setString(std::to_string(score));
  txt_score.setPosition(10.f, 10.f);
  txt_score.setCharacterSize(50);
  txt_score.setOutlineThickness(3);
}

void FlappyBird::events() {
  auto e = std::make_shared<sf::Event>();
  while (window->pollEvent(*e)) {

    if ((e->type = sf::Event::Closed)) {
      window->close();
    }
  }
  // reseta o jogo apertando spaço
  if (gameOver & sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
    pipes.clear();
    bird->setPosition(500.f - flappy.getSize().x / 2.f,
                      300.f - flappy.getSize().y / 2.f);
    gameOver = {false};
    score = {0};
    txt_score.setString(std::to_string(score));
  }
}

void FlappyBird::draw() {
  window->clear(sf::Color::Black);
  window->draw(*background);

  for (auto &p : pipes) {
    window->draw(p);
  }

  window->draw(*bird);

  if (gameOver) {
    window->draw(txt_game_over);
  }
  window->draw(txt_score);
  window->display();
}
void FlappyBird::run() {
  while (window->isOpen()) {
    events();
    game();
    draw();
    ++count;

    if (count == 300) {
      count = 0;
    }
  }
}

// movendo os canos do flappy
void FlappyBird::movePipes() {
  // bota gravidade no click do mouse
  // adicionado bird->getPosition().y > 0 para limitar o click de voo infinito
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
      bird->getPosition().y > 0) {
    gravity = -8.f;
    // faz ele inclinar
    bird->setRotation(-frame - 10.f);
  } else {
    bird->setRotation(frame - 10.f);
  }

  if (count % 150 == 0) {
    int pos = std::rand() % 275 + 175;

    pipeBottom->setPosition(1000, pos + space);
    pipeTop->setPosition(1000, pos);

    pipes.push_back(*pipeBottom);
    pipes.push_back(*pipeTop);
  }
  for (size_t i{}; i < pipes.size(); i++) {
    // colisao com os tubos,pega as dimensões "bounds" dos pipes e do bird
    // adicionado || bird->getPosition().y > 1000 se o passarinho cair é game over tbm

    if (pipes[i].getGlobalBounds().intersects(bird->getGlobalBounds()) ||
        bird->getPosition().y > 1000) {
      bird->move(15.f, 0);
      gameOver = true;
    }

    if (pipes[i].getPosition().x < -100) {
      pipes.erase(pipes.begin() + i);
    }
    pipes[i].move(-4.f, 0);
    // 448 é a cordenada exata que passa o pipe do passarinho, somando 1 ponto
    if (pipes[i].getPosition().x == 448 && !add) {
      // std::cout << pipes[i].getPosition(). << '\n';
      txt_score.setString(std::to_string(++score));
      // macete pra nao duplicar a pontuação
      add = true;
    } else {
      add = false;
    }
  }
}
// controles do game, inicio e fim
void FlappyBird::game() {
  if (!gameOver) {
    setAnimeBird();
    moveBird();
    movePipes();
  }
}
// cria a animação do passarinho
void FlappyBird::setAnimeBird() {
  frame += 0.15f;
  if (frame > 3) {
    frame -= 3;
  }
  // faz bater a asa
  bird->setTextureRect(sf::IntRect(34 * (int)frame, 0, 34, 24));
}

// usa a gravidade pra ele mover na horizontal
void FlappyBird::moveBird() {
  bird->move(0, gravity);
  gravity += 0.5f;
  std::cout << bird->getPosition().y << '\n';
}
