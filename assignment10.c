#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<windows.h>
#include<time.h>

//for coloured text
#define RESET   "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"

//for bold text
#define BOLD "\033[1m"
#define RESET_B "\033[0m"

typedef struct node         // to display the cutomer history
{
    char name[50];
    char address[50];
    char phone_no[11];
    struct node* next;

}node;

typedef struct customer    //to store the customer details
{
    char name[50];
    char address[50];
    char phone_no[10];
    int amount_to_pay;

} customer;

typedef struct item        //to store item details
{
    char product_no[50];
    char product_name[50];
    int quantity;
    int price;

} item;

typedef struct bill_items  //to store product details
{
    char product_no[50];
    char product_name[50];
    int quantity;
    int price_per_piece;
    int net_price;

} bill_items;

//defining prototypes
void mainMenu();         
void administrator();
void customer_func();


void add_new_products()     //to add new products in store
{

    FILE *products;           //file pointer
    products=fopen("products.csv","a");
    if(products == NULL)
    {
        printf(RED"Errror opening file.\n"RESET);
        //exit(0);
    }
    else
    {
        printf(WHITE"Enter number of products : "RESET);
        int num;          //number of records
        scanf("%d",&num);
        printf("\n");

        item prdct[num];  // prdct is array of structures of datatype item
        for(int i = 0; i < num; i++){

            printf(YELLOW"\nEnter product number : ");          //product details 
            scanf("%s",prdct[i].product_no);

            char c;
            scanf("%c",&c);

            printf("Enter product_name : ");
            scanf("%[^\n]s",prdct[i].product_name);

            printf("Enter quantity : ");
            scanf("%d",&prdct[i].quantity);

            printf("Enter price : ");
            scanf("%d",&prdct[i].price);

            printf("\n"RESET);

            fprintf(products,"%s,%s,%d,%d\n",            //writing into a file products.csv
                            prdct[i].product_no,
                            prdct[i].product_name,
                            prdct[i].quantity,
                            prdct[i].price);

            if(ferror(products))
            {
                printf("Error writing to file");
                //exit(0);
            }
            printf(GREEN"Product Added Successfully.\n"RESET);
            
        }
    }

    fclose(products);
}

void newCustomer()   //to add new customer
{

    FILE *file;
    file = fopen("customer_file.csv","a");
    if(file == NULL) 
    {
        printf("Error opening file.\n");
    }
    else
    {
        customer cstmr;                              //structure of datatype customer
        char c;
        scanf("%c",&c);
                                                     
        printf(YELLOW"\nEnter name of the customer : ");     //taking inputs(details of customer)
        scanf("%[^\n]s",cstmr.name);

        char c1;
        scanf("%c",&c1);

        printf("Enter address of customer : ");
        scanf("%[^\n]s",cstmr.address);

        printf("Enter phone_no of customer : ");
        scanf("%s",cstmr.phone_no);

        printf(""RESET);
        cstmr.amount_to_pay=0;
        
        fprintf(file,"%s,%s,%s,%d\n",                  //writing into file customer.csv
                    cstmr.name,
                    cstmr.address,
                    cstmr.phone_no);

        fclose(file);
        // close file
    }
}

