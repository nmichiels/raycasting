//***************************************************************************************************
// TEXTURES.CPP
// Een container om textures te beheren
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#include "Textures.h"


//**************************************************************************************************
// constructor
//**************************************************************************************************
	Textures::Textures(){
		//standaard
		add(0,"images/white.png");
		add(1,"images/redbrick.png");
		add(2,"images/greystone.png");
		add(3,"images/wood.png");
		add(4,"images/grass.jpg");
		add(5,"images/water.bmp");
		add(6,"images/water2.bmp");
		add(7,"images/water3.bmp");
		add(8,"images/water4.bmp");
		add(9,"images/moon.jpg");
		add(10,"images/grid.jpg");
		add(11,"images/bluestone.png");
		add(12,"images/space.bmp");
		add(13,"images/dBarrel.png");
		add(14,"images/dLamp.png");
		add(15,"images/dPillar.png");
		add(16,"images/redbrick.jpg");
		add(17,"images/iron.jpg");
	}
//**************************************************************************************************


//**************************************************************************************************
// Voeg een afbeelding toe	
//**************************************************************************************************
	void Textures::add(int nr, QString pad){
		img hulp;
		hulp.image=new QImage(pad);
		if(hulp.image->isNull())
		{
			delete hulp.image;
			return;
		}
		hulp.pad=pad;
		//verschillende formaten
		QImage a=hulp.image->scaled(16,16);
		QImage b=hulp.image->scaled(64,64);
		QImage c=hulp.image->scaled(256,256);
		hulp.small=new QImage(a);
		hulp.medium=new QImage(b);
		hulp.large=new QImage(c);

		//vector groot genoeg houden
		if(tex.size()<=nr)
			tex.resize(nr+1);
		tex[nr]=hulp;
	}
//**************************************************************************************************


//**************************************************************************************************
// Voeg een afbeelding toe
//**************************************************************************************************
	void Textures::add(int nr, char* pad){
		add(nr,QString(pad));		
	}
//**************************************************************************************************


//**************************************************************************************************
// de originele afbeelding
//**************************************************************************************************
	QImage* Textures::get(int i){
		if(i<tex.size()&&i>=0&& tex[i].pad!="")
			return tex[i].image;
		return tex[0].image;
	}
//**************************************************************************************************


//**************************************************************************************************
// de 64x64 afbeelding
//**************************************************************************************************
	QImage* Textures::getm(int i){
		if(i<tex.size()&&i>=0&& tex[i].pad!="")
				return tex[i].medium;
			return tex[0].medium;
	}
//**************************************************************************************************


//**************************************************************************************************
// de grote afbeelding
//**************************************************************************************************
	QImage* Textures::getl(int i){
		if(i<tex.size()&&i>=0&& tex[i].pad!="")
			return tex[i].large;
		return tex[0].large;
	}
//**************************************************************************************************


//**************************************************************************************************
// de kleine afbeelding
//**************************************************************************************************
	QImage* Textures::gets(int i){
		if(i<tex.size()&&i>=0&& tex[i].pad!="")
			return tex[i].small;
		return tex[0].small;
	}
//**************************************************************************************************


//**************************************************************************************************
// Geef een image op basis van de afstand (hier de hoogte van de afbeelding)
//**************************************************************************************************
	QImage* Textures::getdist(int i,int distance){
		if(!(i<tex.size()  && i>=0))
			return tex[0].small;
		if(distance<50)
		{
			if(tex[i].pad!="")
				return tex[i].small;
			return tex[0].small;
		}
		else if(distance<90)
		{
			if(tex[i].pad!="")
				return tex[i].medium;
			return tex[0].medium;
		}
		else if(distance <150)
		{
			if( tex[i].pad!="")
				return tex[i].large;
			return tex[0].large;
		}
		else
		{
			if( tex[i].pad!="")
				return tex[i].image ;
			return tex[0].image;
		}
	}
//**************************************************************************************************


