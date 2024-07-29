#include "BuildingGenerator.h"
#include "MinecraftDemo/Tools/DebugTools.h"
#include "MinecraftDemo/Tools/HashTools.h"
#include "MinecraftDemo/Tools/NoiseTools.h"
DEFINE_LOG_CATEGORY(LogBuilding);
DEFINE_LOG_CATEGORY(LogArea);

void BuildingGenerator::GenerateBuilding(Chunk& TheChunk,TMap<uint64, int32>& CubeTypePool,
                                         TMap<uint64, int32>& CubeReadyToDisplay,TArray<TTuple<uint64,int32,int32>>& BuildingReadyToDisplay) {

	if(HashTools::NonDeterministicRandom(0,1)>0.985) {
		
		GenerateBuildingArea(TheChunk,CubeTypePool,CubeReadyToDisplay);
		
		DisplayBuildings(TheChunk,CubeTypePool,CubeReadyToDisplay,BuildingReadyToDisplay);
		
		DisplayPath(TheChunk,CubeTypePool,CubeReadyToDisplay);
		
	}
	
	
}

void BuildingGenerator::GenerateBuildingArea(Chunk& TheChunk,TMap<uint64, int32>& CubeTypePool,
	TMap<uint64, int32>& CubeReadyToDisplay) {
	
	CapitalPoint =  {MaxWidth / 2 , MaxWidth / 2};
	FVector2d globalCapitalPointIndex = TheChunk.ChunkPosition * MaxWidth + MaxWidth / 2;
	int32 CapitalHeight = TheChunk.ChunkHeightField[CapitalPoint[0]][CapitalPoint[1]];
	auto greater = [](std::pair<float,TArray<int32>>& a,std::pair<float,TArray<int32>>& b) -> bool {
		return a.first > b.first;
	};
	std::priority_queue<std::pair<float,TArray<int32>>,std::vector<std::pair<float,TArray<int32>>>,decltype(greater)> PointQueue;
	PointQueue.emplace(0,CapitalPoint);
	
	const int32 dx1[9] = {0,1,0,1,-1, 0,-1,-1, 1};
	const int32 dy1[9] = {0,0,1,1, 0,-1,-1, 1,-1};
	const int32 dx2[4] = {3,0,-3, 0};
	const int32 dy2[4] = {0,3, 0,-3};
	
	while(!PointQueue.empty()) {
		float cost=PointQueue.top().first;
		TArray<int32> point=PointQueue.top().second;
		PointQueue.pop();
		
		UE_LOG(LogArea,Log,TEXT("AreaCentralPos[%d,%d]"),point[0],point[1]);
		//以3X3的正方形为一个单位，当前的最优单位解加入域
		for(int i = 0 ; i < 9 ; i++) {
			//DebugTools::ScreenPrint("ddddd");
			
			Area.Emplace(
				HashTools::Vec3HashToUint64(globalCapitalPointIndex.X+dx1[i],
					globalCapitalPointIndex.Y+dy1[i]));
		}

		//最小代价大于7时，放弃拓展
		if(cost > 7) {
			break;
		}
		
		//将上下左右四个方向的单位的中心点加入队列
		for(int i = 0 ; i < 4 ; i++) {
			int32 x = point[0] + dx2[i];
			int32 y = point[1] + dy2[i];
			int32 gx = globalCapitalPointIndex.X + dx2[i];
			int32 gy = globalCapitalPointIndex.Y + dy2[i];
			
			if(Area.Find(HashTools::Vec3HashToUint64(gx,gy))) {
				DebugTools::ScreenPrint("you");
				continue;
			}
			if(x>MaxWidth||y>MaxWidth||x<0||y<0) {
				UE_LOG(LogArea,Log,TEXT("No Prepare[%d,%d]"),x,y);
				DebugTools::ScreenPrint("chaole");
				continue;
			}
			else {
				int32 CentralHeight = TheChunk.ChunkHeightField[x][y];
				int32 deltaHeight = abs(CentralHeight-CapitalHeight);
				float ThisCost = cost + deltaHeight;
				PointQueue.emplace(ThisCost,TArray<int>{x,y});
				DebugTools::ScreenPrint("laile");
				UE_LOG(LogArea,Log,TEXT("Four Prepare[%f,%d,%d]"),ThisCost,x,y);
			}
		}
	}
}

