#include "Boid.h"

using namespace std;

int main(int argc, char **argv) {
  // create window
  sf::RenderWindow window(sf::VideoMode(1280, 720), "Boid Sim");

  // Init flock
  int flockSize = 250;
  std::vector<Boid*> flock;
  for (int i = 0; i < flockSize; i++) {
    Boid *temp = new Boid(window.getSize().y, window.getSize().x, 1.f, 1.f);
    flock.push_back(temp);
  }

  while(window.isOpen()) {
    sf::Event event;
    while(window.pollEvent(event)) {
      if(event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear(sf::Color::Black);

    for (int i = 0; i < flock.size(); i++) {
      // update
      Boid *boid = flock.at(i);
      boid->edgeHandling();
      boid->flock(flock);
      boid->update();

      // draw
      sf::RectangleShape boidShape;
      boidShape.setFillColor(sf::Color::White);
      boidShape.setSize(sf::Vector2f(1.f, 1.f));
      boidShape.setPosition(boid->getPosition().x, boid->getPosition().y);
      window.draw(boidShape);
    }

    window.display();
  }

  return 0;
}