#pragma once

#include <vector>
#include <string>
#include <cstdint>

using namespace std;
typedef vector<uint64_t>::iterator itr;

struct Resto
{
    Resto() : id(0), revenu(0), quantite(0) {}
    Resto(uint32_t id) : id(id) {}
    bool operator<(const Resto& r) const { return id < r.id; }
    bool operator==(const Resto& r) const { return id == r.id; }
    bool operator!=(const Resto& r) const { return id != r.id; }
    uint32_t id;
    uint32_t revenu;
    uint32_t quantite;
    bool choisi;
    float GetRentabilite() { return (float)revenu / (float)quantite; }
};
struct HeuristicSwap
{
    int sub1 = -1;
    int sub2 = -1;
    int add1 = -1;
    int add2 = -1;
};

vector<Resto> ChargerExemplaire(string path, uint32_t& capaciteFournisseur);

vector<Resto> Glouton(vector<Resto>& restos, uint32_t capaciteFournisseur);

vector<Resto> ProgDyn(vector<Resto>& restos, uint32_t capaciteFournisseur);

vector<Resto> Heuristique(vector<Resto>& restos, uint32_t capaciteFournisseur);
