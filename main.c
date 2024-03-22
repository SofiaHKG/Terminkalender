#include <stdio.h>
#include <stdlib.h>

// Enumeration zur Definition des Typs eines Termins
enum appointmentType
{
    friends,        // Termin mit Freunden 'f'
    business,       // Geschaeftlicher Termin 'b'
    pause           // Pause 'p'
};

// Struktur um einen Termin (appointment) zu speichern
struct appointment
{
    enum appointmentType type;      // Enumeration Typ
    int time;                       // Integer Beginn
    int duration;                   // Integer Dauer
};

void errorMessage();
void printAppointments(struct appointment * schedule, int length);
void printInformation(struct appointment * schedule, int length);
struct appointment createAppointment();
void addAppointment(struct appointment * schedule, int * length);
void deleteAppointment(struct appointment * schedule, int * length);

int main()
{
    struct appointment schedule[10];
    int length = 0;
    char option;

    while(1) {
        printf("\nNeuen Termin (n), Termin loeschen (d), Termine auflisten (l), Informationen ausgeben (i), Programm beenden (x)");
        printf("\nAuswahl: ");
        scanf(" %c", &option);

        if (option == 'x')
            break;
        else {
            switch (option) {
            case 'n':
                addAppointment(schedule, &length);
                break;
            case 'd':
                deleteAppointment(schedule, &length);
                break;
            case 'l':
                if (length > 0)
                    printAppointments(schedule, length);
                break;
            case 'i':
                printInformation(schedule, length);
                break;
            default:
                errorMessage();
            }
        }
    }

    return 0;
}

// Funktion zum Ausgeben der Error-Message
void errorMessage()
{
    printf("\nUngueltige Eingabe!");
}

// Ausgeben der Termine mit ihrem Index, ihrem Typ, ihrem Beginn und ihrer Dauer
void printAppointments(struct appointment * schedule, int length)
{
    char * nameOf[] = { "Freunde", "Geschaeftlich", "Pause" };       // Namen der Termintypen

    for (int i = 0; i < length; i++) {
        printf("\n%d: %s: %d Uhr, Dauer %dh", i + 1, nameOf[schedule[i].type], schedule[i].time, schedule[i].duration);
    }
}

// Ausgeben von Informationen(Terminkollision, welche Termine abgesagt werden sollten, ungeplante Zeit, Anzahl der Termintypen)
void printInformation(struct appointment * schedule, int length)
{
    int collision = 0;

    // Kollision
    for (int i = 0; i < length - 1; i++) {
        if (schedule[i].time + schedule[i].duration > schedule[i + 1].time) {
            collision++;
            printf("\nEntweder Termin %d oder Termin %d sollte abgesagt werden.", i + 1, i + 2);
            break;
        }
    }

    // Keine Kollision
    if (!collision) {
        printf("\nDer Kalender hat keine Kollisionen!");
        for (int i = 0; i < length - 1; i++) {
            if (schedule[i].time + schedule[i].duration < schedule[i + 1].time) {
                int unplannedTime = schedule[i + 1].time - (schedule[i].time + schedule[i].duration);
                printf("\nZwischen den Terminen %d und %d liegen %dh ungeplante Zeit.", i + 1, i + 2, unplannedTime);
            }
        }
    }

    // Counter für Anzahl der Termintypen
    int businessCount = 0;
    int friendsCount = 0;
    int pauseCount = 0;

    for (int i = 0; i < length; i++) {
        switch (schedule[i].type) {
            case business:
                businessCount++;
                break;
            case friends:
                friendsCount++;
                break;
            case pause:
                pauseCount++;
                break;
        }
    }

    printf("\nSie haben %d Geschaeftstermin(e), %d Termin(e) mit Freunden und %d Pause(n) in Ihrem Kalender.", businessCount, friendsCount, pauseCount);
}

// Funktion zum Erstellen eines Termins
struct appointment createAppointment()
{
    struct appointment newAppointment;
    char typeInput;

    while(1) {
        printf("\nGib einen Typen ein: Freunde (f), Geschaeftlich (b), Pause (p): ");
        scanf(" %c", &typeInput);
        if (typeInput != 'f' && typeInput != 'b' && typeInput != 'p') {
            errorMessage();
            continue;
        } else {
            newAppointment.type = (typeInput == 'f') ? friends : (typeInput == 'b') ? business : pause;
            break;
        }
    }

    while(1) {
        printf("\nGib eine Uhrzeit ein (8-21): ");
        scanf("%d", &newAppointment.time);
        if (newAppointment.time < 8 || newAppointment.time > 21) {
            errorMessage();
            continue;
        } else {
            break;
        }
    }

    while(1) {
        int maximumDuration = 22 - newAppointment.time;
        printf("\nGib eine Dauer ein (1-%d): ", maximumDuration);
        scanf("%d", &newAppointment.duration);
        if (newAppointment.duration > maximumDuration || newAppointment.duration < 1) {
            errorMessage();
            continue;
        } else {
            break;
        }
    }

    return newAppointment;
};

// Kontrolliert ob der Kalender voll ist (maximal 10 Termine) & Einsortieren von neuen Terminen
void addAppointment(struct appointment * schedule, int * length)
{
    if (*length >= 10) {
        printf("\nDer Kalender ist voll!");
        return;
    }

    struct appointment newAppointment = createAppointment();
    int newIndex = 0;

    while (newIndex < *length && newAppointment.time > schedule[newIndex].time) {
        newIndex++;
    }

    for (int i = *length; i > newIndex; i--) {
        schedule[i] = schedule[i - 1];
    }

    schedule[newIndex] = newAppointment;
    (*length)++;
}

// Kontrolliert ob Kalender leer ist und wenn nicht, dann Aufforderung zur Auswahl eines Termins zum Löschen
void deleteAppointment(struct appointment * schedule, int * length)
{
    if (*length == 0) {
        printf("\nDer Kalender ist leer!");
        return;
    }

    printAppointments(schedule, *length);
    int deleteIndex;

    printf("\nWelchen dieser Termine moechten Sie loeschen? (1-%d): ", *length);

    while(1) {
        scanf("%d", &deleteIndex);
        if (deleteIndex < 1 || deleteIndex > *length) {
            printf("Ungueltige Eingabe!");
            continue;
        } else {
            break;
        }
    }

    for (int i = deleteIndex - 1; i < (*length) - 1; i++) {
        schedule[i] = schedule[i + 1];
    }

    (*length)--;

    printf("\nTermin %d wurde geloescht!", deleteIndex);
}
