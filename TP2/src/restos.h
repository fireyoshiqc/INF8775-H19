#pragma once

#include <vector>
#include <string>
#include <cstdint>

using namespace std;
typedef vector<uint64_t>::iterator itr;

struct Resto
{
    uint32_t id;
    uint32_t revenu;
    uint32_t quantite;
    bool choisi;
    float GetRentabilite() { return (float)revenu / (float)quantite; }
};

vector<Resto> ChargerExemplaire(string path, uint32_t& capaciteFournisseur);

vector<Resto> Glouton(vector<Resto>& restos, uint32_t capaciteFournisseur);

vector<Resto> ProgDyn(vector<Resto>& restos, uint32_t capaciteFournisseur);
