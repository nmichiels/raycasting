//***************************************************************************************************
// ENGINE.CPP
// De engine voor de raycasing. Het algoritme van raycasting + sprites wordt hier uitgerekend en 
// uitgetekend
//***************************************************************************************************
// AUTEUR: KENNETH & NICK
// NICK: Basis van draw functie voor muren en vloeren
// KENNETH: Alles verder uitgewerkt en geoptimaliseerd, vloeren en plafond aangepast en symmetrisch
//			gemaakt, sprites uittekenen, triggers aanroepen, ...
//***************************************************************************************************

#include "Engine.h"
#include "QSound.h"

//** Constructor ***********************************************************************************
// Om de 10 miliseconde wordt het scherm ververst
//**************************************************************************************************
Engine::Engine(World* world, Viewer* viewer, Navigation *navigation,Ui::RaycastingQTClass *ui,Triggers *triggers )
{
	//QSound::play ( QString("music/05-Into the Black Hole- The Eye of the Universe-Halo of Darkness-The-Ayreon.WAV") );
	m_Triggers=triggers;
	m_Ui=ui;
	m_World = world;
	m_Viewer=viewer;
	m_Navigation=navigation;
	done= new bool[480];
	oldX=0;
	oldY=0;
	m_Timer = new QTimer();
	connect( m_Timer, SIGNAL(timeout()), this, SLOT(Redraw()) ); // scherm verversen
	connect( m_Timer, SIGNAL(timeout()), navigation, SLOT(Update()) ); // camerabewegingen verversen
	connect( m_Timer, SIGNAL(timeout()), this, SLOT(triggerCheck()) ); // triggers checken
	m_Timer->start(20);
}

//** Destructor* ***********************************************************************************
//**************************************************************************************************
Engine::~Engine()
{
	delete(m_Timer);
	delete [] done;
}
//*************************************************************************************************
// Kijkt of er een trigger mag worden geactiveerd. 
//**************************************************************************************************
void Engine::triggerCheck(){
	if(oldX != (int)(m_Viewer->x)/64 || oldY != (int)(m_Viewer->y)/64 || m_World->trigger)
	{	
		oldX=(m_Viewer->x)/64;
		oldY=(m_Viewer->y)/64;
		if((*m_World)[oldX][oldY].trigger!=0)
			m_Triggers->get((*m_World)[oldX][oldY].trigger)->startTasks(false,m_Viewer,m_World,m_Ui);
		}
}

//*************************************************************************************************
// Interpreteert de richting van een uitgezonden ray en plaatst deze in een enum 
//**************************************************************************************************
void Engine::InterpretRayDirection(double angle)
{
	if (angle > 0.0 && angle < 90.0)
		m_RayDirection = FIRST_QUADRANT;
	else if ( angle > 90.0 && angle < 180.0)
		m_RayDirection = SECOND_QUADRANT;
	else if ( angle > 180.0 && angle < 270.0)
		m_RayDirection = THIRD_QUADRANT;
	else if ( angle > 270.0 && angle < 360.0)
		m_RayDirection = FOURTH_QUADRANT;
	else if (angle == 0.0)
		m_RayDirection = RIGHT;
	else if (angle == 90.0)
		m_RayDirection = UP;
	else if (angle == 180.0)
		m_RayDirection = LEFT;
	else if (angle == 270.0)
		m_RayDirection = DOWN;
}


//*************************************************************************************************
// De functie die aangeroepen wordt om het frame te hertekenen
// Deze roept dus heel het raycasting-mechanisme aan 
//**************************************************************************************************
void Engine::Redraw()
{

	sort sortCasting;							// Geeft aan of de ray horizontal of vertical gebotst is
	int  wallGridX, wallGridY;					// Celcoordinaten van de cel waartegen de ray botst
	double wallX, wallY;	
	double angle;								// hoek waarin de ray wordt uitgeschoten
	double distance;							// afstand van camera tot botsingspunt
	int projectedSliceHeight;					// aantal pixels hoog dat de muur moet worden uitgetekend
	double rayAngle;							// hoek die moet worden opgeteld bij de ray voor elke horizontale pixel af te gaan
	QImage* img;

	rayAngle = (m_Viewer->FOV)/640.0;			// grootte gezichtsveld delen door aantal pixels om alle pixels te bereiken
	angle=( m_Viewer->viewAngle + m_Viewer->FOV/2.0 ); // beginnen aan de linkerkant van het gezichtsveld
	color=(m_World->r!=0 || m_World->g!=0 || m_World->b!=0  );
	for (int i=0; i<640; i++){
		for(int z=0;z<480;z++)
			done[z]=false;
		// hoek tussen de 0° en 360° houden
		if (angle < 0){
			angle = 360.0+angle;
		}
		else if (angle >= 360){
			angle = angle - 360.0;
		}
		// zet een enum die aangeeft in welke richting de ray gaat
		InterpretRayDirection(angle);	
		// Ray uitzenden in richting van angle
		// Deze functie gaat celgegevens en castinggegevens opvullen in de overige parameters
		distance = Cast(angle, sortCasting, wallGridX, wallGridY, wallX, wallY,i);
		projectedSliceHeight =64.0/distance * (double)m_Viewer->DPP +2.0 ; 
		//berekent eventueel extra kleuren
		if(color){
			r=m_World->r*distance/100;
			g=m_World->g*distance/100;
			b=m_World->b*distance/100;
		}
		//tekent de muur
		if (distance != -1) // als er een botsing plaatsvindt
		{
			//correctie
			wallGridY=abs(wallGridY);
			wallGridX=abs(wallGridX);
			wallGridY=wallGridY%m_World->WorldWidth();
			wallGridX=wallGridX%m_World->WorldHeight();
			wallCast(wallX,wallY,wallGridX,wallGridY,projectedSliceHeight,sortCasting,i);

		}

		if(m_Viewer->looking == 240) // als projection plane perfect in het midden staat is floor en ceiling symmetrisch
				floorCeilSymCast(angle, (m_Viewer->looking)+(projectedSliceHeight/2 ), i);
			else
				floorCeilAsCast(angle,(m_Viewer->looking)+(projectedSliceHeight/2 ),(m_Viewer->looking)-(projectedSliceHeight/2 ), i);
			
		angle -= (double)rayAngle;
	}
	wait();
	start();
}
//**************************************************************************************************
//update vanuit apparte thread
//**************************************************************************************************