//**************************************************************************************************
// Geef image volgens afstand^2
//**************************************************************************************************
QImage* Textures::getdist2(int i, int distance){
	if(!(i<tex.size()  && i>=0))
			return tex[0].small;
	
	if( distance>1200000)
		{//controle op leeg
			if(tex[i].pad!="")
				return tex[i].small;
			//standaard
			return tex[0].small;
		}
		else if(distance>360000)
		{
			if( tex[i].pad!="")
				return tex[i].medium;
			return tex[0].medium;
		}
		else if(distance >15000)
		{
			if( tex[i].pad!="")
				return tex[i].large;
			return tex[0].large;
		}
		else
		{
			if( tex[i].pad!="")
				return tex[i].image ;
			return tex[0].image;
		}
	}
//**************************************************************************************************


//**************************************************************************************************
// Geeft het pad terug
//**************************************************************************************************
	QString Textures::getUrl(int i){
		return tex[i].pad;
	}
//**************************************************************************************************


//**************************************************************************************************
// geeft een vector aan pixels terug
//**************************************************************************************************
	QVector<QRgb>  Textures::colortable(int i){
		return tex[i].image->colorTable();
	}
//**************************************************************************************************


//**************************************************************************************************
// Geeft een pixel terug
//**************************************************************************************************
	QRgb Textures::px(int i, int x, int y){
		return tex[i].image->pixel(x,y);
	}
//**************************************************************************************************


//**************************************************************************************************
// Geeft de vectorgrootte terug
//**************************************************************************************************
	int Textures::getSize(){
		return tex.size();
	}
//**************************************************************************************************


//**************************************************************************************************
// destructor
//**************************************************************************************************
	Textures::~Textures(){
		Clear();
	}
//**************************************************************************************************


//**************************************************************************************************
// Verwijder een textuur
//**************************************************************************************************
	void Textures::del(int i){
		if(i<tex.size() && tex[i].pad!="")
		{
			delete tex[i].image;
			delete tex[i].large;
			delete tex[i].medium;
			tex[i].pad="";
		}
	}
//**************************************************************************************************


//**************************************************************************************************
// Lees in vanaf een file
//**************************************************************************************************
	void Textures::fromString(fstream *s)
	{
		int i;
		char* a=new char[222];
		int j=0;
		(*s) >> a;
		//indien er textures zijn
		if(a[0]!='#')
			while(true)
			{
				(*s)>>i;
				//indien einde lijst
				if(i==-1)
					break;
				//lees volledig pad in en haal de spaties voor en achteraan weg
				(*s).getline(a,221,'\n');
				j=0;
				while(a[j]==' '){
					j++;
				}
				for(int k=strlen(a)-1;k>0;k++)
				{
					if(a[k]==' ')
						a[k]='\0';
					else 
						break;
				}
				//toevoegen
				add(i,a+j);
				cout << "ingeladen texture: " << a << " nr:"<<i<<endl;
			}
		
	}

//**************************************************************************************************


//**************************************************************************************************
// Schrijf de textureslijst naar een string
//**************************************************************************************************
	string Textures::toString(){
		stringstream s;
		s<<"\n"<<"textures:"<<"\n";
		vector<img>::iterator it=tex.begin();
		for(int i=0;i<tex.size() ;i++ )
		{//enkel gebruikte plekken uit de container opslaan
			if(tex[i].pad!="")
				s<<i<<" "<<tex[i].pad.toStdString()<<" \n";
			cout << "ingeladen texture: " << tex[i].pad.toStdString() << endl;
		}
		//afsluiten met -1
		s<<"-1\n";
		//de volledige wereldfile afsluiten
		s<<'#';
		return s.str();
	}
//**************************************************************************************************


//**************************************************************************************************
// Leeg alle textures
//**************************************************************************************************
	void Textures::Clear()
	{
		for(int i=0;i<tex.size() ;i++ )
		{//indien deze gebruikt wordt
			if (tex[i].pad != "")
				tex[i].Clear();
		}
			tex.clear();
	}
//**************************************************************************************************