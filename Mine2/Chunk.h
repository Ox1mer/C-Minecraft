#pragma once

#include <vector>
#include <cstddef>
#include "Position3D.h"
#include "Block.h"
#include <windows.h>

class Chunk {
public:
    static constexpr int chunkSize = 16;
    static constexpr int chunkHeight = 100;

    explicit Chunk(const Position3D& chunkCoords);

    bool addBlockToChunk(const Block& block);
    bool removeBlockFromChunk(int gx, int gy, int gz);
    Block* findBlockInChunk(int gx, int gy, int gz);
    std::vector<Block*> getNearestBlocks(int gx, int gy, int gz);
    std::vector<Block*> getChunkBlocks();

private:
    Position3D chunkCoords;
    std::vector<Block> blocks;
    std::vector<bool> occupied;

    inline int  getIndex(int lx, int ly, int lz) const;
    inline bool isWithinChunk(int gx, int gz)           const;
    inline bool isValidLocal(int lx, int ly, int lz)    const;
};
