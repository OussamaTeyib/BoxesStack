/* a simple database (stack of boxes) where you can add, modify and remove items (boxes). *
 * Written by Oussama Teyib .                                                             *  
 * February 2023.                                                                         */

#include<stdio.h>
#include<string.h> // for strncpy();
#include<stdlib.h> // for system("cls");
#define MAX 100

typedef struct {
    int id;
    char content[MAX];
} Box;

int main(void)
{
    int id = 1; // for the ids of the boxes
    int option, redo;
    Box box;

    FILE *stack = fopen("stack.bin", "wb+");
    if (!stack)
    {  
       printf("the Stack is closed!\n"); // :)
       return -1;
    }

    // to do multiple things in one execution
    while (1) 
    {
        system("cls");
        printf("Choose an option: \n");
        printf("	1: Add a box\n");
        printf("	2: Modify a box\n");  
        printf("	3: remove a box\n");
        printf("	4: Search for a box\n");
        printf("	5: List boxes\n");
        printf("	0: Exit\n\n");

        do
        {
            printf("Your choice: ");
            fflush(stdin); 
            scanf("%d", &option);
            if (option < 0 || option > 5)
                printf("\nEnter a VALID option\n");
        } while(option < 0 || option > 5);   
   
        switch (option)
        {
            case 1: // adding
                redo = 'Y';
                while ('Y' == redo || 'y' == redo)
                {
                    fseek(stack, 0, SEEK_CUR);
                    box.id = id;
                    printf("Enter the content of the box: ");
                    fflush(stdin);
                    fgets(box.content, MAX, stdin);
                     
                    if (1 == fwrite(&box, sizeof box, 1, stack))
                    {
                        id++; // id counter increase whenever a box is added
                        printf("The box is added\n\n");
                    }

                    printf("Again? (Y/N): ");
                    fflush(stdin);
                    redo = getchar();
                }
                break;

            case 2: // modifying
                redo = 'Y';
                while ('Y' == redo || 'y' == redo)
                {
                    rewind(stack);

                    int tmpID;
                    printf("Enter the id of the box you want to modify: ");
                    scanf("%d", &tmpID);

                    int found = 0;
                    while (1 == fread(&box, sizeof box, 1, stack))
                    {
                        if (box.id == tmpID)
                        {
                            found = 1;
                            char newContent[MAX];
                            printf("Enter the new content: ");
                            fflush(stdin);
                            fgets(newContent, MAX, stdin);
                            strncpy(box.content, newContent, MAX);
                            // cast size_t (sizeof box) to long (offset parametre) to avoid overflowing
                            fseek(stack, -(long)sizeof box, SEEK_CUR);
                            fwrite(&box, sizeof box, 1, stack);
                            printf("The box is modified\n\n");
                            break;
                         }
                    }

                    if (!found)
                        printf("The box is not found!\n\n");

                    printf("Again? (Y/N): ");
                    fflush(stdin);
                    redo = getchar();
                }
                break;

            case 3: // removing               
                redo = 'Y';              
                while ('Y' == redo || 'y' == redo)
                {
                    rewind(stack);

                    int tmpID;
                    printf("Enter the id of the box you want to remove: ");
                    scanf("%d", &tmpID);

                    // create a temp file to store the updated data (after removing the intendded box)
                    FILE *copy = fopen("copy.bin", "wb");
                    if (!copy)
                    {
                        printf("Can not remove the box!");
                        return -1;
                    }

                    int found = 0;
                    while (1 == fread(&box, sizeof box, 1, stack))
                    {
                        if (box.id == tmpID)
                        {
                            found = 1;
                            id--; // id counter decrease whenever a box is removed
                            printf("The box is removed\n\n");
                            continue;  
                        }
 
                        if (found) // if a box is removed, the next boxes must decrese their ids.
                            box.id--; 
 
                        fwrite(&box, sizeof box, 1, copy);
                    }  
                     
                    if (!found)
                    {
                        printf("The box is not found!\n\n");
                        fclose(copy);                          
                        remove("copy.bin");
                    }
                    else
                    {
                        fclose(stack);
                        fclose(copy);
                        remove("stack.bin");
                        rename("copy.bin", "stack.bin");
                        /* "r" mode is necessary because: "w" mode removes the content and the "a" mode doesn't allow *
                         * the modification of the content before the current EOF even if the file is seeked to BOF   */
                        stack = fopen("stack.bin", "rb+");
                    }

                    printf("Again? (Y/N): ");
                    fflush(stdin);
                    redo = getchar();
                }
                break;

            case 4: // searching        
                redo = 'Y';
                while ('Y' == redo || 'y' == redo)
                {
                    rewind(stack);

                    int tmpID;
                    printf("Enter the ID of the box: ");
                    scanf("%d", &tmpID);

                    int found = 0;
                    while(1 == fread(&box, sizeof box, 1, stack))
                    {
                        if (box.id == tmpID)
                        {
                            found = 1;
                            printf("Box #%d\n", box.id);
                            printf("	%s\n", box.content);
                            printf("\n");
                            break;
                        }
                    }

                    if(!found)
                        printf("The box is not found!\n\n");

                    printf("Again? (Y/N): ");
                    fflush(stdin);
                    redo = getchar();
                }
                break;

            case 5: // list the stack
                redo = 'Y';
                while ('Y' == redo || 'y' == redo)
                {
                    rewind(stack);

                    int isEmpty = 1;
                    printf("\nThe content of the stack: \n");
                    while(1 == fread(&box, sizeof box, 1, stack))
                    {
                        isEmpty = 0;
                        printf("Box #%d\n", box.id);
                        printf("	%s\n", box.content);
                        printf("\n");
                    }

                    if (isEmpty)
                        printf("Zzz... The Stack is EMPTY\n\n");

                    printf("Again? (Y/N): ");
                    fflush(stdin);
                    redo = getchar();
                }
                break;

            case 0: // exiting
                fclose(stack);
                remove("stack.bin"); // remove the file. I have no need to it ;)
                printf("Bye!");
                return 0;
        }
    }
}