void Engine::run(){
	emit ScreenUpdate();
}
//**************************************************************************************************

//**************************************************************************************************
//tekent de muren uit
//**************************************************************************************************
void Engine::wallCast(double &wallX,double &wallY,int &wallGridX,int &wallGridY,int &projectedSliceHeight,sort &sortCasting,int &i){
	QRgb value;
	int factor,factor2;
	int Tdistance=sqrt((wallX-(double)m_Viewer->x)*(wallX-(double)m_Viewer->x)+(wallY-(double)m_Viewer->y)*(wallY-(double)m_Viewer->y));
	//afbeelding opvragen afhankelijk van de afstand
	QImage* image=getWallTexture(wallGridX,wallGridY,sortCasting,projectedSliceHeight);
	factor=image->height();
	factor2=image->width();
	int Y=min(479-m_Viewer->looking,(projectedSliceHeight/2));
	//spiegelen indien nodig
	if (sortCasting == HORIZONTALLY){	// De dichtbijzijnste botsing is een horizontale
		int xoffset;
		if(this->m_RayDirection==THIRD_QUADRANT || this->m_RayDirection==FOURTH_QUADRANT)
			xoffset = factor2-(int)(wallX *factor2/64)% (int)factor2-1;		// horizontale plaats van de texture waar ray gebotst is
		else
			xoffset = (int)(wallX *factor2/64)% (int)factor2;	
		double yplus = factor / (double)projectedSliceHeight; // optelwaarde om hele texture in SliceHeight te laten passen
		double yoffset = 0;				// deze moet varieren van bovenaan tot onderaan texture
		for (int j = min(479-m_Viewer->looking,-(projectedSliceHeight/2)); j <Y; j++)
		{
			//indien geen sprite ervoor staat of de muur staat dichter
			// neem de juiste pixel uit de juiste texture van de cel en print deze op plaats (i, j) op het scherm
			if( done[j+m_Viewer->looking]==false || SpriteDist >Tdistance){
				if(color)
				{
					value=image->pixel(xoffset, int(yoffset));
					Screen::Point(i, (m_Viewer->looking + j),qRgb(max(min(r+qRed(value),255),0),max(min(g+qGreen(value),255),0),max(min(b+qBlue(value),255),0) ));
				}else
					Screen::Point(i, (m_Viewer->looking + j),image->pixel(xoffset, int(yoffset)));					
			}
			yoffset += yplus;
		}
	}
	else {								// De dichtbijzijnste botsing is een verticale
		double xoffset = 0;				// varieren van bovenaan tot onderaan texture
		double xplus = factor / (double)projectedSliceHeight;
		int yoffset;
		//spiegelen indien nodig
		if(this->m_RayDirection==SECOND_QUADRANT || this->m_RayDirection==THIRD_QUADRANT)
			yoffset = factor2-int(wallY * factor2/64.0)% factor2-1;		// horizontale plaats van de texture waar ray gebotst is
		else
			yoffset = int(wallY * factor2/64.0)% factor2;		// horizontale plaats van de texture waar ray gebotst is
			
		for (int j = min(479-m_Viewer->looking,-(projectedSliceHeight/2)); j <Y; j++)
		{
			// neem de juiste pixel uit de juiste texture van de cel en print deze op plaats (i, j) op het scherm
			if( done[m_Viewer->looking + j]==false || SpriteDist >Tdistance){
				if(color){
					value=image->pixel(yoffset, (int)xoffset);
					Screen::Point(i, (m_Viewer->looking + j),qRgb(max(min(r+qRed(value),255),0),max(min(g+qGreen(value),255),0),max(min(b+qBlue(value),255),0) ));			
				}else
					Screen::Point(i, (m_Viewer->looking + j),image->pixel(yoffset, (int)xoffset));			
			}
			//cte waarde bij de coordinaat tellen
			xoffset += xplus;
		}
	}
}
	

//**************************************************************************************************
//geeft de juiste textuur terug afhankelijk van de noord-,oost-,zuid- of west-muur
//**************************************************************************************************

