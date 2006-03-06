/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGSCALEIMAGE_H
#define UDGSCALEIMAGE_H

/**  Aquesta classe passant-li un fitxer dicom ens retorna una imatge de la mida de pixels especificada. Converteix la imatge a PGM i l'escala, reduint considerablement el tamany de la imatge
  */

/*  Aquesta classe �s una modificaci� del programa dcmj2pnm. Per qualsevol dubte o per afegir funcions a aquesta classe consultar-la!!!
  */

namespace udg {

/**
@author marc
*/
class ScaleImage{
public:
    ScaleImage();
    
    ~ScaleImage();
    
    int dicom2lpgm(const char* ,const char *,int);
    
};

};

#endif
