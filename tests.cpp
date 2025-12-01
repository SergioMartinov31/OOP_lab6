#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include "objects/npc/npc.h"
#include "objects/dragon/dragon.h"
#include "objects/frog/frog.h"
#include "objects/knight/knight.h"

TEST(NPCCreation, CreateDragon) {
    Dragon d("D", 10, 20);
    EXPECT_EQ(d.type, DragonType);
    EXPECT_EQ(d.name, "D");
    EXPECT_EQ(d.x, 10);
    EXPECT_EQ(d.y, 20);
}

TEST(NPCCreation, CreateFrog) {
    Frog f("F", 5, 6);
    EXPECT_EQ(f.type, FrogType);
    EXPECT_EQ(f.name, "F");
    EXPECT_EQ(f.x, 5);
    EXPECT_EQ(f.y, 6);
}

TEST(NPCCreation, CreateKnight) {
    Knight k("K", 7, 8);
    EXPECT_EQ(k.type, KnightType);
    EXPECT_EQ(k.name, "K");
    EXPECT_EQ(k.x, 7);
    EXPECT_EQ(k.y, 8);
}

TEST(Serialization, SaveAndLoadDragon) {
    auto d = std::make_shared<Dragon>("SavedD", 1, 2);
    std::stringstream ss;
    d->save(ss);
    int type;
    ss >> type;
    EXPECT_EQ(type, DragonType);
    Dragon loaded(ss);
    EXPECT_EQ(loaded.name, "SavedD");
    EXPECT_EQ(loaded.x, 1);
    EXPECT_EQ(loaded.y, 2);
}

TEST(Serialization, SaveAndLoadFrog) {
    auto f = std::make_shared<Frog>("SavedF", 3, 4);
    std::stringstream ss;
    f->save(ss);
    int type;
    ss >> type;
    EXPECT_EQ(type, FrogType);
    Frog loaded(ss);
    EXPECT_EQ(loaded.name, "SavedF");
    EXPECT_EQ(loaded.x, 3);
    EXPECT_EQ(loaded.y, 4);
}

TEST(Serialization, SaveAndLoadKnight) {
    auto k = std::make_shared<Knight>("SavedK", 5, 6);
    std::stringstream ss;
    k->save(ss);
    int type;
    ss >> type;
    EXPECT_EQ(type, KnightType);
    Knight loaded(ss);
    EXPECT_EQ(loaded.name, "SavedK");
    EXPECT_EQ(loaded.x, 5);
    EXPECT_EQ(loaded.y, 6);
}

TEST(Distance, CloseNPCs) {
    auto d = std::make_shared<Dragon>("D1", 0, 0);
    auto f = std::make_shared<Frog>("F1", 3, 4);
    EXPECT_TRUE(d->is_close(f, 10));
    EXPECT_TRUE(d->is_close(f, 5));
    EXPECT_FALSE(d->is_close(f, 4));
}

TEST(Distance, FarNPCs) {
    auto d = std::make_shared<Dragon>("D1", 0, 0);
    auto f = std::make_shared<Frog>("F1", 100, 100);
    EXPECT_FALSE(d->is_close(f, 100));
    EXPECT_TRUE(d->is_close(f, 150));
}

TEST(Distance, SamePosition) {
    auto d = std::make_shared<Dragon>("D1", 50, 50);
    auto f = std::make_shared<Frog>("F1", 50, 50);
    EXPECT_TRUE(d->is_close(f, 0));
    EXPECT_TRUE(d->is_close(f, 1));
}

TEST(FightRules, FrogEatsDragon) {
    auto frog = std::make_shared<Frog>("F1", 0, 0);
    auto dragon = std::make_shared<Dragon>("D1", 0, 0);
    bool result = dragon->accept(frog);
    EXPECT_TRUE(result);
}

TEST(FightRules, FrogEatsFrog) {
    auto f1 = std::make_shared<Frog>("F1", 0, 0);
    auto f2 = std::make_shared<Frog>("F2", 0, 0);
    bool result = f2->accept(f1);
    EXPECT_TRUE(result);
}

TEST(FightRules, FrogEatsKnight) {
    auto frog = std::make_shared<Frog>("F1", 0, 0);
    auto knight = std::make_shared<Knight>("K1", 0, 0);
    bool result = knight->accept(frog);
    EXPECT_TRUE(result);
}

TEST(FightRules, KnightKillsDragon) {
    auto knight = std::make_shared<Knight>("K1", 0, 0);
    auto dragon = std::make_shared<Dragon>("D1", 0, 0);
    bool result = dragon->accept(knight);
    EXPECT_TRUE(result);
}

