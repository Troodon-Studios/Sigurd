#include "ModuleTile.h"

UModuleTile::UModuleTile()
{
	const int Values[] = { 255, 127, 119, 95, 87, 85, 31, 29, 23, 21, 17, 7, 5, 1 };
	const FColor Colors[] = { FColor::White, FColor::Red, FColor::Green, FColor::Blue, FColor::Yellow, FColor::Cyan, FColor::Magenta, FColor::Orange, FColor::Purple, FColor::Turquoise, FColor::Silver, FColor::Emerald, FColor(0,128,128), FColor(128,128,0) };
	for (int i = 0; i < 14; i++)
	{
		ModuleMesh.Add(Values[i], nullptr);
		ModuleColor.Add(Values[i], Colors[i]);
	}
	UsedColors.Empty();
}