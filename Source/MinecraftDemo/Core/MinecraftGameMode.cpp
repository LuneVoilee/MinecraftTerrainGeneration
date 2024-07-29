// Fill out your copyright notice in the Description page of Project Settings.
#include "MinecraftGameMode.h"
#include "../Meshes/Chunk.h"
#include "MinecraftGameMode.h"
#include "SkeletalMeshAttributes.h"
#include "..\Generators\BiomeGenerator.h"
#include "MinecraftDemo/Generators/BuildingGenerator.h"
#include "MinecraftDemo/Generators/HeightFieldGenerator.h"
#include "MinecraftDemo/Generators/HumidityGenerator.h"
#include "MinecraftDemo/Generators/PlantGenerator.h"
#include "MinecraftDemo/Generators/TemperatureGenerator.h"
#include "MinecraftDemo/Meshes/Cube.h"
#include "MinecraftDemo/Tools/DebugTools.h"
#include "MinecraftDemo/Tools/HashTools.h"


void AMinecraftGameMode::UpdatePlayerLocation(FVector Location) {
	Player_LocInChunk = FVector2D(floor(((int)Location.X)/(100*MaxWidth)),floor(((int)Location.Y)/(100.*MaxWidth)));
	
	//UE_LOG(LogTemp,Log,TEXT("Playerlocation:%f,%f"),Location.X,Location.Y);
}

void AMinecraftGameMode::UpdateChunk() {
	QUICK_SCOPE_CYCLE_COUNTER(1)
	FVector2D PlayerPosition = Player_LocInChunk;
	//UE_LOG(LogTemp,Log,TEXT("ChunkPlayerlocation:%f,%f"),PlayerPosition.X,PlayerPosition.Y);
	for(int i = -LoadRadius;i <= LoadRadius;i++) {
		for(int j = -LoadRadius;j <= LoadRadius;j++) {
			FVector2D ChunkPosition = {PlayerPosition.X+i,PlayerPosition.Y+j};
			uint64 Key = HashTools::Vec3HashToUint64(ChunkPosition.X,ChunkPosition.Y);
			
			if(!ChunkPool.Contains(Key)) {
				ChunkPool.Emplace(Key,Chunk(ChunkPosition));
				Chunk& TheChunk = ChunkPool[Key];
				LoadChunk(TheChunk);
				LoadCubeType(TheChunk);
			}
		}
	}
	
	for(int i = -DisplayRadius;i <= DisplayRadius;i++) {
		for(int j = -DisplayRadius;j <= DisplayRadius;j++) {
			FVector2D ChunkPosition = {PlayerPosition.X + i,PlayerPosition.Y + j};
			uint64 Key = HashTools::Vec3HashToUint64(ChunkPosition.X,ChunkPosition.Y);
			
			if(!DisplayedChunk.Contains(Key)) {
				DisplayedChunk.Emplace(Key);
				BuildingGenerator::GenerateBuilding(ChunkPool[Key],CubeTypePool,CubeReadyToDisplay,BuildingReadyToDisplay);
				PlantGenerator::GeneratorPlant(ChunkPool[Key],CubeTypePool,CubeReadyToDisplay);
				DisplayChunk(ChunkPool[Key]);
			}
		}
	}
	GenerateBuildingBlocks();
}

void AMinecraftGameMode::LoadChunk(Chunk& TheChunk) {
	HeightFieldGenerator::GeneratorHeightField(TheChunk);
	TemperatureGenerator::GenerateTemperatureField(TheChunk);
	HumidityGenerator::GenerateHumidityField(TheChunk);
	BiomeGenerator::GenerateBiomeField(TheChunk);
	
}

void AMinecraftGameMode::LoadCubeType(Chunk& TheChunk) {
	
	for(int i = 0;i < MaxWidth;i++) {
		for(int j = 0;j < MaxWidth;j++) {
			for(int32 depth = 0;depth <= CubeMaxDepth;depth++) {
				int32 Height = TheChunk.ChunkHeightField[i][j]-depth;
				int32 Type = CalculateCubeType(depth,TheChunk,i,j);
				uint64 CubePosKey = HashTools::Vec3HashToUint64(
					TheChunk.ChunkPosition.X*MaxWidth+i,
					TheChunk.ChunkPosition.Y*MaxWidth+j,
					Height
				);
				
				if(!CubeTypePool.Contains(CubePosKey)) {
					CubeTypePool.Emplace(CubePosKey,Type);
					//CubeReadyToDisplay.Emplace(CubePosKey,Type);
				}
			}
		}
	}
}



