// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Player.h"
#include "unit_test_framework.h"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(test_player_insertion) {
  Player * human = Player_factory("NotRobot", "Human");
  ostringstream oss;
  oss << * human;
  ASSERT_EQUAL(oss.str(), "NotRobot");
  oss.str("");
  Player * alice = Player_factory("Alice", "Simple");
  oss << *alice;
  ASSERT_EQUAL(oss.str(), "Alice");
  delete alice;
  delete human;
}

static void add_cards(Player *player) {
  Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
  for (int i = 0; i < 5; ++i) {
    player->add_card(two_spades);
  }
}

TEST(test_simple_player_make_trump_public) {
  Player * bob = Player_factory("Bob", "Simple");
  add_cards(bob);

  Card nine_spades(Card::RANK_NINE, Card::SUIT_SPADES);
  string trump = Card::SUIT_SPADES;
  bool orderup = bob->make_trump(
      nine_spades,    //upcard
      true,           //Bob is also the dealer
      1,              //first round
      trump           //suit ordered up (if any)
  );
  ASSERT_FALSE(orderup);
  ASSERT_EQUAL(trump, Card::SUIT_SPADES);

  delete bob;
}

TEST(test_simple_player_make_trump_1st_round) {
    Player * rae = Player_factory("Rae", "Simple");
    //Cards without left bowers
    Card king_diamonds(Card::RANK_KING, Card::SUIT_DIAMONDS);
    Card jack_hearts(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card queen_diamonds(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
    Card ten_hearts(Card::RANK_TEN, Card::SUIT_HEARTS);
    Card king_clubs(Card::RANK_KING, Card::SUIT_CLUBS);
    rae->add_card(king_diamonds);
    rae->add_card(jack_hearts);
    rae->add_card(queen_diamonds);
    rae->add_card(ten_hearts);
    rae->add_card(king_clubs);
    
    //no trump cards
    Card nine_spades(Card::RANK_NINE, Card::SUIT_SPADES);
    string trump = Card::SUIT_SPADES;
    bool orderup = rae->make_trump(
      nine_spades,    //upcard
      true,           //Bob is also the dealer
      1,              //first round
      trump           //suit ordered up (if any)
    );
    ASSERT_FALSE(orderup);
    ASSERT_EQUAL(trump, Card::SUIT_SPADES);
    
    //two trump face cards
    Card nine_diamonds(Card::RANK_NINE, Card::SUIT_DIAMONDS);
    string trump1 = Card::SUIT_DIAMONDS;
    bool orderup1 = rae->make_trump(
      nine_diamonds,    //upcard
      false,           //Bob is also the dealer
      1,              //first round
      trump1           //suit ordered up (if any)
    );
    ASSERT_TRUE(orderup1);
    ASSERT_EQUAL(trump1, Card::SUIT_DIAMONDS);
    
    
    bool orderup4 = rae->make_trump(
      nine_diamonds,    //upcard
      true,           //Bob is also the dealer
      1,              //first round
      trump1           //suit ordered up (if any)
    );
    ASSERT_TRUE(orderup4);
    ASSERT_EQUAL(trump1, Card::SUIT_DIAMONDS);
    
    //one trump face card
    Card nine_hearts(Card::RANK_NINE, Card::SUIT_HEARTS);
    string trump2 = Card::SUIT_HEARTS;
    bool orderup2 = rae->make_trump(
      nine_hearts,    //upcard
      false,           //Bob is also the dealer
      1,              //first round
      trump2           //suit ordered up (if any)
    );
    ASSERT_FALSE(orderup2);
    ASSERT_EQUAL(trump2, Card::SUIT_HEARTS);
    
    bool orderup5 = rae->make_trump(
      nine_hearts,    //upcard
      true,           //Bob is also the dealer
      1,              //first round
      trump2           //suit ordered up (if any)
    );
    ASSERT_FALSE(orderup5);
    ASSERT_EQUAL(trump2, Card::SUIT_HEARTS);
    
    //one trump face card, two trump cards total, plus trump face upcard
    Card queen_hearts(Card::RANK_QUEEN, Card::SUIT_HEARTS);
    string trump3 = Card::SUIT_HEARTS;
    bool orderup3 = rae->make_trump(
      queen_hearts,    //upcard
      false,           //Bob is also the dealer
      1,              //first round
      trump3           //suit ordered up (if any)
    );
    ASSERT_FALSE(orderup3);
    ASSERT_EQUAL(trump3, Card::SUIT_HEARTS);
    
    bool orderup6 = rae->make_trump(
      queen_hearts,    //upcard
      true,           //Bob is also the dealer
      1,              //first round
      trump3           //suit ordered up (if any)
    );
    ASSERT_FALSE(orderup6);
    ASSERT_EQUAL(trump3, Card::SUIT_HEARTS);
    
    delete rae;
    
}
TEST(test_simple_player_make_trump_1st_round_2) {
    Player * rea = Player_factory("Rea", "Simple");
    Card king_diamonds(Card::RANK_KING, Card::SUIT_DIAMONDS);
    Card jack_hearts(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card nine_diamonds(Card::RANK_NINE, Card::SUIT_DIAMONDS);
    Card ten_hearts(Card::RANK_TEN, Card::SUIT_HEARTS);
    Card king_clubs(Card::RANK_KING, Card::SUIT_CLUBS);
    rea->add_card(king_diamonds);
    rea->add_card(jack_hearts);
    rea->add_card(nine_diamonds);
    rea->add_card(ten_hearts);
    rea->add_card(king_clubs);
    
    Card queen_hearts(Card::RANK_QUEEN, Card::SUIT_HEARTS);
    string trump2 = Card::SUIT_HEARTS;
    bool orderup2 = rea->make_trump(
      queen_hearts,    //upcard
      false,           //Bob is also the dealer
      1,              //first round
      trump2           //suit ordered up (if any)
    );
    ASSERT_FALSE(orderup2);
    ASSERT_EQUAL(trump2, Card::SUIT_HEARTS);
    
    Card queen_diamonds(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
    string trump1 = Card::SUIT_DIAMONDS;
    bool orderup1 = rea->make_trump(
      queen_diamonds,    //upcard
      false,           //Bob is also the dealer
      1,              //first round
      trump1           //suit ordered up (if any)
    );
    ASSERT_TRUE(orderup1);
    ASSERT_EQUAL(trump1, Card::SUIT_DIAMONDS);
    delete rea;
}

TEST(test_simple_player_make_trump_2st_round) {
    Player * rae = Player_factory("Rae", "Simple");
    Card nine_diamonds(Card::RANK_NINE, Card::SUIT_DIAMONDS);
    Card jack_hearts(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card ten_spades(Card::RANK_TEN, Card::SUIT_SPADES);
    Card ten_hearts(Card::RANK_TEN, Card::SUIT_HEARTS);
    Card king_clubs(Card::RANK_KING, Card::SUIT_CLUBS);
    rae->add_card(nine_diamonds);
    rae->add_card(jack_hearts);
    rae->add_card(ten_spades);
    rae->add_card(ten_hearts);
    rae->add_card(king_clubs);
    
    //no trump cards
    Card nine_clubs(Card::RANK_NINE, Card::SUIT_CLUBS);
    string trump = Card::SUIT_CLUBS;
    bool orderup = rae->make_trump(
      nine_clubs,
      false,
      2,
      trump
    );
    ASSERT_FALSE(orderup);
    ASSERT_EQUAL(trump, Card::SUIT_CLUBS);
    //screw the dealer
    bool orderup3 = rae->make_trump(
      nine_clubs,
      true,
      2,
      trump
    );
    ASSERT_TRUE(orderup3);
    ASSERT_EQUAL(trump, Card::SUIT_SPADES);
    //has left bower
    Card queen_hearts(Card::RANK_QUEEN, Card::SUIT_HEARTS);
    string trump2 = Card::SUIT_HEARTS;
    bool orderup2 = rae->make_trump(
      queen_hearts,
      false,
      2,
      trump2
    );
    ASSERT_TRUE(orderup2);
    ASSERT_EQUAL(trump2, Card::SUIT_DIAMONDS);
    //dealer
    bool orderup4 = rae->make_trump(
      queen_hearts,
      true,
      2,
      trump2
    );
    ASSERT_TRUE(orderup4);
    ASSERT_EQUAL(trump2, Card::SUIT_DIAMONDS);
    //has right bower
    Card queen_diamonds(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
    string trump3 = Card::SUIT_DIAMONDS;
    bool orderup5 = rae->make_trump(
      queen_diamonds,
      false,
      2,
      trump3
    );
    ASSERT_TRUE(orderup5);
    ASSERT_EQUAL(trump3, Card::SUIT_HEARTS);
    //dealer
    bool orderup6 = rae->make_trump(
      queen_diamonds,
      true,
      2,
      trump3
    );
    ASSERT_TRUE(orderup6);
    ASSERT_EQUAL(trump3, Card::SUIT_HEARTS);
    //one trump face card
    Card nine_spades(Card::RANK_NINE, Card::SUIT_SPADES);
    string trump4 = Card::SUIT_SPADES;
    bool orderup7 = rae->make_trump(
      nine_spades,
      false,
      2,
      trump4
    );
    ASSERT_TRUE(orderup7);
    ASSERT_EQUAL(trump4, Card::SUIT_CLUBS);
    //dealer
    bool orderup8 = rae->make_trump(
      nine_spades,
      true,
      2,
      trump4
    );
    ASSERT_TRUE(orderup8);
    ASSERT_EQUAL(trump4, Card::SUIT_CLUBS);
    delete rae;
}

TEST(test_simple_player_lead_card) {
  Player * bob = Player_factory("Bob", "Simple");
  add_cards(bob);
  Card nine_spades(Card::RANK_NINE, Card::SUIT_SPADES);
  bob->add_and_discard(nine_spades);
  Card card_led = bob->lead_card(Card::SUIT_SPADES);
  ASSERT_EQUAL(card_led, nine_spades); //check led card
  delete bob;
    //normal
    Player * rae = Player_factory("Rae", "Simple");
    Card nine_diamonds(Card::RANK_NINE, Card::SUIT_DIAMONDS);
    Card jack_hearts(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card ten_spades(Card::RANK_TEN, Card::SUIT_SPADES);
    Card ten_hearts(Card::RANK_TEN, Card::SUIT_HEARTS);
    Card king_clubs(Card::RANK_KING, Card::SUIT_CLUBS);
    Card queen_clubs(Card::RANK_QUEEN, Card::SUIT_CLUBS);
    rae->add_card(nine_diamonds);
    rae->add_card(jack_hearts);
    rae->add_card(ten_spades);
    rae->add_card(ten_hearts);
    rae->add_card(king_clubs);
    rae->add_and_discard(queen_clubs);
    Card card_led1_1 = rae->lead_card(Card::SUIT_CLUBS);
    Card card_led1_2 = rae->lead_card(Card::SUIT_CLUBS);
    Card card_led1_3 = rae->lead_card(Card::SUIT_CLUBS);
    Card card_led1_4 = rae->lead_card(Card::SUIT_CLUBS);
    Card card_led1_5 = rae->lead_card(Card::SUIT_CLUBS);
    ASSERT_EQUAL(card_led1_1, jack_hearts);
    ASSERT_EQUAL(card_led1_2, ten_hearts);
    ASSERT_EQUAL(card_led1_3, ten_spades);
    ASSERT_EQUAL(card_led1_4, king_clubs);
    ASSERT_EQUAL(card_led1_5, queen_clubs);
    delete rae;
    //only trump cards left, including left bower
    Player * rea = Player_factory("Rea", "Simple");
    Card nine_clubs(Card::RANK_NINE, Card::SUIT_CLUBS);
    Card jack_spades(Card::RANK_JACK, Card::SUIT_SPADES);
    Card ten_clubs(Card::RANK_TEN, Card::SUIT_CLUBS);
    rea->add_card(nine_clubs);
    rea->add_card(jack_spades);
    rea->add_card(ten_clubs);
    rea->add_card(ten_hearts);
    rea->add_card(king_clubs);
    rea->add_and_discard(queen_clubs);
    Card card_led2_1 = rea->lead_card(Card::SUIT_CLUBS);
    Card card_led2_2 = rea->lead_card(Card::SUIT_CLUBS);
    Card card_led2_3 = rea->lead_card(Card::SUIT_CLUBS);
    Card card_led2_4 = rea->lead_card(Card::SUIT_CLUBS);
    Card card_led2_5 = rea->lead_card(Card::SUIT_CLUBS);
    ASSERT_EQUAL(card_led2_1, jack_spades);
    ASSERT_EQUAL(card_led2_2, king_clubs);
    ASSERT_EQUAL(card_led2_3, queen_clubs);
    ASSERT_EQUAL(card_led2_4, ten_clubs);
    ASSERT_EQUAL(card_led2_5, nine_clubs);
    delete rea;
    //only trump cards left, including left and right bower
    Player * jenny = Player_factory("Jenny", "Simple");
    Card jack_clubs(Card::RANK_JACK, Card::SUIT_CLUBS);
    jenny->add_card(jack_spades);
    jenny->add_card(jack_clubs);
    jenny->add_card(ten_clubs);
    jenny->add_card(ten_hearts);
    jenny->add_card(king_clubs);
    jenny->add_and_discard(queen_clubs);
    Card card_led3_1 = jenny->lead_card(Card::SUIT_CLUBS);
    Card card_led3_2 = jenny->lead_card(Card::SUIT_CLUBS);
    Card card_led3_3 = jenny->lead_card(Card::SUIT_CLUBS);
    Card card_led3_4 = jenny->lead_card(Card::SUIT_CLUBS);
    Card card_led3_5 = jenny->lead_card(Card::SUIT_CLUBS);
    ASSERT_EQUAL(card_led3_1, jack_clubs);
    ASSERT_EQUAL(card_led3_2, jack_spades);
    ASSERT_EQUAL(card_led3_3, king_clubs);
    ASSERT_EQUAL(card_led3_4, queen_clubs);
    ASSERT_EQUAL(card_led3_5, ten_clubs);
    delete jenny;
    //break the tie by suit
    Player * sue = Player_factory("Sue", "Simple");
    Card ten_diamonds(Card::RANK_TEN, Card::SUIT_DIAMONDS);
    Card king_spades(Card::RANK_KING, Card::SUIT_SPADES);
    Card king_hearts(Card::RANK_KING, Card::SUIT_HEARTS);
    sue->add_card(nine_diamonds);
    sue->add_card(jack_hearts);
    sue->add_card(king_spades);
    sue->add_card(king_hearts);
    sue->add_card(king_clubs);
    sue->add_and_discard(ten_diamonds);
    Card card_led4_1 = sue->lead_card(Card::SUIT_DIAMONDS);
    Card card_led4_2 = sue->lead_card(Card::SUIT_DIAMONDS);
    Card card_led4_3 = sue->lead_card(Card::SUIT_DIAMONDS);
    Card card_led4_4 = sue->lead_card(Card::SUIT_DIAMONDS);
    Card card_led4_5 = sue->lead_card(Card::SUIT_DIAMONDS);
    ASSERT_EQUAL(card_led4_1, king_clubs);
    ASSERT_EQUAL(card_led4_2, king_hearts);
    ASSERT_EQUAL(card_led4_3, jack_hearts);
    ASSERT_EQUAL(card_led4_4, ten_diamonds);
    ASSERT_EQUAL(card_led4_5, nine_diamonds);
    delete sue;
    //discard the upcard
    Player * jenn = Player_factory("Jenn", "Simple");
    jenn->add_card(jack_spades);
    jenn->add_card(jack_clubs);
    jenn->add_card(ten_clubs);
    jenn->add_card(queen_clubs);
    jenn->add_card(king_clubs);
    jenn->add_and_discard(nine_clubs);
    Card card_led5_1 = jenn->lead_card(Card::SUIT_CLUBS);
    Card card_led5_2 = jenn->lead_card(Card::SUIT_CLUBS);
    Card card_led5_3 = jenn->lead_card(Card::SUIT_CLUBS);
    Card card_led5_4 = jenn->lead_card(Card::SUIT_CLUBS);
    Card card_led5_5 = jenn->lead_card(Card::SUIT_CLUBS);
    ASSERT_EQUAL(card_led5_1, jack_clubs);
    ASSERT_EQUAL(card_led5_2, jack_spades);
    ASSERT_EQUAL(card_led5_3, king_clubs);
    ASSERT_EQUAL(card_led5_4, queen_clubs);
    ASSERT_EQUAL(card_led5_5, ten_clubs);
    delete jenn;
}

TEST(test_simple_player_play_card) {
    Card nine_diamonds(Card::RANK_NINE, Card::SUIT_DIAMONDS);
    Card ten_diamonds(Card::RANK_TEN, Card::SUIT_DIAMONDS);
    Card jack_diamonds(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card queen_diamonds(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
    Card king_diamonds(Card::RANK_KING, Card::SUIT_DIAMONDS);
    Card ace_diamonds(Card::RANK_ACE, Card::SUIT_DIAMONDS);
    Card nine_clubs(Card::RANK_NINE, Card::SUIT_CLUBS);
    Card ten_clubs(Card::RANK_TEN, Card::SUIT_CLUBS);
    Card jack_clubs(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card queen_clubs(Card::RANK_QUEEN, Card::SUIT_CLUBS);
    Card king_clubs(Card::RANK_KING, Card::SUIT_CLUBS);
    Card ace_clubs(Card::RANK_ACE, Card::SUIT_CLUBS);
    Card nine_hearts(Card::RANK_NINE, Card::SUIT_HEARTS);
    Card ten_hearts(Card::RANK_TEN, Card::SUIT_HEARTS);
    Card jack_hearts(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card queen_hearts(Card::RANK_QUEEN, Card::SUIT_HEARTS);
    Card king_hearts(Card::RANK_KING, Card::SUIT_HEARTS);
    Card ace_hearts(Card::RANK_ACE, Card::SUIT_HEARTS);
    Card nine_spades(Card::RANK_NINE, Card::SUIT_SPADES);
    Card ten_spades(Card::RANK_TEN, Card::SUIT_SPADES);
    Card jack_spades(Card::RANK_JACK, Card::SUIT_SPADES);
    Card queen_spades(Card::RANK_QUEEN, Card::SUIT_SPADES);
    Card king_spades(Card::RANK_KING, Card::SUIT_SPADES);
    Card ace_spades(Card::RANK_ACE, Card::SUIT_SPADES);

  Player * bob = Player_factory("Bob", "Simple");
  add_cards(bob);
  string trump = Card::SUIT_HEARTS;
  Card card_played = bob->play_card(nine_diamonds, trump); //Diamonds is led
  Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
  ASSERT_EQUAL(card_played, two_spades);
  delete bob;
    
    
    
    Player * rae = Player_factory("Rae", "Simple");
    rae->add_card(nine_diamonds);
    rae->add_card(jack_hearts);
    rae->add_card(ten_clubs);
    rae->add_card(ten_hearts);
    rae->add_card(king_clubs);
    //able to follow suit, right bower
    Card card_played1_1 = rae->play_card(nine_hearts, Card::SUIT_HEARTS);
    ASSERT_EQUAL(card_played1_1, jack_hearts);
    //able to follow suit, not trump
    Card card_played1_2 = rae->play_card(nine_clubs, Card::SUIT_HEARTS);
    ASSERT_EQUAL(card_played1_2, king_clubs);
    //see if the card is deleted
    Card card_played1_3 = rae->play_card(nine_hearts, Card::SUIT_HEARTS);
    ASSERT_EQUAL(card_played1_3, ten_hearts);
    Card card_played1_4 = rae->play_card(nine_hearts, Card::SUIT_HEARTS);
    ASSERT_EQUAL(card_played1_4, nine_diamonds);
    Card card_played1_5 = rae->play_card(nine_hearts, Card::SUIT_HEARTS);
    ASSERT_EQUAL(card_played1_5, ten_clubs);
    delete rae;
    
    Player * emily = Player_factory("Emily", "Simple");
    emily->add_card(nine_diamonds);
    emily->add_card(jack_hearts);
    emily->add_card(ten_spades);
    emily->add_card(ten_hearts);
    emily->add_card(king_clubs);
    Card card_played2_1 = emily->play_card(ten_diamonds, Card::SUIT_HEARTS);
    ASSERT_EQUAL(card_played2_1, nine_diamonds);
    Card card_played2_2 = emily->play_card(ten_diamonds, Card::SUIT_HEARTS);
    ASSERT_EQUAL(card_played2_2, ten_spades);
    Card card_played2_3 = emily->play_card(ten_diamonds, Card::SUIT_HEARTS);
    ASSERT_EQUAL(card_played2_3, king_clubs);
    Card card_played2_4 = emily->play_card(ten_diamonds, Card::SUIT_HEARTS);
    ASSERT_EQUAL(card_played2_4, ten_hearts);
    Card card_played2_5 = emily->play_card(ten_diamonds, Card::SUIT_HEARTS);
    ASSERT_EQUAL(card_played2_5, jack_hearts);
    delete emily;
    
    Player * sue = Player_factory("Sue", "Simple");
    sue->add_card(nine_diamonds);
    sue->add_card(jack_hearts);
    sue->add_card(king_spades);
    sue->add_card(king_hearts);
    sue->add_card(king_clubs);
    Card card_played3_1 = sue->play_card(queen_diamonds, Card::SUIT_DIAMONDS);
    ASSERT_EQUAL(card_played3_1, jack_hearts);
    Card card_played3_2 = sue->play_card(queen_diamonds, Card::SUIT_DIAMONDS);
    ASSERT_EQUAL(card_played3_2, nine_diamonds);
    Card card_played3_3 = sue->play_card(queen_diamonds, Card::SUIT_DIAMONDS);
    ASSERT_EQUAL(card_played3_3, king_spades);
    Card card_played3_4 = sue->play_card(queen_diamonds, Card::SUIT_DIAMONDS);
    ASSERT_EQUAL(card_played3_4, king_hearts);
    Card card_played3_5 = sue->play_card(queen_diamonds, Card::SUIT_DIAMONDS);
    ASSERT_EQUAL(card_played3_5, king_clubs);
    delete sue;
    
    Player * john = Player_factory("John", "Simple");
    john->add_card(jack_hearts);
    john->add_card(jack_clubs);
    john->add_card(ten_clubs);
    john->add_card(queen_clubs);
    john->add_card(king_clubs);
    Card card_played4_1 = john->play_card(jack_spades, Card::SUIT_CLUBS);
    ASSERT_EQUAL(card_played4_1, jack_clubs);
    Card card_played4_2 = john->play_card(jack_spades, Card::SUIT_CLUBS);
    ASSERT_EQUAL(card_played4_2, king_clubs);
    Card card_played4_3 = john->play_card(jack_spades, Card::SUIT_CLUBS);
    ASSERT_EQUAL(card_played4_3, queen_clubs);
    Card card_played4_4 = john->play_card(jack_spades, Card::SUIT_CLUBS);
    ASSERT_EQUAL(card_played4_4, ten_clubs);
    Card card_played4_5 = john->play_card(jack_spades, Card::SUIT_CLUBS);
    ASSERT_EQUAL(card_played4_5, jack_hearts);
    delete john;
}


TEST_MAIN()
