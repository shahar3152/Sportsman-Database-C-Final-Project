#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Line_len 256
#define Max_len 80
#define Max_Fname 10
#define Max_Lname 15

//Event struct
typedef struct {
    char* p2title;
    char* p2location;
    unsigned int year;
} event;

//Sportsman struct
typedef struct {
    unsigned int id;
    char Fname[Max_Fname];
    char Lname[Max_Lname];
    char* p2club;
    unsigned int gen;
    event* p2events;
    int Nevents;
} sportsman;

//Get a string
//Delete the space at the start of a string
void deleteSpace(char* str)
{
    int i = 0;
    if (*str != ' ')
    {
        return;
    }
    while (str[i] == ' ')
    {
        i++;

    }
    strcpy(str, str + i);
}

//Get file's name  
//Return the number of lines in the file
int countLines(const char* fileName)
{
    int count = 0;
    char c = 'A';
    FILE* file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Unable to open file\n");
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
        printf("Unable to open file\n");
        return;
    }

    *size = countLines(sportsmanFileName); // Count the number of lines in SportsmanData

    // Allocate memory for the sportsman array
    if (*sportsmanArr == NULL)
    {
        printf("Failed to allocate memory\n");
        return;
    }

    // ignore first line
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) // Reads a line from the sportsman file and stores it in the line array while making sure it doesn't read more characters than can fit in it
    {
        sportsman s;
        char club[Max_len];
        sscanf(line, "%d;%[^;];%[^;];%[^;];%d", &s.id, s.Fname, s.Lname, club, &s.gen); // Reads from the line string according to the sportsman file format
        deleteSpace(s.Fname);
        deleteSpace(s.Lname);
        deleteSpace(club);
        // Allocates memory for the new club's name and then copies the contents of club into it because 
        // club is a local variable and its memory will be freed once the function ends.
        s.p2club = _strdup(club);
        s.Nevents = 0;
        s.p2events = NULL;
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
        printf("Unable to open file\n");
        return;
    }

    // Reads a line from the event file and stores it in the line array while making sure it doesn't read more characters than can fit in it
    fgets(line, sizeof(line), file);
    while (fgets(line, sizeof(line), file))
    {
        int id = 0, year = 0;
        char eventName[Max_len], location[Max_len];
        sscanf(line, "%d,%[^,],%[^,],%d", &id, eventName, location, &year); // Reads from the line string according to the events file format
        deleteSpace(eventName);
        deleteSpace(location);

        for (int i = 0; i < size; i++)
        {
            if (sportsmanArr[i].id == id)
            {
                if (sportsmanArr[i].p2events == NULL)
                {
                    sportsmanArr[i].p2events = (event*)malloc((sportsmanArr[i].Nevents + 1) * sizeof(event));;
                }
                else
                {
                    sportsmanArr[i].p2events = (event*)realloc(sportsmanArr[i].p2events, (sportsmanArr[i].Nevents + 1) * sizeof(event));
                }

                if (sportsmanArr[i].p2events == NULL)
                {
                    printf("Failed to allocate memory\n");
                    return;
                }

                event e;
                e.p2title = _strdup(eventName); // Allocates memory for the new event's name and then copies the contents of eventName into it because its a local variable and its memory will be freed once the function ends
                e.p2location = _strdup(location); // Allocates memory for the new location's name and then copies the contents of location into it because its a local variable and its memory will be freed once the function ends
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
int addSportsman(sportsman** sportsMan, int* size)
{
    sportsman s;
    printf("Enter the sportsman's id: ");
    scanf("%d", &s.id);
    for (int i = 0; i < *size; i++) //Checks if the sportsman id already exist in the array
    {
        if ((*sportsMan)[i].id == s.id)
        {
            printf("This sportsman already exists\n");
            return 0;
        }
    }
    getchar();
    printf("Enter the sportsman's first name: ");

    fgets(s.Fname, Max_Fname, stdin);
    s.Fname[strlen(s.Fname) - 1] = '\0';

    printf("Enter the sportsman's last name: ");
    fgets(s.Lname, Max_Lname, stdin);
    s.Lname[strlen(s.Lname) - 1] = '\0';

    printf("Enter the sportsman's club: ");
    char club[Max_len];
    fgets(club, Max_len, stdin);
    s.p2club = _strdup(club);

    printf("Enter the sportsman's gender (0 for male, 1 for female): ");
    scanf("%d", &s.gen);
    getchar();

    s.p2events = NULL;
    s.Nevents = 0;
    *sportsMan = (sportsman*)realloc(*sportsMan, (*size + 1) * sizeof(sportsman));
    if (*sportsMan == NULL)
    {
        printf("Failed to allocate memory\n");
        return 0;
    }
    (*sportsMan)[*size] = s; //Store's the sportsman data in the array
    (*size)++;
    return 1;
}

//Get id of a sportsman, array of sportsman and its size
//Adds event to the sportsman's events- return 1 if its added the event and 0 if its not
int addEvent(const int id, sportsman* sportsMan, const int size)
{
    for (int i = 0; i < size; i++)
    {
        if (sportsMan[i].id == id) //Checks if the sportsman id matches
        {
            event e;
            printf("Enter the event's title: ");
            char title[Max_len];
            scanf("%s", title);
            e.p2title = _strdup(title); //Allocates memory for the new event's name and then copies the contents of eventName into it because its a local variable and its memory will be freed once the function ends

            printf("Enter the event's location: ");
            char location[Max_len];
            scanf("%s", location);
            e.p2location = _strdup(location); //Allocates memory for the new location's name and then copies the contents of location into it because its a local variable and its memory will be freed once the function ends

            printf("Enter the event's year: ");
            scanf("%d", &e.year);

            for (int j = 0; j < sportsMan[i].Nevents; j++)
            {
                if (strcmp(sportsMan[i].p2events[j].p2title, e.p2title) == 0 && strcmp(sportsMan[i].p2events[j].p2location, e.p2location) == 0 && sportsMan[i].p2events[j].year == e.year) //Checks if the event already exist in the sportsman event list
                {
                    printf("The event already exists in the sportsman's event list\n\n");
                    return 0;
                }
            }

            sportsMan[i].p2events = (event*)realloc(sportsMan[i].p2events, (sportsMan[i].Nevents + 1) * sizeof(event));
            if (sportsMan[i].p2events == NULL)
            {
                printf("Failed to allocate memory\n");
                return 0;
            }
            sportsMan[i].p2events[sportsMan[i].Nevents] = e; //Store's the event in the sportsman event list
            sportsMan[i].Nevents++;

            return 1;
        }
    }
    printf("Sportsman does not exist.\n\n");
    return 0;
}

//Get the last name of a sportsman, array of sportsman and its size
//Print events name and their year that the sportsman participated in them
int printEvents(const char* lastName, sportsman* sportsMan, const int size)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(sportsMan[i].Lname, lastName) == 0) //Checks if the sportsman last name exists
        {
            if (sportsMan[i].Nevents == 0)
            {
                printf("The sportsman event list is empty.\n");
                return 0;
            }
            else
            {
                for (int j = 0; j < sportsMan[i].Nevents; j++) //Prints the sportsman list of events
                {
                    printf("Event Name: %s, Year: %d\n", sportsMan[i].p2events[j].p2title, sportsMan[i].p2events[j].year);
                }
                printf("\n");
                return 1;
            }
        }
    }
    printf("Sportsman does not exist.\n");
    return -1;
}

