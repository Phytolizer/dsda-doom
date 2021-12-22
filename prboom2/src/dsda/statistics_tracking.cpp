#include "statistics_tracking.pb.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
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
  }

  return true;
}

void dsda_TrySaveStatistics(void) {
  statistics::protocol::Statistics message;
  statistics::protocol::KillStatistics *kill_stats = message.mutable_kill();
  std::size_t i = 0;
  for (auto per_enemy : dsda_statistics.kill_statistics.per_enemy) {
    if (per_enemy.total == 0) {
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
    ++i;
  }

  std::ostringstream output_path;
  output_path << dsda_DataDir() << "/statistics.dat";
  std::cout << "dsda_TrySaveStatistics: " << output_path.str() << "\n";
  std::ofstream output{output_path.str(), std::ios::out | std::ios::binary};
  message.SerializeToOstream(&output);
}
