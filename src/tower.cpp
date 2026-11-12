//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#include "tower.h"
#include "logger.h"
#include <cstdlib>
#include <limits>

Tower::Tower(int x, int y, int rateOfFire)
    : position({x, y}),
      rateOfFire(rateOfFire),
      lastFireTime(std::chrono::steady_clock::now()),
      bAutoFire(true)
    {}

void Tower::Update(std::vector<Enemy*> enemies,
                   std::vector<Bullet>& bullets) {
    if (bAutoFire) {
        FireAtEnemy(enemies, bullets);
    }
}

void Tower::FireAtEnemy(const std::vector<Enemy*>& enemies,std::vector<Bullet>& bullets) 
{
    auto currentTime = std::chrono::steady_clock::now();
    auto timeElapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFireTime);

    if (timeElapsed >= std::chrono::milliseconds(rateOfFire)) {
        Enemy* closestEnemy = FindClosestEnemy(enemies, true);

  
        if (closestEnemy == nullptr && !bAutoFire) 
        {
            closestEnemy = FindClosestEnemy(enemies, false);
        }

        if (closestEnemy == nullptr) 
        {
            return;  // no enemies (or, in auto-fire, none lined up yet)
        }

        const Vector2D enemyPos = closestEnemy->GetPosition();
        if (enemyPos.x == position.x && enemyPos.y == position.y) 
        {
            return;  // on the tower's cell: direction would be {0,0}
        }
        if (bAutoFire) {
            int distance = CalculateDistance(closestEnemy->GetPosition(), position);

            if (distance <= ENEMYRANGE) {
                FireBulletAtEnemy(*closestEnemy, bullets);
                lastFireTime = currentTime;
            }
        } else {
            FireBulletAtEnemy(*closestEnemy, bullets);
            lastFireTime = currentTime;
        }
    }
}

bool Tower::GetAutoFire() const 
{ 
    return bAutoFire; 
}

void Tower::SetAutoFire(bool autoFire) 
{ 
    bAutoFire = autoFire; 
}

void Tower::ManualFire(std::vector<Enemy*> enemies,std::vector<Bullet>& bullets) 
{
    FireAtEnemy(enemies, bullets);
}

void Tower::FireBulletAtEnemy(Enemy& enemy, std::vector<Bullet>& bullets) 
{
    Vector2D direction = {0, 0};
    if (enemy.GetPosition().x > position.x) direction.x = 1;
    if (enemy.GetPosition().x < position.x) direction.x = -1;
    if (enemy.GetPosition().y > position.y) direction.y = 1;
    if (enemy.GetPosition().y < position.y) direction.y = -1;

    bullets.emplace_back(position, direction);
}

int Tower::CalculateDistance(const Vector2D& a, const Vector2D& b)
{
    const int dx = std::abs(a.x - b.x);
    const int dy = std::abs(a.y - b.y);
    return dx > dy ? dx : dy;
}

Enemy* Tower::FindClosestEnemy(const std::vector<Enemy*>& enemies, bool onlyOnFiringLine) const
{
    Enemy* closest = nullptr;
    int closestDistance = std::numeric_limits<int>::max();
    bool closestOnLine = false;

    for (Enemy* enemy : enemies) 
    {
        if (enemy == nullptr || !enemy->IsActive()) 
        {
            continue;
        }

        const Vector2D enemyPos = enemy->GetPosition();
        const bool onLine = IsOnFiringLine(enemyPos);

        if (onlyOnFiringLine && !onLine) 
        { 
            continue;
        }

        const int distance = CalculateDistance(position, enemyPos);

        const bool isCloser = distance < closestDistance;
        const bool winsTie = distance == closestDistance && onLine && !closestOnLine;

        if (isCloser || winsTie) 
        {
            closest = enemy;
            closestDistance = distance;
            closestOnLine = onLine;
        }
    }

    return closest;
}

bool Tower::IsOnFiringLine(const Vector2D& target) const
{
    const int dx = std::abs(target.x - position.x);
    const int dy = std::abs(target.y - position.y);
    return dx == 0 || dy == 0 || dx == dy;
}
