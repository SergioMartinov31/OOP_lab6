#include "knight.h"
#include "../npc/npc.h"
#include "../dragon/dragon.h"
#include "../frog/frog.h"
#include <iostream>

Knight::Knight(const std::string& name, int x, int y) : NPC(KnightType, name, x, y) {}
Knight::Knight(std::istream& is) : NPC(KnightType, is) {}

void Knight::print(std::ostream& os) {
    os << *this;
}

void Knight::save(std::ostream& os) {
    os << KnightType << std::endl;
    NPC::save(os);
}

bool Knight::visit(std::shared_ptr<Dragon> other) {
    (void)other;
    fight_notify(std::static_pointer_cast<NPC>(other), true);
    return true;
}

bool Knight::visit(std::shared_ptr<Frog> other) {
    (void)other;
    return false;
}

bool Knight::visit(std::shared_ptr<Knight> other) {
    (void)other;
    return false;
}

bool Knight::accept(std::shared_ptr<NPC> attacker) {
    return attacker->visit(std::dynamic_pointer_cast<Knight>(shared_from_this()));
}

std::ostream& operator<<(std::ostream& os, Knight& knight) {
    os << "Knight: " << knight.name << " " << *static_cast<NPC*>(&knight) << std::endl;
    return os;
}
