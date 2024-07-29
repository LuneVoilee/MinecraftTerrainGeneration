#include "BuildingGenerator.h"
#include "MinecraftDemo/Tools/DebugTools.h"
#include "MinecraftDemo/Tools/HashTools.h"
#include "MinecraftDemo/Tools/NoiseTools.h"
DEFINE_LOG_CATEGORY(LogBuilding);
DEFINE_LOG_CATEGORY(LogArea);

void BuildingGenerator::GenerateBuilding(TMap<uint64, int32>& HeightMap, Chunk& TheChunk,
                                         TMap<uint64, int32>& CubeTypePool, TMap<uint64, int32>& CubeReadyToDisplay, TArray<TTuple<uint64, int32, int32>>&
                                         BuildingReadyToDisplay) {

	
	if(HashTools::NonDeterministicRandom(0,1)>0.7) {
		
		globalOffset = TheChunk.ChunkPosition * MaxWidth;
		
		GenerateBuildingArea(HeightMap,TheChunk,CubeTypePool,CubeReadyToDisplay);
		
		DisplayBuildings(HeightMap,TheChunk,CubeTypePool,CubeReadyToDisplay, BuildingReadyToDisplay);
		
		//DisplayPath(TheChunk,CubeTypePool,CubeReadyToDisplay);
		
	}
	
}

void BuildingGenerator::GenerateBuildingArea(TMap<uint64, int32>& HeightMap, Chunk& TheChunk,
                                             TMap<uint64, int32>& CubeTypePool, TMap<uint64, int32>& CubeReadyToDisplay) {
	
	CapitalPoint =  {MaxWidth / 2 , MaxWidth / 2};
	
	int32 CapitalHeight = TheChunk.ChunkHeightField[CapitalPoint[0]][CapitalPoint[1]];
	auto greater = [](std::pair<float,Point>& a,std::pair<float,Point>& b) -> bool {
		return a.first > b.first;
	};
	std::priority_queue<std::pair<float,Point>,std::vector<std::pair<float,Point>>,decltype(greater)> PointQueue;
	PointQueue.emplace(0,CapitalPoint);
	
	const int32 dx1[9] = {0,1,0,1,-1, 0,-1,-1, 1};
	const int32 dy1[9] = {0,0,1,1, 0,-1,-1, 1,-1};
	const int32 dx2[4] = {3,0,-3, 0};
	const int32 dy2[4] = {0,3, 0,-3};
	
	while(!PointQueue.empty()) {
		float cost=PointQueue.top().first;
		Point point=PointQueue.top().second;
		PointQueue.pop();
		
		
		//以3X3的正方形为一个单位，当前的最优单位解加入域
		for(int i = 0 ; i < 9 ; i++) {
			UE_LOG(LogArea,Warning,TEXT("AreaPos[%d,%d]"),(int32)globalOffset.X+point[0]+dx1[i],(int32)globalOffset.Y+point[1]+dy1[i]);
			UE_LOG(LogArea,Log,TEXT("1"));
			Point tempP(globalOffset.X+point[0]+dx1[i],globalOffset.Y+point[1]+dy1[i]);
			UE_LOG(LogArea,Log,TEXT("2"));
			Point* p = Area.Find(tempP);
			UE_LOG(LogArea,Log,TEXT("3"));
			if(!p) {
				Area.Emplace(tempP);
			}
			else {
				UE_LOG(LogArea,Log,TEXT("wocwocwoc"));
			}
		}

		//最小代价大于7时，放弃拓展
		if(cost > 7) {
			break;
		}
		
		//将上下左右四个方向的单位的中心点加入队列
		for(int i = 0 ; i < 4 ; i++) {
			int32 x = point[0] + dx2[i];
			int32 y = point[1] + dy2[i];
			int32 gx = globalOffset.X + point[0] + dx2[i];
			int32 gy = globalOffset.Y + point[1] + dy2[i];
			
			if(Area.Find({gx,gy})) {
				//DebugTools::ScreenPrint("you");
				continue;
			}
			else {
				int32 CentralHeight = getHeight(HeightMap,TheChunk,gx,gy);
				int32 deltaHeight = abs(CentralHeight-CapitalHeight);
				float ThisCost = cost + deltaHeight;
				PointQueue.emplace(ThisCost,Point{x,y});
				//DebugTools::ScreenPrint("laile");
				UE_LOG(LogArea,Log,TEXT("Four Prepare[%f,%d,%d]"),ThisCost,x,y);
			}
		}
	}
}

