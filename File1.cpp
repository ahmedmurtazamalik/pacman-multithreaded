#include <iostream>
#include <sys/types.h>
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h>

#include "movement.h"

using namespace std;
using namespace sf;
sem_t intro;

pthread_t movementT;
pthread_t getcoinsT;

bool intro1=0;


void prestartingwork(Event &e)
{    
        while(GW.RWMain->pollEvent(e))
        {
            GW.RWMain->clear();
            if(e.type == Event::KeyPressed)
            {
                if(e.key.code == Keyboard::Enter)
                {
                    intro1 =1;
                    break;
                }
            }
            else if (e.type == sf::Event::TextEntered)
            {
                if (e.text.unicode < 128)
                {
                    name += (char)e.text.unicode;
                }
            }
            showInputname();
            GW.RWMain->draw(*GW.getinput);
            GW.RWMain->draw(*GW.introText);
            GW.RWMain->draw(*GW.name);
            GW.RWMain->display();

        }

    

    
}
void * setbasics(void * args)
{
    

    //setting walls in maze array
    //outside walls
    for(int i =0; i<GW.blocksWidth; i++)
    {
        wallsArr[i][0]=1;
        wallsArr[i][GW.blocksHeight-1] =1;
    }

    for(int i=0; i< GW.blocksHeight; i++)
    {
        wallsArr[0][i] = 1;
        wallsArr[GW.blocksWidth-1][i] =1;
    }
    //central pathways
    wallsArr[0][GW.blocksHeight/2] =0;
    wallsArr[GW.blocksWidth-1][GW.blocksHeight/2] =0;

    //all inside walls
    placeWalls(); 
    //placing coins
    CoinsSet();
    
    setStartingText();
    setupGhosts();
    getUserInputText();
    
    pthread_exit(NULL);
}

int main()
{
    Event e;
    srand(time(0));
    pthread_t introT;
    pthread_t GhostsT[4];
    sem_init(&intro,0,0);
    sem_init(&movementSem,0,1);
    sem_init(&coinSem,0,1);
    Clock clock;

    double time;
    // pthread_mutex_init(&ghost1,nullptr);
    // pthread_mutex_init(&ghost2,nullptr);
    // pthread_mutex_init(&ghost3,nullptr);
    // pthread_mutex_init(&ghost4,nullptr);
    time = clock.getElapsedTime().asSeconds();
    pthread_create(&introT, NULL, setbasics, NULL);
    usleep(200);
    pthread_mutex_init(&movementMutex, nullptr);
    pthread_mutex_init(&coinsMutex, nullptr);

    int idx =0;
    
    pthread_create(&movementT, NULL, movement, NULL);
    usleep(300);
    pthread_create(&getcoinsT, NULL, getCoins, NULL);
    usleep(300);
    
    for(int i =0; i< 4; i++)
    {
        pthread_create(&GhostsT[i], NULL, moveGhosts, (void*)&Gptr[i]);
        usleep(300);
    }

    while(!intro1)
        prestartingwork(e);

    while(1)
    {
        while(GW.RWMain->pollEvent(e))
        {

            if(e.type == Event::KeyPressed)
            {
                if(e.key.code == Keyboard::X)
                {
                    pthread_mutex_destroy(&movementMutex);
                    pthread_mutex_destroy(&coinsMutex);
                
                    GW.RWMain->close();
                    return 0;
                }
            }
        }
        down =0; rightpress=0;
        double speed =0.5;
        inputupdate =0;
        userinputHandler();
        //time = clock.getElapsedTime().asSeconds();
        
        if(!lives)
        {
            GW.RWMain->close();
            return 0;
        }

        GW.time = GW.clock.getElapsedTime().asSeconds();
        //return 0;
        Draw();
        drawKey();
        showScore();
    }
    

    return 0;
}