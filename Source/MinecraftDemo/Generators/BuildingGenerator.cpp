#include "BuildingGenerator.h"

#include <functional>

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
	auto greater = [](std::pair<int32,Point>& a,std::pair<int32,Point>& b) -> bool {
		return a.first > b.first;
	};
	std::priority_queue<std::pair<int32,Point>,std::vector<std::pair<int32,Point>>,decltype(greater)> PointQueue;
	PointQueue.emplace(0,CapitalPoint);
	
	const int32 dx1[9] = {0,1,0,1,-1, 0,-1,-1, 1};
	const int32 dy1[9] = {0,0,1,1, 0,-1,-1, 1,-1};
	const int32 dx2[4] = {3,0,-3, 0};
	const int32 dy2[4] = {0,3, 0,-3};
	int count = 0;
	while(!PointQueue.empty()) {
		int32 cost=PointQueue.top().first;
		Point point=PointQueue.top().second;
		PointQueue.pop();
		count++;
		
		//以3X3的正方形为一个单位，当前的最优单位解加入域
		for(int i = 0 ; i < 9 ; i++) {
			//UE_LOG(LogArea,Warning,TEXT("AreaPos[%d,%d]"),(int32)globalOffset.X+point[0]+dx1[i],(int32)globalOffset.Y+point[1]+dy1[i]);
			//UE_LOG(LogArea,Log,TEXT("1"));
			Point tempP(globalOffset.X+point[0]+dx1[i],globalOffset.Y+point[1]+dy1[i]);
			//UE_LOG(LogArea,Log,TEXT("2"));
			Point* p = Area.Find(tempP);
			//UE_LOG(LogArea,Log,TEXT("3"));
			if(!p) {
				Area.Emplace(tempP);
			}
		}

		//最小代价大于5时，放弃拓展
		if(cost > 3) {
			//UE_LOG(LogArea,Warning,TEXT("asdf"));
			break;
		}
		//最多循环30次
		if(count>30) {
			//UE_LOG(LogArea,Warning,TEXT("zxcv"));
			break;
		}
		
		//将上下左右四个方向的单位的中心点加入队列
		for(int i = 0 ; i < 4 ; i++) {
			int32 x = point[0] + dx2[i];
			int32 y = point[1] + dy2[i];
			int32 gx = globalOffset.X + point[0] + dx2[i];
			int32 gy = globalOffset.Y + point[1] + dy2[i];
			
			if(Area.Find({gx,gy})) {
				//UE_LOG(LogArea,Log,TEXT("这个中心点已经在Area有了"));
				continue;
			}
			else {
				int32 CentralHeight = getHeight(HeightMap,gx,gy);
				int32 deltaHeight = abs(CentralHeight-CapitalHeight);
				int32 ThisCost = cost + deltaHeight;
				PointQueue.emplace(ThisCost,Point{x,y});
				//DebugTools::ScreenPrint("laile");
				//UE_LOG(LogArea,Log,TEXT("进队列了[%d,%d,%d]"),ThisCost,x,y);
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
		if(!TryToPlaceOneHouse(HeightMap,buildingSize[type][0],buildingSize[type][1],type,rotate,point,CubeTypePool,BuildingReadyToDisplay)) {
			continue;
		}

		int32 Offset = 5 + (int32)HashTools::NonDeterministicRandom(0,100) % 3;
		
		for(int i = 0 ; i < 4 ; i++) {
			
			PointQueue.push(Point{point[0]+dx[i]*(Offset+buildingSize[type][0]),
				point[1]+dy[i]*(Offset+buildingSize[type][1])});
		}
		
	}
}

bool BuildingGenerator::TryToPlaceOneHouse(TMap<uint64, int32>& HeightMap,int32 size0, int32 size1, int32 type, int32 rotate, Point point, TMap<
                                           uint64, int32>& CubeTypePool, TArray<TTuple<uint64, int32, int32>>& BuildingReadyToDisplay) {
	int32 rotateIndex = rotate % 2;
	int32 horizontal, vertical;
	rotateIndex == 0 ? horizontal = size0 / 2 , vertical = size1 / 2 : horizontal = size1 / 2 , vertical = size0 / 2 ;
	float faverageHeight = 0.;
	TMap<uint64,int32> tempMap = HeightMap;
	std::vector<int32> temp;
	for(int i = -horizontal ; i < horizontal ; i++) {
		for(int j = -vertical ; j < vertical ; j++) {
			int32 gx = globalOffset.X+point[0]+i;
			int32 gy = globalOffset.Y+point[1]+j;
			//不在发展域
			if(!Area.Find(Point(gx,gy))) {
				//UE_LOG(LogArea,Warning,TEXT("can't find block in Area,PosNow is[%d,%d]"),gx,gy)
				//DebugTools::ScreenPrint("Area");
				return false;
			}
			
			int32 height = getHeight(HeightMap,gx,gy);
			if(height == INT_MIN) {
				return false;
			}
			temp.push_back(height);
			//空气
			int* result = CubeTypePool.Find(HashTools::Vec3HashToUint64(gx,gy,height));
			if(result&&(*result)==0) {
				//DebugTools::ScreenPrint("Air");
				return false;
			}
			
			faverageHeight += static_cast<float>(height);
		}
	}
	faverageHeight /= static_cast<float>(size0*size1);
	int averageHeight = static_cast<int>(faverageHeight);
	
	for(int i = -horizontal ; i < horizontal ; i++) {
		for(int j = -vertical ; j < vertical ; j++) {
			int gx = globalOffset.X+point[0]+i,gy = globalOffset.Y+point[1]+j;
			Area.Remove(Point(gx,gy));
			//bool isSetSuccess = setHeight(HeightMap,gx,gy,averageHeight);
			//if(!isSetSuccess) {
				//HeightMap = tempMap;
				//return false;
			//}
		}
	}
	//
	// //插入空气，以免生成树木花草
	// for(int i=-horizontal-3;i<horizontal+3;++i)
	// 	for(int j=-vertical-3;j<vertical+3;++j)
	// 	{
	// 		int gx = globalOffset.X+point[0]+i,gy = globalOffset.Y+point[1]+j;
	// 		int32 height = getHeight(HeightMap,gx,gy)+1;
	// 		if(height == INT_MIN+1) {
	// 			return false;
	// 		}
	// 		uint64 index = HashTools::Vec3HashToUint64(gx,gy,height);
	// 		if(CubeTypePool.Find(index)) {
	// 			CubeTypePool[index] = 0;
	// 		}
	// 		else {
	// 			CubeTypePool.Emplace(index,0);
	// 		}
	// 	}

	BuildingReadyToDisplay.Emplace(HashTools::Vec3HashToUint64(globalOffset.X+point[0],globalOffset.Y+point[1],averageHeight+1),type+1,rotate);
	//UE_LOG(LogArea,Log,TEXT("aaaassss fromheight:%d"),averageHeight+1);
	Area.Emplace(Point(globalOffset.X+point[0]-horizontal,globalOffset.Y+point[1]-vertical));
	BuildingPoint.Push(FVector2D(point[0]-horizontal,point[1]-vertical));
	return true;
	
}

void BuildingGenerator::DisplayPath(Chunk& TheChunk, TMap<uint64, int32>& CubeTypePool,
                                    TMap<uint64, int32>& CubeReadyToDisplay) {
}

int32 BuildingGenerator::getHeight(TMap<uint64, int32>& HeightMap,int32 gx, int32 gy) {

		uint64 key = HashTools::Vec3HashToUint64(gx,gy);
		int32* p = HeightMap.Find(key);
		if(p) {
			return *p;
		}
		else {
			//UE_LOG(LogArea,Log,TEXT("cant get height"));
			//Tools::ScreenPrint("sb1");
			return INT_MIN;
		}
}

bool BuildingGenerator::setHeight(TMap<uint64, int32>& HeightMap, int32 gx, int32 gy,
                                  int32 value) {
	
	uint64 key = HashTools::Vec3HashToUint64(gx,gy);
	
	int32* p = HeightMap.Find(key);
	if(p) {
		*p = value;
		return true;
	}
	else {
		return false;
	}
	
}
//
// //记录深度
// int PathFinder::depth = 0;        
//
// //深度限制
// int PathFinder::depthLimit = 400;     
//
// //存储OpenNode的内存空间
// std::vector<OpenNode> PathFinder::pointList = std::vector<OpenNode>(depthLimit);
//
// //函数接口：检查是否有障碍物（不可走）
// std::function<bool(FVector2D pos)> PathFinder::inBarrier = nullptr;
// 	
// //函数接口：计算权值预测公式
// std::function<TPair<float,float>(FVector2D pos,FVector2D endPos,float cost)> PathFinder::weightFormula = nullptr;
//
// std::priority_queue<OpenNode*, std::vector<OpenNode*>, OpenPointPtrCompare> PathFinder::openlist = {};
//
// TArray<FVector2D> PathFinder::findPath(FVector2D startPos,FVector2D endPos) {
//     //清理数据结构
//     pointList.clear();
//     while(!openlist.empty())
//         openlist.pop();
//     depth = 0;
//
//     TArray<FVector2D> road;
//     //创建并开启一个父节点
//     openlist.push(createOpenNode(startPos, endPos, 0, nullptr));
//
//     OpenNode* toOpen = nullptr;
//     bool found = false;
//     //重复寻找预测和花费之和最小节点开启检查
//     while (!openlist.empty()){
//         toOpen = openlist.top();        
//         //将父节点从openlist移除
//         openlist.pop();
//         // 找到终点后，则停止搜索
//         if (toOpen->pos.X == endPos.X && toOpen->pos.Y ==endPos.Y) {
//             found = true;
//             break;
//         }
//         //若超出一定深度，则搜索失败
//         if (depth >= depthLimit) {
//             break;
//         }
//         open(*toOpen,endPos);
//     }
//
//     if(!found){
// 	    UE_LOG(LogTemp, Warning, TEXT("path unfound %d"),depth);
//     }
//     else{
// 	    UE_LOG(LogTemp, Warning, TEXT("path found %d"),depth);
//         for (auto rs = toOpen; rs != nullptr; rs = rs->father) {
//             road.Push(rs->pos);
//         }
//     }
//
//     return road;
// }
//
//
// void PathFinder::setConditionInBarrier(std::function<bool(FVector2D pos)> func){
//     inBarrier = func;
// }
//
// void PathFinder::setWeightFormula(std::function<TPair<float,float>(FVector2D pos,FVector2D endPo,float cost)> func){
//     weightFormula = func;
// }
//
// OpenNode* PathFinder::createOpenNode(FVector2D pos,FVector2D endPos, float cost, OpenNode* fatherNode){
//     TPair<float,float> pair = weightFormula(pos,endPos,cost);
//     pointList.push_back(OpenNode(pos,pair.Get<0>(),pair.Get<1>(),fatherNode));
//     return &pointList.back();
// }
//
// void PathFinder::open(OpenNode& OpenNode,FVector2D endPos) {
// 	//八方的位置
// 	const int direction[8][2] = {{1,0},{0,1},{-1,0},{0,-1}};
//     //深度+1
//     depth++;
//
//     //检查p点八方的点
//     for (int i = 0; i < 4; ++i)
//     {   
//         FVector2D newPos = OpenNode.pos + FVector2D(direction[i][0],direction[i][1]);
//         if (!inBarrier(newPos)) {
//             openlist.push(createOpenNode(newPos,endPos, OpenNode.cost+1.0f, &OpenNode));
//         }
//     }
//}