void modify_products(item buy_items[],int size_of_buy_items)  //to modify the availability of products
{

    //reading whole file and storing all info in array (array of structure)

    FILE *prdct=fopen("products.csv","r");
    item array[50];                            // to strore the items available

    int read=0;           
    int records=0;                             //number of records
    do
    {
        read=fscanf(prdct,"%49[^,],%49[^,],%d,%d\n",
                array[records].product_no,
                array[records].product_name,
                &array[records].quantity,
                &array[records].price);


        records++;

        if(read != 4 &&  (!feof(prdct)))
        {
            printf("File formatting issue.\n");
        }

        if(ferror(prdct))
        {
            printf("Error reading file.\n");
        }
    }
    while(!feof(prdct));

    fclose(prdct);
    //close the file

    FILE *file1;                          //just a new name for a file pointer
    file1 = fopen("products.csv","w");    //opening a file
    if(file1 == NULL)
    {
        printf("Error opening file.\n");
    }
    else
    {
        //modifying the array

        for(int i = 0; i < size_of_buy_items; i++)
        {
            for(int j = 0; j < records; j++)
            {
                if(strcmp(array[j].product_no , buy_items[i].product_no) == 0)
                {   
                    array[j].quantity -= buy_items[i].quantity;
                    break;
                }
            }
        }

        //writing modified information into the products.csv file
        for(int i = 0; i < records; i++)
        {
            fprintf(file1,"%s,%s,%d,%d\n",
                            array[i].product_no,
                            array[i].product_name,
                            array[i].quantity,
                            array[i].price);
        }
    }

    fclose(file1);
    //close file
    mainMenu();   //return to mainmen
    

}

void cart()         //to add products in cart
{

    FILE *prdct;
    prdct=fopen("products.csv","r");    //opening the file
    int total_amount=0;                 //total amount of purchased items
    item buy_items[50];                 //to store the purchased items
    item array[50];                     // to strore the items available


    int read=0; 
    int records=0;                      //number of products in products.csv file
    do
    {   
        //reading the data from products.csv file
        read=fscanf(prdct,"%49[^,],%49[^,],%d,%d\n",
                array[records].product_no,
                array[records].product_name,
                &array[records].quantity,
                &array[records].price);

        records++;
        
        if(read != 4 &&  (!feof(prdct)))
        {
            printf(RED"File formatting issue.\n");
        }

        if(ferror(prdct))
        {
            printf("Error reading file.\n"RESET);
        }

    }
    while(!feof(prdct));

    fclose(prdct);
    //close the file


    
    int index = 0;                  //index of array of structs (buy_items)
    int num_of_products_bought = 0;


    buy: printf("");                //label   for   goto    statement 
    item product1;                  //product1 is structure name of datatype item
    printf(CYAN"\nPlease provide the product details which are available\n\n"RESET);


    //taking input for required product
 
    printf(YELLOW"Enter product_no of product to buy : ");
    scanf("%s",product1.product_no);

    char c1;
    scanf("%c",&c1);

    printf("Enter product name : ");
    scanf("%[^\n]s",product1.product_name);

    printf("Enter quantity : ");
    scanf("%d",&product1.quantity);

    printf(""RESET);
    
    for(int i = 0; i < records; i++)
    {
        //checking whether product no and name are same or not 
        if(strcmp(array[i].product_no , product1.product_no) == 0  && strcmp(array[i].product_name,product1.product_name)==0)  
        {   
            if(product1.quantity > array[i].quantity)
            {
                printf(RED"\nNot enoungh quantity available of required product.\n"RESET);
                break;
            }
            else
            {
                array[i].quantity -= product1.quantity;                //updating the quantity in array
                product1.price = array[i].price;    

                buy_items[index] = product1;
                index++;     

                total_amount += (array[i].price * product1.quantity);  
                num_of_products_bought++;
                
                printf(GREEN"\nProduct Added to cart Successfully.\n"RESET);
                break;
            }
        }
        else
        {
            if(i == records-1) 
            {
                printf(RED"Required product is not available.\n"RESET);
            }
        }
    }

    char ch[2];
    printf(YELLOW"\nDo you want to buy more(y/n) : "); 
    scanf("%s",ch);
    printf("\n"RESET);
    
    if(strcmp(ch,"y") == 0){
        goto buy;
    }

    
    FILE *buy;                                 //opening the file
    buy = fopen("bought_items.csv","a");
    if(buy == NULL)
    {
        printf(RED"Error in opening file.\n"RESET);
    }
    else
    {   
        //writing the details of purchased items in bought_items.csv file

        for(int i=0; i<index; i++)
        {
            fprintf(buy,"%s,%s,%d,%d\n",
                    buy_items[i].product_no,
                    buy_items[i].product_name,
                    buy_items[i].quantity,
                    buy_items[i].price);
        }
    }

    fclose(buy);
    //close the file

    //modify the availability of products
    modify_products(buy_items , index);

} 

