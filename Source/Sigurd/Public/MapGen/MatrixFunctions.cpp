#include "MatrixFunctions.h"

void FMatrixFunctions::PrintMatrix(TArray<TArray<int>> Matrix, const bool IsError)
{
    for (int i = 0; i < Matrix.Num(); i++)
    {
        FString Line = "";
        for (int j = 0; j < Matrix[i].Num(); j++)
        {
            Line += FString::Printf(TEXT("%d "), Matrix[i][j]);
        }

        if (IsError)
        {
        	UE_LOG(LogTemp, Error, TEXT("%s"), *Line);
        }
        else
        {
        	UE_LOG(LogTemp, Warning, TEXT("%s"), *Line);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT(" "));
}

int FMatrixFunctions::GetNeighboursCount(TArray<TArray<int>> Matrix) 
{
	int Neighbors = 0;
	for (int i = 0; i < Matrix.Num(); i++)
	{
		for (int j = 0; j < Matrix[i].Num(); j++)
		{
			if (Matrix[i][j])
			{
				Neighbors++;
			}
		}
	}
	return Neighbors;
}

TArray<TArray<int>> FMatrixFunctions::GetNeighbours(const FVector2D GridSize ,const int I, const int J,TArray<TArray<int>>& ModuleNumbers)
{

	TArray<TArray<int>> Matrix;
	Matrix.SetNum(3);
	for (int i = 0; i < 3; i++)
	{
		Matrix[i].SetNum(3, false);
	}
    
	// Up Down Left Right
	if (I > 0 && ModuleNumbers[I - 1][J] != 0)
	{
		Matrix[0][1] = 1;
	}
	if (I < GridSize.X - 1 && ModuleNumbers[I + 1][J] != 0)
	{
		Matrix[2][1] = 1;
	}
	if (J > 0 && ModuleNumbers[I][J - 1] != 0)
	{
		Matrix[1][0] = 1;
	}
	if (J < GridSize.Y - 1 && ModuleNumbers[I][J + 1] != 0)
	{
		Matrix[1][2] = 1;
	}

	// Diagonals
	if (I > 0 && J < GridSize.Y - 1 && ModuleNumbers[I - 1][J + 1] != 0)
	{
		Matrix[0][2] = 1;
	}
	if (I < GridSize.X - 1 && J < GridSize.Y - 1 && ModuleNumbers[I + 1][J + 1] != 0)
	{
		Matrix[2][2] = 1;
	}
	if (I < GridSize.X - 1 && J > 0 && ModuleNumbers[I + 1][J - 1] != 0)
	{
		Matrix[2][0] = 1;
	}
	if (I > 0 && J > 0 && ModuleNumbers[I - 1][J - 1] != 0)
	{
		Matrix[0][0] = 1;
	}
    
	return Matrix;
}

TArray<TArray<int>> FMatrixFunctions::RotateMatrix(const TArray<TArray<int>>& Matrix)
{
	const int N = Matrix.Num();
	TArray<TArray<int>> RotatedMatrix;
	RotatedMatrix.SetNum(N);
	for (int i = 0; i < N; i++)
	{
		RotatedMatrix[i].SetNum(N);
		for (int j = 0; j < N; j++)
		{
			RotatedMatrix[i][j] = Matrix[N - j - 1][i];
		}
	}
	return RotatedMatrix;
}

bool FMatrixFunctions::CompareMatrix(TArray<TArray<int>> Matrix1, const int PlotNum, const int X, const int Y, TArray<TArray<int>>& ModuleRotations) 
{
	TArray<TArray<int>> Matrix2 = FilterMatrix[PlotNum];
	Matrix1[1][1] = false;
	Matrix2[1][1] = false;
	
	for (int i = 0; i <= 3; i++)
	{
		bool bIsEqual = true;
		for (int j = 0; j < Matrix1.Num(); j++)
		{
			for (int k = 0; k < Matrix1[j].Num(); k++)
			{
				// If the element is 0 or 1, then check if they are equal
				if ((Matrix2[j][k] == 0 || Matrix2[j][k] == 1) && Matrix1[j][k] != Matrix2[j][k])
				{
					bIsEqual = false;
					break;
				}
			}
			if (!bIsEqual)
			{
				break;
			}
		}
		if (bIsEqual)
		{
			ModuleRotations[X][Y] += 1-i;
			return true;
		}
		Matrix2 = RotateMatrix(Matrix2);
	}

	return false;
}
