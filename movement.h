#include "Starting.h"

bool collision(double x, double y,int &a, int &b)
{

    double vres = y/40 - int(y/40);
    bool rightcheck =0, hwide =0;
    double hres = x/40 - int(x/40);
    double remainingheight = 1- vres;

    
    if(vres > 0.40)
        rightcheck=1;
    if(hres > 0.40)
        hwide =1;


    if(!a && !b)//no movement
        return 0;

    
    
    if(a<0) //left collision
    {
        if(wallsArr[int(x/40)][int(y/40)])
            a =0;
        if(rightcheck)
            if(wallsArr[int(x/40)][int(y/40)+1])
                a=0;;

    }
    else if(a>0) //right
    {
        if(hwide)
            if(wallsArr[int(x/40)+1][int(y/40)])
                a=0;
        if(rightcheck)
            if(wallsArr[int(x/40)+1][int(y/40)+1])
                a=0;
    }
    
    if(b > 0)
    {
        
        
        if(wallsArr[int(x/40)][int(y/40)+1] && (remainingheight < 0.65))
            b=0;
        if(hwide)
            if(wallsArr[int(x/40)+1][int(y/40)+1])
                b=0;
    }
    if(b<0)
    {
        if(wallsArr[int(x/40)][int(y/40)-1] && (vres < 0.2))
        { 
            b=0;
            return 1;
        }
        if(hwide)
           if(wallsArr[int(x/40)+1][int(y/40)-1] && (vres < 0.2))
               b=0;
    }



    return 0;
}

void *  checkCollisionWithCherry(void * args) {
    
    FloatRect pacmanBounds = GW.PacS->getGlobalBounds();
    FloatRect cherryBounds = GW.cherrySprite->getGlobalBounds();
    FloatRect cherryBounds2 = GW.cherrySprite2->getGlobalBounds();

    // Check if Pacman bounding box intersects with the cherry bounding box
    if (pacmanBounds.intersects(cherryBounds) && GW.cherry1) { //hits cherry1 
        
        
        numberofPellets--;
        turnGhostsBlue();
        GW.cherry1 =0;
        GW.starttime = GW.clock.getElapsedTime().asSeconds();
        
        
    }
    if(pacmanBounds.intersects(cherryBounds2) && GW.cherry2 ) //hits cherry 2
    {
        numberofPellets--;
        turnGhostsBlue();
        GW.cherry2=0;
        GW.starttime = GW.clock.getElapsedTime().asSeconds();

    }
    
    if(GW.time > (GW.starttime + 5))
    {
        turnGhostsUnBlue();
        GW.cherry1 =1;
        GW.cherry2 =1;
    }

    return nullptr;

}
void* getCoins(void* args)
{
    while(1)
    {
       
        pthread_mutex_lock(&coinsMutex);
        //fetching ghost x, y values
        
        double x = GW.PacS->getPosition().x; 
        double y = GW.PacS->getPosition().y;
        //finding horiozntal and vertical deviation
        double h_dev = x - int(x);
        double v_dev = y - int(y);

        
        pthread_mutex_unlock(&coinsMutex);
        
        
        if(coins[int(x/40)][int(y/40)]) //coin on exact block
        {
            coins[int(x/40)][int(y/40)] =0;
            score++;
        }
    }
        
return nullptr;    
}

void userinputHandler(void)
{

        if (Keyboard::isKeyPressed(Keyboard::Up))   //Just another way to detect key presses without event listener
        {
            down =-1;
        }            
        else if(Keyboard::isKeyPressed(Keyboard::Down)){
                down =1;
        }
        
        if(Keyboard::isKeyPressed(Keyboard::Right)){
            rightpress =1;
        }
        else if(Keyboard::isKeyPressed(Keyboard::Left)){
            rightpress =-1;
        }
        inputupdate=1;
}
void* movement(void* args)
{
    while(1){
        
        double speed = 0.08;
        pthread_mutex_lock(&movementMutex);
        usleep(200);
        pthread_mutex_lock(&coinsMutex);
        double x = GW.PacS->getPosition().x;
        double y = GW.PacS->getPosition().y;
    
        while(!inputupdate);
        if(down && rightpress)
        {
            speed /=1.21;
        }
        if(!GW.cherryeaten){
            checkCollisionWithCherry(nullptr);
        }


        if(((int(x/40)) == (GW.blocksWidth-1)) && (int(y/40) == (GW.blocksHeight/2))) //right end
        {
            GW.PacS->setPosition((1)*40,(GW.blocksHeight/2)*40);
        }
        collision(GW.PacS->getPosition().x,GW.PacS->getPosition().y, rightpress, down );
        GW.PacS->move(speed*rightpress,speed*down);
        if((int(x/40) == 0) && (int(y/40) == (GW.blocksHeight/2))) //left end
        {
            GW.PacS->setPosition((GW.blocksWidth-2)*40,(GW.blocksHeight/2)*40);
        }
        pthread_mutex_unlock(&coinsMutex);
        pthread_mutex_unlock(&movementMutex);
    }
    

return nullptr;
}
void resetGhost(Ghost &g, int num);

void setupGhosts()
{   
    Gptr = new Ghost[4]();
    for(int i =0; i<4; i++)
    {
        Gptr[i].x = 80;
        Gptr[i].y = 40;
        Gptr[i].totalTime = (i+10) * 20;
        Gptr[i].setSpeed();
        Gptr[i].Gid =i;
        Gptr[i].inhome=0;
        Gptr[i].on =0;
        
    }
    Gptr[1].x = WIDTH -80;
    Gptr[2].y =HEIGHT-80;
    Gptr[3].x = WIDTH - 80;
    Gptr[3].y = HEIGHT - 80;
}
void resetGhost(Ghost &g, int num)
{
   
    g.x = 80;
    g.y = 40;
    if(num == 1)
    {
        g.x = WIDTH -80;
    }
    else if(num ==2 )
    {
        
        g.y = HEIGHT-80;
    }
    else if(num == 3)
    {
        g.x = WIDTH - 80;
        g.y = HEIGHT - 80;
    }
}

