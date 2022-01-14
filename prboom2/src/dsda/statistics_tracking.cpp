#include "dsda/name.hpp"
#include "statistics_tracking.pb.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include "d_player.h"
#include "dsda/data_organizer.h"
#include "statistics_tracking.h"
}

static dsda_Statistics dsda_statistics = {0};

dboolean dsda_TryLoadStatistics(void) {
  std::ostringstream path_stream;
  path_stream << dsda_DataDir() << "/statistics.dat";
  std::ifstream statistics_dat{path_stream.str(),
                               std::ios::in | std::ios::binary};
  statistics::protocol::Statistics message;
  if (!message.ParseFromIstream(&statistics_dat)) {
    return false;
  }

  std::memset(&dsda_statistics, 0, sizeof(dsda_statistics));
  const statistics::protocol::KillStatistics &kill_statistics = message.kill();
  for (auto kill_per_enemy : kill_statistics.per_enemy()) {
    for (auto kill_per_weapon : kill_per_enemy.per_weapon()) {
      dsda_statistics.kill_statistics.per_enemy[kill_per_enemy.enemy_id()]
          .per_weapon[kill_per_weapon.weapon_id()] = kill_per_weapon.count();
      dsda_statistics.kill_statistics.per_enemy[kill_per_enemy.enemy_id()]
          .total += kill_per_weapon.count();
      dsda_statistics.kill_statistics.total += kill_per_weapon.count();
    }
    dsda_statistics.kill_statistics.per_enemy[kill_per_enemy.enemy_id()]
        .infight += kill_per_enemy.infight();
    dsda_statistics.kill_statistics.per_enemy[kill_per_enemy.enemy_id()]
        .suicide += kill_per_enemy.suicide();
  }

  return true;
}

void dsda_TrySaveStatistics(void) {
  statistics::protocol::Statistics message;
  statistics::protocol::KillStatistics *kill_stats = message.mutable_kill();
  std::size_t i = 0;
  for (auto per_enemy : dsda_statistics.kill_statistics.per_enemy) {
    if (per_enemy.total == 0) {
      ++i;
      continue;
    }
    statistics::protocol::EnemyKillStatistics *enemy_stats =
        kill_stats->add_per_enemy();
    enemy_stats->set_enemy_id(i);
    std::size_t j = 0;
    for (auto per_weapon : per_enemy.per_weapon) {
      statistics::protocol::KillStatistic *kill = enemy_stats->add_per_weapon();
      kill->set_weapon_id(j);
      kill->set_count(per_weapon);
      ++j;
    }
    enemy_stats->set_infight(per_enemy.infight);
    enemy_stats->set_suicide(per_enemy.suicide);
    ++i;
  }

  std::ostringstream output_path;
  output_path << dsda_DataDir() << "/statistics.dat";
  std::cout << "dsda_TrySaveStatistics: " << output_path.str() << " ("
            << dsda_statistics.kill_statistics.total << " total kills)\n";
  std::ofstream output{output_path.str(), std::ios::out | std::ios::binary};
  message.SerializeToOstream(&output);

  output_path.str("");
  output_path << dsda_DataDir() << "/statistics.txt";
  std::ofstream txt_output{output_path.str()};
  txt_output << "dsda-doom kill statistics\n";
  txt_output << "-------------------------\n";
  txt_output << "\n";
  txt_output << "per enemy:\n";
  i = 0;
  for (auto enemy : dsda_statistics.kill_statistics.per_enemy) {
    if (enemy.total > 0) {
      txt_output << "  " << i;
      std::string enemy_name;
      if (dsda_TryGetEnemyName(i, &enemy_name)) {
        txt_output << " (" << enemy_name << ")";
      }
      txt_output << ": " << enemy.total << " total kills\n";
    }
    ++i;
  }
  txt_output << "\nper weapon:\n";
  size_t weapon_counts[NUMWEAPONS] = {0};
  for (auto enemy : dsda_statistics.kill_statistics.per_enemy) {
    i = 0;
    for (auto weapon : enemy.per_weapon) {
      weapon_counts[i] += weapon;
      ++i;
    }
  }
  i = 0;
  for (size_t weapon : weapon_counts) {
    if (weapon > 0) {
      txt_output << "  " << i;
      std::string weapon_name;
      if (dsda_TryGetWeaponName(i, &weapon_name)) {
        txt_output << " (" << weapon_name << ")";
      }
      txt_output << ": " << weapon << " total kills\n";
    }
    ++i;
  }
}

void dsda_TrackKill(mobj_t *source, mobjtype_t target) {
  std::cout << "dsda_TrackKill: " << target;
  std::string enemy_name;
  if (dsda_TryGetEnemyName(target, &enemy_name)) {
    std::cout << " (" << enemy_name << ")";
  }
  if (source) {
    std::string source_type;
    if (source->player) {
      dsda_statistics.kill_statistics.per_enemy[target]
          .per_weapon[source->player->readyweapon]++;
      std::string weapon_name;
      if (dsda_TryGetWeaponName(source->player->readyweapon, &weapon_name)) {
        std::cout << " by " << weapon_name;
      }
    } else if (dsda_TryGetEnemyName(source->type, &source_type)) {
      dsda_statistics.kill_statistics.per_enemy[target].infight++;
      std::cout << " by infighting from " << source_type;
    }
  } else {
    dsda_statistics.kill_statistics.per_enemy[target].suicide++;
    std::cout << " (suicide)";
  }
  dsda_statistics.kill_statistics.per_enemy[target].total++;
  dsda_statistics.kill_statistics.total++;
  std::cout << "\n";
}
