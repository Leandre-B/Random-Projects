#include "parse_level.h"

Level foo(){
    Level lvl;

    std::ifstream f("../levels/test.json");
    json data = json::parse(f);
    lvl.width = data["width"];
    lvl.height = data["height"];

    lvl.game.resize(lvl.width, std::vector<char>(lvl.height));
    for (auto &ligne : lvl.game) {
        for (char &val : ligne) {
            val='n';
        }
    }

    for (auto& t : data["blocks"]) {
        lvl.game [t[0]][t[1]] = 'b';
    }
    for (auto& t : data["spikes"]) {
        lvl.game[t[0]][t[1]] = 's';
    }

    lvl.spawn_coord.first = data["spawn"][0];
    lvl.spawn_coord.second = data["spawn"][1];

    return lvl;

}
