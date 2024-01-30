#include "ModuleTile.h"

UModuleTile::UModuleTile()
{
	const int Values[] = { 255, 127, 119, 95, 87, 85, 31, 29, 23, 21, 17, 7, 5, 1 };
	for (int i = 0; i < 14; i++)
	{
		Matrix.Add(Values[i], SetUpMatrix(Values[i]));
		ModuleMesh.Add(Values[i], nullptr);
		ModuleColor.Add(Values[i], SetUpColors(Values[i]));
	}

	UsedColors.Empty();
	
}

FColor UModuleTile::SetUpColors(const int ModuleNumber)
{
	FColor NewColor;

	switch (ModuleNumber)
	{
	case 255:
		NewColor = FColor::White;
		break;
	case 127:
		NewColor = FColor::Red;
		break;
	case 119:
		NewColor = FColor::Green;
		break;
	case 95:
		NewColor = FColor::Blue;
		break;
	case 87:
		NewColor = FColor::Yellow;
		break;
	case 85:
		NewColor = FColor::Cyan;
		break;
	case 31:
		NewColor = FColor::Magenta;
		break;
	case 29:
		NewColor = FColor::Orange;
		break;
	case 23:
		NewColor = FColor::Purple;
		break;
	case 21:
		NewColor = FColor::Turquoise;
		break;
	case 17:
		NewColor = FColor::Silver;
		break;
	case 7:
		NewColor = FColor::Emerald;
		break;
	case 5:
		NewColor = FColor(0,128,128); // Teal
		break;
	case 1:
		NewColor = FColor(128,128,0); // Olive
		break;
	default:
		NewColor = FColor::Black;
	}

	return NewColor;
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

