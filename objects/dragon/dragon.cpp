#include "dragon.h"
#include "../npc/npc.h"
#include "../frog/frog.h"
#include "../knight/knight.h"
#include <iostream>

Dragon::Dragon(const std::string& name, int x, int y) : NPC(DragonType, name, x, y) {}
Dragon::Dragon(std::istream& is) : NPC(DragonType, is) {}

void Dragon::print(std::ostream& os) {
    os << *this;
}

void Dragon::save(std::ostream& os) {
    os << DragonType << std::endl;
    NPC::save(os);
}

bool Dragon::visit(std::shared_ptr<Dragon> other) {
    (void)other;
    return false;
}

bool Dragon::visit(std::shared_ptr<Frog> other) {
    (void)other;
    return false;
}

bool Dragon::visit(std::shared_ptr<Knight> other) {
    // Dragon kills Knight
    fight_notify(std::static_pointer_cast<NPC>(other), true);
    return true;
}

bool Dragon::accept(std::shared_ptr<NPC> attacker) {
    return attacker->visit(std::dynamic_pointer_cast<Dragon>(shared_from_this()));
}

std::ostream& operator<<(std::ostream& os, Dragon& dragon) {
    os << "Dragon: " << dragon.name << " " << *static_cast<NPC*>(&dragon) << std::endl;
    return os;
}
