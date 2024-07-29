#include "BiomeGenerator.h"

void BiomeGenerator::GenerateBiomeField(Chunk& TheChunk) {
	for(int i = 0 ; i < MaxWidth ; i++) {
		for(int j = 0 ; j < MaxWidth ; j++) {
			float temperature = TheChunk.ChunkTemperatureField[i][j];
			float humidity = TheChunk.ChunkHumidityField[i][j];
			Biome CubeBiome = None;

			if(temperature > 0.1)  {
				
				if(humidity<0.3) {
					CubeBiome = Desert;
				}
				
				else if(0.3<=humidity&&humidity<0.5){
					CubeBiome = Dry;
				}
				else {
					CubeBiome = Green;
				}
				
			}
			else {
				
				if(humidity<0.5) {
					CubeBiome = Stone;
				}
				else {
					CubeBiome = Snow;
				}
				
			}
			
			TheChunk.ChunkBiomeField[i][j] = int32(CubeBiome);
			//UE_LOG(LogTemp,Log,TEXT("ChunkField[%d][%d]: Biome:%d; Height:%d,temperature:%f; humidity:%f"),i,j,CubeBiome,TheChunk.ChunkHeightField[i][j],temperature,humidity);
		}
	}
}
