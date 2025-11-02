#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Line_len 256
#define Max_len 80

// Event struct
typedef struct 
{
    char* p2title;
    char* p2location;
    unsigned int year;
} event;

// Sportsman struct
typedef struct 
{
    unsigned int id;
    char Fname[Max_len];
    char Lname[Max_len];
    char* p2club;
    unsigned int gen;
    event* p2events;
    int Nevents;
} sportsman;

// Function prototypes
int countLines(const char* fileName);
void FromFile2Sportsman(const char* sportsmanFileName, sportsman** sportsmanArr, int* size);
void FromFile2Events(const char* eventFileName, sportsman* sportsmanArr, const int size);
int addSportsman(sportsman** sportsMan, int* size);
int addEvent(const int id, sportsman* sportsMan, const int size);
int printEvents(const char* lastName, sportsman* sportsMan, const int size);
int countEvent(char* E, int Y, sportsman* sportsMan, int size);
void BestClub(sportsman* sportsMan, int size);
int CheckSameEvent(int id, sportsman* sportsMan, int size);
void printSortedEvents(char* C, sportsman* sportsMan, int size);
void deleteEvent(char* E, int Y, sportsman* sportsMan, int size);
void NewClub(char* C1, char* C2, sportsman* sportsMan, int size);
void printMenu(sportsman* sportsMan, int size);

void main()
{
    int size = countLines("SportsmanData.txt");
    sportsman* sportsMan = (sportsman*)malloc(size * sizeof(sportsman));
    if (sportsMan == NULL)
    {
        printf("Failed to allocate memory\n");
        return 1;
    }
    FromFile2Sportsman("SportsmanData.txt", &sportsMan, &size);
    FromFile2Events("EventData.txt", sportsMan, size);
    printMenu(sportsMan, size);

    // Free allocated memory
    for (int i = 0; i < size; i++)
    {
        free(sportsMan[i].p2club);
        for (int j = 0; j < sportsMan[i].Nevents; j++)
        {
            free(sportsMan[i].p2events[j].p2title);
            free(sportsMan[i].p2events[j].p2location);
        }
        free(sportsMan[i].p2events);
    }
    free(sportsMan);
}

//Get file's name  
//Return the number of lines in the file
int countLines(const char* fileName)
{
    int count = 0;
    char c;
    FILE* file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Unable to open file: %s\n", fileName);
        return -1;
    }

    while ((c = fgetc(file)) != EOF) // Reads the file character by character until it reaches the end of the file
    {
        if (c == '\n')
            count++;
    }
    fclose(file);
    return count;
}

//Get the file SportsmanData, array of sportsman and a its size
//Read the file and fill the array with information
void FromFile2Sportsman(const char* sportsmanFileName, sportsman** sportsmanArr, int* size)
{
    int i = 0;
    char line[Line_len];

    FILE* file = fopen(sportsmanFileName, "r");
    if (file == NULL)
    {
        printf("Unable to open file: %s\n", sportsmanFileName);
        return;
    }

    *size = countLines(sportsmanFileName); // Count the number of lines in SportsmanData
    *sportsmanArr = (sportsman*)malloc(*size * sizeof(sportsman)); // Allocate memory for the sportsman array
    if (*sportsmanArr == NULL)
    {
        printf("Failed to allocate memory\n");
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)) // Reads a line from the sportsman file and stores it in the line array while making sure it doesn't read more characters than can fit in it
    {
        sportsman s;
        char club[Max_len];
        sscanf(line, "%u;%[^;];%[^;];%[^;];%u", &s.id, s.Fname, s.Lname, club, &s.gen); // Reads from the line string according to the sportsman file format
        s.p2club = strdup(club); // Allocates memory for the new club's name and then copies the contents of club into it because club is a local variable and its memory will be freed once the function ends.
        (*sportsmanArr)[i] = s;  // Store's the sportsman data in the array
        i++;
    }
    fclose(file);
}

