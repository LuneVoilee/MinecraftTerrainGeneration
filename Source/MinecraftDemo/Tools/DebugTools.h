#pragma once

class DebugTools {
public:
	static void ScreenPrint(const FString& String);
	static void ScreenPrint(const FColor& Color , const FString& String);
	DebugTools()=delete;
	~DebugTools()=delete;
};
