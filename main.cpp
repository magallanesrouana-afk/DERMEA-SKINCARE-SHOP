#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <queue>
#include <stack>
#include <algorithm>
#include <cctype>
#include <conio.h>  // For _getch() on Windows
using namespace std;

// DERMÉA - Skincare Shop Inventory and Search Engine
// Data structures: Singly Linked List, Stack, Queue, Binary Search Tree
// Algorithms: Linear Search, BST Search, Bubble Sort

struct Product {
    int id;
    string name;
    string category;
    string brand;
    double price;
    int stockQuantity;
    Product* next;
};

struct Order {
    int orderId;
    int productId;
    string productName;
    int quantity;
    double total;
};

struct TreeNode {
    Product data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const Product& p) : data(p), left(nullptr), right(nullptr) {}
};

class DermeaShop {
private:
    Product* head;
    TreeNode* root;
    queue<Order> checkoutQueue;
    stack<string> recentlyViewed;
    int nextOrderId;

    void clearTree(TreeNode* node) {
        if (node == nullptr) return;
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }

    TreeNode* insertTree(TreeNode* node, const Product& p) {
        if (node == nullptr) return new TreeNode(p);

        if (p.id < node->data.id)
            node->left = insertTree(node->left, p);
        else if (p.id > node->data.id)
            node->right = insertTree(node->right, p);

        return node;
    }

    TreeNode* searchTree(TreeNode* node, int id) const {
        if (node == nullptr || node->data.id == id)
            return node;

        if (id < node->data.id)
            return searchTree(node->left, id);

        return searchTree(node->right, id);
    }

    void rebuildTree() {
        clearTree(root);
        root = nullptr;

        for (Product* current = head; current != nullptr; current = current->next)
            root = insertTree(root, *current);
    }

    Product* findById(int id) const {
        Product* current = head;

        while (current != nullptr) {
            if (current->id == id)
                return current;

            current = current->next;
        }

        return nullptr;
    }

    bool idExists(int id) const {
        return findById(id) != nullptr;
    }

