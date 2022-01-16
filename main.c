
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct Movie
{
    char * title;
    int year;
    char * languages;
    double rating;
    struct Movie *next;
};
int menu(struct Movie* head, struct Movie*[]);
void movieYear(struct Movie*, int); // Movie* means this will result in a Movie whereas *Movie means give me address ??
void specificLang(struct Movie*, const char*);

struct Movie* createMovie(char * title, int year, char * langs, double r) {
    struct Movie *currMovie = malloc(sizeof(struct Movie));

    currMovie->title = calloc(strlen(title) + 1, sizeof(char));
    strcpy(currMovie->title, title);

    currMovie->year = year;

    currMovie->languages = calloc(strlen(langs) + 1, sizeof(char));
    strcpy(currMovie->languages, langs);

    currMovie->rating = r;

    currMovie->next = NULL;

    return currMovie;

}

const int maxYear = 2021;
const int minYear = 1900;
const int totalYears = maxYear - minYear;
const int RUN = -1459;
const int END = -2593;

int main(void) {


    struct Movie* highestRating[totalYears];
    for (int i = 0; i < totalYears; ++i) {
        highestRating[i] = NULL;
    }
    char* newFilePath = "./newFile.txt";
    char* moviePath = "C:\\Users\\njc19\\CLionProjects\\untitled\\res\\movies.csv";

    char buffer[100];

    FILE *fp;
    fp = fopen(moviePath, "r");
    struct Movie* head = NULL;
    struct Movie* tail = NULL;
    int linesRead = 0;
    int validMovies = 0;
    while(!feof(fp))
    {

        char * input = fgets(buffer, 100,fp);
        char * title;
        char * year;
        char * languages;
        char * rating;

        struct Movie* currentMovie;

        if(linesRead > 1)
        {


            title = strtok(input, ",");


            year = strtok(NULL,",");

            if(year != NULL)
            {
                int y = atoi(year);

                languages = strtok(NULL,",");

                rating = strtok(NULL,",");

                char * temp; //used to create a double
                double r = strtod(rating,&temp);



                currentMovie = createMovie(title,y,languages,r);

                // updating array with highest ratings for each year starting at 1900
                int index = y - minYear;

                if(highestRating[index] != NULL)
                {
                    double previousRating = highestRating[index]->rating;
                    double currentRating = r;
                    if(currentRating > previousRating){
                        highestRating[index] = currentMovie;
                    }
                }
                else
                {
                    highestRating[index] = currentMovie;
                }




                if(head == NULL)
                {
                    head = currentMovie;
                    tail = currentMovie;
                }
                else
                {
                    tail->next = currentMovie;
                    tail = currentMovie;
                }
                validMovies++;
            }



        }

        linesRead++;


    }

    printf("Processed file %s for %d movies\n", moviePath, validMovies);

    int value = RUN;
    while(value == RUN)
    {
        value = menu(head,highestRating);
    }

    fclose(fp);

    /**
     * 1. be able to read the file -done
     *
     * 2. Break the file into rows - done
     *
     * 3. Break a row in to values delimited by commas -done!!!!!
     *
     *      3.1 We can always expect 4 values
     *          Value1 = The title
     *          Value2 = Year
     *          Value3 = Languages
     *          Value4 = Rating
     *  3.99 Print processed file
     *  4. Begin to create a menu
     *
     *  ... more intstructions here!
     */


    return 0;

}


int menu(struct Movie* head, struct Movie* ratingArray[])
{
    printf("\n1. Show movies released in the specified year\n"
           "2. Show highest rated movie for each year\n"
           "3. Show the title and year of release of all movies in a specific language\n"
           "4. Exit from the program\n"
           "Enter a choice from 1 to 4\n");

    int choice;
    scanf("%d", &choice);

    //printf("%d", choice);

    switch(choice){
        case 1:
            printf("Please enter target Year ->");
            int targetYear;
            scanf("%d", &targetYear);
            movieYear(head,targetYear);
            return RUN;
        case 2:

            for (int i = 0; i < totalYears; ++i) {

                if(ratingArray[i] != NULL)
                {
                    struct Movie* currentMovie = ratingArray[i];
                    printf("%d %.1f %s\n", currentMovie->year, currentMovie->rating, currentMovie->title);
                }

            }

            return RUN;
        case 3:
            printf("Enter the language for which you want to see:");
            char targetLang[30];
            scanf("%s", targetLang);
            specificLang(head, targetLang);
            return RUN;
        case 4:
            printf("Quitting... ");
            return END;

    }

    return END;
}

void movieYear(struct Movie* head, int targetYear){
    struct Movie* iter = head;
    int moviesFound = 0;
    while(iter != NULL)
    {
        if(iter->year == targetYear) {
            printf("%s \n", iter->title);
            moviesFound++;
        }
        iter = iter->next;

    }

    if(moviesFound == 0){
        printf("No movies found for the year: %d \n", targetYear);
    }
}

void specificLang(struct Movie* head,  const char* targetLang){
    struct Movie* iter = head;
    int moviesFound = 0;

    while(iter != NULL)
    {
        char * currentString = iter->languages;
        char * token = strtok(currentString, "[];");
        int compValue = strcmp(token, targetLang);
        if(compValue == 0){//this means that they will be equivalent
            printf("%d %s\n", iter->year, iter->title);
            moviesFound++;
        }
        else{
            token = strtok(NULL, ";");
            while(token != NULL){

                compValue = strcmp(token, targetLang);
                if(compValue == 0){//this means that they will be equivalent
                    printf("%d %s\n", iter->year, iter->title);
                    moviesFound++;

                }
                token = strtok(NULL, "[];");

            }

        }
        iter = iter->next;
    }

    if(moviesFound == 0){
        printf("No movies found for the Language: %s \n", targetLang);
    }
}