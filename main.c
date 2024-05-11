#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#define CLEAR_SCREEN system("cls")
#else
#define CLEAR_SCREEN system("clear")
#endif

#define MAX_CARDS 108
#define MAX_HAND_SIZE 20
#define MAX_PLAYERS 4
#define NUM_COLORS 4
#define NUM_VALUES 15

typedef struct {
    char color[10];
    char number[15];
} Card;

typedef struct {
    char name[20];
    Card hand[MAX_HAND_SIZE];
    int hand_size;
    int isHuman;
} Player;

// Function Prototypes
void initializeDeck(Card* deck);
void shuffleDeck(Card* deck);
void initializePlayers(Player* players, int numPlayers);
void dealInitialCards(Player* players, int numPlayers, Card* deck, int* deckIndex);
void printHand(Player player);
int canPlay(Card topCard, Card handCard);
void playGame(Player* players, int numPlayers, Card* deck);
void pressEnterToContinue();
void displayInstructions();
Card drawCard(Card* deck, int* deckIndex);
int handleSpecialCard(Card* topCard, Player* players, int currentPlayer, int* direction, int numPlayers, Card* deck, int* deckIndex);

void initializeDeck(Card* deck) {
    char* colors[] = {"Red", "Yellow", "Green", "Blue"};
    char* numbers[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "Skip", "Reverse", "Draw Two", "Wild", "Wild Draw Four"};
    int i, j, k = 0, count;

    for (i = 0; i < NUM_COLORS; i++) {
        for (j = 0; j < 13; j++) {
            count = (j == 0) ? 1 : 2;
            while (count--) {
                strcpy(deck[k].color, colors[i]);
                strcpy(deck[k].number, numbers[j]);
                k++;
            }
        }
    }

    for (j = 13; j < NUM_VALUES; j++) {
        count = 4;
        while (count--) {
            strcpy(deck[k].color, "Black");
            strcpy(deck[k].number, numbers[j]);
            k++;
        }
    }

    shuffleDeck(deck);
}

void shuffleDeck(Card* deck) {
    srand(time(NULL));
    for (int i = 0; i < MAX_CARDS; i++) {
        int r = rand() % MAX_CARDS;
        Card temp = deck[i];
        deck[i] = deck[r];
        deck[r] = temp;
    }
}

void initializePlayers(Player* players, int numPlayers) {
    for (int i = 0; i < numPlayers; i++) {
        sprintf(players[i].name, "Player %d", i + 1);
        players[i].hand_size = 0;
        players[i].isHuman = (i == 0) ? 1 : 0;
    }
}

void dealInitialCards(Player* players, int numPlayers, Card* deck, int* deckIndex) {
    for (int i = 0; i < numPlayers; i++) {
        for (int j = 0; j < 7; j++) {
            players[i].hand[players[i].hand_size++] = deck[(*deckIndex)++];
        }
    }
}

void printHand(Player player) {
    printf("%s's Hand (%d cards): ", player.name, player.hand_size);
    for (int i = 0; i < player.hand_size; i++) {
        printf("[%s %s] ", player.hand[i].color, player.hand[i].number);
    }
    printf("\n");
}

int canPlay(Card topCard, Card handCard) {
    return (strcmp(topCard.color, handCard.color) == 0 || strcmp(topCard.number, handCard.number) == 0 ||
            strcmp(handCard.number, "Wild") == 0 || strcmp(handCard.number, "Wild Draw Four") == 0);
}

Card drawCard(Card* deck, int* deckIndex) {
    if (*deckIndex < MAX_CARDS) {
        return deck[(*deckIndex)++];
    } else {
        Card emptyCard = {"None", "None"};
        return emptyCard;
    }
}

int handleSpecialCard(Card* topCard, Player* players, int currentPlayer, int* direction, int numPlayers, Card* deck, int* deckIndex) {
    int nextPlayer = (currentPlayer + *direction + numPlayers) % numPlayers;
    if (strcmp(topCard->number, "Skip") == 0) {
        nextPlayer = (nextPlayer + *direction + numPlayers) % numPlayers;
    } else if (strcmp(topCard->number, "Reverse") == 0) {
        *direction *= -1;
        nextPlayer = (currentPlayer + *direction + numPlayers) % numPlayers;
    } else if (strcmp(topCard->number, "Draw Two") == 0) {
        for (int i = 0; i < 2; i++) {
            players[nextPlayer].hand[players[nextPlayer].hand_size++] = drawCard(deck, deckIndex);
        }
    } else if (strcmp(topCard->number, "Wild Draw Four") == 0) {
        for (int i = 0; i < 4; i++) {
            players[nextPlayer].hand[players[nextPlayer].hand_size++] = drawCard(deck, deckIndex);
        }
    }
    if (strcmp(topCard->number, "Wild") == 0 || strcmp(topCard->number, "Wild Draw Four") == 0) {
        if (players[currentPlayer].isHuman) {
            printf("Choose a color (Red, Yellow, Green, Blue): ");
            scanf("%9s", topCard->color);  // Use %9s to avoid buffer overflow
        } else {
            strcpy(topCard->color, "Red"); // Simple AI always chooses Red
        }
    }
    return nextPlayer;
}

