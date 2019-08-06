#include <vector>
#include <optional>
#include <cassert>
#include <string>
#include "food.h"
#include "blob.h"
#include "simulation.h"

extern int g_nameHolder{ 0 };

void walkAndEat(std::vector<Blob> &blobArray, std::vector<Blob> &deadBlobArray, std::vector<Food> &foodArray)
{
	bool staminaCheck;
	do
	{
		staminaCheck = false;
		int length{ static_cast<int>(blobArray.size()) };
		for (int i{ 0 }; i < length; ++i)
		{
			for (int step{ 0 }; step < blobArray[i].getSpeed(); ++step)
			{
				if (blobArray[i].getEnergy() > 0)
				{

					int before = blobArray[i].getName();
					std::optional<int> blobEaten{ std::nullopt };
					staminaCheck = true;
					if (blobArray[i].getFoodEaten() == 0)
					{
						blobEaten = blobArray[i].huntOrRun(blobArray, deadBlobArray, foodArray);
					}
					else if (blobArray[i].getFoodEaten() == 1 && blobArray[i].hasSurplusStamina())
					{
						blobEaten = blobArray[i].huntOrRun(blobArray, deadBlobArray, foodArray);
					}
					else
					{
						blobArray[i].goHome();
					}

					//if a prey blob was eaten
					if (blobEaten.has_value())
					{
						//adjust the loop size
						--length;
						if (i > blobEaten.value())
						{
							/*if the eaten blob was before the hunting blob in the array
							then the hunting blob element value has been reduced by one,
							as all blobs after the eaten have shifted down one in stack,
							filling the	gap*/
							--i;
						}
					}
					assert(before == blobArray[i].getName() && "Blob element tracking mistake");
					blobArray[i].reduceEnergy();
					blobArray[i].recordPath();
				}
			}
		}
	} while (staminaCheck);
};

void naturalSelection(std::vector<Blob> &blobArray, std::vector<Blob> &deadBlobArray)
{
	/*how to delete objects from a vector.
	"it" is the "iterator pointer": points to current element of vector.*/
	for (auto it = blobArray.begin(); it != blobArray.end();)
	{
		// remember A->.function() is same as (*A).function()
		if (!it->atHome() || !it->getFoodEaten())
		{
			//copy blob to deadBlobArray for animation
			Blob deadBlob = *it;
			deadBlobArray.push_back(deadBlob);
			//erase blob
			it = blobArray.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void breed(std::vector<Blob> &blobArray, int day)
{
	int length{ static_cast<int>(blobArray.size()) };
	for (int i{ 0 }; i < length; ++i)
	{
		std::optional<Blob> clone{ blobArray[i].tryToReplicate() };
		if (clone.has_value())
		{
			//To get correct number for new blob name
			clone.value().setName(g_nameHolder);
			clone.value().setBirthday(day);
			++g_nameHolder;
			blobArray.push_back(clone.value());
		}
	}
}

void digestAndSleep(std::vector<Blob> &blobArray)
{
	int length{ static_cast<int>(blobArray.size()) };
	for (int i{ 0 }; i < length; ++i)
	{
		blobArray[i].setFoodEaten(0);
		blobArray[i].sleep();
	}
}

void blobsCarryOutDay(std::vector<Blob> &blobArray, std::vector<Blob> &deadBlobArray, std::vector<Food> &foodArray)
{
	walkAndEat(blobArray, deadBlobArray, foodArray);
	naturalSelection(blobArray, deadBlobArray);
	breed(blobArray, 0);
	digestAndSleep(blobArray);
}
