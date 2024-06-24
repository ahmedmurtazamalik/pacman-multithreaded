#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

//This file contains the most basic items that the game can not function without whatsoever
sf::Music music;
sem_t movementSem;
sem_t coinSem;
bool inputupdate =0;

pthread_mutex_t movementMutex;
pthread_mutex_t coinsMutex;


int ghostincs=0;

#define WIDTH 920
#define HEIGHT 920

using namespace std;
using namespace sf;


string name;
int down;
int rightpress;
bool movementRn=0;
int numberofPellets=2;

bool ** wallsArr;
bool ** coins;
int score =0;
int lives =3;
bool powerup1=1;
bool powerup2=1;
int p1x, p1y;
int p2x, p2y;

struct Ghost
{
    double x;
    double y;
    bool on;
    bool inhome;
    double totalTime;
    double speed;
    bool moving =0;
    int dir;
    int Gid;


    Ghost()
    {
        on =0;
        x =0;
        y =0;
        inhome=0;
        Gid =0;
        
    }
    Ghost(int val)
    {
        on =0;
        x =0;
        y =0;
        inhome=0;
        totalTime = val;
        Gid =0;
        
    }
    void setSpeed()
    {
        speed = HEIGHT/(totalTime*950);
        //speed is in pixels per minute now
    }
};
Ghost * Gptr;

struct GameWindow
{
    RenderWindow *RWMain; //Main window
    Texture * keyt;
    Sprite * keyS;

    Texture * cherry;
    Sprite * cherrySprite;
    bool cherryeaten;
    bool ghostsblue;    
    bool cherry1;
    bool cherry2;

    Texture * BG_T;//Background
    Texture * Wall_T; 
    
    Sprite * BG_S; //Wall
    Sprite * Wall_S;
    
    int blocksWidth; //grid values
    int blocksHeight;
    
    Texture *coinT; //coin
    Sprite * coinS;
    
    Texture * pack; //pacman (reference to pack.png from minecraft @ home)
    Sprite * PacS;  
    
    Text * introText;
    Text * getinput;
    
    Text *t1; //Text for main game
    Font *F1;
    
    Text* name;
    Texture * portal;
    Sprite * portalSprite;

    Texture * heartT;
    Sprite * HeartS;
    Texture * T_g1;
    Texture * T_g2;
    Sprite * S_g1;
    Sprite * S_g2;

    Sprite * cherrySprite2;

    Clock clock;

    double time;
    double starttime;
    Texture * T_g3;
    Texture * T_g4;
    Sprite * S_g3;
    Sprite * S_g4;
    Texture * T_g5;
    Sprite * S_g5;

    Texture * powerup;
    Sprite * PowerSprite;
    Sprite * PowerSprite2;
    bool introDone;
    