    bool getIntegerInput(const string& prompt, int& value) {
        cout << prompt;

        if (cin >> value)
            return true;

        cout << "Invalid input. Please enter a whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

    bool getDoubleInput(const string& prompt, double& value) {
        cout << prompt;

        if (cin >> value)
            return true;

        cout << "Invalid input. Please enter a valid amount.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

    void printProduct(const Product& p) const {
        cout << left << setw(7) << p.id
             << setw(35) << p.name.substr(0, 33)
             << setw(18) << p.brand.substr(0, 16)
             << setw(19) << p.category.substr(0, 17)
             << "P " << right << setw(9) << fixed << setprecision(2) << p.price
             << "   " << p.stockQuantity << '\n';
    }

    void printHeader() const {
        cout << left << setw(7) << "ID"
             << setw(35) << "PRODUCT"
             << setw(18) << "BRAND"
             << setw(19) << "CATEGORY"
             << left << setw(13) << "PRICE"
             << "STOCK\n";
        cout << string(105, '-') << '\n';
    }

    void addInitialProduct(int id, const string& name, const string& category,
                           const string& brand, double price, int stock) {
        Product* p = new Product{id, name, category, brand, price, stock, nullptr};

        if (head == nullptr) {
            head = p;
        } else {
            Product* current = head;
            while (current->next != nullptr)
                current = current->next;
            current->next = p;
        }
    }

    void viewAllProducts() const {
        if (head == nullptr) {
            cout << "\nNo products in inventory.\n";
            return;
        }

        cout << "\n==================== DERMÉA PRODUCT CATALOG ====================\n";
        printHeader();

        for (Product* current = head; current != nullptr; current = current->next)
            printProduct(*current);
    }

    void searchProductByName() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string keyword;
        cout << "\nEnter product name, brand, or category: ";
        getline(cin, keyword);

        // Convert the search keyword to lowercase so searches are
        // not affected by uppercase/lowercase letters.
        transform(keyword.begin(), keyword.end(), keyword.begin(),
                  [](unsigned char ch) { return static_cast<char>(tolower(ch)); });

        if (keyword.empty()) {
            cout << "\nPlease enter a product name or keyword.\n";
            return;
        }

        bool found = false;
        int resultCount = 0;

        cout << "\n==================== SEARCH RESULTS ====================\n";
        printHeader();

        for (Product* current = head; current != nullptr; current = current->next) {
            string productName = current->name;
            string productBrand = current->brand;
            string productCategory = current->category;

            transform(productName.begin(), productName.end(), productName.begin(),
                      [](unsigned char ch) { return static_cast<char>(tolower(ch)); });
            transform(productBrand.begin(), productBrand.end(), productBrand.begin(),
                      [](unsigned char ch) { return static_cast<char>(tolower(ch)); });
            transform(productCategory.begin(), productCategory.end(), productCategory.begin(),
                      [](unsigned char ch) { return static_cast<char>(tolower(ch)); });

            // Search across product name, brand, and category.
            if (productName.find(keyword) != string::npos ||
                productBrand.find(keyword) != string::npos ||
                productCategory.find(keyword) != string::npos) {
                printProduct(*current);
                recentlyViewed.push(current->name);
                found = true;
                resultCount++;
            }
        }

        if (!found) {
            cout << "No matching product found. Please try another keyword.\n";
        } else {
            cout << "\nTotal products found: " << resultCount << '\n';
        }
    }

    void searchProductByIdBST() {
        int id;

        if (!getIntegerInput("\nEnter product ID: ", id))
            return;

        TreeNode* result = searchTree(root, id);

        if (result != nullptr) {
            cout << "\n========== PRODUCT FOUND USING BINARY SEARCH TREE ==========\n";
            printHeader();
            printProduct(result->data);
            recentlyViewed.push(result->data.name);
        } else {
            cout << "\nProduct ID " << id << " was not found.\n";
            cout << "Please check the product ID and try again.\n";
        }
    }

    void filterByBrand() const {
        cout << "\nAvailable partner brands:\n";
        cout << "1. CeraVe\n2. Cetaphil\n3. The Ordinary\n4. COSRX\n";

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string brand;
        cout << "\nEnter brand name exactly (or type All): ";
        getline(cin, brand);

        bool found = false;

        cout << "\n==================== FILTERED PRODUCTS ====================\n";
        printHeader();

        for (Product* current = head; current != nullptr; current = current->next) {
            if (brand == "All" || brand == current->brand) {
                printProduct(*current);
                found = true;
            }
        }

        if (!found)
            cout << "No products found for that brand.\n";
    }

    void sortProductsByPrice() {
        if (head == nullptr || head->next == nullptr) {
            cout << "\nNot enough products to sort.\n";
            return;
        }

        // Bubble sort: swaps product data while keeping linked-list nodes.
        for (Product* end = nullptr; end != head;) {
            Product* current = head;
            Product* lastSwap = nullptr;

            while (current->next != end) {
                if (current->price > current->next->price) {
                    swap(current->id, current->next->id);
                    swap(current->name, current->next->name);
                    swap(current->category, current->next->category);
                    swap(current->brand, current->next->brand);
                    swap(current->price, current->next->price);
                    swap(current->stockQuantity, current->next->stockQuantity);
                    lastSwap = current->next;
                }

                current = current->next;
            }

            end = lastSwap;
            if (lastSwap == nullptr)
                break;
        }

        rebuildTree();

        cout << "\nProducts sorted by price (lowest to highest).\n";
        viewAllProducts();
    }

    void addProduct() {
        Product p{};

        if (!getIntegerInput("\nEnter new product ID: ", p.id))
            return;

        if (idExists(p.id)) {
            cout << "That product ID already exists.\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Product name: ";
        getline(cin, p.name);

        cout << "Category: ";
        getline(cin, p.category);

        cout << "Brand (CeraVe/Cetaphil/The Ordinary/COSRX): ";
        getline(cin, p.brand);

        if (!getDoubleInput("Price (PHP): ", p.price))
            return;

        if (!getIntegerInput("Stock quantity: ", p.stockQuantity))
            return;

        if (p.name.empty() || p.price < 0 || p.stockQuantity < 0) {
            cout << "Invalid product details. Product was not added.\n";
            return;
        }

        Product* newNode = new Product{
            p.id, p.name, p.category, p.brand, p.price, p.stockQuantity, nullptr
        };

        if (head == nullptr) {
            head = newNode;
        } else {
            Product* current = head;
            while (current->next != nullptr)
                current = current->next;
            current->next = newNode;
        }

        rebuildTree();
        cout << "\nProduct added successfully!\n";
    }

    void updateProduct() {
        int id;

        if (!getIntegerInput("\nEnter product ID to update: ", id))
            return;

        Product* p = findById(id);

        if (p == nullptr) {
            cout << "Product not found.\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string newName, newCategory, newBrand;

        cout << "New product name [" << p->name << "]: ";
        getline(cin, newName);
        if (!newName.empty()) p->name = newName;

        cout << "New category [" << p->category << "]: ";
        getline(cin, newCategory);
        if (!newCategory.empty()) p->category = newCategory;

        cout << "New brand [" << p->brand << "]: ";
        getline(cin, newBrand);
        if (!newBrand.empty()) p->brand = newBrand;

        double newPrice;
        int newStock;

        if (!getDoubleInput("New price: ", newPrice))
            return;

        if (!getIntegerInput("New stock quantity: ", newStock))
            return;

        if (newPrice < 0 || newStock < 0) {
            cout << "Invalid values. Product was not updated.\n";
            return;
        }

        p->price = newPrice;
        p->stockQuantity = newStock;

        rebuildTree();
        cout << "\nProduct updated successfully!\n";
    }

    void deleteProduct() {
        int id;

        if (!getIntegerInput("\nEnter product ID to delete: ", id))
            return;

        Product* current = head;
        Product* previous = nullptr;

        while (current != nullptr && current->id != id) {
            previous = current;
            current = current->next;
        }

        if (current == nullptr) {
            cout << "Product not found.\n";
            return;
        }

        if (previous == nullptr)
            head = current->next;
        else
            previous->next = current->next;

        delete current;
        rebuildTree();

        cout << "\nProduct deleted successfully!\n";
    }

    void addToCheckoutQueue() {
        int id, quantity;

        if (!getIntegerInput("\nEnter product ID to order: ", id))
            return;

        Product* p = findById(id);

        if (p == nullptr) {
            cout << "Product not found.\n";
            return;
        }

        cout << "\nProduct: " << p->name
             << "\nAvailable stock: " << p->stockQuantity << '\n';

        if (!getIntegerInput("Quantity: ", quantity))
            return;

        if (quantity <= 0 || quantity > p->stockQuantity) {
            cout << "Invalid quantity or insufficient stock.\n";
            return;
        }

        // Reserve stock when the order is added to the checkout queue.
        p->stockQuantity -= quantity;

        Order order{
            nextOrderId++, p->id, p->name, quantity, p->price * quantity
        };

        checkoutQueue.push(order);

        cout << "\nOrder added to checkout queue successfully!\n";
        cout << "Order number: " << order.orderId << '\n';
    }

    void viewCheckoutQueue() const {
        if (checkoutQueue.empty()) {
            cout << "\nCheckout queue is empty.\n";
            return;
        }

        queue<Order> temp = checkoutQueue;

        cout << "\n================ CHECKOUT QUEUE (FIFO) ================\n";

        while (!temp.empty()) {
            const Order& o = temp.front();

            cout << "Order #" << o.orderId
                 << " | Product: " << o.productName
                 << " | Quantity: " << o.quantity
                 << " | Total: P " << fixed << setprecision(2)
                 << o.total << '\n';

            temp.pop();
        }
    }

    void processNextOrder() {
        if (checkoutQueue.empty()) {
            cout << "\nNo pending orders to process.\n";
            return;
        }

        Order order = checkoutQueue.front();
        checkoutQueue.pop();

        cout << "\n========== ORDER PROCESSED SUCCESSFULLY ==========\n";
        cout << "Order number: " << order.orderId
             << "\nProduct: " << order.productName
             << "\nQuantity: " << order.quantity
             << "\nTotal: P " << fixed << setprecision(2)
             << order.total << '\n';
    }

    void showRecentlyViewed() const {
        if (recentlyViewed.empty()) {
            cout << "\nNo recently viewed products.\n";
            return;
        }

        stack<string> temp = recentlyViewed;

        cout << "\n========== RECENTLY VIEWED PRODUCTS (STACK) ==========\n";

        while (!temp.empty()) {
            cout << "- " << temp.top() << '\n';
            temp.pop();
        }
    }

    void waitForEnter() {
        cout << "\nPress ENTER to return to the main menu...";

        // Wait specifically for the ENTER key.
        // _getch() avoids accidentally consuming a leftover newline from cin.
        int key;
        do {
            key = _getch();
        } while (key != 13 && key != 10);

        cout << '\n';
    }

    void displayMenu() const {
        cout << "\n====================================================\n";
        cout << "                 DERMÉA SKINCARE\n";
        cout << "                Your Skin, Your Care.\n";
        cout << "====================================================\n";
        cout << "[1]  View All Products\n";
        cout << "[2]  Search Product by Name (Linear Search)\n";
        cout << "[3]  Search Product by ID (BST Search)\n";
        cout << "[4]  Filter Products by Brand\n";
        cout << "[5]  Sort Products by Price (Bubble Sort)\n";
        cout << "[6]  Add New Product\n";
        cout << "[7]  Update Product Information\n";
        cout << "[8]  Delete Product\n";
        cout << "[9]  Add Product to Checkout Queue\n";
        cout << "[10] View Checkout Queue\n";
        cout << "[11] Process Next Customer Order\n";
        cout << "[12] View Recently Viewed Products (Stack)\n";
        cout << "[13] Exit\n";
        cout << "----------------------------------------------------\n";
    }

public:
    DermeaShop() : head(nullptr), root(nullptr), nextOrderId(1001) {
        addInitialProduct(101, "Moisturizing Cream", "Moisturizer", "CeraVe", 899.00, 20);
        addInitialProduct(102, "Foaming Facial Cleanser", "Facial Cleanser", "CeraVe", 750.00, 15);
        addInitialProduct(103, "Gentle Skin Cleanser", "Facial Cleanser", "Cetaphil", 520.00, 25);
        addInitialProduct(104, "Moisturizing Lotion", "Moisturizer", "Cetaphil", 680.00, 18);
        addInitialProduct(105, "Niacinamide 10% + Zinc 1%", "Serum", "The Ordinary", 650.00, 12);
        addInitialProduct(106, "Hyaluronic Acid 2% + B5", "Serum", "The Ordinary", 720.00, 10);
        addInitialProduct(107, "Advanced Snail 96 Mucin Essence", "Essence", "COSRX", 850.00, 16);
        addInitialProduct(108, "Low pH Good Morning Gel Cleanser", "Facial Cleanser", "COSRX", 590.00, 14);

        rebuildTree();
    }

    ~DermeaShop() {
        while (head != nullptr) {
            Product* temp = head;
            head = head->next;
            delete temp;
        }

        clearTree(root);
    }

    void run() {
        int choice = 0;

        do {
            displayMenu();

            if (!getIntegerInput("Enter your choice: ", choice)) {
                waitForEnter();
                continue;
            }

            switch (choice) {
                case 1:
                    viewAllProducts();
                    break;

                case 2:
                    searchProductByName();
                    break;

                case 3:
                    searchProductByIdBST();
                    break;

                case 4:
                    filterByBrand();
                    break;

                case 5:
                    sortProductsByPrice();
                    break;

                case 6:
                    addProduct();
                    break;

                case 7:
                    updateProduct();
                    break;

                case 8:
                    deleteProduct();
                    break;

                case 9:
                    addToCheckoutQueue();
                    break;

                case 10:
                    viewCheckoutQueue();
                    break;

                case 11:
                    processNextOrder();
                    break;

                case 12:
                    showRecentlyViewed();
                    break;

                case 13:
                    cout << "\nThank you for shopping with DERMÉA!\n";
                    break;

                default:
                    cout << "\nInvalid choice. Please select 1-13.\n";
                    break;
            }

            if (choice != 13)
                waitForEnter();

        } while (choice != 13);
    }
};

int main() {
    DermeaShop shop;
    shop.run();
    return 0;
}
