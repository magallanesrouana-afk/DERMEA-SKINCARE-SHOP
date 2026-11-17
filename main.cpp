#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <queue>
#include <stack>
using namespace std;

// DERMÉA - Skincare Shop Inventory and Search Engine
// Data structures: singly linked list, stack, queue, and binary search tree.
// Algorithms: linear search, BST search, and bubble sort.

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
        if (!node) return;
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }

    TreeNode* insertTree(TreeNode* node, const Product& p) {
        if (!node) return new TreeNode(p);
        if (p.id < node->data.id)
            node->left = insertTree(node->left, p);
        else if (p.id > node->data.id)
            node->right = insertTree(node->right, p);
        return node;
    }

    TreeNode* searchTree(TreeNode* node, int id) const {
        if (!node || node->data.id == id) return node;
        if (id < node->data.id) return searchTree(node->left, id);
        return searchTree(node->right, id);
    }

    void rebuildTree() {
        clearTree(root);
        root = nullptr;
        for (Product* current = head; current; current = current->next)
            root = insertTree(root, *current);
    }

    Product* findById(int id) const {
        Product* current = head;
        while (current) {
            if (current->id == id) return current;
            current = current->next;
        }
        return nullptr;
    }

    bool idExists(int id) const {
        return findById(id) != nullptr;
    }

    void printProduct(const Product& p) const {
        cout << left << setw(6) << p.id
             << setw(34) << p.name.substr(0, 32)
             << setw(18) << p.brand.substr(0, 16)
             << setw(18) << p.category.substr(0, 16)
             << "P " << fixed << setprecision(2) << setw(10) << p.price
             << p.stockQuantity << '\n';
    }

    void printHeader() const {
        cout << left << setw(6) << "ID" << setw(34) << "PRODUCT"
             << setw(18) << "BRAND" << setw(18) << "CATEGORY"
             << setw(12) << "PRICE" << "STOCK\n";
        cout << string(100, '-') << '\n';
    }

    void addInitialProduct(int id, const string& name, const string& category,
                           const string& brand, double price, int stock) {
        Product* p = new Product{id, name, category, brand, price, stock, nullptr};
        if (!head) head = p;
        else {
            Product* current = head;
            while (current->next) current = current->next;
            current->next = p;
        }
    }

    void viewAllProducts() const {
        if (!head) {
            cout << "\nNo products in inventory.\n";
            return;
        }
        cout << "\n==================== DERMÉA PRODUCT CATALOG ====================\n";
        printHeader();
        for (Product* current = head; current; current = current->next)
            printProduct(*current);
    }

    void searchProductByName() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string keyword;
        cout << "Enter product name or keyword: ";
        getline(cin, keyword);

        bool found = false;
        cout << "\nSearch results:\n";
        printHeader();
        for (Product* current = head; current; current = current->next) {
            if (current->name.find(keyword) != string::npos) {
                printProduct(*current);
                recentlyViewed.push(current->name);
                found = true;
            }
        }
        if (!found) cout << "No matching product found.\n";
    }

    void searchProductByIdBST() {
        int id;
        cout << "Enter product ID: ";
        cin >> id;

        TreeNode* result = searchTree(root, id);
        if (result) {
            cout << "\nProduct found using Binary Search Tree:\n";
            printHeader();
            printProduct(result->data);
            recentlyViewed.push(result->data.name);
        } else {
            cout << "\nProduct ID not found.\n";
        }
    }

    void filterByBrand() const {
        cout << "\nAvailable partner brands:\n";
        cout << "1. CeraVe\n2. Cetaphil\n3. The Ordinary\n4. COSRX\n";
        cout << "Enter brand name exactly (or type All): ";
        string brand;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, brand);

        bool found = false;
        printHeader();
        for (Product* current = head; current; current = current->next) {
            if (brand == "All" || current->brand == brand) {
                printProduct(*current);
                found = true;
            }
        }
        if (!found) cout << "No products found for that brand.\n";
    }

    void sortProductsByPrice() {
        if (!head || !head->next) {
            cout << "\nNot enough products to sort.\n";
            return;
        }

        // Bubble sort swaps product data while preserving linked-list nodes.
        for (Product* end = nullptr; end != head; ) {
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
            if (!lastSwap) break;
        }
        rebuildTree();
        cout << "\nProducts sorted by price (lowest to highest).\n";
        viewAllProducts();
    }

    void addProduct() {
        Product p{};
        cout << "\nEnter new product ID: ";
        cin >> p.id;
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
        cout << "Price (PHP): ";
        cin >> p.price;
        cout << "Stock quantity: ";
        cin >> p.stockQuantity;

        if (p.price < 0 || p.stockQuantity < 0 || p.name.empty()) {
            cout << "Invalid product details. Product was not added.\n";
            return;
        }

        Product* newNode = new Product{p.id, p.name, p.category, p.brand,
                                       p.price, p.stockQuantity, nullptr};
        if (!head) head = newNode;
        else {
            Product* current = head;
            while (current->next) current = current->next;
            current->next = newNode;
        }
        rebuildTree();
        cout << "Product added successfully.\n";
    }

    void updateProduct() {
        int id;
        cout << "Enter product ID to update: ";
        cin >> id;
        Product* p = findById(id);
        if (!p) {
            cout << "Product not found.\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "New product name [" << p->name << "]: ";
        getline(cin, p->name);
        cout << "New category [" << p->category << "]: ";
        getline(cin, p->category);
        cout << "New brand [" << p->brand << "]: ";
        getline(cin, p->brand);
        cout << "New price: ";
        cin >> p->price;
        cout << "New stock quantity: ";
        cin >> p->stockQuantity;

        if (p->price < 0 || p->stockQuantity < 0) {
            cout << "Invalid values. Please update the product again.\n";
            return;
        }
        rebuildTree();
        cout << "Product updated successfully.\n";
    }

    void deleteProduct() {
        int id;
        cout << "Enter product ID to delete: ";
        cin >> id;

        Product* current = head;
        Product* previous = nullptr;
        while (current && current->id != id) {
            previous = current;
            current = current->next;
        }

        if (!current) {
            cout << "Product not found.\n";
            return;
        }

        if (!previous) head = current->next;
        else previous->next = current->next;

        delete current;
        rebuildTree();
        cout << "Product deleted successfully.\n";
    }

    void addToCheckoutQueue() {
        int id, quantity;
        cout << "Enter product ID to order: ";
        cin >> id;
        Product* p = findById(id);

        if (!p) {
            cout << "Product not found.\n";
            return;
        }

        cout << "Product: " << p->name << " | Available stock: "
             << p->stockQuantity << '\n';
        cout << "Quantity: ";
        cin >> quantity;

        if (quantity <= 0 || quantity > p->stockQuantity) {
            cout << "Invalid quantity or insufficient stock.\n";
            return;
        }

        // Reserve stock when the order is added to the checkout queue.
        p->stockQuantity -= quantity;
        Order order{nextOrderId++, p->id, p->name, quantity,
                    p->price * quantity};
        checkoutQueue.push(order);
        cout << "Order added to checkout queue. Order number: "
             << order.orderId << '\n';
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
            cout << "Order #" << o.orderId << " | " << o.productName
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
        cout << "\nOrder #" << order.orderId << " processed successfully!\n";
        cout << "Product: " << order.productName
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
        cout << "\nRecently viewed products (latest first):\n";
        while (!temp.empty()) {
            cout << "- " << temp.top() << '\n';
            temp.pop();
        }
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
        while (head) {
            Product* temp = head;
            head = head->next;
            delete temp;
        }
        clearTree(root);
    }

    void run() {
        int choice;
        do {
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
            cout << "Enter your choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid number.\n";
                continue;
            }

            switch (choice) {
                case 1: viewAllProducts(); break;
                case 2: searchProductByName(); break;
                case 3: searchProductByIdBST(); break;
                case 4: filterByBrand(); break;
                case 5: sortProductsByPrice(); break;
                case 6: addProduct(); break;
                case 7: updateProduct(); break;
                case 8: deleteProduct(); break;
                case 9: addToCheckoutQueue(); break;
                case 10: viewCheckoutQueue(); break;
                case 11: processNextOrder(); break;
                case 12: showRecentlyViewed(); break;
                case 13: cout << "\nThank you for shopping with DERMÉA!\n"; break;
                default: cout << "\nInvalid choice. Please select 1-13.\n";
            }
        } while (choice != 13);
    }
};

int main() {
    DermeaShop shop;
    shop.run();
    return 0;
}
 