#define DEBUG 0

#if DEBUG
#include <cstdio>
#endif

#include <sstream>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

#define POCETNA_X 593.38f
#define POCETNA_Y 260.211f
#define GRID_SIZE 50.f
#define WW 1600U
#define H 900U

sf::Clock dtClock;
sf::Vector2f velocity;
float deltaTime, pW = 76.0f, pH = 162.0f, cW = 50.0f, cH = 50.0f, mov_speed = 700.f;
int score = 0;

#if DEBUG
int log(std::string str) {
	return printf(str.c_str());
}
#endif

template<typename T>
std::string toString(T n) {
	std::stringstream ss;
	ss << n;
	return ss.str();
}

int main() {
	sf::RenderWindow window(sf::VideoMode(WW, H), "2D Zderonja", sf::Style::Close | sf::Style::Resize);
	sf::RectangleShape player(sf::Vector2f(pW, pH)), crv;
	sf::Font font;
	font.loadFromFile("fonts/Roboto-Bold.ttf");
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(50);
	text.setFillColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);
	text.setString("Poeni: " + toString(score));
	text.setPosition(715.0f, 1.60775f);
	player.setPosition(POCETNA_X, POCETNA_Y);
	sf::Texture pT, pT1, bg;
	pT.loadFromFile("img/riba_gore.png");
	pT1.loadFromFile("img/crv.png");
	bg.loadFromFile("img/svemir.png");                                                              
	player.setTexture(&pT);
	sf::Sprite s(bg);
	s.setTexture(bg);
	srand(time((time_t*)NULL));
	sf::SoundBuffer buf;
	buf.loadFromFile("sound/beep-02.wav");
	sf::Sound bip;
	bip.setBuffer(buf);
	std::random_device rd;
	std::uniform_real_distribution<float> cX(0.f, WW);
	std::uniform_real_distribution<float> cY(0.f, H);
	crv.setPosition(cX(rd), cY(rd));
	sf::Event e;
	std::vector<sf::RectangleShape> crvi;
	crv.setSize(sf::Vector2f(cW, cH));
	crv.setPosition(GRID_SIZE * 5, GRID_SIZE * 2);
	crv.setTexture(&pT1);
	crvi.push_back(crv);
	sf::FloatRect nextPos;
	while (window.isOpen()) {
		deltaTime = dtClock.restart().asSeconds();
		while (window.pollEvent(e)) {
			switch (e.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				window.clear();
				player.setPosition(POCETNA_X, POCETNA_Y);
				break;
			}
		}

		velocity.y = 0.f;
		velocity.x = 0.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
			player.setRotation(-90.f);
			velocity.x += -mov_speed * deltaTime;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			player.setRotation(90.f);
			velocity.x += mov_speed * deltaTime;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
			player.setRotation(0.f);
			velocity.y += -mov_speed * deltaTime;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
			player.setRotation(-180.f);
			velocity.y += mov_speed * deltaTime;
		}

		player.setTexture(&pT);

		player.move(velocity);

		for (auto& crv : crvi) {
			sf::FloatRect playerBounds = player.getGlobalBounds();
			sf::FloatRect crvBounds = crv.getGlobalBounds();
			nextPos = playerBounds;
			nextPos.left += velocity.x;
			nextPos.top += velocity.y;
			if (crvBounds.intersects(nextPos)) {
				score++;
				pW += 1.f;
				pH += 5.f;
				if (mov_speed <= 400.f) {
					text.setString("Poeni: " + toString(score));
					bip.play();
					crv.setPosition(cX(rd), cY(rd));
					crv.setRotation(1.0f);
					player.setSize(sf::Vector2f(pW, pH));
				} else {
					mov_speed -= 10.f;
					text.setString("Poeni: " + toString(score));
					bip.play();
					crv.setPosition(cX(rd), cY(rd));
					crv.setRotation(1.0f);
					player.setSize(sf::Vector2f(pW, pH));
				}
				#if DEBUG
				log("mov_speed = " + toString(mov_speed) + '\n');
				#endif
			}
		}

		if (player.getPosition().x < 0.0f) player.setPosition(player.getPosition().x + 2, player.getPosition().y);
		if (player.getPosition().x > (float)WW) player.setPosition(player.getPosition().x - 2, player.getPosition().y);
		if (player.getPosition().y < 0.96f) player.setPosition(player.getPosition().x, player.getPosition().y + 2);
		if (player.getPosition().y > (float)H) player.setPosition(player.getPosition().x, player.getPosition().y - 2);

		window.clear();
		window.draw(s);
		window.draw(text);
		window.draw(player);
		for (auto& i : crvi) window.draw(i);
		window.display();
	}
	return 0;
}