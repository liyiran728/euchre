#include <cassert>
#include "Card.h"
#include "Player.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <array>
#include <sstream>
using namespace std;

class SimplePlayer : public Player {
public:
    SimplePlayer(string name_in) : name(name_in) {
    }
    virtual const string & get_name() const override {
        return name;
    }
    virtual void add_card(const Card &c) override {
        assert(hand.size() < MAX_HAND_SIZE);
        hand.push_back(c);
    }
    virtual bool make_trump(const Card &upcard, bool is_dealer, int round,
                            string &order_up_suit) const override {
        assert(round == 1 || round == 2);
        int num = 0;
        if (round == 1) {
/*      if (upcard.is_face() && upcard.is_trump(upcard.get_suit())) {
            num++;
        }*/
            for (int i = 0; i < MAX_HAND_SIZE; i++) {
                if (hand[i].is_face() &&
                    hand[i].is_trump(upcard.get_suit())) {
                    num++;
                }
            }
            if (num >= 2) {
                order_up_suit = upcard.get_suit();
                return true;
            }
        }
        if (round == 2) {
            for (int i = 0; i < MAX_HAND_SIZE; i++) {
                if (hand[i].is_face() &&
                    hand[i].is_trump(Suit_next(upcard.get_suit()))) {
                    num++;
                }
            }
            if (num >= 1 || is_dealer) {
                order_up_suit = Suit_next(upcard.get_suit());
                return true;
            }
        }
        return false;
    }
    
    virtual void add_and_discard(const Card &upcard) override {
        assert(hand.size() > 0);
        Card lowest = upcard;
        int position = 0;
        for (int i = 0; i < MAX_HAND_SIZE; i++) {
            if (Card_less(hand[i], lowest, upcard.get_suit())) {
                lowest = hand[i];
                position = i;
            }
        }
        if (lowest != upcard) {
            hand[position] = upcard;
        }
    }
    
    virtual Card lead_card(const string &trump) override {
        assert(hand.size() > 0);
        Card lead;
        int position = 0;
        vector<Card> ctrump;
        vector<Card> cnontrump;
        for (unsigned int i = 0; i < hand.size(); i++) {
            if (hand[i].is_trump(trump)) {
                ctrump.push_back(hand[i]);
            }
            else {
                cnontrump.push_back(hand[i]);
            }
        }
        if (cnontrump.size() == 0) {
            lead = ctrump[0];
            for (unsigned int i = 1; i < ctrump.size(); i++) {
                if (Card_less(lead, ctrump[i], trump)) {
                    lead = ctrump[i];
                }
            }
        }
        else {
            lead = cnontrump[0];
            for (unsigned int i = 1; i < cnontrump.size(); i++) {
                if (Card_less(lead, cnontrump[i], trump)) {
                    lead = cnontrump[i];
                }
            }
        }
        for (unsigned int i = 0; i < hand.size(); i++) {
            if (hand[i] == lead) {
                position = i;
            }
        }
        hand.erase(hand.begin() + position);
        return lead;
    }
    
    virtual Card play_card(const Card &led_card,const string &trump) override {
        assert(hand.size() > 0);
        vector<Card> csuit;
        int position = 0;
        Card c;
        for (unsigned int i = 0; i < hand.size(); i++) {
            if (hand[i].get_suit(trump) == led_card.get_suit(trump)) {
                csuit.push_back(hand[i]);
            }
        }
        if (csuit.size() == 0) {
            c = hand[0];
            for (unsigned int i = 1; i < hand.size(); i++) {
                if (Card_less(hand[i], c, trump)) {
                    c = hand[i];
                }
            }
        }
        else {
            c = csuit[0];
            for (unsigned int i = 1; i < csuit.size(); i++) {
                if (Card_less(c, csuit[i], trump)) {
                    c = csuit[i];
                }
            }
        }
        for (unsigned int i = 0; i < hand.size(); i++) {
            if (hand[i] == c) {
                position = i;
            }
        }
        hand.erase(hand.begin() + position);
        return c;
    }
    
private:
    string name;
    vector<Card> hand;
};

class HumanPlayer : public Player {
public:
    HumanPlayer(string name_in) : name(name_in) {
    }
    
    virtual const string & get_name() const override {
        return name;
    }
    
    virtual void add_card(const Card &c) override {
        assert(hand.size() < MAX_HAND_SIZE);
        hand.push_back(c);
        sort(hand.begin(), hand.end());
    }
    
    void print_hand() const {
        for (unsigned int i = 0; i < hand.size(); i++) {
            cout << "Human player " << get_name() << "'s hand: [" << i << "] ";
            cout << hand[i] << endl;
        }
    }
    
    virtual bool make_trump(const Card &upcard, bool is_dealer, int round,
                        string &order_up_suit) const override {
        assert(round == 1 || round == 2);
        string choice;
        print_hand();
        cout << "Human player " << get_name()
             << ", please enter a suit, or \"pass\":" << endl;
        cin >> choice;
        if (choice == "pass") {
            return false;
        }
        else {
            order_up_suit = choice;
            return true;
        }
    }
    
    virtual void add_and_discard(const Card &upcard) override {
        assert(hand.size() > 0);
        int num = 0;
        print_hand();
        cout << "Discard upcard: [-1]" << endl
             << "Human player " << get_name()
             << ", please select a card to discard:" << endl;
        cin >> num;
        if (num != -1) {
            hand[num] = upcard;
        }
        sort(hand.begin(), hand.end());
        cout << endl;
    }
    
    virtual Card lead_card(const string &trump) override {
        assert(hand.size() > 0);
        int num = 0;
        sort(hand.begin(), hand.end());
        print_hand();
        cout << "Human player " << get_name()
             << ", please select a card:" << endl;
        cin >> num;
        Card lead_card = hand[num];
        hand.erase(hand.begin() + num);
        sort(hand.begin(), hand.end());
        return lead_card;
    }
    
    virtual Card play_card(const Card &led_card,const string &trump) override {
        return lead_card(trump);
    }
    
private:
    string name;
    vector<Card> hand;
};

Player * Player_factory(const string &name, const string &strategy) {
    if (strategy == "Simple") {
      // The "new" keyword dynamically allocates an object.
        return new SimplePlayer(name);
    }
    // Repeat for Human Player
    if (strategy == "Human") {
        return new HumanPlayer(name);
    }
    // Invalid strategy if we get here
    assert(false);
    return nullptr;
}

ostream & operator<<(ostream &os, const Player &p) {
    os << p.get_name();
    return os;
}
