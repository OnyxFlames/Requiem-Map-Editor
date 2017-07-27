#pragma once

#include <iostream>
#include <fstream>
#include <array>


#include <SFML/Graphics.hpp>

#define HEIGHT 720
#define WIDTH 1280

class RequiemTileMapMaker
{
private:
	sf::View camera, _default;
	sf::RenderWindow window;
	std::vector<std::vector<sf::Sprite>> map_data;
	std::vector<std::vector<sf::Vector2f>> coord_data;
	sf::Texture spritesheettexture;
	sf::RectangleShape curr_tile;
	unsigned height, width;
	sf::Vector2u curr_tile_coords = { 0, 0 };
	sf::Vector2u sel_tile_coords = { 0, 0 };
	sf::Sprite active_tile;
public:
	RequiemTileMapMaker();
	RequiemTileMapMaker(unsigned map_width, unsigned map_height, const std::string spritesheet);
	~RequiemTileMapMaker();
	void input();
	void update();
	void render();
	void run();
	sf::Sprite& gettile(unsigned x, unsigned y);
	sf::Vector2f getHUD();
	void export_map(const std::string filename);
	void load_map(const std::string &filename)
	{
		std::ifstream map(filename);
		//std::ifstream mappos(filename + "pos");
		std::ifstream mapsize(filename + "size");
		if (!map.is_open() || !mapsize.is_open())
		{
			std::cerr << "Error loading map data for \'" << filename << "\'\n";
			return;
		}
		std::string buff = "";
		mapsize >> buff;
		width = std::stoi(buff);
		mapsize >> buff;
		height = std::stoi(buff);

		//mappos >> buff;
		//pos.x = std::stof(buff);
		//mappos >> buff;
		//pos.y = std::stof(buff);
		buff = "";

		for (unsigned h = 0; h < height; h++)
		{
			std::vector<sf::Vector2f> vecbuff;
			for (unsigned w = 0; w < width; w++)
			{
				float x, y;
				map >> buff;
				x = std::stof(buff);
				map >> buff;
				y = std::stof(buff);
				vecbuff.push_back(sf::Vector2f(x, y));
			}
			coord_data.push_back(vecbuff);
		}

		map_data.resize(height);
		for (unsigned h = 0; h < height; h++)
		{
			map_data[h].resize(width);
			for (unsigned w = 0; w < width; w++)
			{
				map_data[h][w].setTexture(spritesheettexture);
				map_data[h][w].setTextureRect(sf::IntRect((coord_data[h][w].x * 1.0625) * 16, (coord_data[h][w].y * 1.0625) * 16, 16, 16));
			}
		}
	}
};

