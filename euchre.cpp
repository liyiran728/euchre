#include <cassert>
#include "Pack.h"
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

class Game {
private:
    static const int NUM_PLAYER = 4;
    static const int NUM_CARD = 5;
    static const int NUM_TRICK = 5;
    int numhand;
    Pack pack;
    array<Player*, NUM_PLAYER> players;
    int score_to_win = 0;
    string trump = "";
    Card upcard;
    string upcard_suit = "";
    int makerteam;
    int team1score = 0;
    int team2score = 0;
public:
    Game(ifstream &infile) {
        numhand = 0;
        Pack pack(infile);
    }
    void set_score(int score) {
        score_to_win = score;
    }
    void make_players(int num, string name, string strategy) {
        players[num] = Player_factory(name, strategy);
    }
    //shuffle
    Pack shuffle(string option) {
        if (option == "shuffle") {
            pack.shuffle();
        }
        else {
            pack.reset();
        }
        return pack;
    }
    int get_hand() const {
        return numhand;
    }
    Player* Dealer() const {
        return players[numhand % NUM_PLAYER];
    }
    bool is_dealer(Player* p) const {
        if (p->get_name() == Dealer()->get_name()) {
            return true;
        }
        else {
            return false;
        }
    }
    //deal
    void deal() {
        //3, 2, 3, 2
        for (int i = 0; i < 3; i++) {
            players[(get_hand() + 1) % NUM_PLAYER]->add_card(pack.deal_one());
        }
        for (int i = 0; i < 2; i++) {
            players[(get_hand() + 2) % NUM_PLAYER]->add_card(pack.deal_one());
        }
        for (int i = 0; i < 3; i++) {
            players[(get_hand() + 3) % NUM_PLAYER]->add_card(pack.deal_one());
        }
        for (int i = 0; i < 2; i++) {
            players[get_hand() % NUM_PLAYER]->add_card(pack.deal_one());
        }
        //2, 3, 2, 3
        for (int i = 0; i < 2; i++) {
            players[(get_hand() + 1) % NUM_PLAYER]->add_card(pack.deal_one());
        }
        for (int i = 0; i < 3; i++) {
            players[(get_hand() + 2) % NUM_PLAYER]->add_card(pack.deal_one());
        }
        for (int i = 0; i < 2; i++) {
            players[(get_hand() + 3) % NUM_PLAYER]->add_card(pack.deal_one());
        }
        for (int i = 0; i < 3; i++) {
            players[get_hand() % NUM_PLAYER]->add_card(pack.deal_one());
        }
    }
    Card Upcard() {
        upcard = pack.deal_one();
        upcard_suit = upcard.get_suit();
        return upcard;
    }
    void Announce_hand() {
        cout << "Hand " << get_hand() << endl;
        cout << Dealer()->get_name() << " deals" << endl;
        cout << Upcard() << " turned up" << endl;
    }
    //make trump
    void make_trump() {
        int numfalse = 0;
        int makerID = 0;
        for (int i = 0; i < NUM_PLAYER; i++) {
            if (players[(get_hand() + 1 + i) % NUM_PLAYER]->make_trump(
                upcard, is_dealer(players[(get_hand() + 1 + i) % NUM_PLAYER]),
                1, upcard_suit) == false) {
                numfalse++;
                cout << players[(get_hand() + 1 + i) % NUM_PLAYER]->
                                get_name()
                << " passes" << endl;
            }
            else {
                makerID = (get_hand() + 1 + i) % NUM_PLAYER;
                cout << players[(get_hand() + 1 + i) % 4]->
                                get_name()
                << " orders up " << upcard_suit << endl;
                trump = upcard_suit;
                Dealer()->add_and_discard(upcard);
                cout << endl;
                break;
            }
        }
        if (numfalse == 4) {
            for (int i = 0; i < NUM_PLAYER; i++) {
                if (players[(get_hand() + 1 + i) % NUM_PLAYER]->make_trump(
                    upcard,
                    is_dealer(players[(get_hand() + 1 + i) % NUM_PLAYER]),
                                            2, upcard_suit) == false) {
                    cout << players[(get_hand() + 1 + i) % NUM_PLAYER]->
                                    get_name()
                    << " passes" << endl;
                }
                else {
                    makerID = (get_hand() + 1 + i) % NUM_PLAYER;
                    cout << players[(get_hand() + 1 + i) % 4]->
                                    get_name()
                    << " orders up " << upcard_suit << endl;
                    trump = upcard_suit;
                    cout << endl;
                    break;
                }
            }
        }
        if (makerID % 2 == 0) {
            makerteam = 1;
        }
        else {
            makerteam = 2;
        }
    }
    
