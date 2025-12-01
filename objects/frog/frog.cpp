#include "frog.h"
#include "../npc/npc.h"
#include "../dragon/dragon.h"
#include "../knight/knight.h"
#include <iostream>

Frog::Frog(const std::string& name, int x, int y) : NPC(FrogType, name, x, y) {}
Frog::Frog(std::istream& is) : NPC(FrogType, is) {}

void Frog::print(std::ostream& os) {
    os << *this;
}

void Frog::save(std::ostream& os) {
    os << FrogType << std::endl;
    NPC::save(os);
}

bool Frog::visit(std::shared_ptr<Dragon> other) {
    (void)other;
    fight_notify(std::static_pointer_cast<NPC>(other), true);
    return true;
}

bool Frog::visit(std::shared_ptr<Frog> other) {
    (void)other;
    fight_notify(std::static_pointer_cast<NPC>(other), true);
    return true;
}

bool Frog::visit(std::shared_ptr<Knight> other) {
    (void)other;
    fight_notify(std::static_pointer_cast<NPC>(other), true);
    return true;
}

bool Frog::accept(std::shared_ptr<NPC> attacker) {
    return attacker->visit(std::dynamic_pointer_cast<Frog>(shared_from_this()));
}

std::ostream& operator<<(std::ostream& os, Frog& frog) {
    os << "Frog: " << frog.name << " " << *static_cast<NPC*>(&frog) << std::endl;
    return os;
}