QImage* Engine::getWallTexture(int wallGridX, int wallGridY,  sort sortCasting, int distance)
{
	//adhv de richting en positie zal een textuur terug gegeven worden (noord, oost, zuid, west)
	switch (m_RayDirection)
	{
		case FIRST_QUADRANT:
		case LEFT:
		case UP:
			if (sortCasting == this->HORIZONTALLY)
				return m_World->textures.get((*m_World)[wallGridX][wallGridY].south);//,distance);
			else
				return m_World->textures.get((*m_World)[wallGridX][wallGridY].west);//,distance);
			break;
		case SECOND_QUADRANT:
		case RIGHT:
			if (sortCasting == this->HORIZONTALLY)
				return m_World->textures.get((*m_World)[wallGridX][wallGridY].south);//,distance);
			else
				return m_World->textures.get((*m_World)[wallGridX][wallGridY].east);//,distance);
			break;
		case THIRD_QUADRANT:
		case DOWN:
			if (sortCasting == this->HORIZONTALLY)
				return  m_World->textures.get((*m_World)[wallGridX][wallGridY].north);//,distance);
			else
				return  m_World->textures.get((*m_World)[wallGridX][wallGridY].east);//,distance);
			break;
		case FOURTH_QUADRANT:
			if (sortCasting == this->HORIZONTALLY)
				return  m_World->textures.get((*m_World)[wallGridX][wallGridY].north);//,distance);
			else
				return  m_World->textures.get((*m_World)[wallGridX][wallGridY].west);//,distance);
			break;
	}
	return  m_World->textures.get((*m_World)[wallGridX][wallGridY].west);//,distance);

}	