void playGame(Player* players, int numPlayers, Card* deck) {
    int currentPlayer = 0;
    int deckIndex = 0;
    int direction = 1;  // 1 for clockwise, -1 for counterclockwise
    int gameOver = 0;
    Card topCard = deck[deckIndex++];  // Start the game with the first card from the deck

    dealInitialCards(players, numPlayers, deck, &deckIndex);

    while (!gameOver) {
        CLEAR_SCREEN;
        printf("Current Top Card: [%s %s]\n\n", topCard.color, topCard.number);
        for (int i = 0; i < numPlayers; i++) {
            printHand(players[i]);
        }

        if (players[currentPlayer].isHuman) {
            printf("Your turn, %s:\n", players[currentPlayer].name);
            printHand(players[currentPlayer]);

            int played = 0;
            while (!played) {
                printf("Enter the index of the card to play (1-%d) or 0 to draw a card: ", players[currentPlayer].hand_size);
                int choice;
                scanf("%d", &choice);

                if (choice == 0) {
                    Card newCard = drawCard(deck, &deckIndex);
                    if (strcmp(newCard.color, "None") != 0) {  // Check if the deck is not empty
                        players[currentPlayer].hand[players[currentPlayer].hand_size++] = newCard;
                        printf("Drew: [%s %s]\n", newCard.color, newCard.number);
                        if (canPlay(topCard, newCard)) {
                            printf("You can play the drawn card. Do you want to play it? (1 for yes, 0 for no): ");
                            int playDrawn;
                            scanf("%d", &playDrawn);
                            if (playDrawn) {
                                topCard = newCard;
                                players[currentPlayer].hand_size--;  // Remove the last card which was just played
                                played = 1;
                            }
                        }
                    } else {
                        printf("No more cards in the deck.\n");
                    }
                } else if (choice > 0 && choice <= players[currentPlayer].hand_size) {
                    Card chosenCard = players[currentPlayer].hand[choice - 1];  // Adjust index for 0-based array
                    if (canPlay(topCard, chosenCard)) {
                        topCard = chosenCard;
                        for (int i = choice - 1; i < players[currentPlayer].hand_size - 1; i++) {
                            players[currentPlayer].hand[i] = players[currentPlayer].hand[i + 1];
                        }
                        players[currentPlayer].hand_size--;
                        played = 1;
                    } else {
                        printf("Cannot play that card. Please try again.\n");
                    }
                } else {
                    printf("Invalid choice. Please try again.\n");
                }
            }
        } else {  // AI turn
            int played = 0;
            for (int i = 0; i < players[currentPlayer].hand_size && !played; i++) {
                if (canPlay(topCard, players[currentPlayer].hand[i])) {
                    topCard = players[currentPlayer].hand[i];
                    for (int j = i; j < players[currentPlayer].hand_size - 1; j++) {
                        players[currentPlayer].hand[j] = players[currentPlayer].hand[j + 1];
                    }
                    players[currentPlayer].hand_size--;
                    played = 1;
                }
            }
            if (!played) {
                Card newCard = drawCard(deck, &deckIndex);
                if (strcmp(newCard.color, "None") != 0) {  // Check if the deck is not empty
                    players[currentPlayer].hand[players[currentPlayer].hand_size++] = newCard;
                    if (canPlay(topCard, newCard)) {
                        topCard = newCard;
                        players[currentPlayer].hand_size--;  // Remove the last card which was just played
                        played = 1;
                    }
                } else {
                    printf("AI Player %d cannot play and no more cards to draw.\n", currentPlayer + 1);
                }
            }
        }

        // Move to the next player according to the current direction
        currentPlayer = handleSpecialCard(&topCard, players, currentPlayer, &direction, numPlayers, deck, &deckIndex);

        // Check if current player has won
        if (players[currentPlayer].hand_size == 0) {
            printf("%s wins the game!\n", players[currentPlayer].name);
            gameOver = 1;
        }
        pressEnterToContinue();
    }
}

void pressEnterToContinue() {
    printf("Press Enter to continue...\n");
    while (getchar() != '\n');  // Clear the input buffer
}

void displayInstructions() {
    printf("Welcome to UNO! Play cards matching the top card's color or number. Special cards change the game flow.\n");
}

int main() {
    Card deck[MAX_CARDS];
    Player players[MAX_PLAYERS];
    int numPlayers;

    printf("=== WELCOME TO UNO ===\n");
    printf("Enter the number of players (2 to 4): ");
    scanf("%d", &numPlayers);
    getchar();  // Consume newline character after number input

    if (numPlayers < 2 || numPlayers > 4) {
        printf("Invalid number of players. Game supports 2 to 4 players only.\n");
        return 0;
    }

    initializePlayers(players, numPlayers);
    initializeDeck(deck);
    displayInstructions();
    playGame(players, numPlayers, deck);
    return 0;
}
