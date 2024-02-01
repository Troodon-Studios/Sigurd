#pragma once
#include "CoreMinimal.h"

class FMatrixFunctions
{
	
public:
	static auto PrintMatrix(TArray<TArray<int>> Matrix) -> void;
	static bool CompareMatrix(TArray<TArray<int>> Matrix1, const int PlotNum, int X, int Y, TArray<TArray<int>>& ModuleRotations);
	static TArray<TArray<int>> RotateMatrix(const TArray<TArray<int>>& Matrix);
	static TArray<TArray<int>> GetNeighbours(FVector2D GridSize ,const int I, const int J,TArray<TArray<int>>& ModuleNumbers);
	static int GetNeighboursCount(TArray<TArray<int>> Matrix);
	
	inline static TMap<int,TArray<TArray<int>>> FilterMatrix = TMap<int,TArray<TArray<int>>>();
	inline static TMap<int, TArray<int>> NeighborsNumbersMap = TMap<int, TArray<int>>();

	
	// Constructor
	FMatrixFunctions()
	{
		int o = 1;
		int x = 0;
		int i = 2;

		FilterMatrix.Add(255, {{o,o,o},{o,x,o},{o,o,o}});
		FilterMatrix.Add(127, {{x,o,o},{o,x,o},{o,o,o}});
		FilterMatrix.Add(119, {{x,o,o},{o,x,o},{o,o,x}});
		FilterMatrix.Add(95,  {{x,o,o},{o,x,o},{x,o,o}});
		FilterMatrix.Add(87,  {{x,o,o},{o,x,o},{x,o,x}});
		FilterMatrix.Add(85,  {{x,o,x},{o,x,o},{x,o,x}});
		FilterMatrix.Add(31,  {{i,o,o},{x,x,o},{i,o,o}});
		FilterMatrix.Add(29,  {{i,o,x},{x,x,o},{i,o,o}});
		FilterMatrix.Add(23,  {{i,o,o},{x,x,o},{i,o,x}});
		FilterMatrix.Add(21,  {{i,o,x},{x,x,o},{i,o,x}});
		FilterMatrix.Add(17,  {{i,o,i},{x,x,x},{i,o,i}});
		FilterMatrix.Add(7,   {{i,o,o},{x,x,o},{x,x,i}});
		FilterMatrix.Add(5,   {{i,o,x},{x,x,o},{x,x,i}});
		FilterMatrix.Add(1,   {{i,o,i},{x,x,x},{x,x,x}});
		FilterMatrix.Add(0,   {{x,x,x},{x,x,x},{x,x,x}});
		
		NeighborsNumbersMap.Add(1, {1});
		NeighborsNumbersMap.Add(2, {5, 17, 1});
		NeighborsNumbersMap.Add(3, {7, 21, 5, 1, 17});
		NeighborsNumbersMap.Add(4, {29, 85, 23, 5, 7, 17, 21});
		NeighborsNumbersMap.Add(5, {31, 87, 7, 17, 21, 23, 29});
		NeighborsNumbersMap.Add(6, {95, 119, 17, 23, 29, 31});
		NeighborsNumbersMap.Add(7, {127, 31});
		NeighborsNumbersMap.Add(8, {255});
	}
	
};
