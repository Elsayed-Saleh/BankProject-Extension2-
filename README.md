# Bank Management System - Users & Permissions

An extended version of the Bank Management System developed using C++.

## 📌 Description

This project extends the Bank Management System by adding a user management and permissions system.

The application allows different users to access specific features according to their assigned permissions.

In addition to client management and banking transactions, the system now supports users, permissions, access control, and protected operations.

## ✨ Features

### 👥 Client Management

- Display all clients
- Add new clients
- Delete clients
- Update client information
- Find clients by account number
- Prevent duplicate account numbers
- Store client information in a text file

### 💰 Banking Transactions

- Deposit money
- Withdraw money
- Check available balance before withdrawal
- Display total balance
- Save updated account balances

### 🔐 Users & Permissions

- Store users in a separate file
- Username and password authentication
- Assign permissions to users
- Support full system access
- Control access to different system features
- Display an access denied message when a user does not have permission

## 🛡️ Permission System

The application uses permissions to control access to different features.

Available permissions include:

- List Clients
- Add New Client
- Delete Client
- Update Clients
- Find Client
- Transactions
- Manage Users
- Full Access

Permissions are represented using bit flags, allowing multiple permissions to be combined for one user.

## 🗂️ Main System Features

The system contains:

- Client Management
- Transaction Management
- User Management
- Permission Management

## 💾 Data Storage

The application uses text files to store data.

### Client Data

```text
ClintsDetails.txt

```
### Users Data
```text
Users.txt