//Get the file EventData, array of sportsman and a its size
//Read the file and fill the array with information
void FromFile2Events(const char* eventFileName, sportsman* sportsmanArr, const int size)
{
    char line[Line_len];

    FILE* file = fopen(eventFileName, "r");
    if (file == NULL) {
        printf("Unable to open file: %s\n", eventFileName);
        return;
    }

    while (fgets(line, sizeof(line), file)) // Reads a line from the event file and stores it in the line array while making sure it doesn't read more characters than can fit in it
    {
        int id = 0, year = 0;
        char eventName[Max_len], location[Max_len];
        sscanf(line, "%d,%[^,],%[^,],%d", &id, eventName, location, &year); // Reads from the line string according to the events file format

        for (int i = 0; i < size; i++)
        {
            if (sportsmanArr[i].id == id)
            {
                sportsmanArr[i].p2events = (event*)realloc(sportsmanArr[i].p2events, (sportsmanArr[i].Nevents + 1) * sizeof(event));
                if (sportsmanArr[i].p2events == NULL)
                {
                    printf("Failed to allocate memory\n");
                    fclose(file);
                    return;
                }
                event e;
                e.p2title = strdup(eventName); // Allocates memory for the new event's name and then copies the contents of eventName into it because its a local variable and its memory will be freed once the function ends
                e.p2location = strdup(location); // Allocates memory for the new location's name and then copies the contents of location into it because its a local variable and its memory will be freed once the function ends
                e.year = year;
                sportsmanArr[i].p2events[sportsmanArr[i].Nevents] = e; // Adds the event 'e' to the end of the sportsman's event array
                sportsmanArr[i].Nevents++;
            }
        }
    }
    fclose(file);
}

//Get array of sportsman and its size
//Adds sportsman to the array- return 1 if its added the sportsman and 0 if its not
int addSportsman(sportsman * *sportsMan, int* size)
{
    sportsman s;
    printf("Enter the sportsman's id: ");
    scanf("%u", &s.id);

    for (int i = 0; i < *size; i++) // Checks if the sportsman id already exists in the array
    {
        if ((*sportsMan)[i].id == s.id)
        {
            printf("This sportsman already exists\n");
            return 0;
        }
    }
    printf("Enter the sportsman's first name: ");
    scanf("%80s", s.Fname); // Limit input to avoid buffer overflow

    printf("Enter the sportsman's last name: ");
    scanf("%80s", s.Lname); // Limit input to avoid buffer overflow

    printf("Enter the sportsman's club: ");
    char club[Max_len];
    scanf("%s", club);
    s.p2club = strdup(club);

    printf("Enter the sportsman's gender (0 for male, 1 for female): ");
    scanf("%u", &s.gen);

    s.p2events = NULL;
    s.Nevents = 0;
    *sportsMan = (sportsman*)realloc(*sportsMan, (*size + 1) * sizeof(sportsman));
    if (*sportsMan == NULL)
    {
        printf("Failed to allocate memory\n");
        return 0;
    }
    (*sportsMan)[*size] = s; // Store's the sportsman data in the array
    (*size)++;
    return 1;
}