void BuildingGenerator::DisplayBuildings(TMap<uint64, int32>& HeightMap, Chunk& TheChunk,
                                         TMap<uint64, int32>& CubeTypePool, TMap<uint64, int32>& CubeReadyToDisplay, TArray<TTuple<uint64, int32, int32>>&
                                         BuildingReadyToDisplay) {
	//三种房子的Size
	const int32 buildingSize[3][2]={{10,6},{8,6},{6,6}};
	const int32 dx[4] = {1,-1,0,0};
	const int32 dy[4] = {0,0,1,-1};
	
	std::queue<Point> PointQueue;
	PointQueue.push(CapitalPoint);

	while(!PointQueue.empty()) {
		Point point = PointQueue.front();
		PointQueue.pop();

		int32 type = (int32)HashTools::NonDeterministicRandom(1,100) % 3;
		int32 rotate = (int32)HashTools::NonDeterministicRandom(1,100) % 4;
		if(!TryToPlaceOneHouse(HeightMap,TheChunk,buildingSize[type][0],buildingSize[type][1],type,rotate,point,CubeTypePool, BuildingReadyToDisplay)) {
			continue;
		}

		int32 Offset = 5 + (int32)HashTools::NonDeterministicRandom(0,100) % 3;
		
		for(int i = 0 ; i < 4 ; i++) {
			
			PointQueue.push(Point{point[0]+dx[i]*(Offset+buildingSize[type][0]),
				point[1]+dy[i]*(Offset+buildingSize[type][1])});
		}
		
	}
}

bool BuildingGenerator::TryToPlaceOneHouse(TMap<uint64, int32>& HeightMap, Chunk& TheChunk, int32 size0, int32 size1, int32 type, int32 rotate, Point point, TMap<
                                           uint64, int32>& CubeTypePool, TArray<TTuple<uint64, int32, int32>>& BuildingReadyToDisplay) {
	int32 rotateIndex = rotate % 2;
	int32 horizontal, vertical;
	rotateIndex == 0 ? horizontal = size0 , vertical = size1 : horizontal = size1 , vertical = size0 ;
	float averageHeight = 0;
	for(int i = -horizontal ; i < horizontal ; i++) {
		for(int j = -vertical ; j < vertical ; j++) {
			//不在发展域
			
			DebugTools::ScreenPrint("1");
			
			if(!Area.Find(Point(globalOffset.X+point[0]+i,globalOffset.Y+point[1]+j))) {
				UE_LOG(LogArea,Warning,TEXT("can't find block in Area,PosNow is[%f,%f]"),globalOffset.X+point[0]+i,globalOffset.Y+point[1]+j)
				return false;
			}
			
			//空气
			int32 height = TheChunk.ChunkHeightField[point[0]+i][point[1]+j];
			int* result = CubeTypePool.Find(HashTools::Vec3HashToUint64(globalOffset.X+point[0]+i,globalOffset.X+point[1]+j,height));
			if(result&&(*result)==0){return false;}
			
			DebugTools::ScreenPrint("2");
			averageHeight += height;
		}
	}
	
	averageHeight /= (size0*size1);
	averageHeight = floor(averageHeight + 0.5);
	
	for(int i = -horizontal ; i < horizontal ; i++) {
		for(int j = -vertical ; j < vertical ; j++) {
			setHeight(HeightMap,TheChunk,point[0]+i,point[1]+j,averageHeight);
			TheChunk.ChunkHeightField[point[0]+i][point[1]+j] = averageHeight;
			Area.Remove(Point(globalOffset.X+point[0]+i,globalOffset.Y+point[1]+j));
		}
	}

	//插入空气，以免生成树木花草
	for(int i=-horizontal-1;i<horizontal+1;++i)
		for(int j=-vertical-1;j<vertical+1;++j)
		{
			uint64 index = HashTools::Vec3HashToUint64(globalOffset.X+point[0]+i,globalOffset.Y+point[1]+j,TheChunk.ChunkHeightField[point[0]+i][point[1]+j]+1);
			if(CubeTypePool.Find(index)) {
				CubeTypePool[index] = 0;
			}
			else {
				CubeTypePool.Emplace(index,0);
			}
		}
	
	BuildingReadyToDisplay.Emplace(HashTools::Vec3HashToUint64(globalOffset.X+point[0],globalOffset.Y+point[1],averageHeight+1),type+1,rotate);
	Area.Emplace(Point(globalOffset.X+point[0]-horizontal,globalOffset.Y+point[1]-vertical));
	BuildingPoint.Push(FVector2D(point[0]-horizontal,point[1]-vertical));
	return true;
	
}

void BuildingGenerator::DisplayPath(Chunk& TheChunk, TMap<uint64, int32>& CubeTypePool,
                                    TMap<uint64, int32>& CubeReadyToDisplay) {
}

int32 BuildingGenerator::getHeight(TMap<uint64, int32>& HeightMap, const Chunk& TheChunk, int32 gx, int32 gy) {
	try {
		uint64 key = HashTools::Vec3HashToUint64(gx,gy);
		int32* p = HeightMap.Find(key);
		if(p) {
			return *p;
		}
		else {
			DebugTools::ScreenPrint("sb2");
			return 0;
		}
	}
	catch (...) {
		UE_LOG(LogArea,Error,TEXT("gx:%d,gy:%d"),gx,gy);
	}
	
}

void BuildingGenerator::setHeight(TMap<uint64, int32>& HeightMap, const Chunk& TheChunk, int32 gx, int32 gy,
                                  int32 value) {
	
	uint64 key = HashTools::Vec3HashToUint64(gx,gy);
	
	int32* p = HeightMap.Find(key);
	if(p) {
		*p = value;
	}
	else {
		DebugTools::ScreenPrint("sb2");
	}
	
}
