// Fill out your copyright notice in the Description page of Project Settings.

#include "GridPathfinder.h"
#include "Engine.h"

constexpr EDirectionEnum EveryDirection[] =
	{ EDirectionEnum::DE_Backward, EDirectionEnum::DE_Forward, EDirectionEnum::DE_Left, EDirectionEnum::DE_Right };


TArray<EDirectionEnum> GridPathfinder::GetPath
	(const ATile* Origin, const ATile* Destiny, int32 MaxDepth, const AGrid* Grid)
{
	return GetPath(Grid->GetPosition(Origin), Grid->GetPosition(Destiny), MaxDepth, Grid);
}

TArray<EDirectionEnum> GridPathfinder::GetPath
	(const FPosition& Origin, const FPosition& Destiny, int32 MaxDepth, const AGrid* Grid)
{
	if (!Grid->isEmpty(Destiny) || MaxDepth == 0)
	{
		return TArray<EDirectionEnum>();
	}

	TArray<TArray<EDirectionEnum>> PossiblePaths;

	/* First exploration */
	for (const auto& Direction : EveryDirection)
	{
		TArray<EDirectionEnum> NewPath{ Direction };

		FPosition PathPosition(GetWhereAmI(Origin, NewPath));

		if (PathPosition == Destiny)
		{
			return NewPath;
		}

		if (Grid->isEmpty(PathPosition))
		{
			PossiblePaths.Add(NewPath);
		}
	}

	/* Aqu� es donde ocurre la explosi�n combinatoria */
	for (uint8 CurrentDepth = 1; CurrentDepth < MaxDepth; CurrentDepth++)
	{
		uint32 StartingPathsIndex = PossiblePaths.Num(); // Needed because Possible Paths will grow

		for (uint32 PathIndex = 0; PathIndex < StartingPathsIndex; PathIndex++)
		{
			for (const auto& Direction : EveryDirection)
			{
				TArray<EDirectionEnum> NewPath(PossiblePaths[PathIndex]);
				NewPath.Add(Direction);

				FPosition PathPosition(GetWhereAmI(Origin, NewPath));

				if (PathPosition == Destiny)
				{
					return NewPath;
				}

				if (Grid->isEmpty(PathPosition))
				{
					PossiblePaths.Add(NewPath);
				}
			}
		}
	}

	return TArray<EDirectionEnum>();
}

FPosition GridPathfinder::GetWhereAmI
	(FPosition Origin, const TArray<EDirectionEnum>& Try)
{
	for (const auto& Direction : Try)
	{
		switch (Direction)
		{
			case EDirectionEnum::DE_Forward:
				Origin.Row++;
				break;

			case EDirectionEnum::DE_Backward:
				Origin.Row--;
				break;

			case EDirectionEnum::DE_Right:
				Origin.Column++;
				break;

			case EDirectionEnum::DE_Left:
				Origin.Column--;
				break;

			default:
				break;
		}
	}

	return Origin;

}

void GridPathfinder::PrintPath(const TArray<EDirectionEnum>& Path)
{
	for (const auto& Direction : Path)
	{
		switch (Direction)
		{
		case EDirectionEnum::DE_Forward:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Arriba");
			break;

		case EDirectionEnum::DE_Backward:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Abajo");
			break;

		case EDirectionEnum::DE_Right:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Derecha");
			break;

		case EDirectionEnum::DE_Left:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Izquierda");
			break;

		default:
			break;
		}
	}
}