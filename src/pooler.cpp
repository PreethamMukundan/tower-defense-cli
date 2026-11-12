//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#include "pooler.h"

#include <algorithm>

Pooler::Pooler(size_t minimumPoolSize) : minimumPoolSize(minimumPoolSize) 
{
    freeEnemies.reserve(minimumPoolSize);
    Grow(minimumPoolSize);
}

size_t Pooler::PoolSize()
{
    return enemies.size();
}

Enemy* Pooler::SpawnEnemy(const Vector2D& position)
{
    if (freeEnemies.empty()) {
        Grow(1); 
    }

    Enemy* enemy = freeEnemies.back();
    freeEnemies.pop_back();

    
    *enemy = Enemy();
    enemy->Activate(position);
    return enemy;
}

void Pooler::DespawnEnemy(Enemy* enemy)
{
    if (enemy == nullptr || !enemy->IsActive() || !Owns(enemy)) {
        return;
    }
    enemy->Deactivate();
    freeEnemies.push_back(enemy);
}

std::vector<Enemy*> Pooler::GetActiveEnemies() const
{
    std::vector<Enemy*> active;
    active.reserve(enemies.size());
    for (const auto& enemy : enemies) {
        if (enemy->IsActive()) {
            active.push_back(enemy.get());
        }
    }
    return active;
}

void Pooler::Clear()
{
    while (enemies.size() > minimumPoolSize) {
        enemies.pop_back();
    }

    freeEnemies.clear();
    for (auto& enemy : enemies) {
        enemy->Deactivate();
        freeEnemies.push_back(enemy.get());
    }
}

std::unique_ptr<Enemy> Pooler::CreateEnemy() 
{
    return std::make_unique<Enemy>();
}

void Pooler::Grow(size_t count)
{
    for (size_t i = 0; i < count; ++i) 
    {
        enemies.push_back(CreateEnemy());
        freeEnemies.push_back(enemies.back().get());
    }
}

bool Pooler::Owns(const Enemy * enemy) const
{
    return std::any_of(enemies.begin(), enemies.end(),[enemy](const std::unique_ptr<Enemy>& e) { return e.get() == enemy; });
}
