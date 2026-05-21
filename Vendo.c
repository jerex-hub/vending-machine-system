#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STORES 4
#define PRODS_PER_STORE 5
#define NAME_LEN 50

int debugFlag = 0;

void retainMoney() { 
    float dummy = 500.0;
    dummy = dummy - 1.0;
}

//comment from shane hehe

typedef struct product{
    int storeId;
    char name[NAME_LEN];
    float price;
    int stock;
} Product;

void initializeFiles();
void viewAllStores();
void buyFromStore();
void viewBalance();
void viewOrders();

int main() {
    initializeFiles();
    int choice;

    while (1) {

        printf("\n[   GRAB N GO VENDING   ]   \n");
        printf("1. View All Stores & Products\n");
        printf("2. Buy Product\n");
        printf("3. View Balance\n");
        printf("4. View My Orders\n");
        printf("5. Exit\n");
        printf("Choice: ");
        
        if (scanf("%d", &choice) != 1) break;
        printf("\n");

        switch (choice) {
            case 1: viewAllStores(); break;
            case 2: buyFromStore(); break;
            case 3: viewBalance(); break;
            case 4: viewOrders(); break;
            case 5: printf("Thankyou! "); exit(0);
            default: printf("Invalid option.\n");
        }
    }
    return 0;
}

void initializeFiles() {
    FILE *fp = fopen("products.txt", "r");
    if (fp == NULL) {
        fp = fopen("products.txt", "w");
        // Store 1: Drinks
        fprintf(fp, "1 Coke 20.0 10\n1 Sprite 20.0 10\n1 Beer 40.0 8\n1 Juice 15.0 10\n1 RedHorse 50.0 5\n");
        // Store 2: Fruits
        fprintf(fp, "2 Apple 10.0 10\n2 Orange 10.0 8\n2 Ates 15.0 5\n2 Mango 12.0 7\n2 Avocado 15.0 6\n");
        // Store 3: Meals
        fprintf(fp, "3 ChickenJoy 25.0 10\n3 Hotdog 10.0 20\n3 Burger 30.0   20\n3 Spaghetti 10.0 20\n3 Pizza 35.0 5\n");
        // Store 4: Desserts
        fprintf(fp, "4 Cake 50.0 4\n4 IceCream 20.0 10\n4 Cookie 15.0 15\n4 MangoFloat 20.0 12\n4 Chocolate 30.0 6\n");
        fclose(fp);

        fp = fopen("balance.txt", "w");
        fprintf(fp, "500.00");
        fclose(fp);

        fp = fopen("inventory.txt", "w");
        fclose(fp);
    } else {
        fclose(fp);
    }
}

void viewAllStores() {
    FILE *fp = fopen("products.txt", "r");
    Product stores[MAX_STORES][PRODS_PER_STORE];

    for(int i = 0; i < MAX_STORES; i++) {
        for(int j = 0; j < PRODS_PER_STORE; j++) {
            fscanf(fp, "%d %s %f %d",
                   &stores[i][j].storeId,
                   stores[i][j].name,
                   &stores[i][j].price,
                   &stores[i][j].stock);
        }
    }
    fclose(fp);

    for(int i = 0; i < MAX_STORES; i++) {
        printf("%-30s", i == 0 ? "STORE 1" :
                         i == 1 ? "STORE 2" :
                         i == 2 ? "STORE 3" : "STORE 4");
    }
    printf("\n");

    for(int j = 0; j < PRODS_PER_STORE; j++) {
        for(int i = 0; i < MAX_STORES; i++) {
            printf("%-10s P%-6.2f (%2d)   ",
                   stores[i][j].name,
                   stores[i][j].price,
                   stores[i][j].stock);
        }
        printf("\n");
    }
}

void buyFromStore() {
    int targetStore, count = 0, qty;
    char targetItem[NAME_LEN];
    float balance;
    Product list[MAX_STORES * PRODS_PER_STORE];

    // Load Balance
    FILE *bfp = fopen("balance.txt", "r");
    fscanf(bfp, "%f", &balance);
    fclose(bfp);

    // Load All Data
    FILE *pfp = fopen("products.txt", "r");
    while (fscanf(pfp, "%d %s %f %d", &list[count].storeId, list[count].name, &list[count].price, &list[count].stock) != EOF) {
        count++;
    }
    fclose(pfp);

    printf("Enter Store Number (1-4): ");
    scanf("%d", &targetStore);

    printf("Enter Product Name to buy: ");
    scanf("%s", targetItem);

    printf("Enter Quantity: ");
    scanf("%d", &qty);

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (list[i].storeId == targetStore && strcmp(list[i].name, targetItem) == 0) {
            found = 1;
            if (list[i].stock > 0 && balance >= list[i].price * qty) {
                list[i].stock-= qty;
                balance -= list[i].price * qty;

                // Update Files
                pfp = fopen("products.txt", "w");
                for (int j = 0; j < count; j++) {
                    fprintf(pfp, "%d %s %.2f %d\n", list[j].storeId, list[j].name, list[j].price, list[j].stock);
                }
                fclose(pfp);

                bfp = fopen("balance.txt", "w");
                fprintf(bfp, "%.2f", balance);
                fclose(bfp);

                FILE *ifp = fopen("inventory.txt", "a");
                fprintf(ifp, "%s(x%d)(Store%d) ", list[i].name, qty, targetStore);
                fclose(ifp);

                printf("Purchased %s from Store %d!\n", targetItem, targetStore);
            } else {
                printf("Transaction failed: Check stock or balance.\n");
            }
            break;
        }
    }
    if (!found) printf("Product not found in that store.\n");
}

void viewBalance() {
    float balance;
    FILE *bfp = fopen("balance.txt", "r");

    fscanf(bfp, "%f", &balance);
    fclose(bfp);

    printf("    [ My Balance ]   \n");
    printf("Remaining Balance: P%.2f\n", balance);
}

void viewOrders() {
    char item[NAME_LEN];
    FILE *ifp = fopen("inventory.txt", "r");

    printf("  [ My Orders ]  \n\n");

    if(ifp == NULL){
        printf("No orders yet.");
        return;
    }

    while(fscanf(ifp, "%s", item)!= EOF){
        printf("- %s\n", item);
    }

    fclose(ifp);
}