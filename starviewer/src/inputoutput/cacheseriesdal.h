/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGCACHESERIESDAL_H
#define UDGCACHESERIESDAL_H

class string;
class Status;

namespace udg {

class SeriesMask;
class Series;
class SeriesList;
/** Classe que cont� els m�todes d'acc�s a la Taula series
	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class CacheSeriesDAL
{

public:
    CacheSeriesDAL();

    /** Insereix una s�rie a la cach�
     * @param series Dades de la s�rie
     * @return retorna l'estat de la inserci�
     */
    Status insertSeries( Series * serie ); 

    /** Cerca les s�ries demanades a la m�scara. Important! Aquesta acci� nom�s t� en compte l'StudyUID de la m�scara per fer la cerca, els altres camps de la m�scara els ignorar�!
     * @param  mascar� de la serie amb l' sstudiUID a buscar
     * @param  retorna la llista amb la s�ries trobades
     * @return retorna estat del m�tode
     */
    Status querySeries( SeriesMask mask , SeriesList &list );

    /** Esborra les series que tinguin el studyUID passat per par�metre
      * @param studyUID StudyUID de les imatges a esborrar
      */
    Status deleteSeries ( std::string studyUID );

    ~CacheSeriesDAL();

private:

    /** Construeix la sent�ncia per buscar les s�ries d'un estudi
     * @param mask m�scara de cerca
     * @return sent�ncia sql
     */    
    std::string buildSqlQuerySeries( SeriesMask *seriesMask );
	
};

}

#endif