//Gets an event name 'E', a year 'Y' and an array of sportsman
//Returns the number of sportsman that participated in the 'E' event at the 'Y' year
int countEvent(const char* E, const int Y, const sportsman* sportsMan, const int size)
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < sportsMan[i].Nevents; j++)
        {
            if (strcmp(sportsMan[i].p2events[j].p2title, E) == 0 && sportsMan[i].p2events[j].year == Y) //Checks if a sportsman participated in the 'E' event at the 'Y' year
                count++;
        }
    }
    return count;
}

//Gets an array of sportsman and it size
//Prints the club's name that his sportsman participated in the highest number of events in total and the number of events
void BestClub(const sportsman* sportsMan, const int size)
{
    int maxEvents = 0;
    char bestClub[Max_len];

    if (!size) {
        printf("no clubs are existing\n");
        printf("\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        int clubEvents = 0;
        for (int j = 0; j < size; j++) {
            if (strcmp(sportsMan[i].p2club, sportsMan[j].p2club) == 0) {
                clubEvents += sportsMan[j].Nevents;
            }
        }
        if (clubEvents > maxEvents) {
            maxEvents = clubEvents;
            strcpy(bestClub, sportsMan[i].p2club);
        }
    }
    printf("Best Club: %s, Number of Events: %d\n", bestClub, maxEvents);
    printf("\n");
}

//Gets id, array of sportsman and its size
//Print the events of a sportsman that participated with other sportsmans in the same event
int CheckSameEvent(const int id, const sportsman* sportsMan, const int size)
{
    sportsman* sportsmanI = NULL;
    for (int i = 0; i < size; i++)
    {
        if (sportsMan[i].id == id)
        {
            sportsmanI = &sportsMan[i];
        }
    }
    if (!sportsmanI)
    {
        printf("Sportsman does not exist");
        return 0;
    }

    int flag = 0;
    for (int i = 0; i < (*sportsmanI).Nevents; i++)
    {
        event sportmanEvent = (*sportsmanI).p2events[i];
        for (int j = 0; j < size; j++)
        {
            if (sportsMan[j].id == id) continue;

            for (int k = 0; k < sportsMan[j].Nevents; k++)
            {
                event otherEvent = sportsMan[j].p2events[k];
                if (!strcmp(sportmanEvent.p2title, otherEvent.p2title)
                    && !strcmp(sportmanEvent.p2location, otherEvent.p2location)
                    && sportmanEvent.year == otherEvent.year)
                {
                    flag = 1;
                    printf("Event name: %s, Year: %d, Sportsman name: %s %s\n", sportsMan[j].p2events[k].p2title, sportsMan[j].p2events[k].year, sportsMan[j].Fname, sportsMan[j].Lname);
                }
            }
        }
    }
    if (flag) return 1;

    printf("This sportsman did not participate in any event with other sportsmans.");
    return 0;
}

//Gets club name, a pointer of sportsman array and its size
//Print all events that sportsman in this club participated in ascending order by year
void printSortedEvents(const char* C, const sportsman* sportsMan, const int size)
{
    event* allEvents = NULL;
    int totalEvents = 0;
    for (int i = 0; i < size; i++)
    {
        if (strcmp(sportsMan[i].p2club, C) == 0) //check if the sportsman's club is the same as club C
        {
            for (int j = 0; j < sportsMan[i].Nevents; j++)
            {
                int flag = 0;
                for (int k = 0; k < totalEvents && !flag; k++)
                {
                    //check if the title of the events and the year is different- its not the same event
                    if (!strcmp(allEvents[k].p2title, sportsMan[i].p2events[j].p2title) &&
                        allEvents[k].year == sportsMan[i].p2events[j].year)
                    {
                        flag = 1;
                    }
                }

                if (!flag)
                {
                    totalEvents++;
                    allEvents = realloc(allEvents, (totalEvents) * sizeof(event));
                    if (allEvents == NULL)
                    {
                        printf("Failed to allocate memory\n");
                        return;
                    }

                    //entering the event to the array
                    allEvents[totalEvents - 1] = sportsMan[i].p2events[j];
                }
            }
        }
    }

    if (allEvents == NULL)
    {
        printf("The club doesn't exists\n");
        return;
    }

    // sort allEvents by year
    for (int i = 0; i < totalEvents - 1; i++)
    {
        for (int j = 0; j < totalEvents - i - 1; j++)
        {
            if (allEvents[j].year > allEvents[j + 1].year)
            {
                // Swap
                event temp = allEvents[j];
                allEvents[j] = allEvents[j + 1];
                allEvents[j + 1] = temp;
            }
        }
    }

    // Print the sorted events without duplicates
    printf("Sorted events:\n");
    for (int i = 0; i < totalEvents; i++)
    {
        printf("Event Name: %s, Year: %d\n", allEvents[i].p2title, allEvents[i].year);
    }
    free(allEvents);
}

//Gets pointer to a name the event, a year, pointer to an array of sportsman and its size
//Delete every event with a specific name (E) of a specific year (Y)
void deleteEvent(const char* E, const int Y, sportsman* sportsMan, const int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < sportsMan[i].Nevents; j++)
        {
            if (strcmp(sportsMan[i].p2events[j].p2title, E) == 0 && sportsMan[i].p2events[j].year == Y)
            {
                // Shift all elements to the left
                for (int k = j; k < sportsMan[i].Nevents - 1; k++)
                {
                    sportsMan[i].p2events[k] = sportsMan[i].p2events[k + 1];
                }
                sportsMan[i].Nevents--;
                sportsMan[i].p2events = realloc(sportsMan[i].p2events, sportsMan[i].Nevents * sizeof(event));
            }
        }
    }
}

