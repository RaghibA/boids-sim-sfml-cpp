#include "Boid.h"

float randFloat(float lowBound, float upBound) {
  return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (upBound -lowBound)));
}

sf::Vector2f multVector(sf::Vector2f v, float scalar) {
  return sf::Vector2f(v.x * scalar, v.y * scalar);
}

sf::Vector2f sumVectors(sf::Vector2f a, sf::Vector2f b) {
  sf::Vector2f sum;
  float xSum = a.x + b.x;
  float ySum = a.y + b.y;

  sum = sf::Vector2f(xSum, ySum);
  return sum;
}

sf::Vector2f diffVectors(sf::Vector2f a, sf::Vector2f b) {
  sf::Vector2f diff;
  float xDiff = a.x - b.x;
  float yDiff= a.y - b.y;

  diff = sf::Vector2f(abs(xDiff), abs(yDiff));
  return diff;
}

sf::Vector2f divVectors(sf::Vector2f v, float scalar) {
  return sf::Vector2f(v.x / scalar, v.y / scalar);
}

sf::Vector2f setMagnitude(sf::Vector2f v, float mag) {
  float curMag = sqrt((v.x * v.x) + (v.y * v.y));
  float newX = v.x * mag / curMag;
  float newY = v.y * mag / curMag;

  return sf::Vector2f(newX, newY);
}

sf::Vector2f limitNorm(sf::Vector2f v, float maxMag) {
  float n = sqrt(v.x * 2 + v.y * 2);
  float f = std::min(n, maxMag) / n;

  return sf::Vector2f(f * v.x, f * v.y);
}

float distVectors(sf::Vector2f a, sf::Vector2f b) {
  float d = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));

  return abs(d);
}

Boid::Boid(int windowHeight, int windowWidth, int maxSpeed, int maxForce) {
  this->winHeight = windowHeight;
  this->winWidth = windowWidth;
  this->maxSpeed = maxSpeed;
  this->maxForce = maxForce;

  this->position = sf::Vector2f(randFloat(0, windowWidth), randFloat(0, windowHeight));
  this->velocity = sf::Vector2f(randFloat(0, 5), randFloat(0, 5));
  this->velocity = setMagnitude(this->velocity, randFloat(1, 2));
  this->acceleration = sf::Vector2f(0, 0);
}

void Boid::setPosition(sf::Vector2f updatedPosition) {this->position = updatedPosition;}
sf::Vector2f Boid::getPosition() { return this->position; }

void Boid::setVelocity(sf::Vector2f updatedVelocity) { this->velocity = updatedVelocity; }
sf::Vector2f Boid::getVelocity() { return this->velocity; }

void Boid::setAcceleration(sf::Vector2f updatedAcceleration) { this->acceleration = updatedAcceleration; }
sf::Vector2f Boid::getAcceleration() { return this->acceleration; }

sf::Vector2f Boid::separation(std::vector<Boid *> flock) {
  float perception = 20;
  sf::Vector2f steeringForce = sf::Vector2f(0.f, 0.f);
  float boidsInFov = 0;

  for (int i = 0; i < flock.size(); i++) {
    Boid *temp = flock.at(i);
    float dist = distVectors(this->position, temp->position);
    if (this != temp && dist < perception)
    {
      sf::Vector2f difPosition = diffVectors(this->position, temp->position);
      difPosition = divVectors(difPosition, dist);
      steeringForce = sumVectors(steeringForce, difPosition);
      boidsInFov += 1;
    }
  }

  if(boidsInFov > 0) {
    steeringForce = divVectors(steeringForce, boidsInFov);
    steeringForce = setMagnitude(steeringForce, maxSpeed);
    steeringForce = diffVectors(steeringForce, this->velocity);
    steeringForce = limitNorm(steeringForce, this->maxForce);
  }

  return steeringForce;
}

sf::Vector2f Boid::alignment(std::vector<Boid *> flock) {
  float perception = 20;
  sf::Vector2f steeringForce = sf::Vector2f(0.f, 0.f);
  float boidsInFov = 0;

  for (int i = 0; i < flock.size(); i++) {
    Boid *temp = flock.at(i);
    float dist = distVectors(this->position, temp->position);
    if (this != temp && dist < perception)
    {
      steeringForce = sumVectors(steeringForce, temp->getVelocity());
      boidsInFov += 1;
    }
  }

  if(boidsInFov > 0) {
    steeringForce = divVectors(steeringForce, boidsInFov);
    steeringForce = setMagnitude(steeringForce, maxSpeed);
    steeringForce = diffVectors(steeringForce, this->velocity);
    steeringForce = limitNorm(steeringForce, this->maxForce);
  }

  return steeringForce;
}

sf::Vector2f Boid::cohesion(std::vector<Boid *> flock) {
  float perception = 30;
  sf::Vector2f steeringForce = sf::Vector2f(0.f, 0.f);
  float boidsInFov = 0;

  for (int i = 0; i < flock.size(); i++) {
    Boid *temp = flock.at(i);
    float dist = distVectors(this->position, temp->position);
    if (this != temp && dist < perception)
    {
      steeringForce = sumVectors(steeringForce, temp->getPosition());
      boidsInFov += 1;
    }
  }

  if(boidsInFov > 0) {
    steeringForce = divVectors(steeringForce, boidsInFov);
    steeringForce = diffVectors(steeringForce, this->position);
    steeringForce = setMagnitude(steeringForce, maxSpeed);
    steeringForce = diffVectors(steeringForce, this->velocity);
    steeringForce = limitNorm(steeringForce, this->maxForce);
  }

  return steeringForce;
}

void Boid::edgeHandling() { // wrap around edge handling //! shoud be changed to edge avoidance
  if(this->position.x > this->winWidth) {
    this->position.x = 0;
  } else if(this->position.x < 0) {
    this->position.x = this->winWidth; 
  }

  if(this->position.y > this->winHeight) {
    this->position.y = 0;
  } else if(this->position.y < 0) {
    this->position.y = this->winHeight;
  }
}

void Boid::flock(std::vector<Boid *> flock) {
  sf::Vector2f separation = this->separation(flock);
  sf::Vector2f alignment = this->alignment(flock);
  sf::Vector2f cohesion = this->cohesion(flock);
  this->acceleration = sumVectors(this->acceleration, separation);
  this->acceleration = sumVectors(this->acceleration, alignment);
  this->acceleration = sumVectors(this->acceleration, cohesion);
}

void Boid::update() {
  this->position = sumVectors(this->position, this->velocity);
  this->velocity = sumVectors(this->velocity, this->acceleration);
  this->velocity = limitNorm(this->velocity, this->maxSpeed);
  this->acceleration = multVector(this->acceleration, 0);
}