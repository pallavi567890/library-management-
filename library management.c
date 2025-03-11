
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 50
#define MAX_AUTHOR 50

// Structure to store book details
struct Book {
    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
};

// Function prototypes
void addBook();
void displayBooks();
void searchBook();
void deleteBook();

int main() {
    int choice;
    while (1) {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Search Book\n");
        printf("4. Delete Book\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear newline from buffer

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: deleteBook(); break;
            case 5: exit(0);
            default: printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

// Function to add a book
void addBook() {
    FILE *file = fopen("library.dat", "ab");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    struct Book book;
    printf("Enter book ID: ");
    scanf("%d", &book.id);
    getchar();
    printf("Enter book title: ");
    fgets(book.title, MAX_TITLE, stdin);
    book.title[strcspn(book.title, "\n")] = 0;  // Remove newline

    printf("Enter book author: ");
    fgets(book.author, MAX_AUTHOR, stdin);
    book.author[strcspn(book.author, "\n")] = 0;

    fwrite(&book, sizeof(struct Book), 1, file);
    fclose(file);

    printf("Book added successfully!\n");
}

// Function to display books
void displayBooks() {
    FILE *file = fopen("library.dat", "rb");
    if (!file) {
        printf("No books found!\n");
        return;
    }

    struct Book book;
    printf("\nID\tTitle\t\tAuthor\n");
    printf("------------------------------------\n");

    while (fread(&book, sizeof(struct Book), 1, file)) {
        printf("%d\t%s\t\t%s\n", book.id, book.title, book.author);
    }

    fclose(file);
}

// Function to search for a book
void searchBook() {
    FILE *file = fopen("library.dat", "rb");
    if (!file) {
        printf("No books found!\n");
        return;
    }

    int searchID;
    printf("Enter book ID to search: ");
    scanf("%d", &searchID);

    struct Book book;
    int found = 0;
    while (fread(&book, sizeof(struct Book), 1, file)) {
        if (book.id == searchID) {
            printf("\nBook Found:\n");
            printf("ID: %d\nTitle: %s\nAuthor: %s\n", book.id, book.title, book.author);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Book not found!\n");
    }

    fclose(file);
}

// Function to delete a book
void deleteBook() {
    FILE *file = fopen("library.dat", "rb");
    if (!file) {
        printf("No books found!\n");
        return;
    }

    FILE *tempFile = fopen("temp.dat", "wb");
    if (!tempFile) {
        printf("Error opening file!\n");
        fclose(file);
        return;
    }

    int deleteID;
    printf("Enter book ID to delete: ");
    scanf("%d", &deleteID);

    struct Book book;
    int found = 0;
    while (fread(&book, sizeof(struct Book), 1, file)) {
        if (book.id == deleteID) {
            found = 1;
        } else {
            fwrite(&book, sizeof(struct Book), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("library.dat");
    rename("temp.dat", "library.dat");

    if (found) {
        printf("Book deleted successfully!\n");
    } else {
        printf("Book ID not found!\n");
    }
}