void raceGhostToKey(Ghost *gptr)
{   
    if(gptr->on)
        return;
    
    //if not on then
    if(gptr->x < WIDTH) //this should move all off ones to x high
    {
        gptr->x = WIDTH + (gptr->Gid * 40);
        gptr->y = 0;
        return;
    }
    //x values set, now time for racing down

    if(gptr->y < 840)
    {
        
        gptr->y += gptr->speed/900;
    }
    else{ //reached key
        resetGhost(gptr[0], gptr->Gid);
        gptr->on =1;
    }
    
}


void ghostCollisionWithPacman(Ghost *gptr)
{
    double x = gptr->x;
    double y = gptr->y;
    double prevx, prevy;
    prevx = GW.S_g1->getPosition().x;
    prevy = GW.S_g1->getPosition().y;

    GW.S_g1->setPosition(x,y);
    FloatRect pacmanBounds = GW.PacS->getGlobalBounds();
    FloatRect ghostbound = GW.S_g1->getGlobalBounds();
    GW.S_g1->setPosition(prevx, prevy);


    

    if(pacmanBounds.intersects(ghostbound)){

        if((!GW.cherry1) || (!GW.cherry2))
        {
            gptr->on =0;
        }
        else
        {
            if(!lives)
                return;
            lives--;
            
        }
      
    }

}
bool moveghostToPowerUp(Ghost * gptr)
{
    int ghostx = int(gptr->x/40);
    int ghosty = int(gptr->y/40);
    if(gptr->Gid == 1)
    {
        if(!powerup1)
            return 0;
        
        if(p1x > ghostx)
        {
            gptr->x += 40;
            return 1; 
        }
        else if(p1x < ghostx)
        {
            gptr->x -= 40;
            return 1;
        }
        else if(p1y > ghosty)
        {
            gptr->y += 40;
            return 1;
        }
        else if(p1y < ghosty )
        {
            gptr->y -= 40;
            return 1;
        }
        else
        {
            powerup1 =0;
            return 1;
        }
    }
    else if(gptr->Gid == 3)
    {
        if(!powerup2)
            return 0;
        
        if(p2x > ghostx)
        {
            gptr->x += 40;
            return 1; 
        }
        else if(p2x < ghostx)
        {
            gptr->x -= 40;
            return 1;
        }
        else if(p2y > ghosty)
        {
            gptr->y += 40;
            return 1;
        }
        else if(p2y < ghosty )
        {
            gptr->y -= 40;
            return 1;
        }
        else
        {
            powerup2 =0;
            return 1;
        }
        

    }
    else 
    {
        return 0;
    }
    return 0;
}
void * moveGhosts(void * args)
{
    Clock clock2;
    Clock clock3;
    double totaltime;
    while(1)
    {
        
        double time = clock2.getElapsedTime().asSeconds();
        totaltime = clock3.getElapsedTime().asSeconds();
        Ghost * G_ptr = (Ghost*)args;
        double userx,usery;
        userx = GW.PacS->getPosition().x;
        usery = GW.PacS->getPosition().y;

        int userxblock, useryblock;
        userxblock = userx/40;
        useryblock = usery/40;

        int ghostxblock;
        int ghostyblock;

        int xmove=1, ymove=1;
        ghostxblock = (G_ptr->x)/40;
        ghostyblock = (G_ptr->y)/40;

        //busy wait while more than one ghost in cs
       
         
        raceGhostToKey(G_ptr);
        
        if(!G_ptr->on)
            continue; //its in home

        
        double waiting_time = 1;
        if(G_ptr->Gid ==1 && (!powerup1))
            waiting_time =0.5;

        if(G_ptr->Gid ==3 && (!powerup2))
            waiting_time = 0.5;

        if(totaltime > 45)
            waiting_time = 1;

        if(time < waiting_time) 
            continue;
        clock2.restart();
        //after 1.5 seconds


        while(ghostincs>0)
        {
            continue;
        } 
            
       
        ghostincs++;
        if(moveghostToPowerUp(G_ptr))
        {
            ghostincs--;
            continue;
        }

        int ghostx = int(G_ptr->x/40);
        int ghosty = int(G_ptr->y/40);
        
        if(userxblock > ghostxblock) // move to the right 
        {
            if(!wallsArr[ghostx+1][ghosty])
            {
                G_ptr->x += 40;
                
            }   
                
        }
        else if(userxblock < ghostxblock)// move to the left
        {
            if(!wallsArr[ghostx-1][ghosty])
            {
                G_ptr->x -= 40;
            }  
        }
        else if(useryblock > ghostyblock) // move down one block
        {
            
            if(!wallsArr[ghostx][ghosty+1])
            {
                G_ptr->y += 40;
            }
        }
        else if (useryblock < ghostyblock) //move up one block
        {
            

            if(!wallsArr[ghostx][ghosty-1])
            {
                
                G_ptr->y -= 40;
            }

        }

        pthread_mutex_lock(&movementMutex);
        ghostCollisionWithPacman(G_ptr);
        pthread_mutex_unlock(&movementMutex);
        ghostincs--;
        continue;
    }
}

void drawKey()
{

    for(int i =0; i<4; i++)
    {
        GW.keyS->setPosition(WIDTH + (i *40), 880);
        GW.RWMain->draw(*GW.keyS);
    }
}


