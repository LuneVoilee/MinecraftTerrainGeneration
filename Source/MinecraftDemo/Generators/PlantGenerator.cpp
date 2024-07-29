#include "PlantGenerator.h"

#include "MinecraftDemo/Tools/NoiseTools.h"


DEFINE_LOG_CATEGORY(LOG_PLANT);
#include "BiomeGenerator.h"
#include "MinecraftDemo/Tools/HashTools.h"

void PlantGenerator::GeneratorPlant(Chunk& TheChunk, TMap<uint64, int32>& CubeTypePool,
                                    TMap<uint64, int32>& CubeReadyToDisplay) {
	for(int i=0;i<MaxWidth;i++) {
		for(int j=0;j<MaxWidth;j++) {
			// UE_LOG(LOG_PLANT,Log,TEXT("ChunkField[%d][%d]: Biome:%d; temperature:%f; humidity:%f"),
			// 	i,j,
			// 	TheChunk.ChunkBiomeField[i][j],
			// 	TheChunk.ChunkTemperatureField[i][j],
			// 	TheChunk.ChunkHumidityField[i][j]);
			//地形为石头，沙漠或空地，无法生成植被
			int32 biome = TheChunk.ChunkBiomeField[i][j];
			if(biome == Stone || biome == Desert || biome == None) {
				continue;
			}
			//方块不存在 或 方块存在，但被挖空（方块ID为0）或者方块ID为9（方块为水）就无法生成
			uint64 CubeKey = HashTools::Vec3HashToUint64(
					  TheChunk.ChunkPosition.X*MaxWidth+i,
					  TheChunk.ChunkPosition.Y*MaxWidth+j,
					  TheChunk.ChunkHeightField[i][j]
				  );
			int32* FindRes = CubeTypePool.Find(CubeKey);
			if(!FindRes) {
				continue;
			}
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "aaaa");
			if(*FindRes == 0 || *FindRes == 9) {
				continue;
			}
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "bbbb");
			//如果上方存在方块，不生成
			uint64 UpperCubeKey = HashTools::Vec3HashToUint64(
					  TheChunk.ChunkPosition.X*MaxWidth+i,
					  TheChunk.ChunkPosition.Y*MaxWidth+j,
					  TheChunk.ChunkHeightField[i][j] + 1
				  );
			if(CubeTypePool.Find(UpperCubeKey)) {
				continue;
			}
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "4444");
			HashTools::SetSeed(1919810);
			
			if(GeneratorTree(TheChunk,CubeTypePool,CubeReadyToDisplay,i,j)) {
				continue;
			}
			
			if(GeneratorFlower(TheChunk,CubeTypePool,CubeReadyToDisplay,i,j)) {
				continue;
			}
			
			
		}
	}
}

bool PlantGenerator::GeneratorFlower(Chunk& TheChunk, TMap<uint64, int32>& CubeTypePool,
	TMap<uint64, int32>& CubeReadyToDisplay,int32 i,int32 j) {
	int32 biome = TheChunk.ChunkBiomeField[i][j];
	
	int32 Type = 0;
	double possibility = abs(HashTools::NonDeterministicRandom(0,1) - 0.5);
	
	//方块ID：20=黄花、21=蓝花、22=蘑菇、23=蘑菇
	//方块ID：11=绿草 12=黄草，13=白草
	switch (biome) {
	case Green:
		if(possibility>0.4) {
			Type = 20 + getRandInt(i * 69 + j,i + j ^ 2) % 4;
		}
		else if(possibility>0.3) {
			Type = 11;
		}
		break;
		
	case Snow:
		if(possibility>0.4) {
			Type = 13;;
		}
		break;
		
	case Dry:
		if(possibility>0.4) {
			Type = 12;
		}
		break;
		
	default:
		break;
		
	}
	
	if(Type==0)
		return false;
	
	DisplayPlant(TheChunk,CubeTypePool,CubeReadyToDisplay,i,j,TheChunk.ChunkHeightField[i][j]+1,Type);
	
	return true;
}

