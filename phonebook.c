#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILENAME "phonebook.dat"
#define MAX_CONTACTS 100
#define MAX_NAME 50
#define MAX_PHONE 15
#define MAX_EMAIL 50
#define MAX_ADDRESS 100
#define MAX_GROUP 20
#define MAX_NOTES 200

typedef struct
{
    int contactID;
    char name[MAX_NAME];
    char phoneNumber[MAX_PHONE];
    char email[MAX_EMAIL];
    char address[MAX_ADDRESS];
    char group[MAX_GROUP];
    char notes[MAX_NOTES];
} Contact;

void displayMenu();
int loadFromFile(Contact contacts[]);
int saveToFile(Contact contacts[], int count);
int findByID(Contact contacts[], int count, int id);
int findByPhone(Contact contacts[], int count, const char *phone);
int findByName(Contact contacts[], int count, const char *name);
int findByGroup(Contact contacts[], int count, const char *group, int results[]);
int isValidName(const char *name);
int isValidPhone(const char *phone);
int isValidEmail(const char *email);
int isValidGroup(const char *group);
void trimNewline(char *str);
void readLine(char *buffer, int size);
void sortContactsByName(Contact contacts[], int count);
void printContact(const Contact *c);

void addContact();
void displayAllContacts();
int searchByName(char name[]);
int searchByNumber(char number[]);
int searchByGroup(char group[]);
void editContact(int index);
void deleteContact(int index);
void deleteByNumber(char number[]);
void groupContacts();

void addContact()
{
    Contact contacts[MAX_CONTACTS];
    int count = loadFromFile(contacts);
    if (count >= MAX_CONTACTS)
    {
        printf("Phonebook is full.\n");
        return;
    }

    Contact c;
    memset(&c, 0, sizeof(Contact));

    int maxID = 0;
    for (int i = 0; i < count; i++)
    {
        if (contacts[i].contactID > maxID)
            maxID = contacts[i].contactID;
    }
    c.contactID = maxID + 1;

    do
    {
        printf("Enter name: ");
        readLine(c.name, MAX_NAME);
    } while (!isValidName(c.name));

    do
    {
        printf("Enter phone number (10 digits): ");
        readLine(c.phoneNumber, MAX_PHONE);
    } while (!isValidPhone(c.phoneNumber));

    if (findByPhone(contacts, count, c.phoneNumber) != -1)
    {
        printf("Error: Phone number already exists!\n");
        return;
    }

    do
    {
        printf("Enter email: ");
        readLine(c.email, MAX_EMAIL);
    } while (!isValidEmail(c.email));

    printf("Enter address: ");
    readLine(c.address, MAX_ADDRESS);

    do
    {
        printf("Enter group (Family/Friends/Work/School/Other): ");
        readLine(c.group, MAX_GROUP);
    } while (!isValidGroup(c.group));

    printf("Enter notes: ");
    readLine(c.notes, MAX_NOTES);

    contacts[count++] = c;
    sortContactsByName(contacts, count);
    saveToFile(contacts, count);
    printf("Contact added successfully.\n");
}

void displayAllContacts()
{
    Contact contacts[MAX_CONTACTS];
    int count = loadFromFile(contacts);

    if (count == 0)
    {
        printf("No contacts found.\n");
        return;
    }

    sortContactsByName(contacts, count);

    for (int i = 0; i < count; i++)
    {
        printf("\n=== Contact %d ===\n", i + 1);
        printContact(&contacts[i]);
    }
}

