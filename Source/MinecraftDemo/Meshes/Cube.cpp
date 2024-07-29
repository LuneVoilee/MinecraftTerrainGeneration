// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"

// Sets default values
ACube::ACube():StaticMesh(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube")))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	this->SetRootComponent(StaticMesh);
	
}

void ACube::InitCubeByType(int32 CubeType) {
	
    if(CubeType<0||CubeType>TotalType){
		return;
	}
	if(!CubeMeshes[CubeType-1]) {
		CubeMeshes[CubeType-1]=LoadObject<UStaticMesh>(this,*("StaticMesh'/Game/Meshes/Block" +
				FString::FromInt(CubeType) + ".Block" +
				FString::FromInt(CubeType)+ "'"));
	}
	StaticMesh->SetStaticMesh(CubeMeshes[CubeType-1]);
	
}