TEST(FightRules, DragonKillsKnight) {
    auto dragon = std::make_shared<Dragon>("D1", 0, 0);
    auto knight = std::make_shared<Knight>("K1", 0, 0);
    bool result = knight->accept(dragon);
    EXPECT_TRUE(result);
}

TEST(FightRules, KnightDoesNotKillFrog) {
    auto knight = std::make_shared<Knight>("K1", 0, 0);
    auto frog = std::make_shared<Frog>("F1", 0, 0);
    bool result = frog->accept(knight);
    EXPECT_FALSE(result);
}

TEST(FightRules, DragonDoesNotKillDragon) {
    auto d1 = std::make_shared<Dragon>("D1", 0, 0);
    auto d2 = std::make_shared<Dragon>("D2", 0, 0);
    bool result = d2->accept(d1);
    EXPECT_FALSE(result);
}

class TestObserver : public IFightObserver {
public:
    int fight_count = 0;
    int win_count = 0;
    int loss_count = 0;
    void on_fight(const std::shared_ptr<NPC> attacker,
                  const std::shared_ptr<NPC> defender, bool win) override {
        (void)attacker;
        (void)defender;
        fight_count++;
        if (win) win_count++; else loss_count++;
    }
};

TEST(Observer, NotificationOnWin) {
    auto observer = std::make_shared<TestObserver>();
    auto frog = std::make_shared<Frog>("F1", 0, 0);
    auto dragon = std::make_shared<Dragon>("D1", 0, 0);
    frog->subscribe(observer); 
    dragon->accept(frog);
    EXPECT_EQ(observer->fight_count, 1);
    EXPECT_EQ(observer->win_count, 1);
    EXPECT_EQ(observer->loss_count, 0);
}

TEST(Observer, NoNotificationOnLoss) {
    auto observer = std::make_shared<TestObserver>();
    auto knight = std::make_shared<Knight>("K1", 0, 0);
    auto dragon = std::make_shared<Dragon>("D1", 0, 0);
    knight->subscribe(observer); 
    dragon->accept(knight);
    EXPECT_EQ(observer->fight_count, 1);
}

TEST(Print, DragonOutput) {
    Dragon d("Dr", 1, 2);
    std::stringstream ss;
    d.print(ss);
    std::string out = ss.str();
    EXPECT_TRUE(out.find("Dragon") != std::string::npos);
    EXPECT_TRUE(out.find("Dr") != std::string::npos);
}

TEST(Print, FrogOutput) {
    Frog f("Fg", 3, 4);
    std::stringstream ss;
    f.print(ss);
    std::string out = ss.str();
    EXPECT_TRUE(out.find("Frog") != std::string::npos);
    EXPECT_TRUE(out.find("Fg") != std::string::npos);
}

TEST(Print, KnightOutput) {
    Knight k("Kt", 5, 6);
    std::stringstream ss;
    k.print(ss);
    std::string out = ss.str();
    EXPECT_TRUE(out.find("Knight") != std::string::npos);
    EXPECT_TRUE(out.find("Kt") != std::string::npos);
}

TEST(Integration, BattleScenario) {
    set_t npcs;
    npcs.insert(std::make_shared<Dragon>("D1", 0, 0));
    npcs.insert(std::make_shared<Frog>("F1", 10, 0));
    npcs.insert(std::make_shared<Knight>("K1", 20, 0));

    EXPECT_EQ(npcs.size(), static_cast<size_t>(3));

    set_t dead_list;
    for (const auto& attacker : npcs) {
        for (const auto& defender : npcs) {
            if ((attacker != defender) && (attacker->is_close(defender, 15))) {
                bool success = defender->accept(attacker);
                if (success) dead_list.insert(defender);
            }
        }
    }

    EXPECT_GE(dead_list.size(), static_cast<size_t>(1));

}

TEST(EdgeCases, EmptyName) {
    Frog f("", 0, 0);
    EXPECT_EQ(f.name, "");
}

TEST(EdgeCases, NegativeCoordinates) {
    Knight k("K", -10, -20);
    EXPECT_EQ(k.x, -10);
    EXPECT_EQ(k.y, -20);
}

TEST(EdgeCases, LargeCoordinates) {
    Dragon d("Big", 100000, 200000);
    EXPECT_EQ(d.x, 100000);
    EXPECT_EQ(d.y, 200000);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
