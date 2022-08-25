#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <vector>
#include <iostream>

class FlappyBird {

  std::shared_ptr<sf::RenderWindow> window;
  float gravity, frame, space;
  int count, score;
  sf::Texture bg, flappy, pipe;
  std::shared_ptr<sf::Sprite> background, bird, pipeBottom, pipeTop;
  std::vector<sf::Sprite> pipes;
  bool gameOver, add;

  sf::Font font;
  sf::Text txt_score, txt_game_over;

protected:
  void events();
  void draw();
  void game();
  void movePipes();
  void setAnimeBird();
  void moveBird();

public:
  FlappyBird();
  void run();
};