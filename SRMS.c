#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student
{
    int roll;
    char name[50];
    float marks;
};

char currentRole[16];
char currentUser[50];

// Function declarations
void mainMenu();
void adminMenu();
void staffMenu();
void guestMenu();

int loginSystem();
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

/* ================= LOGIN MODULE =================== */
int loginSystem()
{
    char username[50], password[50];
    char fileUser[50], filePass[50], fileRole[16];
    int attempts = 0;
    const int maxAttempts = 3;

    while (attempts < maxAttempts)
    {
        printf("\n========= LOGIN SCREEN =========\n");
        printf("Attempt %d of %d\n", attempts + 1, maxAttempts);

        printf("Username: ");
        if (scanf("%49s", username) != 1) {
            // clear input and try again
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        printf("Password: ");
        if (scanf("%49s", password) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        FILE *fp = fopen(CREDENTIAL_FILE, "r");
        if (!fp)
        {
            printf("Error: '%s' not found. Please create the credentials file.\n", CREDENTIAL_FILE);
            return 0;
        }

        int found = 0;
        while (fscanf(fp, "%49s %49s %15s", fileUser, filePass, fileRole) == 3)
        {
            if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0)
            {
                strncpy(currentUser, fileUser, sizeof(currentUser)-1);
                currentUser[sizeof(currentUser)-1] = '\0';
                strncpy(currentRole, fileRole, sizeof(currentRole)-1);
                currentRole[sizeof(currentRole)-1] = '\0';
                found = 1;
                break;
            }
        }
        fclose(fp);

        if (found)
        {
            printf("\nLogin Successful!\nWelcome %s [%s]\n", currentUser, currentRole);
            return 1;
        }
        else
        {
            attempts++;
            if (attempts < maxAttempts)
                printf("Invalid username or password! %d attempt(s) remaining.\n", maxAttempts - attempts);
            else
                printf("Maximum login attempts exceeded. Access denied.\n");
        }
    }

    return 0;
}

/* ================= MAIN MENU =================== */
void mainMenu()
{
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else
        guestMenu();
}

/* ================= ADMIN MENU =================== */
void adminMenu()
{
    int choice;
    do
    {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Add New Student\n");
        printf("2. Display All Records\n");
        printf("3. Search Records\n");
        printf("4. Update Student Details\n");
        printf("5. Delete Student Record\n");
        printf("6. Logout\n");
        printf("Enter choice : ");
        if (scanf("%d", &choice) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); choice = -1; }

        switch (choice)
        {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

/* ================= STAFF MENU =================== */
void staffMenu()
{
    int choice;
    do
    {
        printf("\n===== STAFF MENU =====\n");
        printf("1. Display All Records\n");
        printf("2. Search Records\n");
        printf("3. Update Student Details\n");
        printf("4. Logout\n");
        printf("Enter choice : ");
        if (scanf("%d", &choice) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); choice = -1; }

        switch (choice)
        {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: updateStudent(); break;
            case 4: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

/* ================= GUEST MENU =================== */
void guestMenu()
{
    int choice;
    do
    {
        printf("\n===== GUEST MENU =====\n");
        printf("1. Display All Records\n");
        printf("2. Search Records\n");
        printf("3. Logout\n");
        printf("Enter choice : ");
        if (scanf("%d", &choice) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); choice = -1; }

        switch (choice)
        {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

/* =============== ADD STUDENT =================== */
void addStudent()
{
    FILE *fp = fopen(STUDENT_FILE, "a");
    if (!fp){
        printf("Error opening student file!\n");
        return;
    }

    struct Student s;

    printf("\nEnter Roll Number: ");
    if (scanf("%d", &s.roll) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); printf("Invalid roll number.\n"); fclose(fp); return; }

    printf("Enter Name: ");
    getchar(); // clear newline
    if (fgets(s.name, sizeof(s.name), stdin) == NULL) s.name[0] = '\0';
    s.name[strcspn(s.name, "\n")] = '\0';

    printf("Enter Marks: ");
    if (scanf("%f", &s.marks) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); printf("Invalid marks.\n"); fclose(fp); return; }

    fprintf(fp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(fp);

    printf("Student Added Successfully!\n");
}

/* =============== DISPLAY ALL STUDENTS =================== */
void displayStudents()
{
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp)
    {
        printf("No records found!\n");
        return;
    }

    struct Student s;
    printf("\n===== STUDENT RECORDS =====\n");
    printf("%-10s %-25s %-8s\n", "Roll", "Name", "Marks");
    printf("------------------------------------------------\n");

    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3)
    {
        printf("%-10d %-25s %-8.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
}

/* =============== SEARCH STUDENT =================== */
void searchStudent()
{
    char searchName[50];
    int found = 0;
    struct Student s;
    int choice;

    printf("\nSearch by:\n1. Name\n2. Roll Number\nEnter choice: ");
    if (scanf("%d", &choice) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); printf("Invalid choice.\n"); return; }

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp)
    {
        printf("Error: Unable to open file\n");
        return;
    }

    if (choice == 1)
    {
        printf("Enter Student Name to search: ");
        getchar(); // clear newline
        if (fgets(searchName, sizeof(searchName), stdin) == NULL) searchName[0] = '\0';
        searchName[strcspn(searchName, "\n")] = '\0';

        printf("\n%-10s %-25s %-8s\n", "Roll No", "Name", "Marks");
        printf("------------------------------------------------\n");
        while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3)
        {
            if (strstr(s.name, searchName) != NULL || strcmp(s.name, searchName) == 0)
            {
                printf("%-10d %-25s %-8.2f\n", s.roll, s.name, s.marks);
                found = 1;
            }
        }
    }
    else if (choice == 2)
    {
        int roll, rfound = 0;
        printf("Enter Roll Number to search: ");
        if (scanf("%d", &roll) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); printf("Invalid roll number.\n"); fclose(fp); return; }
        while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3)
        {
            if (s.roll == roll)
            {
                printf("\nFound: Roll: %d, Name: %s, Marks: %.2f\n", s.roll, s.name, s.marks);
                rfound = 1;
                break;
            }
        }
        if (!rfound) printf("No student found with roll number %d\n", roll);
        fclose(fp);
        return;
    }
    else
    {
        printf("Invalid search option.\n");
        fclose(fp);
        return;
    }

    if (!found){
        printf("No student found with name matching '%s'\n", searchName);
    }
    fclose(fp);
}

/* =============== UPDATE STUDENT =================== */
void updateStudent()
{
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp){
        printf("Error opening student file!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp){
        printf("Error creating temporary file!\n");
        fclose(fp);
        return;
    }

    int roll;
    printf("\nEnter Roll Number to Update: ");
    if (scanf("%d", &roll) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); printf("Invalid roll number.\n"); fclose(fp); fclose(temp); return; }

    struct Student s;
    int found = 0;

    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3)
    {
        if (s.roll == roll)
        {
            found = 1;
            printf("Enter New Name: ");
            getchar();
            if (fgets(s.name, sizeof(s.name), stdin) == NULL) s.name[0] = '\0';
            s.name[strcspn(s.name, "\n")] = '\0';

            printf("Enter New Marks: ");
            if (scanf("%f", &s.marks) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); printf("Invalid marks input.\n"); /* keep old marks */ }
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Record Updated Successfully!\n");
    else
        printf("Record Not Found!\n");
}

/* =============== DELETE STUDENT =================== */
void deleteStudent()
{
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp){
        printf("Error opening student file!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp){
        printf("Error creating temporary file!\n");
        fclose(fp);
        return;
    }

    int roll;
    printf("\nEnter Roll Number to Delete: ");
    if (scanf("%d", &roll) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); printf("Invalid roll number.\n"); fclose(fp); fclose(temp); return; }

    struct Student s;
    int found = 0;

    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3)
    {
        if (s.roll == roll)
        {
            found = 1;
            continue; // skip writing this record
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Record Deleted Successfully!\n");
    else
        printf("Record Not Found!\n");
}

/* ================= MAIN =================== */
int main()
{
    printf("======================================\n");
    printf("   STUDENT RECORD MANAGEMENT SYSTEM   \n");
    printf("======================================\n");

    if (loginSystem())
        mainMenu();
    else
        printf("\nLogin failed. Exiting...\n");

    return 0;
}