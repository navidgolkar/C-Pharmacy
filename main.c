/**
This pharmacy management program is written by NAVID GOLKAR
**/
#include <stdio.h>
#include <stdlib.h>
///the conio.h is included so we can use the getch() function, which is used in the login() for the passsword privacy part
#include <conio.h>
#include <string.h>
///the cTYPE.h is included so we can use the tolower() function
#include <ctype.h>
///the folder where the database is saved is defined as FOLDER
#define FOLDER "database.txt"

///defining functions
int login();
void menu ();
void add();
void editanddelete();
void list();
void show(int);
int search_name(char []);
int search_code(unsigned long long);
void edit(int);
void deletep(int);
void sort();
void save();

///define a structure for the expiration date of the drugs
struct date {
    int year;
    int month;
    int day;
};

///define a structure for each drug
struct drug {
    char name[30];
    char type[30];
    ///the barcode needs to be unsigned long long because the usual barcode is 12-digit which can be interpreted as an unsigned long long
    unsigned long long barcode;
    ///the structure of date which was defined is used for the expiration date
    struct date expire;
    float price;
    char company[30];
    int stock;
};

///defining the drugs and the count globally so it can be accessible throughout the code
struct drug l[100];
int i=0,count=0;

int main()
{
    ///the login
    int log=1, j=1;
    while (log==1) {
        //this while won't let the program to move forward unless the correct username and password are entered
        log=login();
    }
    ///defining the path of the database
    FILE *file;
    file = fopen(FOLDER,"a+");
    char line[200];
    char *p;
    ///reading each line from the database and the importing them into the array of drugs
    while (fgets(line,200,file)!=NULL) {
        p=strtok(line," ");
        ///this is in order to lowercase all the characters, so that the sort wouldn't be wrong
        for (int y=0; p[y];y++)
            p[y]=tolower(p[y]);
        strcpy(l[count].name,p);
        p=strtok(NULL, " ");
        for (int y=0; p[y];y++)
            p[y]=tolower(p[y]);
        strcpy(l[count].type,p);
        p=strtok(NULL, " ");
        l[count].barcode=strtoull(p,line,10);
        p=strtok(NULL, "/");
        l[count].expire.year=atoi(p);
        p=strtok(NULL, "/");
        l[count].expire.month=atoi(p);
        p=strtok(NULL, " ");
        l[count].expire.day=atoi(p);
        p=strtok(NULL, " ");
        l[count].price=strtod(p, line);
        p=strtok(NULL, " ");
        for (int y=0; p[y];y++)
            p[y]=tolower(p[y]);
        strcpy(l[count].company,p);
        p=strtok(NULL, "/n");
        l[count].stock=atoi(p);
        count++;
    }
    ///the menu
    while (j!=0) {
        /// this function will clear the screen
        system("cls");
        printf("Enter the number according to what you want to do: \n");
        printf("1. Adding new products\n");
        printf("2. Editing/Deleting existing products\n");
        printf("3. The list of the existing products\n");
        printf("0. Exit program\n");
        scanf("%d", &j);
        switch(j) {
            case 1:
                system("cls");
                printf("Adding new products:\n");
                add();
                break;
            case 2:
                system("cls");
                printf("Editing/Deleting existing products:\n");
                editanddelete();
                break;
            case 3:
                system("cls");
                printf("The list of the existing products:\n\n");
                list();
                break;
            case 0:
                ///closing the pointer of the file
                fclose(file);
                save();
                exit(0);
            default:
                system("cls");
                printf("Invalid input\n");
                break;
        }
    }
    return 0;
}

///the login function
int login() {
    ///the variables 'username' and 'password' are the correct username and password to enter
    char username[10]="admin", password[10]="admin", pass[10]={'\0'}, user[10], c;
    printf("Username: ");
    scanf("%s", &user);
    if (strcmp(user, username)==0) {
        int w=0;
        printf("Password: ");
        ///this while will prevent the password that user is typing to be visible
        while( (c=getch())!= '\r') {
            pass[w] = c;
            printf("*");
            w++;
        }
        if (strcmp(password,pass)==0) {
            return 0;
        }
        else {
            system("cls");
            printf("Invalid password\n");
            return 1;
        }
    }
    else {
        system("cls");
        printf("Invalid username\n");
        return 1;
    }
}