int32 AMinecraftGameMode::CalculateCubeType(int32 Depth,Chunk& TheChunk,int i,int j) {

	//地下石头
	if(Depth>=3){return 2;}
	//地下泥土
	if(Depth>=1){return 3;}
	//地表方块	
	switch(TheChunk.ChunkBiomeField[i][j]) {
	case Snow:	return 10;break;
	case Green:	return 1;break;
	case Dry:	return 3;break;
	case Stone:	return 5;break;
	case Desert:return 4;break;
	default:	return 0;break;
	};
}




void AMinecraftGameMode::DisplayChunk(Chunk& TheChunk) {
	  
	  for(int i = 0;i < MaxWidth;i++) {
	  	for(int j = 0;j < MaxWidth;j++) {
	  		for(int depth = 0;depth <= CubeMaxDepth;depth++) {
	  			
	  			int32 Height = TheChunk.ChunkHeightField[i][j]-depth;
	  			int32 Type = CalculateCubeType(depth,TheChunk,i,j);
	  			uint64 CubePosKey = HashTools::Vec3HashToUint64(
	  				TheChunk.ChunkPosition.X*MaxWidth+i,
	  				TheChunk.ChunkPosition.Y*MaxWidth+j,
	  				Height
	  			);
	  			
	  			if(CubeTypePool.Contains(CubePosKey)) {
	  				CubeReadyToDisplay.Emplace(CubePosKey,Type);
	  			}
	  			
	  		}
	  	}
	  }
	
	  CreateChunk();
}

void AMinecraftGameMode::CreateChunk() {
	
	const float dx[6] = {1.,-1.,0,0,0,0};
	const float dy[6] = {0,0,1.,-1.,0,0};
	const float dz[6] = {0,0,0,0,1.,-1.};
	
	//对于DisplayedChunk队列中的元素，如果其四周均存在Cube，则该元素不需要被添加
	for(auto& Cube : CubeReadyToDisplay) {
		
		uint64 Key = Cube.Get<0>();
		int32 Type = Cube.Get<1>();
		FVector CubePosition = HashTools::Uint64UnhashToVec3(Key);
		 for(int i=0;i<6;i++) {
		 	FVector NearCubePosition = CubePosition;
		 	NearCubePosition[0]+=dx[i];
		 	NearCubePosition[1]+=dy[i];
		 	NearCubePosition[2]+=dz[i];
		 	uint64 NearPos = HashTools::Vec3HashToUint64(NearCubePosition[0],NearCubePosition[1],NearCubePosition[2]);
		 	bool NeedCreate = false;
		 	if(!CubeTypePool.Contains(NearPos) ) {
		 		NeedCreate = true;
		 	}
		 	else {
		 		switch (CubeTypePool[NearPos]) {
		 		case 0:case 9:case 11:case 12:case 13:case 20:case 21:case 22:case 23:case 24:
		 			NeedCreate = true;
		 			break;
		 		}
		 	}
			if(NeedCreate) {
				if(DisplayedCube.Contains(Key)) {
					break;
				}
				CreateCube(Type,CubePosition);
				DisplayedCube.Add({Key,Type});
				break;
			}
		}
	}
	CubeReadyToDisplay.Reset();

}

void AMinecraftGameMode::CreateCube(int32 Type, FVector& Pos) {

	ACube* Cube = GetWorld()->SpawnActor<ACube>(Pos*100,FRotator::ZeroRotator);
	Cube->InitCubeByType(Type);

}
void AMinecraftGameMode::GenerateBuildingBlocks(){
	//if(BuildingReadyToDisplay.Num()==0) {
		//DebugTools::ScreenPrint(FString("SB"));
	//}
	int32 count = 0;
	//根据待显示建筑列表，进行多个ABuilding的创建
	for(auto &tuple: BuildingReadyToDisplay){
		count++;
		uint64 posIndex = tuple.Get<0>();
		int32 buildingIndex = tuple.Get<1>();
		int32 rotate = tuple.Get<2>();
		UE_LOG(LogTemp,Log,TEXT("Count:%d,PosIndex:%llu,Type:%d"),count,posIndex,buildingIndex);
		CreateBuilding(buildingIndex,rotate,HashTools::Uint64UnhashToVec3(posIndex));
	} 

	//显示完建筑就可以清理列表了
	BuildingReadyToDisplay.Reset();
}
bool AMinecraftGameMode::CreateBuilding(int32 id,int32 rotate, FVector pos){
	FString str = TEXT("Blueprint'/Game/Blueprints/BP_Building") + FString::FromInt(id)+TEXT(".BP_Building")+ FString::FromInt(id)+ TEXT("_C'");
	UClass* BPClass = LoadClass<AActor>(nullptr, *str);
	AActor* spawnActor = GetWorld()->SpawnActor<AActor>(BPClass,pos*100+FVector(-50,-50,0), FRotator(0,rotate*90,0));

	return spawnActor!=nullptr;
}