void BuildingGenerator::DisplayBuildings(Chunk& TheChunk, TMap<uint64, int32>& CubeTypePool,
	TMap<uint64, int32>& CubeReadyToDisplay,TArray<TTuple<uint64,int32,int32>>& BuildingReadyToDisplay) {
	//三种房子的Size
	const int32 buildingSize[3][2]={{10,6},{8,6},{6,6}};
	const int32 dx[4] = {1,-1,0,0};
	const int32 dy[4] = {0,0,1,-1};
	
	std::queue<TArray<int32>> PointQueue;
	PointQueue.push(CapitalPoint);

	while(!PointQueue.empty()) {
		TArray<int32> Point = PointQueue.front();
		PointQueue.pop();

		int32 type = (int32)HashTools::NonDeterministicRandom(1,100) % 3;
		int32 rotate = (int32)HashTools::NonDeterministicRandom(1,100) % 4;
		if(!TryToPlaceOneHouse(TheChunk,buildingSize[type][0],buildingSize[type][1],type,rotate,Point,CubeTypePool,BuildingReadyToDisplay)) {
			continue;
		}

		int32 Offset = 5 + (int32)HashTools::NonDeterministicRandom(0,100) % 3;
		
		for(int i = 0 ; i < 4 ; i++) {
			
			PointQueue.push(TArray<int32>{Point[0]+dx[i]*(Offset+buildingSize[type][0]),
				Point[1]+dy[i]*(Offset+buildingSize[type][1])});
		}
		
	}
}

bool BuildingGenerator::TryToPlaceOneHouse(Chunk& TheChunk,int32 size0,int32 size1,int32 type,int32 rotate,TArray<int32> Point,TMap<uint64, int32>& CubeTypePool,TArray<TTuple<uint64,int32,int32>>& BuildingReadyToDisplay) {
	int32 rotateIndex = rotate % 2;
	int32 horizontal = 0 , vertical = 0;
	rotateIndex == 0 ? horizontal = size0 , vertical = size1 : horizontal = size1 , vertical = size0 ;
	float averageHeight = 0;
	for(int i = -horizontal ; i < horizontal ; i++) {
		for(int j = -vertical ; j < vertical ; j++) {
			//不在发展域
			DebugTools::ScreenPrint("1");
			if(!Area.Find(HashTools::Vec3HashToUint64(Point[0]+i,Point[1]+j))) {
				return false;
			}
			DebugTools::ScreenPrint("2");
			//空气
			int32 height = TheChunk.ChunkHeightField[(int32)Point[0]+i][(int32)Point[1]+j];
			int* result = CubeTypePool.Find(HashTools::Vec3HashToUint64(Point[0]+i,Point[1]+j,height));
			if(result&&(*result)==0){return false;}
			DebugTools::ScreenPrint("3");
			averageHeight += height;
			
		}
	}
	
	averageHeight /= (size0*size1);
	averageHeight = floor(averageHeight + 0.5);
	
	for(int i = -horizontal ; i < horizontal ; i++) {
		for(int j = -vertical ; j < vertical ; j++) {
			TheChunk.ChunkHeightField[(int32)Point[0]+i][(int32)Point[1]+j] = averageHeight;
			Area.Remove(HashTools::Vec3HashToUint64(Point[0]+i,Point[1]+j));
		}
	}

	//插入空气，以免生成树木花草
	for(int i=-horizontal-1;i<horizontal+1;++i)
		for(int j=-vertical-1;j<vertical+1;++j)
		{
			uint64 index = HashTools::Vec3HashToUint64(Point[0]+i,Point[1]+j,TheChunk.ChunkHeightField[(int32)Point[0]+i][(int32)Point[1]+j]+1);
			if(CubeTypePool.Find(index)) {
				CubeTypePool[index] = 0;
			}
			else {
				CubeTypePool.Emplace(index,0);
			}
		}
	
	BuildingReadyToDisplay.Emplace(HashTools::Vec3HashToUint64(Point[0],Point[1],averageHeight+1),type+1,rotate);
	Area.Emplace(HashTools::Vec3HashToUint64(Point[0]-horizontal,Point[1]-vertical));
	BuildingPoint.Push(FVector2D(Point[0]-horizontal,Point[1]-vertical));
	return true;
	
}

void BuildingGenerator::DisplayPath(Chunk& TheChunk, TMap<uint64, int32>& CubeTypePool,
	TMap<uint64, int32>& CubeReadyToDisplay) {
}
