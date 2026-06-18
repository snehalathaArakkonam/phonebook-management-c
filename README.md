# 📒 Phonebook Management System (C)

A simple, menu-driven **Phonebook Management System** written in C. It lets you
store, search, edit, delete, and group contacts, with all data persisted to a
local binary file so your contacts survive between runs.

---

## ✨ Features

- **Add Contact** — store name, phone number, email, address, group, and notes.
- **Display All Contacts** — list every saved contact, sorted alphabetically by name.
- **Search by Name** — case-insensitive lookup by contact name.
- **Search by Number** — find a contact by their 10-digit phone number.
- **Search by Group** — list all contacts that belong to a given group.
- **Edit Contact** — update an existing contact (looked up by contact ID).
- **Delete Contact** — remove a contact by phone number.
- **Group Contacts** — view contacts organized into predefined groups.
- **Input validation** — names, phone numbers, emails, and groups are validated.
- **Duplicate protection** — phone numbers must be unique.
- **Persistent storage** — contacts are saved to `phonebook.dat`.

---

## 🗂️ Contact Fields

| Field    | Description                                            | Constraint                                   |
| -------- | ------------------------------------------------------ | -------------------------------------------- |
| ID       | Auto-generated unique contact identifier               | Assigned automatically                       |
| Name     | Contact's full name                                    | Cannot be empty                              |
| Phone    | Contact's phone number                                 | Exactly 10 digits, must be unique            |
| Email    | Contact's email address                                | Must contain `@` and be longer than 3 chars  |
| Address  | Contact's address                                      | Optional free text                           |
| Group    | Category the contact belongs to                        | Family / Friends / Work / School / Other     |
| Notes    | Any additional notes                                   | Optional free text                           |

---

## 🚀 Getting Started

### Prerequisites

- A C compiler such as **GCC** (or any C99-compatible compiler).

### Build

```bash
gcc phonebook.c -o phonebook
```

### Run

```bash
./phonebook
```

> On Windows, run `phonebook.exe` after compiling.

---

## 🧭 Usage

When you run the program, you'll see a menu:

```
=== PHONEBOOK MANAGEMENT ===
1. Add Contact
2. Display All Contacts
3. Search by Name
4. Search by Number
5. Search by Group
6. Edit Contact
7. Delete Contact
8. Group Contacts
9. Exit
Enter choice:
```

Type the number of the action you want and follow the on-screen prompts.

---

## 💾 Data Storage

All contacts are stored in a binary file named **`phonebook.dat`**, created in the
same directory as the executable. The file is loaded automatically on startup and
updated whenever contacts are added, edited, or deleted.

> **Note:** `phonebook.dat` is generated at runtime and does not need to be committed
> to version control.

---

## ⚙️ Limits

| Setting           | Value |
| ----------------- | ----- |
| Max contacts      | 100   |
| Max name length   | 50    |
| Max phone length  | 15    |
| Max email length  | 50    |
| Max address length| 100   |
| Max group length  | 20    |
| Max notes length  | 200   |

---

## 📁 Project Structure

```
PhonebookManagement-c/
├── phonebook.c     # Source code for the phonebook application
├── phonebook.dat   # Auto-generated data file (created at runtime)
└── README.md       # Project documentation
```

---

## 🤝 Contributing

Contributions are welcome! Feel free to open an issue or submit a pull request.

---

## 📝 License

This project is provided for educational purposes. You are free to use, modify, and
distribute it.
