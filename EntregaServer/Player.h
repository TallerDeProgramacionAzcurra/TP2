#ifndef PLAYER_H_
#define PLAYER_H_


#include "MoveableObject.h"
#include "Vector2D.h"
#include "Score.h"
#include "Statistics.h"
#include "Singletons/InputHandler.h"
#include "Singletons/TextureManager.h"
#include "Singletons/GameTimeHelper.h"
#include "Utils/TiposDefinidos.h"
#include "Weapons/PlayerWeapons/BasicWeapon.h"
#include "Weapons/PlayerWeapons/DualWeapon.h"
class Weapon;

#define EXPLOTION_ANIMATION_TIME 1000
#define FLIP_ANIMATION_TIME 1500


class Player : public MoveableObject
{
public:

	//constructor sin argumento: setea m_controllable siempre en true
    Player();
    //constructor que permite determinar si el player reacciona o no a eventos de input. Si se puede o no controlar
    Player(bool canControl);
    virtual ~Player() {}

    virtual void load(int x, int y, int width, int height, int textureID, int numFrames);

    virtual void draw();
    virtual void update();
    virtual void handleInput(InputMessage dataMsg);
    virtual void clean();

    void sendDrawMessage(bool isAlive);

    virtual void collision();

    void setConnected(bool connected) { m_connected = connected; }

    void setWeaponStats(int shootingSpeed, int shootingCooldown, int ownerID, int teamID);
    void setShootingSpeed(int speed);
    void setShootingCooldown(int cooldown);
    void setCollisionDamage(int collisionDamage) { m_collisionDamage = collisionDamage; }
    void setWeapon(Weapon* weapon);

    void setTeamNumber(int number) { m_teamNumber = number; }
    void refreshDirty() { m_movedByPlayer = false; m_dirty = true;}

    void StopFlipAnimation();
    void startFlipAnimation();

    //Getters
    bool isDead() { return m_dead; }
    bool isDying() { return m_dying; }
    bool isDoingFlip() { return m_doingFlip; }
    bool isConnected() { return m_connected; }
    int getTeamNumber() { return m_teamNumber; }
    int getCollisionDamage() { return m_collisionDamage; }
    Statistics getStageStatistics() { return m_stageStats; }
    void resetStageStatistics() { return m_stageStats.reset(); }

    void moveAutomatic(const Vector2D& destination, int speed);

    void damage(int damageReceived);


    void addPoints(int points);
    const int getScore();
    void reset();

	void incrementEnemiesKilledStats (int amount) { m_stageStats.incrementEnemiesKilled(amount); }
	void incrementShootsStats (int amount) { m_stageStats.incrementShoots(amount); }
	void incrementHitsStats(int amount) { m_stageStats.incrementHits(amount); }
	void incrementPointsStats (int amount) { m_stageStats.incrementPoints(amount); }


private:

    Weapon* m_currentWeapon;
    Vector2D m_shootOffset;
    // Determina si el usuario puede controlar este player o no. Sirve por si hay varias instancias de Player en juego.
    // Si m_controllable es false, el Player no reaccionará a eventos de Input
    bool m_connected;
    bool m_doingFlip;

    //Tiempo de animacion de giro
    int m_flipAnimationTime;
    int m_flipRemainingTime;

    //tiempo de animación explosion
    int m_explotionAnimationTime;
    int m_explotionRemainingTime;

    //UN timer para esperar un par de milisegundos desde que el jguador dejo de moverse para arrastrarlo
    const int m_holdQuietTimer;
    int m_currentHoldQuietTime;

    void updateFlipAnimation();
    void explote();
    void updateExplotionAnimation();

    void moveTo(const Vector2D& destination);

    // variables de control de estado
    bool m_dead;
    bool m_dying;
    bool m_exploting;
    bool m_automaticMoving;

    bool m_invulnerable;

    /*******************/
    int m_health;
    int m_collisionDamage;
    bool m_movedByPlayer;
    int m_teamNumber;

    Score m_score;
    Statistics m_stageStats;

    Vector2D m_destination;
    int m_autoMoveSpeed;

    //Modifica el estado del juego de acuerdo al input del jugador
};


#endif /* PLAYER_H_ */
