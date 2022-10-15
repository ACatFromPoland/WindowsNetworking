#include "../Shared/DynamicArray.h"

struct stuff
{
	int n;
};

class Player
{
public:
	stuff n1;
	stuff n2;

	DynamicArray<stuff*> myStuff;

	Player()
	{
		n1 = { 1 };
		n2 = { 2 };

		myStuff.pushBack(&n1);
		myStuff.pushBack(&n2);
	}
};

int main()
{
	{
		DynamicArray<Player> players;
		Player player;
		players.pushBack(player);
	}

	/* ERROR
		Deconstruction of dynamic arrays holding pointers does not work...
	*/

	return 0;
}