//Get id of a sportsman, array of sportsman and its size
//Adds event to the sportsman's events- return 1 if its added the event and 0 if its not
int addEvent(const int id, sportsman* sportsMan, const int size)
{
    for (int i = 0; i < size; i++)
    {
        if (sportsMan[i].id == id) // Checks if the sportsman id matches
        {
            event e;
            printf("Enter the event's title: ");
            char title[Max_len];
            scanf("%s", title);
            e.p2title = strdup(title); // Allocates memory for the new event's name and then copies the contents of title into it

            printf("Enter the event's location: ");
            char location[Max_len];
            scanf("%s", location);
            e.p2location = strdup(location); // Allocates memory for the new location's name and then copies the contents of location into it

            printf("Enter the event's year: ");
            scanf("%u", &e.year);

            for (int j = 0; j < sportsMan[i].Nevents; j++)
            {
                if (strcmp(sportsMan[i].p2events[j].p2title, e.p2title) == 0 && strcmp(sportsMan[i].p2events[j].p2location, e.p2location) == 0 && sportsMan[i].p2events[j].year == e.year) // Checks if the event already exists in the sportsman's event list
                {
                    printf("The event already exists in the sportsman's event list\n");
                    return 0;
                }
            }

            sportsMan[i].p2events = (event*)realloc(sportsMan[i].p2events, (sportsMan[i].Nevents + 1) * sizeof(event));
            if (sportsMan[i].p2events == NULL)
            {
                printf("Failed to allocate memory\n");
                return 0;
            }
            sportsMan[i].p2events[sportsMan[i].Nevents] = e; // Store's the event in the sportsman event list
            sportsMan[i].Nevents++;

            return 1;
        }
    }
    printf("Sportsman does not exist.\n");
    return 0;
}

//Get the last name of a sportsman, array of sportsman and its size
//Print events name and their year that the sportsman participated in them
int printEvents(const char* lastName, sportsman* sportsMan, const int size)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(sportsMan[i].Lname, lastName) == 0) // Checks if the sportsman's last name exists
        {
            if (sportsMan[i].Nevents == 0)
            {
                printf("The sportsman event list is empty.\n");
                return 0;
            }
            else
            {
                for (int j = 0; j < sportsMan[i].Nevents; j++) // Prints the sportsman list of events
                {
                    printf("Event Name: %s, Year: %u\n", sportsMan[i].p2events[j].p2title, sportsMan[i].p2events[j].year);
                }
                return 1;
            }
        }
    }
    printf("Sportsman does not exist.\n");
    return -1;
}

//Gets an event name 'E', a year 'Y' and an arrey of sportsman
//Returns the number of sportsman that participated in the 'E' event at the 'Y' year
int countEvent(char* E, int Y, sportsman* sportsMan, int size)
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < sportsMan[i].Nevents; j++)
        {
            if (strcmp(sportsMan[i].p2events[j].p2title, E) == 0 && sportsMan[i].p2events[j].year == Y) // Checks if a sportsman participated in the 'E' event at the 'Y' year
                count++;
        }
    }
    return count;
}

//Gets an arrey of sportsman and it size
//Prints the club's name that his sportsman participated in the highest number of events in total and the number of events
void BestClub(sportsman* sportsMan, int size)
{
    int maxEvents = 0;
    char bestClub[Max_len] = "";
    for (int i = 0; i < size; i++)
    {
        if (sportsMan[i].Nevents > maxEvents)
        {
            maxEvents = sportsMan[i].Nevents;
            strcpy(bestClub, sportsMan[i].p2club);
        }
    }
    if (maxEvents > 0)
        printf("Best Club: %s, Number of Events: %d\n", bestClub, maxEvents);
    else
        printf("No events found.\n");
}

//Print the events of a sportsman that participated with other sportsmans in the same event
int CheckSameEvent(int id, sportsman* sportsMan, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (sportsMan[i].id == id)
        {
            if (sportsMan[i].Nevents == 0)
                return 0;
            else
                return 1;
        }
    }
    return -1;
}

// Print events of a specified club in a sorted manner
void printSortedEvents(char* C, sportsman* sportsMan, int size)
{
    printf("Events of Club %s:\n", C);
    for (int i = 0; i < size; i++)
    {
        if (strcmp(sportsMan[i].p2club, C) == 0)
        {
            if (sportsMan[i].Nevents == 0)
            {
                printf("No events found for this club.\n");
                return;
            }
            else
            {
                // Sort the events by year before printing
                for (int j = 0; j < sportsMan[i].Nevents - 1; j++)
                {
                    for (int k = 0; k < sportsMan[i].Nevents - 1 - j; k++)
                    {
                        if (sportsMan[i].p2events[k].year > sportsMan[i].p2events[k + 1].year)
                        {
                            event temp = sportsMan[i].p2events[k];
                            sportsMan[i].p2events[k] = sportsMan[i].p2events[k + 1];
                            sportsMan[i].p2events[k + 1] = temp;
                        }
                    }
                }
                for (int j = 0; j < sportsMan[i].Nevents; j++)
                {
                    printf("Event Name: %s, Year: %u\n", sportsMan[i].p2events[j].p2title, sportsMan[i].p2events[j].year);
                }
                return;
            }
        }
    }
    printf("Club not found.\n");
}