bool PlantGenerator::GeneratorTree(Chunk& TheChunk, TMap<uint64, int32>& CubeTypePool,
	TMap<uint64, int32>& CubeReadyToDisplay,int32 i,int32 j) {
	
	int32 biome = TheChunk.ChunkBiomeField[i][j];
	float temperature = TheChunk.ChunkTemperatureField[i][j];
	float humidity = TheChunk.ChunkHumidityField[i][j];
	
	//14=温带木 15=热带木，16=寒带木
	//17=温带树叶 18=寒带树叶，19=热带树叶
	int32 WoodType = 0;
	int32 LeavesType = 0;
	double possibility = abs(HashTools::NonDeterministicRandom(0,1) - 0.5);
	
	switch (biome) {
		
		case Green:
			if(possibility>0.4995) {
				WoodType = 14;
				LeavesType = 17;
			}
			break;
		
		case Snow:
			if(possibility>0.4995) {
				WoodType = 16;
				LeavesType = 18;
			}
			break;
		
		case Dry:
			if(possibility>0.4995) {
				WoodType = 15;
				LeavesType = 19;
			}
			break;
		
		default:
			break;
	}
	
	if(WoodType == 0 || LeavesType == 0 ) {
		return false;
	}

	int32 treeHeight = floor(HashTools::NonDeterministicRandom(1,3))+3;
	for(int k = 1;k<=treeHeight;++k){
		//生成树干
		DisplayPlant(TheChunk,CubeTypePool,CubeReadyToDisplay,i,j,TheChunk.ChunkHeightField[i][j]+k,WoodType) ;
	}
	FVector2d pf(float(i) / MaxWidth / 16, float(j) / MaxWidth / 16);
	int32 t1 = (getRandInt(10*pf.X,10*pf.Y)%1024/1024)*4+1+int32(treeHeight>=5);
	int32 t2 = (getRandInt(5*pf.X,5*pf.Y)%1024/1024)*4+1+int32(treeHeight>=5);
	int32 MaxLeavesHeight = treeHeight;//treeHeight+t1%3;
	int32 MinLeavesHeight = 1+t2%2+int32(treeHeight>=5);
	
	for(int k = MinLeavesHeight ; k<=MaxLeavesHeight ; k++) {
		//贝塞尔曲线计算树叶半径
		float leafRadius = 
			NoiseTools::bezier4(
				FVector2D(0,0)
				,FVector2D(0.25f,t1)
				,FVector2D(0.50f,(t1+t2)/2+1)
				,FVector2D(0.75,t2)
				,FVector2D(1,0)
				,float(k-MinLeavesHeight)/(MaxLeavesHeight-MinLeavesHeight)).Y;
		UE_LOG(LOG_PLANT,Log,TEXT("leafRadius[%d]:%f;t1:%d;t2:%d"),TheChunk.ChunkHeightField[i][j]+1+k,leafRadius,t1,t2);
		//生成树叶
		GenerateLeaves(TheChunk,CubeTypePool,CubeReadyToDisplay,i,j,TheChunk.ChunkHeightField[i][j]+1+k,leafRadius,LeavesType);

	}
	
	return true;
}

void PlantGenerator::GenerateLeaves(
	Chunk& TheChunk,
	TMap<uint64, int32>& CubeTypePool,
	TMap<uint64, int32>& CubeReadyToDisplay,
	int32 x,int32 y,int32 height,int32 radius,int32 LeavesType){
	
	radius = FMath::Clamp(radius,0,3);
	//x,y是树干在Chunk内的坐标位置，x+i\y+j表示以树干为圆心的树叶半径
	for(int i = -2 ; i <= 2 ; ++i)
		for(int j = -2 ; j <= 2 ; ++j){
			if(!leavesTemplate[radius][i+2][j+2])
				continue;
        
			int32 dx = x+i;
			int32 dy = y+j;

			DisplayPlant(TheChunk,CubeTypePool,CubeReadyToDisplay,dx,dy,height,LeavesType);
			
		}
	return;
}



int32 PlantGenerator::getRandInt(int32 a , int32 b) {
	return HashTools::Int32HashToInt32(a)+HashTools::Int32HashToInt32(b);
}

void PlantGenerator::DisplayPlant(Chunk& TheChunk,TMap<uint64, int32>& CubeTypePool,
	TMap<uint64, int32>& CubeReadyToDisplay,int32 i,int32 j,int32 height,int32 Type) {
	
	uint64 CubePosKey = HashTools::Vec3HashToUint64(
					  TheChunk.ChunkPosition.X*MaxWidth+i,
					  TheChunk.ChunkPosition.Y*MaxWidth+j,
					  height
				  );
	if(!CubeTypePool.Contains(CubePosKey)) {
		CubeTypePool.Emplace(CubePosKey,Type);
	}
	else {
		return;
	}
	if(CubeTypePool.Contains(CubePosKey)) {
		CubeReadyToDisplay.Emplace(CubePosKey,Type);
	}
	
}

