// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <string>
#include "Card.h"
using namespace std;
// add any necessary #include or using directives here

// rank and suit names -- do not remove these
constexpr const char* const Card::RANK_TWO;
constexpr const char* const Card::RANK_THREE;
constexpr const char* const Card::RANK_FOUR;
constexpr const char* const Card::RANK_FIVE;
constexpr const char* const Card::RANK_SIX;
constexpr const char* const Card::RANK_SEVEN;
constexpr const char* const Card::RANK_EIGHT;
constexpr const char* const Card::RANK_NINE;
constexpr const char* const Card::RANK_TEN;
constexpr const char* const Card::RANK_JACK;
constexpr const char* const Card::RANK_QUEEN;
constexpr const char* const Card::RANK_KING;
constexpr const char* const Card::RANK_ACE;

constexpr const char* const Card::SUIT_SPADES;
constexpr const char* const Card::SUIT_HEARTS;
constexpr const char* const Card::SUIT_CLUBS;
constexpr const char* const Card::SUIT_DIAMONDS;

Card::Card() {
    rank = RANK_TWO;
    suit = SUIT_SPADES;
}

Card::Card(const std::string &rank_in, const std::string &suit_in) {
    rank = rank_in;
    suit = suit_in;
}

string Card::get_rank() const {
    return rank;
}

string Card::get_suit() const {
    return suit;
}

string Card::get_suit(const string &trump) const {
    if (is_left_bower(trump)) {
        return trump;
    }
    else {
        return suit;
    }
}

bool Card::is_face() const {
    if (rank == RANK_JACK || rank == RANK_QUEEN ||
        rank == RANK_KING || rank == RANK_ACE) {
        return true;
    }
    return false;
}

bool Card::is_right_bower(const string &trump) const {
    if (rank == RANK_JACK && suit == trump) {
        return true;
    }
    return false;
}

bool Card::is_left_bower(const std::string &trump) const {
    if (rank == RANK_JACK && suit == Suit_next(trump)) {
        return true;
    }
    return false;
}

bool Card::is_trump(const std::string &trump) const {
    if (suit == trump || is_left_bower(trump)) {
        return true;
    }
    return false;
}

static int rank_index(const Card &c) {
    for (int i = 0; i < NUM_RANKS; i++) {
        if (RANK_NAMES_BY_WEIGHT[i] == c.get_rank()) {
            return i;
        }
    }
    assert(false);
}

static int suit_index(const Card &c) {
    for (int i = 0; i < NUM_SUITS; i++) {
        if (SUIT_NAMES_BY_WEIGHT[i] == c.get_suit()) {
            return i;
        }
    }
    assert(false);
}

bool operator<(const Card &lhs, const Card &rhs) {
    if (rank_index(lhs) < rank_index(rhs)) {
        return true;
    }
    if ((rank_index(lhs) == rank_index(rhs)) &&
        (suit_index(lhs) < suit_index(rhs))) {
        return true;
    }
    return false;
}

bool operator>(const Card &lhs, const Card &rhs) {
    if (rank_index(lhs) > rank_index(rhs)) {
        return true;
    }
    if ((rank_index(lhs) == rank_index(rhs)) &&
        (suit_index(lhs) > suit_index(rhs))) {
        return true;
    }
    return false;
}

bool operator==(const Card &lhs, const Card &rhs) {
    if ((rank_index(lhs) == rank_index(rhs)) &&
        (suit_index(lhs) == suit_index(rhs))) {
        return true;
    }
    return false;
}

bool operator!=(const Card &lhs, const Card &rhs) {
    if ((rank_index(lhs) == rank_index(rhs)) &&
        (suit_index(lhs) == suit_index(rhs))) {
        return false;
    }
    return true;
}

string Suit_next(const string &suit) {
    if (suit == Card::SUIT_SPADES) {
        return Card::SUIT_CLUBS;
    }
    if (suit == Card::SUIT_CLUBS) {
        return Card::SUIT_SPADES;
    }
    if (suit == Card::SUIT_HEARTS) {
        return Card::SUIT_DIAMONDS;
    }
    if (suit == Card::SUIT_DIAMONDS) {
        return Card::SUIT_HEARTS;
    }
    assert(false);
}

ostream & operator<<(ostream &os, const Card &card) {
    os << card.get_rank() << " of " << card.get_suit();
    return os;
}

bool Card_less(const Card &a, const Card &b, const std::string &trump) {
    if (!a.is_trump(trump) && b.is_trump(trump)) {
        return true;
    }
    if (a.is_trump(trump) && b.is_trump(trump)) {
        if (b.is_right_bower(trump)) {
            return true;
        }
        if (!a.is_right_bower(trump) && b.is_left_bower(trump)) {
            return true;
        }
        if (!a.is_right_bower(trump) && !a.is_left_bower(trump) &&
            !b.is_right_bower(trump) && !b.is_left_bower(trump)) {
            if (a < b) {
                return true;
            }
        }
    }
    if (!a.is_trump(trump) && !b.is_trump(trump)) {
        if (a < b) {
            return true;
        }
    }
    return false;
}

bool Card_less(const Card &a, const Card &b, const Card &led_card,
const std::string &trump) {
    if (led_card.get_suit() != trump) {
        if (a.get_suit() != led_card.get_suit() &&
            a.get_suit() != trump &&
            b.get_suit() == led_card.get_suit()) {
            return true;
        }
        if (a.get_suit() == led_card.get_suit() &&
            b.get_suit() != led_card.get_suit() &&
            b.get_suit() != trump) {
            return false;
        }
        if (a.get_suit() == led_card.get_suit() &&
            b.get_suit() == led_card.get_suit()) {
            if (a < b) {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return Card_less(a, b, trump);
        }
    }
    else {
        return Card_less(a, b, trump);
    }
}
