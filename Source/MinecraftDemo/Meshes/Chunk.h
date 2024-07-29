// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// int32 MaxHeight = 128;
const int32 MaxWidth = 8;


class MINECRAFTDEMO_API Chunk 
{

public:	

	Chunk();
	Chunk(FVector2D Position);
	
	//中央位置
	FVector2D ChunkPosition;
	//高度场
	int32 ChunkHeightField[MaxWidth][MaxWidth];
	
	//chunk地形温度[-1.0f,1.0f]
	float ChunkTemperatureField[MaxWidth][MaxWidth];
	
	//chunk地形湿度[0.0f,1.0f]
	float ChunkHumidityField[MaxWidth][MaxWidth];

	//chunk生物群落属性[1,2,3,4...]
	int32 ChunkBiomeField[MaxWidth][MaxWidth];
};
