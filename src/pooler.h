//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#ifndef POOLER_H
#define POOLER_H

#include <memory>
#include <list>
#include <vector>

#include "enemy.h"

// Implement the following methods:
// Clear()
// SpawnEnemy()
// DespawnEnemy()
// GetActiveEnemies()

class Pooler {
   public:
    Pooler(size_t minimumPoolSize);
    size_t PoolSize();


    ~Pooler() = default;//desctructor

    //no copy constructor
    Pooler(const Pooler&) = delete;
    Pooler& operator=(const Pooler&) = delete;

    Enemy* SpawnEnemy(const Vector2D& position);

 
    void DespawnEnemy(Enemy* enemy);

    std::vector<Enemy*> GetActiveEnemies() const;

    void Clear();

    
   private:
    std::list<std::unique_ptr<Enemy>> enemies;
    size_t minimumPoolSize;

    std::unique_ptr<Enemy> CreateEnemy();
    std::vector<Enemy*> freeEnemies;

    void Grow(size_t count);
    bool Owns(const Enemy* enemy) const;
};

#endif  // POOLER_H