    void play_hand() {
        Card winner;
        int team1trick = 0;
        int winplayerID = (get_hand() + 1) % NUM_PLAYER;
        Player * winplayer = players[(get_hand() + 1) % NUM_PLAYER];
        for (int i = 0; i < NUM_TRICK; i++) {
            array<Card, 4> cards;
            Card lead_card = winplayer->lead_card(trump);
            cards[0] = lead_card;
            cout << cards[0] << " led by " << winplayer->get_name() << endl;
            for (int i = 1; i < NUM_PLAYER; i++) {
                cards[i] = players[(winplayerID+i) % NUM_PLAYER]->
                                    play_card(lead_card, trump);
            cout << cards[i] << " played by "
            << players[(winplayerID+i) % NUM_PLAYER]->get_name() << endl;
            }
            //decide which card wins
            winner = cards[0];
            for (int i = 0; i < 4; i++) {
                if (Card_less(winner, cards[i], lead_card, trump)) {
                    winner = cards[i];
                    winplayer = players[(winplayerID+i) % NUM_PLAYER];
                }
            }
            for (int i = 0; i < NUM_PLAYER; i++) {
                if (players[i]->get_name() == winplayer->get_name()) {
                    winplayerID = i;
                }
            }
            if (winplayerID % 2 == 0) {
                team1trick++;
            }
            cout << winplayer->get_name() << " takes the trick" << endl;
            cout << endl;
        }
        //who won the hand?
        Hand_winner(team1trick);
    }
    void Hand_winner(int team1trick) {
        if (team1trick >= 3) {
            cout << players[0]->get_name() << " and "
                 << players[2]->get_name() << " win the hand" << endl;
            if (makerteam == 1) {
                if (team1trick == 5) {
                    cout << "march!" << endl;
                    team1score += 2;
                }
                else { team1score++; }
            } else {
                cout << "euchred!" << endl;
                team1score += 2;
            }
        }
        else {
            cout << players[1]->get_name() << " and "
                 << players[3]->get_name() << " win the hand" << endl;
            if (makerteam == 2) {
                if (team1trick == 0) {
                    cout << "march!" << endl;
                    team2score += 2;
                } else { team2score++; }
            }
            else {
                cout << "euchred!" << endl;
                team2score += 2;
            }
        }
    }
    void Announce_score() {
        cout << players[0]->get_name() << " and " << players[2]->get_name()
        << " have " << team1score << " points" << endl;
        cout << players[1]->get_name() << " and " << players[3]->get_name()
        << " have " << team2score << " points" << endl;
        cout << endl;
    }
    //delete players
    void player_delete() {
        for (int i = 0; i < NUM_PLAYER; i++) {
            delete players[i];
        }
    }
    void Run_game(string option) {
        while (team1score < score_to_win && team2score < score_to_win) {
            shuffle(option);
            deal();
            Announce_hand();
            make_trump();
            play_hand();
            Announce_score();
            numhand++;
        }
        if (team1score >= score_to_win) {
            cout << players[0]->get_name() << " and " << players[2]->get_name()
                 << " win!" << endl;
        }
        else {
            cout << players[1]->get_name() << " and " << players[3]->get_name()
                 << " win!" << endl;
        }
        player_delete();
    }
};

int main(int argc, char *argv[]) {
    if (argc != 12 || atoi(argv[3]) < 1 || atoi(argv[3]) > 100 ||
        (strcmp(argv[2], "shuffle") != 0 && strcmp(argv[2], "noshuffle") != 0)||
        (strcmp(argv[5], "Simple") != 0 && strcmp(argv[5], "Human") != 0) ||
        (strcmp(argv[7], "Simple") != 0 && strcmp(argv[7], "Human") != 0) ||
        (strcmp(argv[9], "Simple") != 0 && strcmp(argv[9], "Human") != 0) ||
        (strcmp(argv[11], "Simple") != 0 && strcmp(argv[11], "Human") != 0)) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        return 1;
    }
    ifstream infile(argv[1]);
    if (!infile.is_open()) {
        cout << "Error opening " << argv[1] << endl;
    }
    for (int i = 0; i < argc; i++) {
        cout << argv[i] << " ";
    }
    cout << endl;
    Game game(infile);
    game.make_players(0, argv[4], argv[5]);
    game.make_players(1, argv[6], argv[7]);
    game.make_players(2, argv[8], argv[9]);
    game.make_players(3, argv[10], argv[11]);
    game.set_score(atoi(argv[3]));
    string option = argv[2];
    game.Run_game(option);
    return 0;
}