//*************************************************************************************************
// De functie zendt een ray uit in de world en berekent waar hij botst en of dit horizontal of
// vertical gebeurt (Dit gebeurt met het raycasting algoritme)
// Hij vult wallGridX, wallGridY, wallX en wallY op met de gegevens van de gebotste cel
// De returnwaarde is de afstand van de camera tot de gebotste plaats
//**************************************************************************************************
double Engine::Cast(double angle, sort &sortCasting, int &wallGridX, int &wallGridY, double &wallX, double &wallY,int &i)
{
	double Ax;						// X-waarde eerste horizontal snijpunt (of later huidig horizontal snijpunt)
	double Ay;						// Y-waarde eerste horizontal snijpunt (of later huidig horizontal snijpunt)
	double Ya;						// Y-afstand om bij te tellen voor volgend horizontal snijpunt
	double Xa;						// X-afstand om bij te tellen voor volgend horizontal snijpunt
	int gridAy;						// celpositie(y) van huidig horizontal snijpunt
	int gridAx;						// celpositie(x) van huidig horizontal snijpunt
	double horizontalDistance = 0;		// afstand van camera tot horizontale botsing
	bool horizontal = false;		// is er een horizontale botsing gevonden?

	double Bx;						// X-waarde eerste vertical snijpunt (of later huidig vertical snijpunt)
	double By;						// Y-waarde eerste vertical snijpunt (of later huidig vertical snijpunt)
	double Xb;						// X-afstand om bij te tellen voor volgend vertical snijpunt
	double Yb;						// Y-afstand om bij te tellen voor volgend vertical snijpunt
	int gridBy;						// celpositie(y) van huidig vertical snijpunt
	int gridBx;						// celpositie(x) van huidig vertical snijpunt
	double verticalDistance = 0;		// afstand van camera tot verticale botsing
	bool vertical = false;			// is er een verticale botsing gevonden?

	//int wallAx, muurAy, muurBx, muurBy;
	SpriteDist=64*m_World->WorldHeight()*m_World->WorldWidth();

	// hoek omvormen volgens complementaire of supplementaire regel
	double angleCos, angleTan;
	if (m_RayDirection == SECOND_QUADRANT){
		angleTan = 180 - angle; //supplementair
		angleCos = 180 - angle;	//supplementair
		Ay = floor(m_Viewer->y/64.0)*(64) - 1;		// berekenen y-waarde eerste horizontal snijpunt
		Ax = m_Viewer->x - abs(m_Viewer->y-Ay) / tan(angleTan*PI / 180.0); // berekenen x-waarde eerste horizontal snijpunt
		Ya = -64;									// Y verandert vertical constant naar boven
		Xa = -64.0/tan(angleTan*PI / 180.0);			// tan forumule op een rechthoekige driehoek
		gridAx = floor((double)Ax / 64.0);
		gridAy = floor((double)Ay / 64.0);

		Bx = floor(m_Viewer->x/64.0) * (64) - 1;	// berekenen x-waarde eerste vertical snijpunt
		By = m_Viewer->y - abs(m_Viewer->x-Bx) * tan(angleTan*PI / 180.0); // berekenen y-waarde eerste vertical snijpunt
		Yb = -64.0 * tan(angleTan*PI / 180.0);		// tan forumule op een rechthoekige driehoek
		Xb = -64;									// X verandert horizontal constant naar links
		gridBx = floor((double)Bx / 64.0);
		gridBy = floor((double)By / 64.0);
	}
	else if (m_RayDirection == THIRD_QUADRANT){
		angleTan = angle;
		angleCos = angle;
		Ay = floor(m_Viewer->y/64.0)*(64) + 64;		// berekenen y-waarde eerste horizontal snijpunt
		Ax = m_Viewer->x - abs(m_Viewer->y-Ay) / tan(angleTan*PI / 180.0); // berekenen x-waarde eerste horizontal snijpunt
		Ya = 64;									// Y verandert vertical constant naar onder
		Xa = -64/tan(angleTan*PI / 180.0);			// tan forumule op een rechthoekige driehoek
		gridAx = floor((double)Ax/64.0);
		gridAy = floor((double)Ay/64.0);

		Bx = floor((m_Viewer->x)/64.0) * (64)-1 ;	// berekenen x-waarde eerste vertical snijpunt
		By = m_Viewer->y + abs(m_Viewer->x-Bx) * tan(angleTan*PI / 180.0);  // berekenen y-waarde eerste vertical snijpunt
		Yb = 64 * tan(angleTan*PI / 180.0);			// tan forumule op een rechthoekige driehoek
		Xb = -64;									// X verandert horizontal constant naar links
		gridBx = floor((double)Bx/64.0);
		gridBy = floor((double)By/64.0);
	}
	else if (m_RayDirection == FOURTH_QUADRANT){
		angleTan = 360-angle;
		angleCos = 360-angle;
		Ay = floor(m_Viewer->y/64.0)*(64.0) + 64.0;		// berekenen y-waarde eerste horizontal snijpunt
		Ax = m_Viewer->x + abs(m_Viewer->y - Ay ) / tan(angleTan*PI / 180.0); // berekenen x-waarde eerste horizontal snijpunt
		Ya = 64;									// Y verandert vertical constant naar onder
		Xa = 64/tan(angleTan*PI / 180.0);			// tan forumule op een rechthoekige driehoek
		gridAx = floor((double)Ax/64.0);
		gridAy = floor((double)Ay/64.0);

		Bx = floor((m_Viewer->x)/64.0) * (64.0) + 64;	// berekenen x-waarde eerste vertical snijpunt
		By = m_Viewer->y + abs(m_Viewer->x-Bx) * tan(angleTan*PI / 180.0); // berekenen y-waarde eerste vertical snijpunt
		Yb = 64 * tan(angleTan*PI / 180.0);			// tan forumule op een rechthoekige driehoek
		Xb = 64;									// X verandert horizontal constant naar rechts
		gridBx = floor((double)Bx/64.0);
		gridBy = floor((double)By/64.0);
	}
	else if(m_RayDirection == FIRST_QUADRANT){
		angleTan = angle;
		angleCos = angle;
		Ay = floor(m_Viewer->y / 64.0)*(64) - 1;	// berekenen y-waarde eerste horizontal snijpunt
		Ax = m_Viewer->x + abs(m_Viewer->y-Ay) / tan(angle*PI / 180.0); // berekenen x-waarde eerste horizontal snijpunt
		Ya = -64;									// Y verandert vertical constant naar boven
		Xa = 64.0/tan(angle*PI / 180.0);				// tan forumule op een rechthoekige driehoek
		gridAx = floor((double)Ax/64.0);
		gridAy = floor((double)Ay/64.0);

		Bx = floor(m_Viewer->x / 64.0) * (64) + 64;	// berekenen x-waarde eerste vertical snijpunt
		By = m_Viewer->y - abs(m_Viewer->x-Bx) * tan(angle*PI / 180.0); // berekenen y-waarde eerste vertical snijpunt
		Yb = -64.0 * tan(angle*PI / 180.0);			// tan forumule op een rechthoekige driehoek
		Xb = 64;									// X verandert horizontal constant naar rechts
		gridBx = floor((double)Bx/64.0);
		gridBy = floor((double)By/64.0);
	}
	else if (m_RayDirection == RIGHT){
		horizontalDistance = -1; // de ray kan geen horizontale snijpunten hebben
		horizontal = true;
		By = m_Viewer->y;		// wegens vertical er niet verschoven wordt is deze hetzelfde als de camera positie
		Bx = floor(m_Viewer->x/64.0) * (64) + 64; // berekenen x-waarde eerste vertical snijpunt
		Yb = 0;					// vertical moet er niet verschoven worden
		Xb = 64;				// horizontal kan nu verschoven worden over 64px
		gridBx = floor((double)Bx/64.0);
		gridBy = floor((double)By/64.0);
	}
	else if (m_RayDirection == UP){
		verticalDistance = -1; // de ray kan geen verticale snijpunten hebben
		vertical = true;
		Ax = m_Viewer->x;	// x-waarde blijft hetzelfde dus deze komt overeen met camera
		Ay = floor(m_Viewer->y/64.0)*(64) - 1; // berekenen y-waarde eerste horizontal snijpunt
		Ya = -64;			// vertical met 64 verschuiven naar boven
		Xa = 0;				// horizontal niet verschuiven
		gridAx = floor((double)Ax/64.0);
		gridAy = floor((double)Ay/64.0);
	}
	else if (m_RayDirection == LEFT){
		horizontalDistance = -1;  // de ray kan geen horizontale snijpunten hebben
		horizontal = true;
		By = m_Viewer->y;	// y-waarde blijft hetzelfde dus deze komt overeen met camera
		Bx = floor(m_Viewer->x/64.0) * (64) - 1; // berekenen x-waarde eerste vertical snijpunt
		Yb = 0;				// vertical niet verschuiven
		Xb = -64;			// horizontal met 64 verschuiven naar links
		gridBx = floor((double)Bx/64.0);
		gridBy = floor((double)By/64.0);
	}
	else if (m_RayDirection == DOWN){
		verticalDistance = -1; // de ray kan geen verticale snijpunten hebben
		vertical = true;
		Ay = floor(m_Viewer->x/64.0) * (64) + 64; // berekenen y-waarde eerste horizontal snijpunt
		Ax = m_Viewer->x;	// x-waarde blijft hetzelfde dus deze komt overeen met camera
		Ya = 64;			// vertical met 64 verschuiven naar onder
		Xa = 0;				// horizontal niet verschuiven
		gridAx = floor((double)Ax/64.0);
		gridAy = floor((double)Ay/64.0);
	}

	while (!horizontal || !vertical){ // zolang horizontal of vertical geen botsing of out of bound
		if (!horizontal && m_World->isOutOfBound(gridAx, gridAy)){		// horizotaal out of bound zonder botsing
			horizontal = true;
		}
		else if (!vertical && m_World->isOutOfBound(gridBx, gridBy)){	// vertical out of bound zonder botsing
			vertical = true;
		}
		else if (!horizontal && m_World->isWall(gridAx, gridAy)){	// horizontal snijpunt is een botsing
			horizontal = true;
		}else if (!vertical && m_World->isWall(gridBx, gridBy)){	// vertical snijpunt is een botsing
			vertical = true;
		}else if(!horizontal && m_World->isSprite(gridAx,gridAy)){
				Sprite(angle,gridAx,gridAy,Ax,Ay,i);
			if (!horizontal){	// snijpunt geen botsing: waardes optellen voor volgend horizontal snijpunt
				Ax = Ax+Xa;
				Ay = Ay+Ya;
				gridAx = floor(Ax / 64.0);
				gridAy = floor(Ay / 64.0);
			}
			else if (!vertical){	// snijpunt geen botsing: waardes optellen voor volgend vertical snijpunt
				Bx = Bx+Xb;
				By = By+Yb;
				gridBx = floor(Bx / 64.0);
				gridBy = floor(By / 64.0);
			}
			
			

		}else if(!vertical && m_World->isSprite(gridBx,gridBy)){
			
			Sprite(angle,gridBx,gridBy,Bx,By,i);
			if (!horizontal){	// snijpunt geen botsing: waardes optellen voor volgend horizontal snijpunt
				Ax = Ax+Xa;
				Ay = Ay+Ya;
				gridAx = floor(Ax / 64.0);
				gridAy = floor(Ay / 64.0);
			}
			else if (!vertical){	// snijpunt geen botsing: waardes optellen voor volgend vertical snijpunt
				Bx = Bx+Xb;
				By = By+Yb;
				gridBx = floor(Bx / 64.0);
				gridBy = floor(By / 64.0);
			}
		}
		else if (!horizontal){	// snijpunt geen botsing: waardes optellen voor volgend horizontal snijpunt
			Ax = Ax+Xa;
			Ay = Ay+Ya;
			gridAx = floor(Ax / 64.0);
			gridAy = floor(Ay / 64.0);
		}
		else if (!vertical){	// snijpunt geen botsing: waardes optellen voor volgend vertical snijpunt
			Bx = Bx+Xb;
			By = By+Yb;
			gridBx = floor(Bx / 64.0);
			gridBy = floor(By / 64.0);
		}
	}

	

	// afstand van camera tot horizontal en vertical snijpunt berekenen
	horizontalDistance=(Ax-(double)m_Viewer->x)*(Ax-(double)m_Viewer->x)+(Ay-(double)m_Viewer->y)*(Ay-(double)m_Viewer->y);
	verticalDistance=(Bx-(double)m_Viewer->x)*(Bx-(double)m_Viewer->x)+(By-(double)m_Viewer->y)*(By-(double)m_Viewer->y);
	
	
	// bepalen welke botsing het dichtste bij is en welke dus moet worden teruggegeven
	if (horizontalDistance>verticalDistance){ // vertical geen botsing dus horizontal teruggeven
		// verticale is het dichtste bij
		sortCasting = VERTICALLY;
		wallX = Bx;
		wallY = By;
		wallGridX = gridBx;
		wallGridY = gridBy;
		//afrondingsfouten wegwerken
		if(m_RayDirection==SECOND_QUADRANT&&gridBx+1>=0&&m_World->isWall(gridBx+1, gridBy))
		{
			sortCasting = HORIZONTALLY;
			wallX = Ax;
			wallY = Ay;
			wallGridX++;
			return floor(sqrt(horizontalDistance)*cos(abs(angle - m_Viewer->viewAngle)*PI/180.0)); // horizontal kan -1 zijn wordt later bij het tekenen op gechecked
		}else if(m_RayDirection==FIRST_QUADRANT && gridBx-1>=0 && m_World->isWall(gridBx-1, gridBy) )
		{
			sortCasting = HORIZONTALLY;
			wallX = Ax;
			wallY = Ay;
			wallGridX--;
			return floor(sqrt(horizontalDistance)*cos(abs(angle - m_Viewer->viewAngle)*PI/180.0)); // horizontal kan -1 zijn wordt later bij het tekenen op gechecked
		}
		// parameters voor gebotste cel opvullen
	

		// fishbowl effect verwijderen
		return  floor(sqrt(verticalDistance)*cos(abs(angle - m_Viewer->viewAngle)*PI/180.0));
		
	}
	else { sortCasting = HORIZONTALLY;
		wallX = Ax;
		wallY = Ay;
		wallGridX = gridAx;
		wallGridY = gridAy;
		//afrondingsfouten wegwerken
		if(m_RayDirection==THIRD_QUADRANT&&gridAy-1>=0&&m_World->isWall(gridAx, gridAy-1)  )
		{
			sortCasting = VERTICALLY;
			wallX = Bx;
			wallY = By;
			wallGridY--;
			return  floor(sqrt(verticalDistance)*cos(abs(angle - m_Viewer->viewAngle)*PI/180.0));
		}else if(m_RayDirection==SECOND_QUADRANT&&gridAy+1<m_World->WorldWidth()&&m_World->isWall(gridAx, gridAy+1))
		{
			sortCasting = VERTICALLY;
			wallX = Bx;
			wallY = By;
			wallGridY++;
			return  floor(sqrt(verticalDistance)*cos(abs(angle - m_Viewer->viewAngle)*PI/180.0));
		}
		// parameters voor gebotste cel opvullen
	
		// fishbowl effect verwijderen
		return floor(sqrt(horizontalDistance)*cos(abs(angle - m_Viewer->viewAngle)*PI/180.0)); // horizontal kan -1 zijn wordt later bij het tekenen op gechecked
		
	
	}
	
}


