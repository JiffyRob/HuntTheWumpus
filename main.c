#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define PIT_COUNT 3
#define BAT_COUNT 3

#define CAVE_EMPTY 0
#define CAVE_BAT 1
#define CAVE_PIT 2
#define CAVE_WUMPUS 3

#define ACTION_SHOOT 1
#define ACTION_MOVE 2

int get_action() {
    char *line = NULL;
    size_t size = 0;
    while (true) {
        printf("Do you wish to (s)hoot or (m)ove?\n");
        getline(&line, &size, stdin);
        switch (*line) {
            case 'm':
                free(line);
                return ACTION_MOVE;
            case 's':
                free(line);
                return ACTION_SHOOT;
            default:
                free(line);
                printf("Invalid. Try again.\n");
                break;
        }
    }
}

int get_cave(int adjacent1, int adjacent2, int adjacent3) {
    char *line = NULL;
    size_t size = 0;
    while (true) {
        printf("Which room do you wish to go to (%d, %d, or %d)\n", adjacent1, adjacent2, adjacent3);
        getline(&line, &size, stdin);
        char *ptr;
        int value = (int) strtol(line, &ptr, 10);
        if (value == adjacent1) {
            free(line);
            return adjacent1;
        } else if (value == adjacent2) {
            free(line);
            return adjacent2;
        } else if (value == adjacent3) {
            free(line);
            return adjacent3;
        } else {
            printf("Invalid answer.  Try again.\n");
        }
    }
}

int main() {
    const int connections[63] = {
        0, 0, 0,    // No cave 0
        6, 2, 5,    // Cave 1
        1, 8, 3,    // Cave 2
        2, 10, 4,   // Cave 3
        3, 12, 5,   // Cave 4
        1, 4, 14,   // Cave 5
        7, 1, 15,   // Cave 6
        16, 8, 6,   // Cave 7
        9, 2, 7,    // Cave 8
        17, 10, 8,  // Cave 9
        9, 11, 3,   // Cave 10
        10, 18, 12, // Cave 11
        4, 11, 13,  // Cave 12
        14, 12, 19, // Cave 13
        15, 5, 13,  // Cave 14
        16, 14, 20, // Cave 15
        17, 7, 20,  // Cave 16
        18, 9, 16,  // Cave 17
        11, 17, 19, // Cave 18
        20, 13, 18, // Cave 19
        16, 15, 19, // Cave 20
    };

    srand(time(NULL));
    
    int rooms[21] = {0};
    
    for (int i = 0; i < PIT_COUNT; i++) {
        int room = rand() % 20 + 1;
        if (!rooms[room]) {
            rooms[room] = CAVE_PIT;
        }
    }
    for (int i = 0; i < BAT_COUNT; i++) {
        int room = rand() % 20 + 1;
        if (!rooms[room]) {
            rooms[room] = CAVE_BAT;
        }
    }
    
    int wumpus_place = 0;
    
    while (wumpus_place == 0) {
        int room = rand() % 20 + 1;
        if (!rooms[room]) {
            rooms[room] = CAVE_WUMPUS;
            wumpus_place = room;
        }
    }
    
    int player_place = 0;
    
    while (player_place == 0) {
        int room = rand() % 20 + 1;
        if (!rooms[room]) {
            player_place = room;
        }
    }
    
    while (true) {
        printf("You are in room %d\n", player_place);
        switch (rooms[player_place]) {
            case CAVE_EMPTY:
                printf("The cave is empty.\n");
                int adjacent_rooms[3] = {0, 0, 0};
                for (int i = 0; i < 3; i++) {
                    int check_index = player_place * 3 + i;
                    int room_index = connections[check_index];
                    int room_value = rooms[room_index];
                    adjacent_rooms[i] = room_index;
                    switch (room_value) {
                        case CAVE_EMPTY:
                            break;
                        case CAVE_BAT:
                            printf("You hear a shriek.\n");
                            break;
                        case CAVE_WUMPUS:
                            printf("You smell a wumpus!\n");
                            break;
                        case CAVE_PIT:
                            printf("You feel a draft.\n");
                            break;
                        default:
                            printf("You smell a bug in the code.  Bad room number %d given.\n", room_value);
                            break;
                    }
                }
                int action = get_action();
                int next_cave;
                switch (action) {
                    case ACTION_MOVE:
                        next_cave = get_cave(
                                adjacent_rooms[0],
                                adjacent_rooms[1],
                                adjacent_rooms[2]);
                        player_place = next_cave;
                        break;
                    case ACTION_SHOOT:
                        next_cave = get_cave(adjacent_rooms[0], adjacent_rooms[1], adjacent_rooms[2]);;
                        if (rooms[next_cave] == CAVE_WUMPUS) {
                            printf("You hear a wail!  The wumpus is down.  YOU WIN!");
                            return 0;
                        } else {
                            printf("You missed, but the wumpus hears your shot.\n");
                            printf("Oh no! You got eaten by a wumpus!\n");
                            printf("GAME OVER");
                            return 0;
                        }
                    default:
                        printf("Oh no! You get eaten by an impossible code path!\n");
                        printf("Tell the developer that his action getter is borked.\n");
                        return 0;
                }
                break;
                
            case CAVE_BAT:
                printf("There is a bat colony.\n");
                player_place = rand() % 20 + 1;
                printf("A bat picks you up and drops you.\n");
                break;
            case CAVE_WUMPUS:
                printf("Oh no! You got eaten by a wumpus!\n");
                printf("GAME OVER\n");
                return 0;
            case CAVE_PIT:
                printf("Oh no! You fell into a bottomless pit!\n");
                printf("GAME OVER\n");
                return 0;
        }
    }
}