void add() {
    int d=1;
    char str[30];
    ///this while will keep repeating until the user wants to go the menu
    while (d==1) {
        printf ("Enter drug name = ");
        scanf ("%s",&str);
        for (int y=0; str[y];y++)
            str[y]=tolower(str[y]);
        strcpy(l[count].name,str);
        printf ("Enter the type of drug = ");
        scanf ("%s",&str);
        for (int y=0; str[y];y++)
            str[y]=tolower(str[y]);
        strcpy(l[count].type,str);
        printf ("Enter company name = ");
        scanf ("%s",&str);
        for (int y=0; str[y];y++)
            str[y]=tolower(str[y]);
        strcpy(l[count].company,str);
        printf ("Enter the barcode nummber = ");
        scanf ("%llu",&l[count].barcode);
        printf ("Enter the expiration date: \n");
        printf ("Year: ");
        scanf ("%d",&l[count].expire.year);
        printf ("Month: ");
        scanf ("%d",&l[count].expire.month);
        ///checks if the month and the day entered are valid, if not it recalls the function
        if (l[count].expire.month>12||l[count].expire.month<0) {
            system("cls");
            printf("Invalid date!\nPlease try again.");
            deletep(count);
            count++;
            add();
            return 1;
        }
        printf ("Day: ");
        scanf ("%d",&l[count].expire.day);
        if (l[count].expire.day>31||l[count].expire.day<0) {
            system("cls");
            printf("Invalid date!\nPlease try again.");
            deletep(count);
            count++;
            add();
            return 1;
        }
        printf ("Enter how many is in stock = ");
        scanf ("%d",&l[count].stock);
        printf ("Enter price = ");
        scanf ("%f",&l[count].price);
        count++;
        printf ("If you want to add another drug into database enter '1', \nif you want to return to the menu enter '0': \n");
        scanf ("%d",&d);
        if (d==0)
            return 1;
        system("cls");
    }
}

void editanddelete() {
    int in, d=1;
    unsigned long long cd;
    char nm[30];
    printf("Enter '1' to search the name of the drug you want to edit/delete,\nenter '2' to search the barcode of the drug you want to edit/delete,\nenter '0' to return to menu:");
    scanf("%d", &in);
    ///this switch will chose the search by name or the search by barcode
    switch (in) {
        case 1:
            printf("Enter the name of the product: ");
            scanf("%s", &nm);
            for (int y=0; nm[y];y++)
                nm[y]=tolower(nm[y]);
            i=search_name(nm);
            break;
        case 2:
            printf("Enter the barcode of the product: ");
            scanf("%llu", &cd);
            i=search_code(cd);
            break;
        case 0:
            system("cls");
            return 1;
    }
    ///the integer 'i' refers to the index of the array which the user wants to edit/delete
    if (i>=0) {
        printf ("\n");
        show(i);
        printf("To delete the product enter '1', to edit enter '2', to return to menu enter '0':");
        scanf("%d", &in);
        switch (in) {
            case 1:
                deletep(i);
                break;
            case 2:
                edit(i);
                break;
            case 0:
                system("cls");
                return 1;
            break;
        }
    }
    else
        printf ("No product found!\n",l[i].name);
    printf("If you want to edit or delete another product enter '1', to return to menu enter '0': ");
    scanf("%d",&in);
    if (in==1) {
        system("cls");
        ///this functions is recalled if the user wants to delete/edit again
        editanddelete();
    }
    else if (in==0) {
        system("cls");
        return 1;
    }
}

void list() {
    ///we first sort the array of the names alphabetically, then show all the arrays one by one
    sort();
    for (i=0;i<count;i++)
        show(i);
    int d;
    printf ("Enter '0' to return to the menu enter\n");
    scanf ("%d",&d);
    switch (d) {
    case 0:
        system("cls");
        return 1;
    }
}

int search_name(char inp[30]) {
    ///the function to search by name, the returned integer refers to the index of the drug asked
    for (int z=0;z<count;z++) {
        if (strcmp(l[z].name,inp)==0)
            return z;
    }
    ///the minus result, shows that nothing was found
    return -1;
}

int search_code(unsigned long long code) {
    ///the function to search by barcode, the returned integer refers to the index of the drug asked
    for (int z=0;z<count;z++) {
        if (l[z].barcode==code)
            return z;
    }
    return -1;
}

