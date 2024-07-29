// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MinecraftDemo/Meshes/Chunk.h"
#include "MinecraftGameMode.generated.h"

//以角色为中心，周围7*7个的块区作为需要加载的块区，而周围5*5个块区作为需要渲染的块区。
const int32 CubeMaxDepth=10;

const int32 LoadSize = 17;
const int32 LoadRadius = 8;//( LoadRadius / 2 )
//const int32 LoadCenter = 6;//(LoadRadius - 1)

const int32 DisplaySize = 15;
const int32 DisplayRadius = 7;//( DisplaySize / 2 )
//const int32 DisplayCenter = 4;//(DisplayRadius - 1)


/**
 * 
 */
UCLASS()
class MINECRAFTDEMO_API AMinecraftGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateChunk();
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerLocation(FVector location);
	
	
	TMap<uint64,Chunk> ChunkPool={};
	TSet<uint64> DisplayedChunk={};
	
	TMap<uint64,int32> CubeTypePool={};
	TMap<uint64,int32> CubeReadyToDisplay={};
	TMap<uint64,int32> DisplayedCube={};
	TArray<TTuple<uint64,int32,int32>> BuildingReadyToDisplay={};
private:
	//玩家所在的那个Chunk的位置
	FVector2D Player_LocInChunk;
	void LoadChunk(Chunk& TheChunk);
	void LoadCubeType(Chunk& TheChunk);
	void DisplayChunk(Chunk& TheChunk);
	int32 CalculateCubeType(int32 Depth,Chunk& TheChunk,int i,int j);
	void CreateChunk();
	void CreateCube(int32 Type,FVector& Pos);
	void GenerateBuildingBlocks();
	bool CreateBuilding(int32 id, int32 rotate, FVector pos);
};




