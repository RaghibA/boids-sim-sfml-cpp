#ifndef BOID_H
#define BOID_H

#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <iostream>

class Boid {
private:
  sf::Vector2f position;
  sf::Vector2f velocity;
  sf::Vector2f acceleration;

  int winHeight, winWidth, maxSpeed, maxForce;

public:
  Boid(int windowHeight, int windowWidth, int maxSpeed, int maxForce);

  void setPosition(sf::Vector2f updatedPosition);
  sf::Vector2f getPosition();

  void setVelocity(sf::Vector2f updatedVelocity);
  sf::Vector2f getVelocity();

  void setAcceleration(sf::Vector2f updatedAcceleration);
  sf::Vector2f getAcceleration();

  sf::Vector2f separation(std::vector<Boid *> flock);
  sf::Vector2f alignment(std::vector<Boid *> flock);
  sf::Vector2f cohesion(std::vector<Boid *> flock);

  void edgeHandling();
  void flock(std::vector<Boid *> flock);
  void update();
};

#endif