#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BOOKS 100
#define DATA_FILE "library_books.dat"

typedef struct {
    char title[100];
    char author[100];
    char ISBN[13];
    int publicationYear;
    int isAvailable; // 1 for available, 0 for not available
} Book;

Book library[MAX_BOOKS];
int bookCount = 0;

// Function prototypes
void addBook();
void displayBooks();
void updateBook();
void searchBook();
void saveToFile();
void loadFromFile();
void menu();
void clearInputBuffer();

int main() {
    loadFromFile(); // Load records from file at the start
    menu();
    saveToFile(); // Save records to file before exiting
    return 0;
}

void menu() {
    int choice;
    do {
        printf("\n=== Library Book Database Management System ===\n");
        printf("1. Add a new book\n");
        printf("2. Display all books\n");
        printf("3. Update a book record\n");
        printf("4. Search for a book\n");
        printf("5. Save to file\n");
        printf("6. Load from file\n");
        printf("7. Exit\n");
        printf("Enter your choice (1-7): ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1 and 7.\n");
            clearInputBuffer();
            continue;
        }

        clearInputBuffer(); // Clear input buffer after each menu selection

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                displayBooks();
                break;
            case 3:
                updateBook();
                break;
            case 4:
                searchBook();
                break;
            case 5:
                saveToFile();
                break;
            case 6:
                loadFromFile();
                break;
            case 7:
                printf("Exiting the system. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);
}

void addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("Library is full! Cannot add more books.\n");
        return;
    }

    Book newBook;
    printf("Enter the title of the book: ");
    fgets(newBook.title, sizeof(newBook.title), stdin);
    newBook.title[strcspn(newBook.title, "\n")] = '\0'; // Remove trailing newline

    printf("Enter the author of the book: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    newBook.author[strcspn(newBook.author, "\n")] = '\0';

    printf("Enter the ISBN (13 characters): ");
    fgets(newBook.ISBN, sizeof(newBook.ISBN), stdin);
    newBook.ISBN[strcspn(newBook.ISBN, "\n")] = '\0';

    printf("Enter the publication year: ");
    if (scanf("%d", &newBook.publicationYear) != 1 || newBook.publicationYear <= 0) {
        printf("Invalid publication year. Book not added.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    newBook.isAvailable = 1; // Default to available
    library[bookCount++] = newBook;
    printf("Book added successfully!\n");
}

void displayBooks() {
    if (bookCount == 0) {
        printf("No books in the library.\n");
        return;
    }

    printf("\n=== List of Books ===\n");
    for (int i = 0; i < bookCount; i++) {
        printf("Book ID: %d\n", i + 1);
        printf("Title: %s\n", library[i].title);
        printf("Author: %s\n", library[i].author);
        printf("ISBN: %s\n", library[i].ISBN);
        printf("Publication Year: %d\n", library[i].publicationYear);
        printf("Availability: %s\n", library[i].isAvailable ? "Available" : "Not Available");
        printf("-----------------------------\n");
    }
}

void updateBook() {
    int id;
    printf("Enter the book ID to update (1 to %d): ", bookCount);
    if (scanf("%d", &id) != 1 || id < 1 || id > bookCount) {
        printf("Invalid book ID.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    Book *book = &library[id - 1];
    printf("Updating book: %s\n", book->title);

    printf("Enter the new title (leave blank to keep current): ");
    char newTitle[100];
    fgets(newTitle, sizeof(newTitle), stdin);
    if (newTitle[0] != '\n') {
        newTitle[strcspn(newTitle, "\n")] = '\0';
        strcpy(book->title, newTitle);
    }

    printf("Enter the new author (leave blank to keep current): ");
    char newAuthor[100];
    fgets(newAuthor, sizeof(newAuthor), stdin);
    if (newAuthor[0] != '\n') {
        newAuthor[strcspn(newAuthor, "\n")] = '\0';
        strcpy(book->author, newAuthor);
    }

    printf("Enter the new ISBN (leave blank to keep current): ");
    char newISBN[13];
    fgets(newISBN, sizeof(newISBN), stdin);
    if (newISBN[0] != '\n') {
        newISBN[strcspn(newISBN, "\n")] = '\0';
        strcpy(book->ISBN, newISBN);
    }

    printf("Enter the new publication year (or 0 to keep current): ");
    int newYear;
    if (scanf("%d", &newYear) == 1 && newYear > 0) {
        book->publicationYear = newYear;
    }
    clearInputBuffer();

    printf("Is the book available? (1 for Yes, 0 for No): ");
    int availability;
    if (scanf("%d", &availability) == 1 && (availability == 0 || availability == 1)) {
        book->isAvailable = availability;
    }
    clearInputBuffer();

    printf("Book updated successfully!\n");
}

void searchBook() {
    char keyword[100];
    printf("Enter a keyword to search for (title or author): ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = '\0'; // Remove trailing newline

    printf("\n=== Search Results ===\n");
    int found = 0;
    for (int i = 0; i < bookCount; i++) {
        if (strstr(library[i].title, keyword) || strstr(library[i].author, keyword)) {
            printf("Book ID: %d\n", i + 1);
            printf("Title: %s\n", library[i].title);
            printf("Author: %s\n", library[i].author);
            printf("ISBN: %s\n", library[i].ISBN);
            printf("Publication Year: %d\n", library[i].publicationYear);
            printf("Availability: %s\n", library[i].isAvailable ? "Available" : "Not Available");
            printf("-----------------------------\n");
            found = 1;
        }
    }

    if (!found) {
        printf("No books found matching the keyword.\n");
    }
}

void saveToFile() {
    FILE *file = fopen(DATA_FILE, "wb");
    if (file == NULL) {
        printf("Error saving data to file.\n");
        return;
    }
    fwrite(&bookCount, sizeof(int), 1, file);
    fwrite(library, sizeof(Book), bookCount, file);
    fclose(file);
    printf("Data saved to file successfully!\n");
}

void loadFromFile() {
    FILE *file = fopen(DATA_FILE, "rb");
    if (file == NULL) {
        printf("No existing data file found. Starting fresh.\n");
        return;
    }
    fread(&bookCount, sizeof(int), 1, file);
    fread(library, sizeof(Book), bookCount, file);
    fclose(file);
    printf("Data loaded from file successfully!\n");
}

void clearInputBuffer() {
    while (getchar() != '\n');
}
