#pragma once

#include "../npc/npc.h"
#include <memory>

struct Knight : public NPC {
    Knight(const std::string& name, int x, int y);
    Knight(std::istream& is);

    void print(std::ostream& os) override;
    void save(std::ostream& os) override;

    bool visit(std::shared_ptr<Dragon> other) override;
    bool visit(std::shared_ptr<Frog> other) override;
    bool visit(std::shared_ptr<Knight> other) override;

    bool accept(std::shared_ptr<NPC> attacker) override;

    friend std::ostream& operator<<(std::ostream& os, Knight& knight);
};
