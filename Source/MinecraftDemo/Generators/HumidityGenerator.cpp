#include "HumidityGenerator.h"



void HumidityGenerator::GenerateHumidityField(Chunk& TheChunk) {
	
	HashTools::SetSeed(3254434);
	
	TArray<int> size={4,16,64};
	TArray<float> weight = {0.33,0.33,0.33};
	for(int i=0;i<MaxWidth;i++) {
		for(int j=0;j<MaxWidth;j++) {
			for(int d =0;d<3;d++) {
				
				float paramX = ((float)i / (float)MaxWidth + TheChunk.ChunkPosition.X) / size[d];
				float paramY = ((float)j / (float)MaxWidth + TheChunk.ChunkPosition.Y) / size[d];
				
				TheChunk.ChunkHumidityField[i][j] += (NoiseTools::PerlinNoise2D(paramX,paramY)+1)/2
				* weight[d];

				//UE_LOG(LogTemp,Log,TEXT("Chunk:%f,%f * HeightField[%d][%d]:%d\n"),TheChunk.ChunkPosition.X,TheChunk.ChunkPosition.Y,i,j,TheChunk.ChunkHeightField[i][j]);
			
			}
		}
	}
	return;
}