void customer_history()   //to store the customer information
{

    FILE *file;
    file = fopen("customer_file.csv","r");
    if(file==NULL)
    {
        printf("Error opening file.\n");
    }
    else
    {
        node *head,*p;
        head = (node*)malloc(sizeof(node));

        int a = 0,var;
        p = head;
        printf(BOLD YELLOW"\nCustomer History\n"RESET RESET_B);
        printf(CYAN BOLD"----------------------------------------------------------------");
        printf(BOLD"\n|       Customer Name       Customer Address       Phone Number|\n");
        printf("----------------------------------------------------------------\n"RESET_B RESET);

        while(fscanf(file,"%49[^,],%49[^,],%10[^,],%d\n",p->name,p->address,p->phone_no,&var) > 0)
        {
            node *nextnode;
            nextnode = (node*)malloc(sizeof(node));
            if(a==0)
            {
                head->next = nextnode;
                p = nextnode;
            }
            else{
                p->next = nextnode;
                p = nextnode;
            }
            a++;
        }
        p->next = NULL;

        node *q;
        q = head;
        int r=0;
        while(q != NULL && r<a-1)
        {
            printf(CYAN"|%20s%20s%20s  |\n\n",q->name,q->address,q->phone_no);
            q = q->next; 
            r++;
        }
        printf(BOLD"----------------------------------------------------------------\n\n"RESET RESET_B);

    }

    fclose(file);
    mainMenu();

}

void show_cart()   //to display the products in cart
{

    FILE *bought;
    bought=fopen("bought_items.csv","r");
    int total_amount=0;
    item buy_items[100];    //to store the purchased items

    int read=0; 
    int records=0;
    do
    {
        read = fscanf(bought,"%49[^,],%49[^,],%d,%d\n",
                buy_items[records].product_name,
                buy_items[records].product_no,
                &buy_items[records].quantity,
                &buy_items[records].price);

        // if(read==4) records++;

        records++;
        if(read != 4 &&  (!feof(bought)))
        {
            printf(RED"File formatting issue.\n");
        }

        if(ferror(bought))
        {
            printf("Error reading file.\n"RESET);
        }
    }
    while(!feof(bought));
    fclose(bought);

    printf(BOLD CYAN"----------------------------------------------------------------------------------\n");
    printf("|         Product No        |Product Name           |Quantity      |Price of item|\n");
    printf("----------------------------------------------------------------------------------\n" RESET_B);

    for(int i=0; i < records; i++)
    {
        printf(CYAN"|%20s%20s%20d%20d|\n\n",
                buy_items[i].product_no,
                buy_items[i].product_name,
                buy_items[i].quantity,
                buy_items[i].price);
    }
    printf(BOLD"----------------------------------------------------------------------------------\n\n"RESET);

    fclose(bought);
    mainMenu();

}