    bool ** MazeSet();
    bool ** CoinSet();
    GameWindow() //setting background and loading wall texture
    {
        
	    if (!music.openFromFile("GameMusic.ogg")) //playing Toby Fox's Knock You Down in the background of the game
	    {
		    std::cout<<"File not found"<<std::endl;
	    }
	
	    music.play();
	    music.setVolume(40.f); //Reducing sound to make sound effects audible
	
        cherryeaten =0;
        ghostsblue=0;
        introDone =0;
        name = new Text;
        cherry1 =1;
        cherry2 =1;

        cherry = new Texture;
        cherrySprite = new Sprite;
        cherrySprite2 = new Sprite;

        cherry->loadFromFile("power.png");
        cherrySprite->setTexture(*cherry);
        cherrySprite2->setTexture(*cherry);

        cherrySprite2->setPosition(WIDTH/2-10, 80);

        this->F1 = new Font; //setting up fonts
        this->F1->loadFromFile("ROCK.TTF");
        
        BG_S = new Sprite;
        Wall_S = new Sprite;
        RWMain = new RenderWindow(VideoMode(WIDTH+160,HEIGHT+50) ,"PacMan");
        
        blocksWidth = WIDTH / 40;
        blocksHeight = HEIGHT / 40;

        cherrySprite->setPosition(WIDTH/2-15, HEIGHT-80);
        
        BG_T = new Texture;
        Wall_T = new Texture;

        coinS = new Sprite;
        coinT = new Texture;

        pack = new Texture;
        PacS = new Sprite;
        // pack.png stands as a reference to the minecraft@home's pack.png hunt
        
        pack->loadFromFile("pack.png"); //image of pacman
        PacS->setTexture(*pack);
        PacS->setPosition(WIDTH/2, HEIGHT/2);
        PacS->setScale(0.6,0.6);

        portal = new Texture;
        portal->loadFromFile("portal.png");
        portalSprite = new Sprite;
        portalSprite->setTexture(*portal);
        
        heartT = new Texture;
        heartT->loadFromFile("heart.png");

        HeartS = new Sprite;
        HeartS->setTexture(*heartT);

        coinT->loadFromFile("coin.jpg");
        coinS->setTexture(*coinT);
        coinS->setScale(0.08,0.08);
        
        this->t1 = new Text;

        this->t1->setFont(*this->F1);


        this->t1->setCharacterSize(32); //setting text size
        this->t1->setFillColor(sf::Color::White); //setting text color
        this->t1->setStyle(sf::Text::Bold | sf::Text::Underlined); //formatting
        this->t1->setPosition(50,920);
    

        RWMain->draw(*BG_S);
        Wall_T->loadFromFile("Wall.jpg");
        Wall_S->setTexture(*Wall_T);
        wallsArr = MazeSet();
        coins = CoinSet();


        T_g1 = new Texture;
        T_g1->loadFromFile("g1.png"); //add ghost red here
        T_g2 = new Texture;
        T_g2->loadFromFile("g2.png"); //add ghost blue here
        
        
        S_g1 = new Sprite;
        S_g1->setTexture(*T_g1);
        S_g2 = new Sprite;
        S_g2->setTexture(*T_g2);

        T_g3 = new Texture;
        T_g3->loadFromFile("g1.png"); //add ghost red here
        T_g4 = new Texture;
        T_g4->loadFromFile("g2.png"); //add ghost blue here
        
        S_g3 = new Sprite;
        S_g3->setTexture(*T_g1);
        S_g4 = new Sprite;
        S_g4->setTexture(*T_g2);

        T_g5 = new Texture;
        T_g5->loadFromFile("g3.png");
        S_g5 = new Sprite;
        S_g5->setTexture(*T_g5);
        

        introText = new Text;
        introText->setFont(*this->F1);

        keyt = new Texture;
        keyt->loadFromFile("key.png");

        keyS = new Sprite;
        keyS->setTexture(*keyt);
        
        name->setFont(*F1);
      
        name->setPosition(200,200);
        name->setCharacterSize(40); 
        name->setFillColor(sf::Color::Green);
        //name->setStyle(sf::Text::Bold | sf::Text::Underlined);

        powerup = new Texture;
        PowerSprite = new Sprite;
        powerup->loadFromFile("orb.png");
        PowerSprite->setTexture(*powerup);    
        PowerSprite2 = new Sprite;
        PowerSprite2->setTexture(*powerup);
        PowerSprite->setPosition(WIDTH-160, 160);
        PowerSprite2->setPosition(WIDTH-160, HEIGHT -200);
        PowerSprite->setScale(0.3,0.3);
        PowerSprite2->setScale(0.3,0.3);

        p1x = (int(WIDTH-160)/40);
        p1y = (int(160)/40);
        p2x = (int(WIDTH-160)/40);
        p2y = (int(HEIGHT-200)/40);
    }
    void Reset() //resetting logic
    {
        
    }


}GW; //Game Window object GW

bool ** GameWindow::MazeSet()
{   
    int b = this->blocksWidth, h = this->blocksHeight; // currently 20 and 32
    bool ** a = new bool*[b];
    for(int i =0; i<b; i++)
    {
        a[i] = new bool[h]();
        for(int j =0; j<h; j++)
            a[i][j] = 0;
    }
    return a;
}
bool ** GameWindow::CoinSet()
{
    int b = this->blocksWidth, h = this->blocksHeight;

    bool ** a = new bool*[b];
    for(int i =0; i< b; i++)
    {
        a[i] = new bool[h];
        for(int j =0; j< h; j++)
            a[i][j] =0;
    }
    return a;
}

void showInputname()
{
    GW.name->setString(name);
}
void showScore()
{
    
    GW.t1->setString("Score -> " + std::to_string(score));
    GW.RWMain->draw(*GW.t1);
}

void eatCherry()
{

}