#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Pack.h"
#include "Card.h"
#include <array>
#include <string>
using namespace std;

Pack::Pack() {
    next = 0;
    for (int suit = 0; suit < 4; suit++) {
        for (int rank = 0; rank < 6; rank++) {
            cards[suit * 6 + rank] = Card(RANK_NAMES_BY_WEIGHT[7 + rank],
                                            SUIT_NAMES_BY_WEIGHT[suit]);
        }
    }
}

Pack::Pack(istream& pack_input) {
    next = 0;
    string of = " ";
    string rank, suit;
    int num = 0;
    while (pack_input >> rank >> of >> suit) {
        cards[num] = Card(rank, suit);
        num ++;
    }
}

Card Pack::deal_one() {
    return cards[next++];
}

void Pack::reset() {
    next = 0;
}

void Pack::shuffle() {
    array<Card, PACK_SIZE> temp;
    for (int time = 0; time < 7; time++) {
        for (int i = 0; i < PACK_SIZE; i++) {
            if (i % 2 == 0) {
                temp[i] = cards[12 + i / 2];
            }
            if (i % 2 == 1) {
                temp[i] = cards[(i - 1) / 2];
            }
        }
        for (int i = 0; i < PACK_SIZE; i++) {
            cards[i] = temp[i];
        }
    }
    reset();
}

bool Pack::empty() const {
    if (next == 24) {
        return true;
    }
    return false;
}
