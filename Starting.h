#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "essentials.h"
using namespace std;
using namespace sf;

void getUserInputText()
{
    GW.getinput = new Text;
    GW.getinput->setFont(*GW.F1);
    GW.getinput->setPosition(100,100);
    GW.getinput->setCharacterSize(40); 
    GW.getinput->setFillColor(sf::Color::White);
    GW.getinput->setStyle(sf::Text::Bold | sf::Text::Underlined);
    GW.getinput->setString("Enter your name during this window display");

}
void turnGhostsBlue() {

    GW.S_g1->setTexture(*GW.T_g5);
    GW.S_g2->setTexture(*GW.T_g5);
    GW.S_g3->setTexture(*GW.T_g5);
    GW.S_g4->setTexture(*GW.T_g5);
    GW.ghostsblue = 1;
}
void turnGhostsUnBlue() {
    
    GW.S_g1->setTexture(*GW.T_g1);
    GW.S_g2->setTexture(*GW.T_g2);
    GW.S_g3->setTexture(*GW.T_g3);
    GW.S_g4->setTexture(*GW.T_g4);
    GW.S_g1->setColor(Color::White);
    GW.S_g2->setColor(Color::White);
    GW.S_g3->setColor(Color::White);
    GW.S_g4->setColor(Color::White);
    GW.ghostsblue = 0;
}

void setStartingText()
{
    GW.introText->setString("Press enter to start the game!");

    GW.introText->setCharacterSize(32); 
    GW.introText->setFillColor(sf::Color::White);
    GW.introText->setStyle(sf::Text::Bold);
    GW.introText->setPosition((3*WIDTH/4)- WIDTH/2,3*HEIGHT/4);
    

   
}

void placeWalls() //makes the corner shapes 
{
    int h =GW.blocksHeight, b = GW.blocksWidth;
    wallsArr[2][2] =1;
    wallsArr[b-3][2] =1;
    wallsArr[b-3][h-3] =1;
    wallsArr[2][h-3] =1;  

    // the next loop under the top one
    wallsArr[2][6] = 1;
    wallsArr[2][h-7] =1;
    wallsArr[b-3][6] =1;
    wallsArr[b-3][h-7] =1;



    for(int i =1; i< 3;i++)
    {
        wallsArr[4][i]=1; //vertical left wall
        wallsArr[b-5][i] =1; //vertical right wall

        wallsArr[i][4]=1; //horizontal left wall
        wallsArr[i][h-5] =1; //horizontal left lower wall

        wallsArr[b-i-1][h-5] =1; //lower right horizontal
        wallsArr[b-5][h-i-1]=1; //lower right vertical

        wallsArr[4][h-i-1] =1; //lower left vertical
        wallsArr[b-i-1][4] =1; //upper left horizontal

    }

    for(int i =1;i<6;i++) // Secondary loops
    {
        wallsArr[4][i + 3] =1;   //vertical left
        wallsArr[b-5][i + 3] =1; //vertical right

        wallsArr[4][h-4-i] =1;   //vertical lower left
        wallsArr[b-5][h-4-i] =1; //vertical lower right

        wallsArr[i-1][8] = 1;    //horizontal upper left
        wallsArr[b-i][8] = 1;    //horizontal upper right

        wallsArr[i-1][h-9] = 1 ; //horizontal lower left
        wallsArr[b-i][h-9] = 1;  //horizontal lower right
    }

    //secondary loops done

    //now for the "T o T" 
    for(int i =0; i<5 ; i++)
    {
        wallsArr[i+6][2] = 1;   // --
        wallsArr[i+6+6][2] = 1; // -- --
    
        wallsArr[i+6][h-3] =1;  // ^ neechay 
        wallsArr[i+12][h-3] =1;

        wallsArr[8][3+i] =1;    // |
        wallsArr[14][3+i] =1;   // |  |

        wallsArr[8][h-3-i] =1;  // ^ neechay
        wallsArr[14][h-3-i] =1;

        wallsArr[2+i][10] = 1;
        wallsArr[2+i][12] =1;
        wallsArr[b-3-i][10] =1;
        wallsArr[b-3-i][12] =1; 
    }
    
    wallsArr[8][7] =0;
    wallsArr[14][7] =0;
    
    // T T done
    
    // [] to do 

    for(int i =0; i< 3; i++)
    {
        wallsArr[10][4+i]=1;//|
        wallsArr[12][4+i]=1;// | |

        wallsArr[10 + i][4]=1;// |-|
        wallsArr[10+i][6]=1; //  []

        wallsArr[10][h-i-5]=1;//|
        wallsArr[12][h-i-5]=1;// | |

        wallsArr[10 + i][h-5-2]=1;// |-|
        wallsArr[10+i][h-5]=1; //  []
    }
    // ToT done

    for(int i =0; i< 7; i++)
    {
        //horizontal work
        wallsArr[8+i][14] = 1;
        wallsArr[8+i][12] = 1;
        wallsArr[8+i][10] = 1;
        wallsArr[8+i][8]  = 1;

        //vertical work
        wallsArr[6][4 + i] = 1;
        wallsArr[b-7][4+i] = 1;

        wallsArr[6][7 + 4 + i ] = 1;
        wallsArr[b-7][7 + 4 + i] = 1;

    }

    wallsArr[8+3][10] =0;
    wallsArr[8][11] = 1;
    wallsArr[8+6][11] = 1;
    
    wallsArr[6][11] =0;
    wallsArr[8+6+2][11] =0; 

}

