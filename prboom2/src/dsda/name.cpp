#include "dsda/name.hpp"
#include "doomdef.h"
#include "info.h"

bool dsda_TryGetEnemyName(int id, std::string *p_name) {
  switch (id) {
  case MT_PLAYER:
    *p_name = "Player";
    return true;
  case MT_POSSESSED:
    *p_name = "Zombieman";
    return true;
  case MT_SHOTGUY:
    *p_name = "Shotgun Guy";
    return true;
  case MT_VILE:
    *p_name = "Arch-Vile";
    return true;
  case MT_UNDEAD:
    *p_name = "Revenant";
    return true;
  case MT_FATSO:
    *p_name = "Mancubus";
    return true;
  case MT_CHAINGUY:
    *p_name = "Chaingun Guy";
    return true;
  case MT_TROOP:
    *p_name = "Imp";
    return true;
  case MT_SERGEANT:
    *p_name = "Pinky Demon";
    return true;
  case MT_SHADOWS:
    *p_name = "Spectre";
    return true;
  case MT_HEAD:
    *p_name = "Cacodemon";
    return true;
  case MT_BRUISER:
    *p_name = "Baron of Hell";
    return true;
  case MT_KNIGHT:
    *p_name = "Hell Knight";
    return true;
  case MT_SKULL:
    *p_name = "Lost Soul";
    return true;
  case MT_SPIDER:
    *p_name = "Spider Mastermind";
    return true;
  case MT_BABY:
    *p_name = "Arachnotron";
    return true;
  case MT_CYBORG:
    *p_name = "Cyberdemon";
    return true;
  case MT_PAIN:
    *p_name = "Pain Elemental";
    return true;
  case MT_WOLFSS:
    *p_name = "Nazi";
    return true;
  case MT_KEEN:
    *p_name = "Commander Keen";
    return true;
  case MT_BOSSBRAIN:
    *p_name = "Icon of Sin";
    return true;
  case MT_DOGS:
    *p_name = "Dog";
    return true;
  default:
    return false;
  }
}

bool dsda_TryGetWeaponName(int id, std::string *p_name) {
  switch (id) {
  case wp_fist:
    *p_name = "Fist";
    return true;
  case wp_chainsaw:
    *p_name = "Chainsaw";
    return true;
  case wp_pistol:
    *p_name = "Pistol";
    return true;
  case wp_shotgun:
    *p_name = "Shotgun";
    return true;
  case wp_supershotgun:
    *p_name = "Super Shotgun";
    return true;
  case wp_chaingun:
    *p_name = "Chaingun";
    return true;
  case wp_missile:
    *p_name = "Rocket Launcher";
    return true;
  case wp_plasma:
    *p_name = "Plasma Rifle";
    return true;
  case wp_bfg:
    *p_name = "BFG 9000";
    return true;
  default:
    return false;
  }
}
