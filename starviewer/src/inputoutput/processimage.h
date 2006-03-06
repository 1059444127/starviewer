/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#ifndef UDGPROCESSIMAGE_H
#define UDGPROCESSIMAGE_H


namespace udg {

class Image;


/** Classe que s'encarrega de dur a terme unes accions determinades per cada descarrega d'una imatge. Aquesta classe pot ser reimplementada
  * El m�todes d'aquesta classe actualment no duen a terme cap acci�, si s'ha d'utiltizar, s'ha de crear una classe que heredi d'aquesta i 
  * reimplementar els m�todes amb les accions que siguin necess�ries.
@author marc
*/
class ProcessImage{


public:
    
    ProcessImage();

    virtual void process(Image* image);//virtual significa que els fills la podran reimplementar a la seva classe, i que es cridara la dels fills de process no la del pare, quant es declara virtual nom�s s'ha de fer al .h  
    virtual void setErrorRetrieving();
    virtual bool getErrorRetrieving();
    virtual ~ProcessImage();

private:

    
};

};

#endif
