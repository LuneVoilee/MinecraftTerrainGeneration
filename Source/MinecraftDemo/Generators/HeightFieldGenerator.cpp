#include "HeightFieldGenerator.h"

#include <unordered_map>

#include "Field/FieldSystemNoiseAlgo.h"
#include "MinecraftDemo/Tools/HashTools.h"

void HeightFieldGenerator::GeneratorHeightField(Chunk& TheChunk, TMap<uint64, int32>& HeightPool) {
	//丘陵，山脉，平地
	//FString str=FString::Printf(TEXT("ChunkPosition:%f,%f"),TheChunk.ChunkPosition.X,TheChunk.ChunkPosition.Y);
	//GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Yellow, *str);
	TArray<int> size={4,16,64};
	TArray<float> weight = {0.1,0.2,0.7};
	TArray<float> maxHeight={30,50,40};
	for(int i=0;i<MaxWidth;i++) {
		for(int j=0;j<MaxWidth;j++) {
			for(int d =0;d<3;d++) {
				
				float paramX = ((float)i / (float)MaxWidth + TheChunk.ChunkPosition.X) / size[d];
				float paramY = ((float)j / (float)MaxWidth + TheChunk.ChunkPosition.Y) / size[d];
				
				TheChunk.ChunkHeightField[i][j] += (maxHeight[d] + NoiseTools::PerlinNoise2D(paramX,paramY) * maxHeight[d])
				* weight[d];
				//UE_LOG(LogTemp,Log,TEXT("Chunk:%f,%f * HeightField[%d][%d]:%d\n"),TheChunk.ChunkPosition.X,TheChunk.ChunkPosition.Y,i,j,TheChunk.ChunkHeightField[i][j]);
			
			}
			FVector2d offset = TheChunk.ChunkPosition * MaxWidth;
			uint64 key = HashTools::Vec3HashToUint64(offset.X+i,offset.Y+j);
			HeightPool.Add(key,TheChunk.ChunkHeightField[i][j]);
		}
	}
	return;
}