void bill()        //to display the bill
{
    FILE *bought;
    bought=fopen("bought_items.csv","r");

    int total_amount=0;
    item buy_items[100];    //to store the purchased items

    int read=0; 
    int records=0;
    do
    {
        read=fscanf(bought,"%49[^,],%49[^,],%d,%d\n",
                buy_items[records].product_no,
                buy_items[records].product_name,
                &buy_items[records].quantity,
                &buy_items[records].price);


        records++;
        if(read != 4 &&  (!feof(bought)))
        {
            printf(RED"File formatting issue.\n");
        }

        if(ferror(bought))
        {
            printf("Error reading file.\n"RESET);
        }
    }
    while(!feof(bought));
    fclose(bought);

    bill_items bill_array[100];  // array of structures
    int ind=0;                   // for indexing in bill_array

    int hashArray[100]={0};
    int total_payable_amount=0;

    for(int i=0; i<records; i++)
    {
        int total_amount_of_particular_item=0;
        int total_quantity_of_particular_item=0;
        
        if(hashArray[i] != -1)
        {
            for(int j=0; j<records; j++)
            {
                if(strcmp(buy_items[i].product_no , buy_items[j].product_no) == 0  && strcmp(buy_items[i].product_name , buy_items[i].product_name) == 0 )
                {   
                    total_quantity_of_particular_item += (buy_items[j].quantity);
                    total_amount_of_particular_item += (buy_items[j].price * buy_items[j].quantity);
                    hashArray[j] = -1;
                }
            }
            total_payable_amount += total_amount_of_particular_item;

            //push bought items details in bill_array
            strcpy(bill_array[ind].product_no , buy_items[i].product_no);
            strcpy(bill_array[ind].product_name , buy_items[i].product_name);

            bill_array[ind].quantity = total_quantity_of_particular_item;
            bill_array[ind].price_per_piece = buy_items[i].price;

            bill_array[ind].net_price = total_amount_of_particular_item;
            ind++;

        }
    }

    // take the customer details
    newCustomer();
    printf(BOLD GREEN"\n                                          ***BILL***  \n"RESET);
    printf(BOLD RED "\n                                  =====  IITP SuperMart  =====    ");
    //current local date and time
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printf (GREEN "\n\nCurrent local time and date: %s\n"RESET, asctime (timeinfo));


    printf(BOLD CYAN"-------------------------------------------------------------------------------------------------- \n");
    printf("|     Product No        Product Name            Quantity          Price of item       Total Price|\n");
    printf("--------------------------------------------------------------------------------------------------\n"RESET);

    //to display the bill
    for(int i = 0; i < ind; i++)
    {
        printf(CYAN"|%15s%20s%20d%20d%20d |\n\n",
                    bill_array[i].product_no,
                    bill_array[i].product_name,
                    bill_array[i].quantity,
                    bill_array[i].price_per_piece,
                    bill_array[i].net_price);
    }

    printf(BOLD"--------------------------------------------------------------------------------------------------\n"RESET);
    printf(BOLD"Total Payable Amount = %d.00 Rupees                                      Contact No.-->8630836751\n\n"RESET,total_payable_amount);
    printf(BOLD RED"                                         THANK YOU ^_^\n                                         Visit Again!!!");
    printf("\n");
    printf(BOLD GREEN"\n                                          **********");
     printf("\n");
    FILE *file = fopen("bought_items.csv","w");    //to remove previous data of bought_items.csv file
    fclose(file);

    mainMenu();

}

void display_available_products()   //to display the available products
{

    FILE *products;
    products=fopen("products.csv","r");

    if(products==NULL)
    {
        printf("Error opening file.\n");
    }
    else
    {
        printf(BOLD CYAN"-----------------------------------------------------------------------------\n");
        printf("|     Product No        Product Name            Quantity       Price of item|\n");
        printf("-----------------------------------------------------------------------------\n"RESET RESET_B);

        item item1[100];   //array of structs to store products

        int read = 0;
        int records = 0;   //number of records in products.csv file
        do
        {

            read=fscanf(products,"%49[^,],%49[^,],%d,%d\n",
                            item1[records].product_no,
                            item1[records].product_name,
                            &item1[records].quantity,
                            &item1[records].price);


            records++;

            if(read != 4 &&  (!feof(products)))
            {
                printf(RED"File formatting issue.\n");
            }

            if(ferror(products))
            {
                printf("Error reading file.\n"RESET);
            }

        }
        while(!feof(products));

        //printing the available product details
        for(int i = 0; i < records; i++)
        {
            printf(CYAN"|%15s%20s%20d%20d|\n\n",
                    item1[i].product_no,
                    item1[i].product_name,
                    item1[i].quantity,
                    item1[i].price);
        }

    }

    printf(BOLD"-----------------------------------------------------------------------------\n"RESET RESET_B);
    printf("\n");

    fclose(products);
    mainMenu();

}

