#include "TemperatureGenerator.h"


void TemperatureGenerator::GenerateTemperatureField(Chunk& TheChunk) {
	HashTools::SetSeed(4245314532386);

	for(int i=0;i<MaxWidth;i++) {
		for(int j=0;j<MaxWidth;j++) {
				
				float paramX = ((float)i / (float)MaxWidth + TheChunk.ChunkPosition.X) / 16;
				float paramY = ((float)j / (float)MaxWidth + TheChunk.ChunkPosition.Y) / 16;
				
				TheChunk.ChunkTemperatureField[i][j] +=
					(NoiseTools::PerlinNoise2D(paramX,paramY)+1)/2  * 0.6;
				if(TheChunk.ChunkHeightField[i][j]>50) {
					TheChunk.ChunkTemperatureField[i][j] -= 1;
				}
				
				//定义ChunkTemperatureField描述低温程度
				
				//TheChunk.ChunkTemperatureField[i][j] += TheChunk.ChunkHeightField[i][j] / 50;
				//UE_LOG(LogTemp,Log,TEXT("Chunk:%f,%f * HeightField[%d][%d]:%d\n"),TheChunk.ChunkPosition.X,TheChunk.ChunkPosition.Y,i,j,TheChunk.ChunkHeightField[i][j]);
			
		}
	}
}