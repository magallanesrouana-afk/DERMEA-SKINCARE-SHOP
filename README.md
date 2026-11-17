# DERMÉA Skincare Shop
### Your Skin, Your Care.

## Project Description
DERMÉA Skincare Shop is a console-based C++ inventory and search system for skincare products from different partner brands. It allows users to manage products, search and sort inventory, and process customer orders using Data Structures and Algorithms.

## Partner Brands
- CeraVe
- Cetaphil
- The Ordinary
- COSRX

## Data Structures Used

| Data Structure | Purpose |
|---|---|
| Singly Linked List | Stores and manages product inventory |
| Stack | Stores recently viewed products |
| Queue | Manages customer checkout orders using FIFO |
| Binary Search Tree (BST) | Searches products using their ID |

## Algorithms Used
- **Linear Search** – Finds products by name.
- **BST Search** – Finds products by product ID.
- **Bubble Sort** – Arranges products according to price.

## System Features
1. View all products
2. Search products by name
3. Search products by ID
4. Filter products by brand
5. Sort products by price
6. Add new products
7. Update product information
8. Delete products
9. Add products to the checkout queue
10. View pending orders
11. Process customer orders
12. View recently viewed products
13. Exit the system

## Requirements
- C++ compiler
- Code editor or IDE (such as Code::Blocks, Visual Studio Code, or Dev-C++)

## How to Compile and Run

Using g++:

```bash
g++ DERMEA_Skincare_DSA_Project.cpp -o dermea
```

Run on Linux or macOS:

```bash
./dermea
```

Run on Windows:

```bash
dermea.exe
```

## Checkout Queue
When a customer adds a product to the checkout queue, the system checks the available stock and reserves the requested quantity. Orders are processed in First-In, First-Out (FIFO) order.

## Project Information
- **Project Name:** DERMÉA Skincare Shop
- **Project Type:** Data Structures and Algorithms Final Project
- **Programming Language:** C++
- **Application Type:** Console-Based Inventory and Search Engine System

**Tagline:** Your Skin, Your Care.
