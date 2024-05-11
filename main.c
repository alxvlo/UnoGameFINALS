//new edit

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PLAYERS 4
#define MAX_CARDS 108
#define MAX_HAND_SIZE 20

typedef struct {
    char color[10];
    char number[3]; // Including NULL terminator
} Card;

typedef struct {
    char name[20];
    Card hand[MAX_HAND_SIZE];
    int hand_size;
} Player;

// Function to initialize the deck
void initializeDeck(Card* deck) {
    char* colors[] = {"Red", "Yellow", "Green", "Blue"};
    char* numbers[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "Skip", "Reverse", "Draw Two", "Wild", "Wild Draw Four"};
    int i, j, k = 0;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 10; j++) {
            strcpy(deck[k].color, colors[i]);
            strcpy(deck[k].number, numbers[j]);
            k++;
        }
        for (j = 10; j < 15; j++) {
            strcpy(deck[k].color, colors[i]);
            strcpy(deck[k].number, numbers[j]);
            k++;
        }
    }

    // Shuffling the deck
    srand(time(NULL));
    for (i = 0; i < MAX_CARDS; i++) {
        int r = rand() % MAX_CARDS;
        Card temp = deck[i];
        deck[i] = deck[r];
        deck[r] = temp;
    }
}

// Function to initialize players
void initializePlayers(Player* players, int numPlayers) {
    int i;
    for (i = 0; i < numPlayers; i++) {
        sprintf(players[i].name, "Player %d", i + 1);
        players[i].hand_size = 0;
    }
}

// Function to deal initial cards to players
void dealInitialCards(Player* players, int numPlayers, Card* deck, int* deckIndex) {
    int i, j;
    for (i = 0; i < numPlayers; i++) {
        for (j = 0; j < 7; j++) {
            players[i].hand[j] = deck[*deckIndex];
            (*deckIndex)++;
        }
        players[i].hand_size = 7;
    }
}

// Function to print player's hand
void printHand(Player player, int isPlayer) {
    int i;
    printf("%s's Hand: ", player.name);
    if (isPlayer) {
        for (i = 0; i < player.hand_size; i++) {
            printf("[%s %s | %s], ", player.hand[i].color, player.hand[i].number, player.hand[i].color);
        }
    } else {
        printf("[ %d LEFT]", player.hand_size);
    }
    printf("\n");
}

// Function to start the game
void startGame(Player* players, int numPlayers, Card* deck) {
    int deckIndex = 0;
    initializePlayers(players, numPlayers);
    dealInitialCards(players, numPlayers, deck, &deckIndex);
}

// Function to display instructions
void displayInstructions() {
    printf("=== INSTRUCTIONS ===\n");
    printf("Uno is a card game where the objective is to be the first player to get rid of all your cards.\n");
    printf("Each player is dealt 7 cards at the beginning of the game.\n");
    printf("Players take turns to play a card from their hand that matches either the color or the number of the top card on the discard pile.\n");
    printf("If a player doesn't have a matching card, they must draw a card from the draw pile. If that card can be played, they may play it immediately.\n");
    printf("Special cards:\n");
    printf("    - Skip: Skips the next player's turn.\n");
    printf("    - Reverse: Reverses the direction of play.\n");
    printf("    - Draw Two: Forces the next player to draw two cards and skips their turn.\n");
    printf("    - Wild: Can be played on any card, and the player can choose the color to continue.\n");
    printf("    - Wild Draw Four: Can be played on any card, the player can choose the color to continue, and the next player must draw four cards and skip their turn.\n");
    printf("The first player to get rid of all their cards wins the game!\n");
}

// Function to determine if a card can be played
int canPlay(Card topCard, Card handCard) {
    // Wild cards can always be played
    if (strcmp(handCard.number, "Wild") == 0 || strcmp(handCard.number, "Wild Draw Four") == 0) {
        return 1;
    }
    // Cards with matching color, number, or symbol can be played
    if (strcmp(topCard.color, handCard.color) == 0 || strcmp(topCard.number, handCard.number) == 0) {
        return 1;
    }
    return 0;
}

// Function to simulate gameplay
void playGame(Player* players, int numPlayers, Card* deck) {
    int currentPlayer = rand() % numPlayers;
    printf("=== GAME START ===\n");
    printf("Play Order:\n");
    for (int i = 0; i < numPlayers; i++) {
        printf("%s ", players[(currentPlayer + i) % numPlayers].name);
    }
    printf("\n\n");
    // Draw top card
    Card topCard = deck[0];
    printf("Top Card: [%s %s]\n\n", topCard.color, topCard.number);
    // Remove top card from deck
    for (int i = 0; i < MAX_CARDS - 1; i++) {
        deck[i] = deck[i + 1];
    }
    // Deal initial cards
    dealInitialCards(players, numPlayers, deck, &numPlayers);
    // Print initial hands
    for (int i = 0; i < numPlayers; i++) {
        printHand(players[i], i == 0);
    }
    // Game loop
    while (1) {
        // Player's turn
        printf("\n%s's turn:\n", players[currentPlayer].name);
        // Player selects a card to play or draws from the deck
        // Game logic goes here
        // For now, let's break out of the loop after one iteration
        break;
    }
}

int main() {
    Card deck[MAX_CARDS];
    Player players[MAX_PLAYERS];
    int numPlayers, choice;
    char playerName[20];
    
    printf("=== WELCOME TO UNO ===\n");
    printf("Enter your name: ");
    scanf("%s", playerName);
    
    while (1) {
        printf("\nMAIN MENU:\n");
        printf("1. Start\n");
        printf("2. Instructions\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("How many bots do you want to play against? (1 to 3): ");
                scanf("%d", &numPlayers);
                
                if (numPlayers < 0 || numPlayers > 3) {
                    printf("Invalid number of players.\n");
                    break;
                }
                
                initializeDeck(deck);
                startGame(players, numPlayers + 1, deck);
                playGame(players, numPlayers + 1, deck);
                break;
            case 2:
                displayInstructions();
                break;
            case 3:
                printf("Exiting the game.\n");
                return 0;
            default:
                printf("Invalid choice. Please enter a number between 1 and 3.\n");
        }
    }

    return 0;
}