void edit(int z) {
    int c,month,day;
    char data[30];
    ///asks which detail of the specified drug needs to be edited
    printf("Enter the number written before of the detail that you want to change: ");
    scanf("%d",&c);
    switch (c) {
        case 1:
            strset(l[z].name,'\0');
            printf("Enter the new name: ");
            scanf("%s", &data);
            for (int y=0; data[y];y++)
                data[y]=tolower(data[y]);
            strcpy(l[z].name,data);
            printf("\n");
            show(z);
            break;
        case 2:
            strset(l[z].type,'\0');
            printf("Enter the new type: ");
            scanf("%s", &data);
            for (int y=0; data[y];y++)
                data[y]=tolower(data[y]);
            strcpy(l[z].type,data);
            printf("\n");
            show(z);
            break;
        case 3:
            l[z].barcode=0;
            printf("Enter the new barcode: ");
            scanf("%llu", &l[z].barcode);
            printf("\n");
            show(z);
            break;
        case 4:
            printf("Enter the new expiration date: \n");
            printf("year: ");
            scanf("%d", &l[z].expire.year);
            printf("month: ");
            scanf("%d", &month);
            ///checks if the month and the day entered are valid, if not it recalls the function
            if (month>12||month<0) {
                system("cls");
                printf("Invalid date!\nPlease try again.");
                show(z);
                edit(z);
                return 1;
            }
            l[z].expire.month=month;
            printf("day: ");
            scanf("%d", &day);
            if (day>31||day<0) {
                system("cls");
                printf("Invalid date!\nPlease try again.");
                show(z);
                edit(z);
                return 1;
            }
            l[z].expire.day=day;
            printf("\n");
            show(z);
            break;
        case 5:
            l[z].price=0;
            printf("Enter the new price: ");
            scanf("%f", &l[z].price);
            printf("\n");
            show(z);
            break;
        case 6:
            strset(l[z].company,'\0');
            printf("Enter the new company name: ");
            scanf("%s", &data);
            for (int y=0; data[y];y++)
                data[y]=tolower(data[y]);
            strcpy(l[z].company,data);
            printf("\n");
            show(z);
            break;
        case 7:
            l[z].stock=0;
            printf("Enter the new number of stock: ");
            scanf("%d", &l[z].stock);
            printf("\n");
            show(z);
            break;
    }
    printf("Done!\n");
}
void deletep(int z) {
    strset(l[z].name,'\0');
    strset(l[z].type,'\0');
    strset(l[z].company,'\0');
    l[z].barcode=0;
    l[z].expire.year=0,l[z].expire.month=0,l[z].expire.day=0;
    l[z].price=0;
    l[z].stock=0;
    count--;
    i=count;
    printf("Done!\n");
}

void show (int z) {
    ///shows the data of the drug, whose index was called
    printf ("1. drug name: %s\n",l[z].name);
    printf ("2. drug type: %s\n",l[z].type);
    printf ("3. barcode number: %llu\n",l[z].barcode);
    printf ("4. expiration date: %d/%d/%d\n",l[z].expire.year,l[z].expire.month,l[z].expire.day);
    printf ("5. price: %f\n",l[z].price);
    printf ("6. company name: %s\n",l[z].company);
    printf ("7. in stock: %d\n",l[z].stock);
    printf("------------------------------------------------\n");
}

void sort() {
    ///a bubble sort, to sort the drug names alphabetically
    if (count<2)
        return 1;
    struct drug swap;
    for (i=0;i<count-1;i++) {
        for (int x=0;x<count-i-1;x++) {
            if (strcmp(l[x].name,l[x+1].name)>0) {
                swap=l[x];
                l[x]=l[x+1];
                l[x+1]=swap;
            }
        }
    }
}

void save() {
    ///first sorts the data before saving
    sort();
    ///deletes the database so that there wouldn't be duplicate of drugs
    int del = remove("database.txt");
    ///again creates the database and saves the sorted drugs, one by one
    FILE *db = fopen(FOLDER,"a+");
    for (i=0;i<count;i++)
        fprintf(db,"%s %s %llu %d/%d/%d %f %s %d\n",l[i].name, l[i].type, l[i].barcode, l[i].expire.year, l[i].expire.month, l[i].expire.day, l[i].price, l[i].company, l[i].stock);
}