//Gets 2 pointers to 2 names of clubs, a pointer of sportsman array and its size 
//Print into the file "Club.txt" the names of all the events 
//that both clubs sportsman took a part in without repeat any event
void NewClub(const char* C1, const char* C2, const sportsman* sportsMan, const int size)
{
    FILE* file = fopen("Club.txt", "w");
    if (file == NULL)
    {
        printf("Unable to open file\n");
        return;
    }

    event* addedEvents = malloc(size * sizeof(event));
    if (addedEvents == NULL)
    {
        printf("Failed to allocate memory\n");
        return;
    }

    int count = 0;

    //printing in the file all the names of the events that C1, C2 took a part in
    for (int i = 0; i < size; i++)
    {
        // Check if the sportsman belongs to either of the clubs
        if (strcmp(sportsMan[i].p2club, C1) == 0 || strcmp(sportsMan[i].p2club, C2) == 0)
        {
            for (int j = 0; j < sportsMan[i].Nevents; j++)
            {
                // Check if the event is already added
                int flag = -1;
                for (int k = 0; k < count; k++)
                {

                    if (strcmp(addedEvents[k].p2title, sportsMan[i].p2events[j].p2title) == 0 && addedEvents[k].year == sportsMan[i].p2events[j].year)
                    {
                        flag = k;
                        break;
                    }
                }
                // If event is not already added, add it to the file and addedEvents array
                if (flag == -1)
                {
                    fprintf(file, "%s,%d\n", sportsMan[i].p2events[j].p2title, sportsMan[i].p2events[j].year);
                    addedEvents[count] = sportsMan[i].p2events[j];
                    count++;
                }
            }
        }
    }
    fclose(file);
    free(addedEvents);

}

