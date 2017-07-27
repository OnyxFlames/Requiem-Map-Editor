#include "RequiemTileMapMaker.hpp"



RequiemTileMapMaker::RequiemTileMapMaker()
{
}
RequiemTileMapMaker::RequiemTileMapMaker(unsigned map_width, unsigned map_height, const std::string spritesheet)
	: window(sf::VideoMode(WIDTH, HEIGHT), "Requiem Map Maker 0.0.1")
{
	spritesheettexture.loadFromFile(spritesheet);
	height = map_height;
	width = map_width;
	map_data.resize(map_height);
	for (unsigned h = 0; h < map_height; h++)
	{
		map_data[h].resize(map_width);
		for (unsigned w = 0; w < map_width; w++)
		{
			map_data[h][w].setTexture(spritesheettexture);
			map_data[h][w].setTextureRect(sf::IntRect(0, 0, 16, 16));
			map_data[h][w].setPosition(w * 16.f, h * 16.f);
		}
	}
	active_tile.setTexture(spritesheettexture);
	active_tile.setTextureRect(sf::IntRect((unsigned)(sel_tile_coords.x * 1.0625) * 16, (unsigned)(sel_tile_coords.y * 1.0625) * 16, 16, 16));
	active_tile.setScale(6.f, 6.f);

	curr_tile.setFillColor(sf::Color(0, 0, 0, 0));
	curr_tile.setOutlineColor(sf::Color::Red);
	curr_tile.setOutlineThickness(1.f);
	curr_tile.setSize({ 16.f, 16.f });
	curr_tile.setPosition(0, 0);
	camera = window.getView();
	_default = camera;
}

void RequiemTileMapMaker::input()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				window.close();
				break;
			case sf::Keyboard::Space:
				gettile(curr_tile_coords.x, curr_tile_coords.y).setTextureRect(sf::IntRect((sel_tile_coords.x * 1.0625) * 16, (sel_tile_coords.y * 1.0625) * 16, 16, 16));
				return;
			case sf::Keyboard::W:
				if (curr_tile.getPosition().y < 0)
					curr_tile.setPosition(curr_tile.getPosition().x, 0);
				curr_tile.move(0, -16);
				curr_tile_coords.y--;
				break;
			case sf::Keyboard::A:
				if (curr_tile.getPosition().x < 0)
					curr_tile.setPosition(0, curr_tile.getPosition().y);
				curr_tile.move(-16, 0);
				if (curr_tile_coords.x > 0)
					curr_tile_coords.x--;
				break;
			case sf::Keyboard::S:
				if (curr_tile.getPosition().y > height * 16)
					curr_tile.setPosition(curr_tile.getPosition().x, 0);
				curr_tile.move(0, 16);

				curr_tile_coords.y++;
				break;
			case sf::Keyboard::D:
				if (curr_tile.getPosition().x > width * 16)
					curr_tile.setPosition(curr_tile.getPosition().x, 0);
				curr_tile.move(16, 0);
				curr_tile_coords.x++;
				break;
			case sf::Keyboard::I:
				camera.move(0, -16);
				break;
			case sf::Keyboard::J:
				camera.move(-16, 0);
				break;
			case sf::Keyboard::K:
				camera.move(0, 16);
				break;
			case sf::Keyboard::L:
				camera.move(16, 0);
				break;
			case sf::Keyboard::P:
				camera = _default;
				break;
			case sf::Keyboard::Up:
				sel_tile_coords.y--;
				break;
			case sf::Keyboard::Left:
				sel_tile_coords.x--;
				break;
			case sf::Keyboard::Down:
				sel_tile_coords.y++;
				break;
			case sf::Keyboard::Right:
				sel_tile_coords.x++;
				break;
			case sf::Keyboard::Return:
				export_map("export");
				break;
			case sf::Keyboard::LShift:
				load_map("DirtPath.rmap");
				break;
			}
			break;
		case sf::Event::MouseMoved:
			//curr_tile.setPosition(event.mouseMove.x, event.mouseMove.y);
			break;
		}
	}
}
void RequiemTileMapMaker::update()
{
	window.setView(camera);
	active_tile.setPosition((getHUD().x + WIDTH) - active_tile.getGlobalBounds().width, 0);
	active_tile.setTextureRect(sf::IntRect((sel_tile_coords.x * 1.0625) * 16, (sel_tile_coords.y * 1.0625) * 16, 16, 16));
	if (curr_tile_coords.x * 16 > active_tile.getPosition().x && curr_tile_coords.y * 16 < active_tile.getPosition().y + active_tile.getGlobalBounds().height)
	{
		active_tile.setColor(sf::Color(255, 255, 255, 127));
	}
	else
		active_tile.setColor(sf::Color(255, 255, 255, 255));
}
void RequiemTileMapMaker::render()
{
	window.clear();
	for (auto &m : map_data)
	{
		for (auto &mm : m)
		{
			window.draw(mm);
		}
	}
	window.draw(curr_tile);
	window.draw(active_tile);
	window.display();
}
void RequiemTileMapMaker::run()
{
	while (window.isOpen())
	{
		input();
		update();
		render();
	}
}
sf::Sprite& RequiemTileMapMaker::gettile(unsigned x, unsigned y)
{
	return map_data[y][x];
}
sf::Vector2f RequiemTileMapMaker::getHUD()
{
	sf::Vector2f ret;
	ret.x = camera.getCenter().x - camera.getSize().x / 2;
	ret.y = camera.getCenter().y - camera.getSize().y / 2;
	return ret;
}
void RequiemTileMapMaker::export_map(const std::string filename)
{
	std::ofstream rmap(filename + ".rmap");
	std::ofstream rmappos(filename + ".rmappos");
	std::ofstream rmapsize(filename + ".rmapsize");
	for (unsigned h = 0; h < height; h++)
	{
		map_data[h].resize(width);
		for (unsigned w = 0; w < width; w++)
		{
			rmap << " " << (map_data[h][w].getTextureRect().left / 16) - w * 1.0625 << " " << (map_data[h][w].getTextureRect().top / 16) - h * 1.0625 << " ";
		}
		rmap << "\n";
	}
	rmappos << "0 0";
	rmapsize << width << " " << height;
}
RequiemTileMapMaker::~RequiemTileMapMaker()
{
}
