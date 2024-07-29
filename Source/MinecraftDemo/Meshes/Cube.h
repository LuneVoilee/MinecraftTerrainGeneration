// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cube.generated.h"

//一共23种方块+0代表空气
const int32 TotalType = 24;
static UStaticMesh* CubeMeshes[TotalType];

UCLASS()
class MINECRAFTDEMO_API ACube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACube();
	
	inline int32 GetCubeType() { 
		return Type;
	}

	void InitCubeByType(int32 CubeType);
	
	
	
protected:
	UPROPERTY(EditAnywhere)
	int32 Type = 0;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	
};
