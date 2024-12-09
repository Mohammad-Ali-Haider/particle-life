#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;
const int GAME_RES[2] = {(int)(sf::VideoMode::getFullscreenModes()[0].width/2), (int)(sf::VideoMode::getFullscreenModes()[0].height/2)};
// const int GAME_RES[2] = {700, 700};
const int PARTICLE_DENSITY = 60;

const sf::Color YELLOW = sf::Color(255, 247, 13);
const sf::Color GREEN = sf::Color(39, 255, 15);
const sf::Color BLUE = sf::Color(15, 139, 255);
const sf::Color RED = sf::Color(255, 15, 15);
const sf::Color PURPLE = sf::Color(91, 15, 255);

const float EDGE_OFFSET = 100.0f;

int getRandomNumber(int min, int max) {
    return (int)((double)rand() / RAND_MAX * (max - min) + min);
}

sf::Vector2f originShift(sf::Vector2f vector){
    return vector - sf::Vector2f(GAME_RES[0]/2, GAME_RES[1]/2);
}

sf::Vector2f normalize(sf::Vector2f vector){
    return vector / sqrt(vector.x * vector.x + vector.y * vector.y);
}

float magnitude(sf::Vector2f vector){
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

template <typename T>
std::vector<T> combineVectors(const std::vector<std::vector<T>>& vectors) {
    std::vector<T> combinedVector;

    for (const auto& vector : vectors) {
        combinedVector.insert(combinedVector.end(), vector.begin(), vector.end());
    }

    return combinedVector;
}

class Atom{
    private:
        int mass = 1;
        int radius = 1;
        float minDistance = 30.0f;
        float maxDistance = 100.0f;
        sf::Vector2f positon = sf::Vector2f(0, 0);
        sf::Vector2f velocity;
        sf::Vector2f acceleration;
        sf::Color color = sf::Color(255, 255, 255);
    public:
        sf::CircleShape shape;
        Atom(){}
        Atom(sf::Vector2f pos, int mass, int radius, const sf::Color& color): color(color){
            this->mass = mass;
            this->radius = radius;
            this->positon = pos;
            shape.setOrigin(sf::Vector2f(this->radius, this->radius));
            shape.setPosition(this->positon);
            shape.setRadius(this->radius);
            shape.setFillColor(this->color);
        }

        bool operator!=(const Atom& other) const {
            return this->positon != other.positon;
        }

        void update(vector<Atom>& atoms, float deltaTime, float a) {
            sf::Vector2f force(0.0f, 0.0f);
            // if (this->positon.x > GAME_RES[0] + EDGE_OFFSET){
            //     this->positon.x -= (GAME_RES[0] + EDGE_OFFSET);
            // }
            // if (this->positon.x < 0 - EDGE_OFFSET){
            //     this->positon.x += (GAME_RES[0] + EDGE_OFFSET);
            // }
            // if (this->positon.y > GAME_RES[1] + EDGE_OFFSET){
            //     this->positon.y -= (GAME_RES[1] + EDGE_OFFSET);
            // }
            // if (this->positon.y < 0 - EDGE_OFFSET){
            //     this->positon.y += (GAME_RES[1] + EDGE_OFFSET);
            // }
            if (this->positon.x > GAME_RES[0] - EDGE_OFFSET){
                this->positon.x = GAME_RES[0] - EDGE_OFFSET;
            }
            if (this->positon.x < 0 + EDGE_OFFSET){
                this->positon.x = 0 + EDGE_OFFSET;
            }
            if (this->positon.y > GAME_RES[1] - EDGE_OFFSET){
                this->positon.y = GAME_RES[1] - EDGE_OFFSET;
            }
            if (this->positon.y < 0 + EDGE_OFFSET){
                this->positon.y = 0 + EDGE_OFFSET;
            }
            for (Atom& atom: atoms){
                if (&atom != this){
                    if ((magnitude(this->positon - atom.positon)) > this->minDistance && (magnitude(this->positon - atom.positon)) < this->maxDistance){
                        force += normalize(atom.positon - this->positon) * (1 - abs(2 * magnitude(this->positon - atom.positon) - this->maxDistance - this->minDistance) / (this->maxDistance - this->minDistance)) * a * 0.500f;
                    }else if((magnitude(this->positon - atom.positon)) <= this->minDistance){
                        force += normalize(atom.positon - this->positon) * ((magnitude(this->positon - atom.positon) / (this->minDistance)) - 1) * 10.0f;
                    }else{
                        force += sf::Vector2f(0.0f, 0.0f);
                    }
                }
            }

            // this->acceleration = force / (float)this->mass;
            // this->velocity = this->acceleration * deltaTime;
            // this->positon += this->velocity * deltaTime;
            this->positon += force / (float)this->mass * deltaTime;
            shape.setPosition(this->positon);
        }

        void setMinDistance(float minDis){
            this->minDistance = minDis;
        }

        void setMaxDistance(float maxDis){
            this->maxDistance = maxDis;
        }
};

class App{
    private:
        int width;
        int height;
        sf::RenderWindow window;
    public:
        App(int width, int height) : window(sf::VideoMode(width, height), "Life"){
            this->width = width;
            this->height = height;
        }

        void draw(vector<Atom>& entity) {
            window.clear();

            for (auto e: entity) {
                window.draw(e.shape);
            }

            window.display();
        }

        void run(){
            sf::Clock clock;
            sf::Time elapsed;
            bool running = true;
            vector<Atom> green_p;
            vector<Atom> blue_p;
            vector<Atom> purple_p;
            vector<Atom> particles;

            for (int i = 0; i < floor((width)/PARTICLE_DENSITY); i++){
                for (int j = 0; j < floor((height)/PARTICLE_DENSITY); j++){
                    // green_p.push_back(Atom(sf::Vector2f(getRandomNumber(0, width), getRandomNumber(0, height)), 1, 5, GREEN));
                    // blue_p.push_back(Atom(sf::Vector2f(getRandomNumber(0, width), getRandomNumber(0, height)), 1, 5, BLUE));
                    // purple_p.push_back(Atom(sf::Vector2f(getRandomNumber(0, width), getRandomNumber(0, height)), 1, 5, PURPLE));
                    int offset = PARTICLE_DENSITY/2;
                    // yellow_p.push_back(Atom(sf::Vector2f(i * PARTICLE_DENSITY + offset, j * PARTICLE_DENSITY + offset), 1, 5, sf::Color(0, getRandomNumber(100,255), getRandomNumber(100,255))));
                    green_p.push_back(Atom(sf::Vector2f(i * PARTICLE_DENSITY + offset, j * PARTICLE_DENSITY + offset), 1, 5, GREEN));
                    blue_p.push_back(Atom(sf::Vector2f(i * PARTICLE_DENSITY + offset + offset / 2, j * PARTICLE_DENSITY + offset + offset / 2), 1, 5, BLUE));
                    purple_p.push_back(Atom(sf::Vector2f(i * PARTICLE_DENSITY + offset + offset, j * PARTICLE_DENSITY + offset + offset), 1, 5, PURPLE));
                }
            }

            while (running){
                sf::Event event;
                elapsed = clock.restart();
                sf::sleep(sf::milliseconds(16) - elapsed);

                // for (auto& c: green_p){
                //     // c.setMaxDistance(250.0f);
                //     c.update(green_p, elapsed.asSeconds(), 25.0f);
                //     c.update(purple_p, elapsed.asSeconds(), 15.0f);
                //     c.update(blue_p, elapsed.asSeconds(), -30.0f);
                // }
                // for (auto& c: blue_p){
                //     // c.setMaxDistance(250.0f);
                //     c.update(blue_p, elapsed.asSeconds(), 20.0f);
                //     c.update(green_p, elapsed.asSeconds(), 25.0f);
                //     c.update(purple_p, elapsed.asSeconds(), -15.0f);
                // }
                // for (auto& c: purple_p){
                //     c.update(purple_p, elapsed.asSeconds(), 15.0f);
                //     c.update(blue_p, elapsed.asSeconds(), 10.0f);
                //     c.update(green_p, elapsed.asSeconds(), 25.0f);
                // }
                
                //Right round
                for (auto& c: green_p){
                    // c.setMaxDistance(250.0f);
                    c.update(green_p, elapsed.asSeconds(), 20.0f);
                    c.update(purple_p, elapsed.asSeconds(), 20.0f);
                    c.update(blue_p, elapsed.asSeconds(), -20.0f);
                }
                for (auto& c: blue_p){
                    // c.setMaxDistance(250.0f);
                    c.update(blue_p, elapsed.asSeconds(), 20.0f);
                    c.update(green_p, elapsed.asSeconds(), 20.0f);
                    c.update(purple_p, elapsed.asSeconds(), -20.0f);
                }
                for (auto& c: purple_p){
                    c.update(purple_p, elapsed.asSeconds(), 20.0f);
                    c.update(blue_p, elapsed.asSeconds(), 20.0f);
                    c.update(green_p, elapsed.asSeconds(), -20.0f);
                }

                // for (auto& c: green_p){
                //     // c.setMaxDistance(250.0f);
                //     c.update(green_p, elapsed.asSeconds(), 20.0f);
                //     c.update(purple_p, elapsed.asSeconds(), 20.0f);
                //     c.update(blue_p, elapsed.asSeconds(), -20.0f);
                // }
                // for (auto& c: blue_p){
                //     // c.setMaxDistance(250.0f);
                //     c.update(blue_p, elapsed.asSeconds(), 20.0f);
                //     c.update(green_p, elapsed.asSeconds(), 20.0f);
                //     c.update(purple_p, elapsed.asSeconds(), 15.0f);
                // }
                // for (auto& c: purple_p){
                //     c.update(purple_p, elapsed.asSeconds(), 20.0f);
                //     c.update(blue_p, elapsed.asSeconds(), 20.0f);
                //     c.update(green_p, elapsed.asSeconds(), -20.0f);
                // }

                particles = combineVectors(vector<vector<Atom>>{blue_p, green_p, purple_p});

                draw(particles);

                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed){
                        window.close();
                        running = false;
                    }
                }
            }
        }
};

int main()
{
    App APP(GAME_RES[0], GAME_RES[1]);

    APP.run();

    return 0;
}