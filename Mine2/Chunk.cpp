#include "Chunk.h"

Chunk::Chunk(const Position3D& chunkCoords)
    : chunkCoords(chunkCoords)
{
    size_t total = static_cast<size_t>(chunkSize)
        * chunkSize
        * chunkHeight;
    blocks.resize(total);
    occupied.assign(total, false);
}

bool Chunk::addBlockToChunk(const Block& block) {
    auto pos = block.transform.getPosition();
    int gx = int(pos.x), gy = int(pos.y), gz = int(pos.z);

    // Глобальные границы
    if (!isWithinChunk(gx, gz) || gy < 0 || gy >= chunkHeight)
        return false;

    int lx = gx - chunkCoords.x * chunkSize;
    int ly = gy;
    int lz = gz - chunkCoords.z * chunkSize;

    if (!isValidLocal(lx, ly, lz))
        return false;

    int idx = getIndex(lx, ly, lz);
    if (idx < 0 || idx >= static_cast<int>(blocks.size())) {
        return false;
    }

    blocks[idx] = block;
    occupied[idx] = true;
    return true;
}

bool Chunk::removeBlockFromChunk(int gx, int gy, int gz) {
    if (!isWithinChunk(gx, gz) || gy < 0 || gy >= chunkHeight)
        return false;

    int lx = gx - chunkCoords.x * chunkSize;
    int ly = gy;
    int lz = gz - chunkCoords.z * chunkSize;

    if (!isValidLocal(lx, ly, lz))
        return false;

    int idx = getIndex(lx, ly, lz);
    if (idx < 0 || idx >= static_cast<int>(blocks.size())) {
        return false;
    }

    if (!occupied[idx])
        return false;
    occupied[idx] = false;
    return true;
}

Block* Chunk::findBlockInChunk(int gx, int gy, int gz) {
    if (!isWithinChunk(gx, gz) || gy < 0 || gy >= chunkHeight)
        return nullptr;

    int lx = gx - chunkCoords.x * chunkSize;
    int ly = gy;
    int lz = gz - chunkCoords.z * chunkSize;

    if (!isValidLocal(lx, ly, lz))
        return nullptr;

    int idx = getIndex(lx, ly, lz);
    if (idx < 0 || idx >= static_cast<int>(blocks.size())) {
        return nullptr;
    }

    return occupied[idx] ? &blocks[idx] : nullptr;
}

std::vector<Block*> Chunk::getNearestBlocks(int gx, int gy, int gz) {
    static constexpr int offs[6][3] = {
        { 1,0,0 }, { -1,0,0 },
        { 0,1,0 }, {  0,-1,0 },
        { 0,0,1 }, {  0,0,-1 }
    };
    std::vector<Block*> res;
    res.reserve(6);
    for (auto& o : offs) {
        if (auto* b = findBlockInChunk(gx + o[0], gy + o[1], gz + o[2]))
            res.push_back(b);
    }
    return res;
}

std::vector<Block*> Chunk::getChunkBlocks() {
    std::vector<Block*> res;
    res.reserve(blocks.size());
    for (int i = 0, n = static_cast<int>(blocks.size()); i < n; ++i) {
        if (occupied[i])
            res.push_back(&blocks[i]);
    }
    return res;
}

inline int Chunk::getIndex(int lx, int ly, int lz) const {
    return lx
        + lz * chunkSize
        + ly * (chunkSize * chunkSize);
}

inline bool Chunk::isWithinChunk(int gx, int gz) const {
    int minX = chunkCoords.x * chunkSize;
    int maxX = minX + chunkSize;
    int minZ = chunkCoords.z * chunkSize;
    int maxZ = minZ + chunkSize;
    return gx >= minX && gx < maxX
        && gz >= minZ && gz < maxZ;
}

inline bool Chunk::isValidLocal(int lx, int ly, int lz) const {
    return lx >= 0 && lx < chunkSize
        && ly >= 0 && ly < chunkHeight
        && lz >= 0 && lz < chunkSize;
}