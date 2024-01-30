#include "ModuleTile.h"

UModuleTile::UModuleTile()
{
	const int Values[] = { 255, 127, 119, 95, 87, 85, 31, 29, 23, 21, 17, 7, 5, 1 };
	const FColor Colors[] = { FColor::White, FColor::Red, FColor::Green, FColor::Blue, FColor::Yellow, FColor::Cyan, FColor::Magenta, FColor::Orange, FColor::Purple, FColor::Turquoise, FColor::Silver, FColor::Emerald, FColor(0,128,128), FColor(128,128,0) };
	for (int i = 0; i < 14; i++)
	{
		Matrix.Add(Values[i], SetUpMatrix(Values[i]));
		ModuleMesh.Add(Values[i], nullptr);
		ModuleColor.Add(Values[i], Colors[i]);
	}

	UsedColors.Empty();
	
}

TArray<TArray<bool>> UModuleTile::SetUpMatrix(const int ModuleNumber)
{

	bool o = true;
	bool x = false;
	TArray<TArray<bool>> NewMatrix;
	
	switch (ModuleNumber)
	{
	case 255:
		NewMatrix = {
		{o,o,o},
		{o,o,o},
		{o,o,o}
		};
		break;
	case 127:
		NewMatrix = {
		{x,o,o},
		{o,o,o},
		{o,o,o}
		};
		break;
	case 119:
		NewMatrix = {
		{x,o,o},
		{o,o,o},
		{o,o,x}
		};
		break;
	case 95:
		NewMatrix = {
		{x,o,o},
		{o,o,o},
		{x,o,o}
		};
		break;
	case 87:
		NewMatrix = {
		{x,o,o},
		{o,o,o},
		{x,o,x}
		};
		break;
	case 85:
		NewMatrix = {
		{x,o,x},
		{o,o,o},
		{x,o,x}
		};
		break;
	case 31:
		NewMatrix = {
		{x,o,o},
		{x,o,o},
		{x,o,o}
		};
		break;
	case 29:
		NewMatrix = {
		{x,o,x},
		{x,o,o},
		{x,o,o}
		};
		break;
	case 23:
		NewMatrix = {
		{x,o,o},
		{x,o,o},
		{x,o,x}
		};
		break;
	case 21:
		NewMatrix = {
		{x,o,x},
		{x,o,o},
		{x,o,x}
		};
		break;
	case 17:
		NewMatrix = {
		{x,o,x},
		{x,o,x},
		{x,o,x}
		};
		break;
	case 7:
		NewMatrix = {
		{x,o,o},
		{x,o,o},
		{x,x,x}
		};
		break;
	case 5:
		NewMatrix = {
		{x,o,x},
		{x,o,o},
		{x,x,x}
		};
		break;
	case 1:
		NewMatrix = {
		{x,o,x},
		{x,o,x},
		{x,x,x}
		};
	default:
		NewMatrix = {
		{x,x,x},
		{x,x,x},
		{x,x,x}
		};
		break;
	}

	return NewMatrix;

}

