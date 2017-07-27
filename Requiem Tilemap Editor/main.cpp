#include <SFML/Graphics.hpp>

#include "RequiemTileMapMaker.hpp"

int main(int argc, char * argv[])
{

	RequiemTileMapMaker rtmm(64, 32, "../resources/textures/city.png");
	rtmm.run();
	return 0;
}