//*************************************************************************************************
// Deze functie plaatst een sprite als deze tijdens de cast opgemerkt wordt
//*************************************************************************************************
void Engine::Sprite(double &angle, int &wallGridX, int &wallGridY, double &wallX, double &wallY,int &i){
	//volgens assenstelsel, zie tekening verslag
	//algemeen: de offset wordt berekend door een driehoek te maken van de viewer tot
	//het midden van het spritevakje, van daar naar het projectiepunt en van daar terug naar 
	//de viewer
	//s2=x2;
	double s2=sqrt(float(((64*wallGridX+31)-m_Viewer->x)*((64*wallGridX+31)-m_Viewer->x)+((64*wallGridY+31)-m_Viewer->y)*((64*wallGridY+31)-m_Viewer->y)));
	double x1=0,y1=0,y2=0;
	double s1=sqrt(((64*wallGridX+31)-wallX)*((64*wallGridX+31)-wallX)+((64*wallGridY+31)-wallY)*((64*wallGridY+31)-wallY));
	double s3=sqrt((wallX-m_Viewer->x)*(wallX-m_Viewer->x)+(wallY-m_Viewer->y)*(wallY-m_Viewer->y));
	//hoogtelijn=s3*s1/(2*R)
	double y3=1/(2*(s2/sqrt(float((s1+s2+s3)*(-s1+s2+s3)*(s1-s2+s3)*(s1+s2-s3)))));
	double sinus=y3/s3;
	double cosin=1-sinus*sinus;
	double x3=s3*cosin;//hoogtelijn
	double offsetHulp=y3*s2/x3;
	if(offsetHulp>=32)
		return;
	if(s2==0)
		return;
	double cosx2=double(64.0*(double)wallGridX+31.0-(double)m_Viewer->x)/s2;
	double sinx2=double(64.0*(double)wallGridY+31.0-(double)m_Viewer->y)/s2;
	double angleX2=acos(cosx2)*i180P;
	double anglesX2=-asin(sinx2)*i180P;
	//de sprite wordt geflipt afhankelijk van het kwadrant en de 2 hoeken tov elkaar
	if(anglesX2>0 && angleX2<90)//1
		switch(m_RayDirection){
		case FIRST_QUADRANT:
			 if(angle>angleX2)
				offsetHulp=-offsetHulp;
			break;
		case SECOND_QUADRANT:
				offsetHulp=-offsetHulp;
			break;
		}
	else if(anglesX2>0 && angleX2>90 )//2
		switch(m_RayDirection){
		case SECOND_QUADRANT:
			if(angle>angleX2 )
				offsetHulp=-offsetHulp;
			break;
		case THIRD_QUADRANT:
				offsetHulp=-offsetHulp;
			break;
	}
	else if(anglesX2<0 && angleX2>90)//3
		switch(m_RayDirection){
		case THIRD_QUADRANT:
			if(angle>-angleX2+360 )
				offsetHulp=-offsetHulp;
			break;
		case FOURTH_QUADRANT:
				offsetHulp=-offsetHulp;
			break;
	}
	else if(anglesX2<0 && angleX2<90)//4
		switch(m_RayDirection){
		case FIRST_QUADRANT:
				offsetHulp=-offsetHulp;
			break;
		case FOURTH_QUADRANT:
			if(angle>-angleX2+360 )
				offsetHulp=-offsetHulp;
			break;
	}
	else if(wallGridY*64+31-m_Viewer->y==0){
		if(m_Viewer->viewAngle<90 || m_Viewer->viewAngle>270 ){
			if(m_Viewer->y-wallY > 0)
				offsetHulp=-offsetHulp;
			}else if(m_Viewer->y-wallY < 0)
				offsetHulp=-offsetHulp;
	}else if(wallGridY*64+31-m_Viewer->y==0){
		if( m_Viewer->viewAngle<180){
			if(m_Viewer->x-wallX > 0)
				offsetHulp=-offsetHulp;
		}else if(m_Viewer->x-wallX < 0)
			offsetHulp=-offsetHulp;
	}
			
	offsetHulp+=32;
	int offset=0;
	int projectedSliceHeight=64.0 * m_Viewer->DPP/s2;
	QImage *image;
	double yplus;
	double factor,factor2;
	double yoffset;
	//afbeelding opvragen en parameters zetten
	image=m_World->textures.getdist((*m_World)[wallGridX][wallGridY].sprite,projectedSliceHeight);//s2);
	factor=image->height();
	factor2=image->width();
	yplus=factor / (double)projectedSliceHeight;
	offset=floor(offsetHulp*factor2/64.0);
	yoffset=max(0,yplus*(-m_Viewer->looking+(projectedSliceHeight/2)-1));
	int Y=min(479-m_Viewer->looking,(projectedSliceHeight/2));// deze moet varieren van bovenaan tot onderaan texture
	if(offset<0 || offset >=factor)
		return;
	QRgb rgb;//om overbodige berekeningen te vermijden
	//voor als er kleuren bijkomen op het scherm
	if(color){
		r=m_World->r*s2/100;
		g=m_World->g*s2/100;
		b=m_World->b*s2/100;
	}

	int screenYoffset=max(-m_Viewer->looking,-(projectedSliceHeight/2))+m_Viewer->looking;
	for (int j =screenYoffset-m_Viewer->looking ; j<Y; j++)
	{
		screenYoffset++;
		//indien daar nog geen andere sprite kwam of deze sprite staat het dichtst
		if(done[screenYoffset]==false || s2 < SpriteDist){
			rgb=image->pixel(offset,(int)yoffset);
			if(qAlpha(rgb)>= 20){
				if(color)
					Screen::Point(i,screenYoffset,  qRgb(max(min(r+qRed(rgb),255),0),max(min(g+qGreen(rgb),255),0),max(min(b+qBlue(rgb),255),0) ));
				else
					Screen::Point(i,screenYoffset, rgb);
				done[screenYoffset]=true;
			}
		}
		yoffset += yplus;					
	}
	if((int)s2<SpriteDist)
		SpriteDist=s2;
}
//*************************************************************************************************
// Deze berekent en tekent de vloer en ceiling als het projection plane niet gecentreerd is
//*************************************************************************************************
void Engine::floorCeilAsCast(double &angle, int intersection1,int intersection2, int &column)
{
	int point;
	double projectionCorrection;
	double currentFloorY;
	double currentFloorX;
	int currentTexY;
	int currentTexX;
	int factor1, factor2;
	QImage *im;
	double projection;
	double distance;
	int height, width;

	//waardes op voorhand berekenen voor snelheid
	double x14offset=cos(angle*PI/180.0);
	double y14offset=cos((90 - angle)*PI/180.0);
	double y23offset=cos((-90 + angle)*PI/180.0);
	double x23offset=cos((180-angle )*PI/180.0);
	QRgb rgb;
	height=m_World->WorldHeight();
	width=m_World->WorldWidth();


	int einde=max(intersection1+intersection2,480);
	for (point = intersection1; point < einde; point++)
	{
		//projectiecorrectie berekenen
		if (point-m_Viewer->looking != 0)
			projection = (double)32 / (double)(point-m_Viewer->looking) * (double)m_Viewer->DPP; // rechte afstand	
		else
			projection = (double)32 / 1*(double)m_Viewer->DPP; // rechte afstand
		projectionCorrection = (double)projection / cos(abs(angle - m_Viewer->viewAngle)*PI/180.0) ; // echte afstand

	
		//de huidige coordinaat adhv het kwadrant bepalen
//incrementeren en decrementeren ter correctie(anders schuift de vloer nog  lichtjes uit de muur)
		switch (m_RayDirection)
		{
			case SECOND_QUADRANT:
				currentFloorY = abs((double)m_Viewer->y - y23offset*projectionCorrection)+1.0;
				currentFloorX = abs((double)m_Viewer->x - x23offset*projectionCorrection)+1.0;
				break;
			case THIRD_QUADRANT:
				currentFloorY = abs((double)m_Viewer->y - y23offset*projectionCorrection)-1.0;
				currentFloorX = abs((double)m_Viewer->x - x23offset*projectionCorrection)+1.0;
				break;
			case FIRST_QUADRANT:
				currentFloorY = abs((double)m_Viewer->y - y14offset*projectionCorrection)+1.0;
				currentFloorX = abs((double)m_Viewer->x + x14offset*projectionCorrection)-1.0;
				break;
			case FOURTH_QUADRANT:
				currentFloorY = abs((double)m_Viewer->y - y14offset*projectionCorrection)-1.0;
				currentFloorX = abs((double)m_Viewer->x + x14offset*projectionCorrection)-1.0;
				break;
			case UP:
				currentFloorY = abs((double)m_Viewer->y - y14offset*projectionCorrection)-1.0;
				currentFloorX = m_Viewer->x;
				break;
			case DOWN:
				currentFloorY = abs((double)m_Viewer->y - y14offset*projectionCorrection)+1.0;
				currentFloorX = m_Viewer->x;
				break;
			case LEFT:
				currentFloorY = m_Viewer->y;
				currentFloorX = abs((double)m_Viewer->x + x14offset*projectionCorrection)-1.0;
				break;
			default:
				currentFloorY = m_Viewer->y;
				currentFloorX = abs((double)m_Viewer->x + x14offset*projectionCorrection)+1.0;
				break;
		}
		//afstand tot dat punt
		int distance2=((currentFloorX-m_Viewer->x)*(currentFloorX-m_Viewer->x)+(currentFloorY-m_Viewer->y)*(currentFloorY-m_Viewer->y)   );
		
		if(color)
		{
			distance=sqrt((double)distance2);
			r=m_World->r*distance/100;
			g=m_World->g*distance/100;
			b=m_World->b*distance/100;
		}

		//indien er nog geen sprite is en het ligt binnen het canvas
		if( point<480  && point>=0 && !done[point]){
			//afbeelding opvragen, parameters en coordinaten bepalen en het punt plaatsen 
			im=m_World->textures.getdist2((*m_World)[int(currentFloorX/64.0)%height][int(currentFloorY/64.0)%width].floor,distance2);
			factor1=im->height();	
			factor2=im->width();
			currentTexX=(int(currentFloorX * (double)factor2/64.0)%factor2);
			currentTexY=(int(currentFloorY * (double)factor1/64.0)%factor1);
			if(!color)
				Screen::Point(column, point, im->pixel(currentTexX,currentTexY));
			else {
				rgb=im->pixel(currentTexX,currentTexY);
				Screen::Point(column, point, qRgb(max(min(r+qRed(rgb),255),0),max(min(g+qGreen(rgb),255),0),max(min(b+qBlue(rgb),255),0) ));
			}
		}
		
		//indien er nog geen sprite is en het ligt binnen het canvas
		if( (intersection2-(point-intersection1))>=0 && (intersection2-(point-intersection1))<480 && !done[intersection2-(point-intersection1)])
		{	//afbeelding opvragen, parameters en coordinaten bepalen en het punt plaatsen 
			im=m_World->textures.getdist2((*m_World)[int(currentFloorX/64.0)%height][int(currentFloorY/64.0)%width].ceil,distance2);
			factor1=im->height();
			factor2=im->width();
			currentTexX=(int(currentFloorX * (double)(factor2)/64.0)%factor2);
			currentTexY=(int(currentFloorY * (double)(factor1)/64.0)%factor1);
			if(!color)
				Screen::Point(column, intersection2-(point-intersection1), im->pixel(currentTexX,currentTexY));
			else{
				rgb=im->pixel(currentTexX,currentTexY);
				Screen::Point(column, intersection2-(point-intersection1), qRgb(max(min(r+qRed(rgb),255),0),max(min(g+qGreen(rgb),255),0),max(min(b+qBlue(rgb),255),0) ));
			}
		}		
	}
}