//Gets a pointer of sportsman array and its size
//Print the menu of the program and starts the function
void printMenu(sportsman* sportsMan, int size)
{
    int choice;
    do {
        //the menu
        printf("1. Add a sportsman.\n");
        printf("2. Add an event to a sportsman.\n");
        printf("3. Print a sportsman's events.\n");
        printf("4. Print the number of sportsman that participated in a specific event.\n");
        printf("5. Print the best club.\n");
        printf("6. Print the events of a sportsman that participated with other sportsmans in the same event.\n");
        printf("7. Print the events of a club in a sorted manner.\n");
        printf("8. Delete every event with a specific name of a specific year.\n");
        printf("9. Create a new club.\n");
        printf("0. Exit and save data.\n");
        printf("Enter your choice: ");;
        scanf("%d", &choice);

        switch (choice) 
        {
        //Activate the function that add a sportsman
        case 1:
        {
            if (addSportsman(&sportsMan, &size)) 
            {
                printf("Sportsman has been added to the list.\n\n");
            }
            break;
        }

        //Activate the function that add an event to a sportsman
        case 2:
        {
            int id;
            printf("Enter the sportsman's id: ");
            scanf("%d", &id);
            int addEventReturn= addEvent(id, sportsMan, size);
            if (addEventReturn == 1)
            {
                printf("Event has been added to the sportsman event list.\n\n");
            }
            break;
        }

        //Activate the function that print a sportsman's events
        case 3:
        {
            char lastName[Max_Lname];
            printf("Enter the sportsman's last name: ");
            scanf("%s", lastName);
            printEvents(lastName, sportsMan, size);
            break;
        }

        //Activate the function that print the number of sportsman that participated in a specific event
        case 4:
        {
            char eventName[Max_len];
            int year;
            printf("Enter the event's name: ");
            getchar();
            fgets(eventName, Max_len, stdin);
            eventName[strlen(eventName) - 1] = '\0';

            printf("Enter the event's year: ");
            scanf("%d", &year);
            printf("In the event '%s' there were %d sportsman in the year %d.\n\n", eventName, countEvent(eventName, year, sportsMan, size), year);
            break;
        }

        //Activate the function that print the best club
        case 5:
        {
            BestClub(sportsMan, size);
            break;
        }

        //Activate the function that print the events of a sportsman that participated with other sportsmans in the same event
        case 6:
        {
            int id;
            printf("Enter the sportsman's id: ");
            scanf("%d", &id);

            CheckSameEvent(id, sportsMan, size);
            break;
        }

        //Activate the function that print the events of a club in a sorted manner
        case 7:
        {
            char clubName[Max_len];
            printf("Enter the club's name: ");
            getchar();
            fgets(clubName, Max_len, stdin);
            clubName[strlen(clubName) - 1] = '\0';
            printSortedEvents(clubName, sportsMan, size);
            break;
        }

        //Activate the function that delete every event with a specific name of a specific year
        case 8:
        {
            char eventName[Max_len];
            int year;
            printf("Enter the event's name: ");
            getchar();
            fgets(eventName, Max_len, stdin);
            eventName[strlen(eventName) - 1] = '\0';
            printf("Enter the event's year: ");
            scanf("%d", &year);

            deleteEvent(eventName, year, sportsMan, size);
            printf("The event has been deleted.\n");
            break;
        }

        //Activate the function that create a new club
        case 9:
        {
            char clubName1[Max_len];
            char clubName2[Max_len];
            printf("Enter the first club's name: ");
            getchar();
            fgets(clubName1, Max_len, stdin);
            clubName1[strlen(clubName1) - 1] = '\0';

            printf("Enter the second club's name: ");
            fgets(clubName2, Max_len, stdin);
            clubName2[strlen(clubName2) - 1] = '\0';

            NewClub(clubName1, clubName2, sportsMan, size);
            printf("New club has been created.\n\n");
            break;
        }

        //Exit and save data
        case 0:
        {
            FILE* file = fopen("SportsmanData.txt", "w");
            if (file == NULL)
            {
                printf("Unable to open file\n");
                return;
            }
            fprintf(file, "format:sportsman_id;first_name;last_name;club_name;gender");

            for (int i = 0; i < size; i++)
            {
                fprintf(file, "\n");
                fprintf(file, "%d;%s;%s;%s;%d", sportsMan[i].id, sportsMan[i].Fname, sportsMan[i].Lname, sportsMan[i].p2club, sportsMan[i].gen);
            }
            fclose(file);

            file = fopen("EventData.txt", "w");
            if (file == NULL) {
                printf("Unable to open file\n");
                return;
            }

            fprintf(file, "format:sportsman_id,event_name,location,year");

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < sportsMan[i].Nevents; j++)
                {
                    fprintf(file, "\n");
                    fprintf(file, "%d,%s,%s,%d", sportsMan[i].id, sportsMan[i].p2events[j].p2title, sportsMan[i].p2events[j].p2location, sportsMan[i].p2events[j].year);
                }
            }
            fclose(file);
            break;
        }

        //activates default
        default:
        {
            printf("Invalid choice.\n");
        }
        }
    } while (choice != 0);
}

void main()
{
    int size = countLines("SportsmanData.txt");

    sportsman* sportsMan = (sportsman*)malloc(size * sizeof(sportsman));
    if (sportsMan == NULL)
    {
        printf("Failed to allocate memory\n");
        return;
    }

    FromFile2Sportsman("SportsmanData.txt", &sportsMan, &size);
    FromFile2Events("EventData.txt", sportsMan, size);
    printMenu(sportsMan, size);

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
    return;
}