void CoinsSet()//place coins in all their areas
{
    int breadth = GW.blocksWidth;
    int height = GW.blocksHeight;
    for(int i =0; i< breadth; i++)//coins in everyplace without walls
    {
        for(int j =0; j<height; j++)
        {
            if(!wallsArr[i][j])
                coins[i][j] =1;
        }
        
        //add exceptions to the rule
        for(int i=9; i<14; i++)
            coins[i][GW.blocksHeight/2] =0;
        // "T O T"        
        coins[11][GW.blocksHeight-6] = 0;
        coins[11][5] = 0;

        // { }
        coins[0][GW.blocksHeight/2] = 0;
        coins[GW.blocksWidth-1][GW.blocksHeight/2] =0;

        coins[GW.blocksWidth/2][GW.blocksHeight-2]=0;

    }


}


void Draw() //draws maze and the walls in it
{
    GW.RWMain->draw(*GW.BG_S);
    int b, h;
    b = GW.blocksWidth;
    h = GW.blocksHeight;
    for(int i =0; i< b; i++)
    {
        for(int j =0; j < h; j++)
        {
            if(wallsArr[i][j])
            {
                GW.Wall_S->setPosition(i * 40, j * 40);
                GW.RWMain->draw(*GW.Wall_S);
            }
        }
    }
    
    for(int i=0; i< b ; i++)
    {
        for(int j=0; j< h; j++)
        {
            if(coins[i][j])//draw coins there
            {
                GW.coinS->setPosition(i*40 + 10, j * 40 + 10);
                GW.RWMain->draw(*GW.coinS);
            }
        }
    }

    GW.S_g1->setPosition(Gptr[0].x,Gptr[0].y);
    GW.S_g2->setPosition(Gptr[1].x,Gptr[1].y);
    GW.S_g3->setPosition(Gptr[2].x,Gptr[2].y);
    GW.S_g4->setPosition(Gptr[3].x,Gptr[3].y);

   
    GW.RWMain->draw(*GW.S_g1);
    GW.RWMain->draw(*GW.S_g2);
    GW.RWMain->draw(*GW.S_g3);
    GW.RWMain->draw(*GW.S_g4);


    GW.portalSprite->setPosition(WIDTH-40, HEIGHT/2 - 20);
    GW.RWMain->draw(*GW.portalSprite);
    GW.portalSprite->setPosition(0, HEIGHT/2 - 20);
    GW.RWMain->draw(*GW.portalSprite); 

    if(GW.cherry1)
    {
        GW.RWMain->draw(*GW.cherrySprite);

    }
     if(GW.cherry2)
    {
        GW.RWMain->draw(*GW.cherrySprite2);

    }


    GW.portalSprite->setPosition(WIDTH-40, HEIGHT/2 - 20);
    GW.RWMain->draw(*GW.portalSprite);
    GW.portalSprite->setPosition(0, HEIGHT/2 - 20);
    GW.RWMain->draw(*GW.portalSprite); 
    
    GW.RWMain->draw(*GW.PacS);
    for(int i =0; i<lives; i++)
    {
        GW.HeartS->setPosition(2*WIDTH/3 + i * 45, HEIGHT + 10);
        GW.RWMain->draw(*GW.HeartS);
    }
    if(powerup1)
    {
        GW.RWMain->draw(*GW.PowerSprite);
    }
    if(powerup2)
    {
        GW.RWMain->draw(*GW.PowerSprite2);
    }

    GW.RWMain->display();
    GW.RWMain->clear();
}