//*************************************************************************************************
// Deze berekent en tekent de vloer en plafond als het projection plane gecentreerd is
// Hierdoor kan de vloer en plafond symmetrisch worden uitgetekend
//*************************************************************************************************
void Engine::floorCeilSymCast(double &angle, int intersection, int &column)
{	
	if(intersection>479 || intersection<0)
		return;
	int point;
	double projectionCorrection;
	double currentFloorY;
	double currentFloorX;
	int currentTexY;
	int currentTexX;
	double projection;
	QImage* im;
	int factor1, factor2,height,width;
	//waardes op voorhand berekenen voor snelheid
	double x14offset=cos(angle*PI/180.0);
	double y14offset=cos((90 - angle)*PI/180.0);
	double y23offset=cos((-90 + angle)*PI/180.0);
	double x23offset=cos((180-angle )*PI/180.0);
	double distance;
	QRgb rgb;
	height=m_World->WorldHeight();
	width=m_World->WorldWidth();
	//constante correctie
	double constant=double(32*m_Viewer->DPP)/cos((angle - m_Viewer->viewAngle)*P180);
	for (point = max(0,intersection-1); point < 480; point++)
	{
		projectionCorrection = constant / (double)(point-240); // echte afstand

	//incrementeren en decrementeren ter correctie(anders schuift de vloer nog  lichtjes uit de muur)
		switch (m_RayDirection)
		{
			case SECOND_QUADRANT:
				currentFloorY = abs((double)m_Viewer->y - y23offset*projectionCorrection)+1.0;
				currentFloorX = abs((double)m_Viewer->x - x23offset*projectionCorrection)+1.0;
				break;
			case THIRD_QUADRANT:
				currentFloorY = abs((double)m_Viewer->y - y23offset*projectionCorrection)-1.0;
				currentFloorX = abs((double)m_Viewer->x - x23offset*projectionCorrection)+1.0;
				break;
			case FIRST_QUADRANT:
				currentFloorY = abs((double)m_Viewer->y - y14offset*projectionCorrection)+1.0;
				currentFloorX = abs((double)m_Viewer->x + x14offset*projectionCorrection)-1.0;
				break;
			case FOURTH_QUADRANT:
				currentFloorY = abs((double)m_Viewer->y - y14offset*projectionCorrection)-1.0;
				currentFloorX = abs((double)m_Viewer->x + x14offset*projectionCorrection)-1.0;
				break;
			case UP:
				currentFloorY = abs((double)m_Viewer->y - y14offset*projectionCorrection)-1.0;
				currentFloorX = m_Viewer->x;
				break;
			case DOWN:
				currentFloorY = abs((double)m_Viewer->y - y14offset*projectionCorrection)+1.0;
				currentFloorX = m_Viewer->x;
				break;
			case LEFT:
				currentFloorY = m_Viewer->y;
				currentFloorX = abs((double)m_Viewer->x + x14offset*projectionCorrection)-1.0;
				break;
			default:
				currentFloorY = m_Viewer->y;
				currentFloorX = abs((double)m_Viewer->x + x14offset*projectionCorrection)+1.0;
				break;
		}
		//afstand
		int distance2=(currentFloorX-m_Viewer->x)*(currentFloorX-m_Viewer->x)+(currentFloorY-m_Viewer->y)*(currentFloorY-m_Viewer->y);
		//telkens controleren op sprite, afbeelding opvragen, coordinaten bepalen en uittekenen
		
		if(color)
		{
			distance=sqrt((double)distance2);
			r=m_World->r*distance/100;
			g=m_World->g*distance/100;
			b=m_World->b*distance/100;
		}
		if(done[point]==false ){
			im=m_World->textures.getdist2((*m_World)[int(currentFloorX/64)%height][int(currentFloorY/64)%width].floor,distance2);
			factor1=im->height();
			factor2=im->width();
			currentTexX=(int(currentFloorX * (double)(factor2)/64.0)%factor2);
			currentTexY=(int(currentFloorY * (double)(factor1)/64.0)%factor1);	
			if(color){
				rgb=im->pixel(currentTexX,currentTexY);
				Screen::Point(column, point,qRgb(max(min(r+qRed(rgb),255),0),max(min(g+qGreen(rgb),255),0),max(min(b+qBlue(rgb),255),0) ));
			}else
				Screen::Point(column, point,im->pixel(currentTexX,currentTexY));
		}
		if(done[479-point]==false ){
			im=m_World->textures.getdist2((*m_World)[int(currentFloorX/64)%height][int(currentFloorY/64)%width].ceil,distance2);
			factor1=im->height();
			factor2=im->width();
			currentTexX=(int(currentFloorX * (double)factor2/64.0)%factor2);
			currentTexY=(int(currentFloorY * (double)factor1/64.0)%factor1);
			if(color)
			{
				rgb=im->pixel(currentTexX,currentTexY);
				Screen::Point(column, 479-point,qRgb(max(min(r+qRed(rgb),255),0),max(min(g+qGreen(rgb),255),0),max(min(b+qBlue(rgb),255),0) ));
			}else
				Screen::Point(column, 479-point, im->pixel(currentTexX,currentTexY));	
		}			
	}


}