int searchByName(char name[])
{
    Contact contacts[MAX_CONTACTS];
    int count = loadFromFile(contacts);

    for (int i = 0; i < count; i++)
    {
        if (strcasecmp(contacts[i].name, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

int searchByNumber(char number[])
{
    Contact contacts[MAX_CONTACTS];
    int count = loadFromFile(contacts);

    for (int i = 0; i < count; i++)
    {
        if (strcmp(contacts[i].phoneNumber, number) == 0)
        {
            return i;
        }
    }
    return -1;
}

int searchByGroup(char group[])
{
    Contact contacts[MAX_CONTACTS];
    int count = loadFromFile(contacts);
    int found = 0;

    for (int i = 0; i < count; i++)
    {
        if (strcasecmp(contacts[i].group, group) == 0)
        {
            if (!found)
                printf("\nContacts in group '%s':\n", group);
            printContact(&contacts[i]);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No contacts found in group '%s'.\n", group);
    }
    return found;
}

void editContact(int index)
{
    Contact contacts[MAX_CONTACTS];
    int count = loadFromFile(contacts);

    if (index < 0 || index >= count)
    {
        printf("Invalid contact index.\n");
        return;
    }

    printf("Editing contact ID %d\n", contacts[index].contactID);

    printf("Current name: %s\n", contacts[index].name);
    printf("New name: ");
    readLine(contacts[index].name, MAX_NAME);
    if (!isValidName(contacts[index].name))
    {
        printf("Invalid name.\n");
        return;
    }

    printf("Current phone: %s\n", contacts[index].phoneNumber);
    printf("New phone: ");
    readLine(contacts[index].phoneNumber, MAX_PHONE);
    if (!isValidPhone(contacts[index].phoneNumber))
    {
        printf("Invalid phone.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        if (i != index && strcmp(contacts[i].phoneNumber, contacts[index].phoneNumber) == 0)
        {
            printf("Error: Phone number already exists!\n");
            return;
        }
    }

    printf("New email: ");
    readLine(contacts[index].email, MAX_EMAIL);
    if (!isValidEmail(contacts[index].email))
    {
        printf("Invalid email.\n");
        return;
    }

    printf("New address: ");
    readLine(contacts[index].address, MAX_ADDRESS);

    printf("New group (Family/Friends/Work/School/Other): ");
    readLine(contacts[index].group, MAX_GROUP);
    if (!isValidGroup(contacts[index].group))
    {
        printf("Invalid group.\n");
        return;
    }

    printf("New notes: ");
    readLine(contacts[index].notes, MAX_NOTES);

    sortContactsByName(contacts, count);
    saveToFile(contacts, count);
    printf("Contact updated successfully.\n");
}

void deleteContact(int index)
{
    Contact contacts[MAX_CONTACTS];
    int count = loadFromFile(contacts);

    if (index < 0 || index >= count)
    {
        printf("Invalid contact index.\n");
        return;
    }

    for (int i = index; i < count - 1; i++)
    {
        contacts[i] = contacts[i + 1];
    }

    saveToFile(contacts, count - 1);
    printf("Contact deleted successfully.\n");
}

void deleteByNumber(char number[])
{
    Contact contacts[MAX_CONTACTS];
    int count = loadFromFile(contacts);
    int idx = -1;

    for (int i = 0; i < count; i++)
    {
        if (strcmp(contacts[i].phoneNumber, number) == 0)
        {
            idx = i;
            break;
        }
    }

    if (idx == -1)
    {
        printf("Contact not found.\n");
        return;
    }

    deleteContact(idx);
}

void groupContacts()
{
    char group[MAX_GROUP];
    printf("=== GROUPS ===\n1. Family\n2. Friends\n3. Work\n4. School\n5. Other\n");
    printf("Enter group name: ");
    readLine(group, MAX_GROUP);
    searchByGroup(group);
}

int saveToFile(Contact contacts[], int count)
{
    FILE *fp = fopen(FILENAME, "wb");
    if (!fp)
    {
        printf("Error opening file for writing.\n");
        return 0;
    }

    fwrite(&count, sizeof(int), 1, fp);
    fwrite(contacts, sizeof(Contact), count, fp);
    fclose(fp);
    return 1;
}

int loadFromFile(Contact contacts[])
{
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp)
        return 0;

    int count = 0;
    if (fread(&count, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        return 0;
    }

    if (count < 0 || count > MAX_CONTACTS)
    {
        fclose(fp);
        return 0;
    }

    if (fread(contacts, sizeof(Contact), count, fp) != (size_t)count)
    {
        fclose(fp);
        return 0;
    }

    fclose(fp);
    return count;
}

void displayMenu()
{
    printf("\n=== PHONEBOOK MANAGEMENT ===\n");
    printf("1. Add Contact\n");
    printf("2. Display All Contacts\n");
    printf("3. Search by Name\n");
    printf("4. Search by Number\n");
    printf("5. Search by Group\n");
    printf("6. Edit Contact\n");
    printf("7. Delete Contact\n");
    printf("8. Group Contacts\n");
    printf("9. Exit\n");
    printf("Enter choice: ");
}

int findByID(Contact contacts[], int count, int id)
{
    for (int i = 0; i < count; i++)
    {
        if (contacts[i].contactID == id)
            return i;
    }
    return -1;
}

int findByPhone(Contact contacts[], int count, const char *phone)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(contacts[i].phoneNumber, phone) == 0)
            return i;
    }
    return -1;
}

int findByName(Contact contacts[], int count, const char *name)
{
    for (int i = 0; i < count; i++)
    {
        if (strcasecmp(contacts[i].name, name) == 0)
            return i;
    }
    return -1;
}

int findByGroup(Contact contacts[], int count, const char *group, int results[])
{
    int found = 0;
    for (int i = 0; i < count; i++)
    {
        if (strcasecmp(contacts[i].group, group) == 0)
        {
            results[found++] = i;
        }
    }
    return found;
}

int isValidName(const char *name)
{
    return name && strlen(name) > 0;
}

int isValidPhone(const char *phone)
{
    if (!phone || strlen(phone) != 10)
        return 0;
    for (int i = 0; i < 10; i++)
    {
        if (!isdigit((unsigned char)phone[i]))
            return 0;
    }
    return 1;
}

int isValidEmail(const char *email)
{
    return email && strchr(email, '@') != NULL && strlen(email) > 3;
}

int isValidGroup(const char *group)
{
    return strcasecmp(group, "Family") == 0 ||
           strcasecmp(group, "Friends") == 0 ||
           strcasecmp(group, "Work") == 0 ||
           strcasecmp(group, "School") == 0 ||
           strcasecmp(group, "Other") == 0;
}

void trimNewline(char *str)
{
    if (!str)
        return;
    str[strcspn(str, "\n")] = '\0';
}

void readLine(char *buffer, int size)
{
    if (!fgets(buffer, size, stdin))
    {
        buffer[0] = '\0';
        return;
    }
    trimNewline(buffer);
}

void sortContactsByName(Contact contacts[], int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (strcasecmp(contacts[i].name, contacts[j].name) > 0)
            {
                Contact temp = contacts[i];
                contacts[i] = contacts[j];
                contacts[j] = temp;
            }
        }
    }
}

void printContact(const Contact *c)
{
    if (!c)
        return;
    printf("ID: %d\n", c->contactID);
    printf("Name: %s\n", c->name);
    printf("Phone: %s\n", c->phoneNumber);
    printf("Email: %s\n", c->email);
    printf("Address: %s\n", c->address);
    printf("Group: %s\n", c->group);
    printf("Notes: %s\n", c->notes);
}

int main()
{
    int choice;
    Contact contacts[MAX_CONTACTS];
    int contactCount = loadFromFile(contacts);
    (void)contactCount;

    do
    {
        displayMenu();
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;
            printf("Invalid input.\n");
            continue;
        }
        while (getchar() != '\n')
            ;

        switch (choice)
        {
        case 1:
            addContact();
            break;
        case 2:
            displayAllContacts();
            break;
        case 3:
        {
            char name[MAX_NAME];
            printf("Enter name to search: ");
            readLine(name, MAX_NAME);
            int idx = searchByName(name);
            if (idx != -1)
            {
                Contact temp[MAX_CONTACTS];
                loadFromFile(temp);
                printContact(&temp[idx]);
            }
            else
            {
                printf("Contact not found.\n");
            }
            break;
        }
        case 4:
        {
            char number[MAX_PHONE];
            printf("Enter number to search: ");
            readLine(number, MAX_PHONE);
            int idx = searchByNumber(number);
            if (idx != -1)
            {
                Contact temp[MAX_CONTACTS];
                loadFromFile(temp);
                printContact(&temp[idx]);
            }
            else
            {
                printf("Contact not found.\n");
            }
            break;
        }
        case 5:
            groupContacts();
            break;
        case 6:
        {
            int id;
            printf("Enter contact ID to edit: ");
            scanf("%d", &id);
            while (getchar() != '\n')
                ;
            Contact temp[MAX_CONTACTS];
            int count = loadFromFile(temp);
            int idx = findByID(temp, count, id);
            if (idx != -1)
                editContact(idx);
            else
                printf("Contact not found.\n");
            break;
        }
        case 7:
        {
            char number[MAX_PHONE];
            printf("Enter number to delete: ");
            readLine(number, MAX_PHONE);
            deleteByNumber(number);
            break;
        }
        case 8:
            groupContacts();
            break;
        case 9:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice.\n");
        }
    } while (choice != 9);

    return 0;
}