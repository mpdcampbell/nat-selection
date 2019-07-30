#include <vector>
#include <optional>
#include <cassert>
#include <string>
#include "food.h"
#include "blob.h"
#include "simulation.h"

extern int g_nameHolder{ 0 };

void walkAndEat(std::vector<Blob> &blobArray, std::vector<Food> &foodArray)
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
					std::string before = blobArray[i].getName();
					std::optional<int> eatenBlob{ std::nullopt };
					staminaCheck = true;
					if (blobArray[i].getFoodEaten() == 0)
					{
						eatenBlob = blobArray[i].huntOrRun(foodArray, blobArray);
					}
					else if (blobArray[i].getFoodEaten() == 1 && blobArray[i].hasSurplusStamina())
					{
						eatenBlob = blobArray[i].huntOrRun(foodArray, blobArray);
					}
					else
					{
						blobArray[i].goHome();
					}

					//if a prey blob was eaten
					if (eatenBlob.has_value())
					{
						//adjust the loop size
						--length;
						if (i > eatenBlob.value())
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
				}
			}
		}
	} while (staminaCheck);
};

void naturalSelection(std::vector<Blob> &blobArray)
{
	/*how to delete objects from a vector.
	"it" is the "iterator pointer": points to current element of vector.*/
	for (auto it = blobArray.begin(); it != blobArray.end();)
	{
		// remember A->.function() is same as (*A).function()
		if (!it->atHome() || !it->getFoodEaten())
		{
			it = blobArray.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void breed(std::vector<Blob> &blobArray)
{
	int length{ static_cast<int>(blobArray.size()) };
	for (int i{ 0 }; i < length; ++i)
	{
		std::optional<Blob> clone{ blobArray[i].tryToReplicate() };
		if (clone.has_value())
		{
			//To get correct number for new blob name
			clone.value().setName("Blob #" + std::to_string(g_nameHolder++));
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

void ageBlobs(std::vector<Blob> &blobArray)
{
	for (int i{ 0 }; i < blobArray.size(); ++i)
	{
		blobArray[i].setAge(blobArray[i].getAge() + 1);
	}
}

void blobsCarryOutDay(std::vector<Blob> &blobArray, std::vector<Food> &foodArray)
{
	walkAndEat(blobArray, foodArray);
	naturalSelection(blobArray);
	breed(blobArray);
	digestAndSleep(blobArray);
	ageBlobs(blobArray);
}
