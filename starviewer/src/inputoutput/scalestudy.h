/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGSCALESTUDY_H
#define UDGSCALESTUDY_H

#include <string>

namespace udg {

class SeriesList;
class Status;
class ImageMask;

/** Escala les imatges del mig de cada s�rie d'un estudi, per poder ser previsualitzades quant es consulta la cach�
@author Grup de Gr�fics de Girona  ( GGG )

*/
class ScaleStudy
{

public:
    
    ///Constructor de la classe
    ScaleStudy();
    
    /** Escala una imatge de cada s�rie per poder fer la previsualitzaci� a la cach� local del studyUID 
     * @param Uid de l'estudi a escakar
     */
    void scale( std::string studyUID );
        
    /** Cerca les series de l'estudi
     * @param El UID de l'estudi a cercar
     * @param [out] retorna les s�ries de l'estudi
     * @return retorna l'estat del m�tode
     */
    Status getSeriesOfStudy( std::string studyUID , SeriesList &seriesList );
        
    /** Compta el n�mero d'imatges de l'estudi
     * @param m�scara de les imatges a comptar
     * @param n�mero d'imatges
     * @return retorna l'estat del m�tode
     */
    Status countImageNumber( ImageMask , int &number );
    
    /** busca el path retatiu de la imatge que se li passa per par�metre
     * @param m�scara de la imatge a buscar el path relatiu
     * @param path relatiu, respecte el directori de l'estudi
     * @return retorna l'estat del m�tode
     */
    Status imageRelativePath( ImageMask , std::string &relPath );
    
    ///Destructor de la classe
    ~ScaleStudy();
};

};

#endif
