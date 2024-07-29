#include "DebugTools.h"
void DebugTools::ScreenPrint(const FString& String) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, String);
}
void DebugTools::ScreenPrint(const FColor& Color,const FString& String) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, String);
}