// Delete an event of a sportsman
void deleteEvent(char* E, int Y, sportsman* sportsMan, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < sportsMan[i].Nevents; j++)
        {
            if (strcmp(sportsMan[i].p2events[j].p2title, E) == 0 && sportsMan[i].p2events[j].year == Y) // Checks if the event matches
            {
                // Free memory for event title and location
                free(sportsMan[i].p2events[j].p2title);
                free(sportsMan[i].p2events[j].p2location);

                // Shift remaining events to fill the gap
                for (int k = j; k < sportsMan[i].Nevents - 1; k++)
                {
                    sportsMan[i].p2events[k] = sportsMan[i].p2events[k + 1];
                }
                sportsMan[i].Nevents--; // Decrement the count of events
                printf("Event deleted successfully.\n");
                return;
            }
        }
    }
    printf("Event not found.\n");
}

// Change the club of all sportsmen from one club to another
void NewClub(char* C1, char* C2, sportsman* sportsMan, int size)
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if (strcmp(sportsMan[i].p2club, C1) == 0) // Checks if the sportsman belongs to club C1
        {
            sportsMan[i].p2club = strdup(C2); // Change the club to C2
            count++;
        }
    }
    if (count > 0)
        printf("Club changed successfully for %d sportsmen.\n", count);
    else
        printf("No sportsmen found in Club %s.\n", C1);
}

// Print the menu options
void printMenu(sportsman* sportsMan, int size)
{
    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Add a sportsman\n");
        printf("2. Add an event\n");
        printf("3. Print events of a sportsman\n");
        printf("4. Count the number of times an event occurred\n");
        printf("5. Find the best club\n");
        printf("6. Print events of a specified club in a sorted manner\n");
        printf("7. Delete an event of a sportsman\n");
        printf("8. Change the club of all sportsmen from one club to another\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addSportsman(&sportsMan, &size);
            break;
        case 2:
            printf("Enter the sportsman's id: ");
            int id;
            scanf("%d", &id);
            addEvent(id, sportsMan, size);
            break;
        case 3:
            printf("Enter the sportsman's last name: ");
            char lastName[Max_len];
            scanf("%s", lastName);
            printEvents(lastName, sportsMan, size);
            break;
        case 4:
            printf("Enter the event's name and year: ");
            char eventName[Max_len];
            int year;
            scanf("%s %d", eventName, &year);
            printf("Event '%s' occurred %d times.\n", eventName, countEvent(eventName, year, sportsMan, size));
            break;
        case 5:
            BestClub(sportsMan, size);
            break;
        case 6:
            printf("Enter the club name: ");
            char clubName[Max_len];
            scanf("%s", clubName);
            printSortedEvents(clubName, sportsMan, size);
            break;
        case 7:
            printf("Enter the event's name and year to delete: ");
            char eventToDelete[Max_len];
            int yearToDelete;
            scanf("%s %d", eventToDelete, &yearToDelete);
            deleteEvent(eventToDelete, yearToDelete, sportsMan, size);
            break;
        case 8:
            printf("Enter the name of the club you want to change: ");
            char oldClub[Max_len];
            scanf("%s", oldClub);
            printf("Enter the new name of the club: ");
            char newClub[Max_len];
            scanf("%s", newClub);
            NewClub(oldClub, newClub, sportsMan, size);
            break;
        case 9:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 9);
}
