/*
    Вариант 13: Дракон, Жаба, Странствующий рыцарь
    Правила:
      - Жаба ест всех (включая жаб)
      - Рыцарь убивает драконов
      - Дракон убивает рыцарей
*/

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <set>
#include <cstdlib>

#include "objects/npc/npc.h"
#include "objects/dragon/dragon.h"
#include "objects/frog/frog.h"
#include "objects/knight/knight.h"

class TextObserver : public IFightObserver {
private:
    TextObserver() {}
public:
    static std::shared_ptr<IFightObserver> get() {
        static TextObserver instance;
        return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver*) {});
    }

    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override {
        if (win) {
            std::cout << std::endl << "Murder --------" << std::endl;
            attacker->print(std::cout);
            defender->print(std::cout);
        }
    }
};

class FileObserver : public IFightObserver {
private:
    std::ofstream fs;
    FileObserver() { fs.open("log.txt"); }
public:
    ~FileObserver() { fs.close(); }
    static std::shared_ptr<IFightObserver> get() {
        static FileObserver instance;
        return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver*) {});
    }

    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override {
        if (win) {
            fs << std::endl << "Murder --------" << std::endl;
            attacker->print(fs);
            defender->print(fs);
        }
    }
};

std::shared_ptr<NPC> factory(std::istream& is) {
    std::shared_ptr<NPC> result;
    int type{0};
    if (is >> type) {
        switch (type) {
        case DragonType:
            result = std::make_shared<Dragon>(is);
            break;
        case FrogType:
            result = std::make_shared<Frog>(is);
            break;
        case KnightType:
            result = std::make_shared<Knight>(is);
            break;
        default:
            break;
        }
    } else {
        std::cerr << "Unexpected NPC type while loading." << std::endl;
    }

    if (result) {
        result->subscribe(TextObserver::get());
        result->subscribe(FileObserver::get());
    }

    return result;
}

std::shared_ptr<NPC> factory(NpcType type, const std::string& name, int x, int y) {
    std::shared_ptr<NPC> result;
    switch (type) {
    case DragonType:
        result = std::make_shared<Dragon>(name, x, y);
        break;
    case FrogType:
        result = std::make_shared<Frog>(name, x, y);
        break;
    case KnightType:
        result = std::make_shared<Knight>(name, x, y);
        break;
    default:
        break;
    }
    if (result) {
        result->subscribe(TextObserver::get());
        result->subscribe(FileObserver::get());
    }
    return result;
}

void save(const set_t& array, const std::string& filename) {
    std::ofstream fs(filename);
    fs << array.size() << std::endl;
    for (auto& n : array)
        n->save(fs);
    fs.flush();
    fs.close();
}

set_t load(const std::string& filename) {
    set_t result;
    std::ifstream is(filename);
    if (is.good() && is.is_open()) {
        int count;
        is >> count;
        for (int i = 0; i < count; ++i)
            result.insert(factory(is));
        is.close();
    } else {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const set_t& array) {
    for (auto& n : array)
        n->print(os);
    return os;
}

set_t fight(const set_t& array, size_t distance) {
    set_t dead_list;
    std::set<NPC*> dead_raw; 

    std::vector<std::shared_ptr<NPC>> list(array.begin(), array.end());
    size_t n = list.size();

    for (size_t i = 0; i < n; ++i) {
        auto A = list[i];
        if (!A) continue;

        for (size_t j = i + 1; j < n; ++j) {
            auto B = list[j];
            if (!B) continue;


            if (dead_raw.count(A.get()) || dead_raw.count(B.get()))
                continue;

            if (!A->is_close(B, distance))
                continue;

            bool A_kills_B = B->accept(A);


            bool B_kills_A = A->accept(B);

            if (A_kills_B) {
                dead_raw.insert(B.get());
                dead_list.insert(B);
                A->fight_notify(B, true);
            }

            if (B_kills_A) {
                dead_raw.insert(A.get());
                dead_list.insert(A);
                B->fight_notify(A, true);
            }
        }
    }
    return dead_list;
}

int main() {
    set_t array;
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::cout << "Generating ..." << std::endl;
    for (size_t i = 0; i < 100; ++i) {
        NpcType type = static_cast<NpcType>(std::rand() % 3 + 1);
        std::string name;
        switch (type) {
        case DragonType: name = "dragon_"; break;
        case FrogType: name = "frog_"; break;
        case KnightType: name = "knight_"; break;
        default: name = "unknown_"; break;
        }
        name += std::to_string(i);
        array.insert(factory(type, name, std::rand() % 501, std::rand() % 501));
    }

    std::cout << "Saving ..." << std::endl;
    save(array, "npc.txt");

    std::cout << "Loading ..." << std::endl;
    array = load("npc.txt");

    std::cout << "Fighting ..." << std::endl << array;

    for (size_t distance = 20; (distance <= 100) && !array.empty(); distance += 10) {
        auto dead_list = fight(array, distance);
        for (auto& d : dead_list)
            array.erase(d);
        std::cout << "Fight stats ----------" << std::endl
                  << "distance: " << distance << std::endl
                  << "killed: " << dead_list.size() << std::endl
                  << std::endl << std::endl;
    }

    std::cout << "Survivors: " << array;

    return 0;
}
