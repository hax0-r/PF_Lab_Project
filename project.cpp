#include <stdio.h>
#include <string.h>

#define MAX_MOVIES 5
#define MAX_SEATS 50
#define MAX_TICKETS 100

int free_ids[MAX_TICKETS];
int free_count = 0;

char *movies[MAX_MOVIES] = {
    "Avengers", "Karachi To Lahore", "Love Guru", "Dhoom3", "Titanic"};
int total_seats[MAX_MOVIES] = {30, 20, 25, 40, 1};
int booked[MAX_MOVIES][MAX_SEATS] = {0};

int ticket_ids[MAX_TICKETS], ticket_movie[MAX_TICKETS], ticket_seat[MAX_TICKETS];
int ticket_count = 0;

int queue[MAX_TICKETS], queue_movie[MAX_TICKETS], queue_seat[MAX_TICKETS];
int front = 0, rear = 0;

void displayMovies() {
	printf("===============================\n");
    printf("Movies:\n");
    for (int i = 0; i < MAX_MOVIES; i++) {
        int available = 0;
        for (int j = 0; j < total_seats[i]; j++)
            if (!booked[i][j]) available++;
        printf("%d. %s (%d seats left)\n", i + 1, movies[i], available);
    }
    printf("===============================\n");
}

void showAvailableSeats(int m) {
    printf("Available seats: ");
    for (int i = 0; i < total_seats[m]; i++) {
        if (!booked[m][i]) {
            printf("%d ", i + 1);
        }
    }
    printf("\n");
}

void bookTicket() {
    int m, s = -1;
    displayMovies();
    printf("Enter movie ID: ");
    scanf("%d", &m);
    m--;
    if (m < 0 || m >= MAX_MOVIES) return;

    int available = 0;
    for (int i = 0; i < total_seats[m]; i++)
        if (!booked[m][i]) { available++; s = i + 1; }

    if (available == 0) {
        printf("No seats left.\n");
        return;
    }

    if (available > 1) {
        showAvailableSeats(m);
        printf("Enter seat number from the list above: ");
        scanf("%d", &s);
        if (s < 1 || s > total_seats[m] || booked[m][s - 1]) {
            printf("Seat not available.\n");
            return;
        }
    } else {
        printf("Only 1 seat left. Seat %d assigned.\n", s);
    }

    // Prevent queue overflow
    if (rear >= MAX_TICKETS) {
        printf("Booking queue full. Try later.\n");
        return;
    }

    // Reuse cancelled ticket ID if available
    int id;
    if (free_count > 0) {
        id = free_ids[--free_count];
    } else {
        id = ++ticket_count;
    }

    queue[rear] = id;
    queue_movie[rear] = m;
    queue_seat[rear++] = s;

    printf("Ticket request queued. ID: %d\n", id);
}

void processBookings() {
    if (front == rear) {
        printf("No bookings to process. Please reserve a seat first.\n");
        return;
    }

    while (front < rear) {
        int id = queue[front];
        int m = queue_movie[front];
        int s = queue_seat[front++];
        if (!booked[m][s - 1]) {
            booked[m][s - 1] = 1;
            ticket_ids[id - 1] = id;
            ticket_movie[id - 1] = m;
            ticket_seat[id - 1] = s;
            printf("Ticket %d booked: %s seat %d\n", id, movies[m], s);
        } else {
            printf("Ticket %d failed (seat already booked)\n", id);
        }
    }

    // Reset queue after processing
    front = rear = 0;
}

void cancelTicket() {
    int found = 0;
    
    printf("===============================\n");
    
    for (int i = 0; i < ticket_count; i++) {
        if (ticket_ids[i]) {
            printf("ID: %d, Movie: %s, Seat: %d\n", ticket_ids[i], movies[ticket_movie[i]], ticket_seat[i]);
            found = 1;
        }
    }
    
    printf("===============================\n");
    if (!found) {
    	printf("No tickets booked.\n");
    	return;
	}

	
    int id;
    printf("Enter Ticket ID to cancel: ");
    scanf("%d", &id);
    if (id <= 0 || id > ticket_count || ticket_ids[id - 1] == 0) {
        printf("Ticket not found.\n");
        return;
    }
    int m = ticket_movie[id - 1];
    int s = ticket_seat[id - 1];
    booked[m][s - 1] = 0;
    ticket_ids[id - 1] = 0;
    free_ids[free_count++] = id;
    printf("Ticket %d cancelled.\n", id);
}

void viewTickets() {
    printf("Booked Tickets:\n");
    int found = 0;
    for (int i = 0; i < ticket_count; i++) {
        if (ticket_ids[i]) {
            printf("ID: %d, Movie: %s, Seat: %d\n", ticket_ids[i], movies[ticket_movie[i]], ticket_seat[i]);
            found = 1;
        }
    }
    if (!found) printf("No tickets booked.\n");
}

int main() {
    int choice;
    do {
        printf("\n1. Display Movies\n2. Book Ticket\n3. Process Bookings (if payment recevied from customer)\n4. Cancel Ticket\n5. View Tickets\n6. Exit\nChoice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: displayMovies(); break;
            case 2: bookTicket(); break;
            case 3: processBookings(); break;
            case 4: cancelTicket(); break;
            case 5: viewTickets(); break;
            case 6: printf("Goodbye!\n"); break;
            default: printf("Invalid!\n");
        }
    } while (choice != 6);
    return 0;
}
