#ifndef __DSDA_STATISTICS_TRACKING__
#define __DSDA_STATISTICS_TRACKING__

#include "doomdef.h"
#include "doomtype.h"
#include "info.h"

typedef struct {
  uint_64_t total;
  uint_64_t per_weapon[NUMWEAPONS];
} dsda_EnemyKillStatistics;

typedef struct {
  uint_64_t total;
  dsda_EnemyKillStatistics per_enemy[DOOM_NUMMOBJTYPES];
} dsda_KillStatistics;

typedef struct {
  dsda_KillStatistics kill_statistics;
} dsda_Statistics;

dboolean dsda_TryLoadStatistics(void);
void dsda_TrySaveStatistics(void);
void dsda_TrackKill(weapontype_t weapon, mobjtype_t target);

#endif