void administrator()                //administrator options
{
    
    printf(BOLD YELLOW"\n01.Add New Products\n\n");
    printf("02.Display Available Products\n\n");
    printf("03.Show Customer History\n\n");
    printf("04.Main Menu\n\n"RESET);


    printf(CYAN"Please Enter an Option Number(1/2/3/4): "RESET);
    int choice;
    scanf("%d",&choice);

    switch (choice) 
    {
        case 1:
            add_new_products();
            break;
        case 2:
            display_available_products();
            break;
        case 3:
            customer_history();
            break;
        case 4:
            mainMenu();
            break;
        default:
            printf("Wrong choice\n Please select a valid choice\n");
            break;
    }

    mainMenu();

}

void customer_func()             //customer options
{
    
    printf(BOLD YELLOW"\n01.Display Available Products\n\n");
    printf("02.Buy/Add in Cart\n\n");

    printf("03.Cart\n\n");
    printf("04.Bill\n\n");

    printf("05.MainMenu\n\n");
    printf("\n"RESET);

    printf(CYAN"Please Enter an Option Number(1/2/3/4/5): "RESET);

    int choice;
    scanf("%d",&choice);
    
    switch (choice)
    {
        case 1:
            display_available_products();
            break;
        case 2:
            cart();
            break;
        case 3:
            show_cart();
            break;
        case 4:
            bill();
            break;
        case 5:
            mainMenu();
            break;
        default:
            printf(BOLD RED"Wrong choice\n Please select a valid choice\n"RESET RESET_B);
            break;
    }

}

void mainMenu()             //mainmenu
{
    char str[200];
    printf(BOLD WHITE" _____________________________________________________________________________________________________________________");
    strcpy(str,BOLD WHITE"\n                                       Welcome to IITP SuperMart   \n"RESET_B RESET);

    for(int i=0; i<strlen(str); i++)
    {
        printf("%c",str[i]);
        Sleep(25);
    }
    
    
    mainmenu: printf("");

    printf(BOLD YELLOW"\n01. ADMINISTRATOR\n\n");
	printf("02. CUSTOMER\n\n");
    printf("03. ABOUT US\n\n");
	printf("04. EXIT\n\n"RESET);

	printf(CYAN"Please Enter an Option Number(1/2/3/4): "RESET);

    int choice;
    scanf("%d",&choice);

    char ch,password[10],q[10]="sonalika";

    switch (choice)
    {
        case 1:
            printf("Enter Password:\t");
            int i=0;
            for(i = 0; i < 8; i++)
            {
                ch = getch();
                password[i] = ch;
                ch = '*' ;
                printf("%c",ch);
            }

            password[i]='\0';

            if(strcmp(password,q))
            {
                printf("\n\t");
                printf(RED"\nWrong Password Please Try Again"RESET);
                printf("\n\n");
                goto mainmenu;
            }
            else
            {   
                printf(GREEN"\n\n--> Access Granted\n"RESET);
                administrator();
            }
            break;
        case 2:
            customer_func();
            break;
        case 3:

        printf(BOLD WHITE"           ---------------------------------------------------------------------------------------------------\n"RESET);
            printf(BOLD WHITE"                    The Objective of E-Billing System is to provide a more efficient, cost-effective, \n                    and environment friendly way for business to bill and recieve payments from customers,\n                      while also streamlining the billing process and improve customer satisfaction.\n                           Additionally, we focussed on ensuring smooth experience for customers\n                             and allow businesses to generate bills, manage customer records \n                                  and add the products in stock. \n\n");
           printf(BOLD RED"                                           ^^^^  HOPE U LIKE OUR PROJECT  ^^^^ \n");
          
           printf(WHITE"            ---------------------------------------------------------------------------------------------------\n");
           goto mainmenu;
            break;
        case 4:
            printf(BOLD RED"                                         THANK YOU ^_^\n                                         Visit Again!!!\n"RESET);
            exit(0);
            break;
        default:
            printf(RED"Wrong choice\nPlease select a valid choice\n"RESET);
            goto mainmenu;
            break;
    }

}

int main()
{
    mainMenu();

}