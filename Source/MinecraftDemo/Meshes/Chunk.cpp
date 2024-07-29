
#include "Chunk.h"

Chunk::Chunk():ChunkHeightField{0},ChunkTemperatureField{0},ChunkHumidityField{0},ChunkBiomeField{0},ChunkPosition({0}) {}
Chunk::Chunk(FVector2D Position):ChunkHeightField{0},ChunkTemperatureField{0},ChunkHumidityField{0},ChunkBiomeField{0},ChunkPosition(Position) {}
