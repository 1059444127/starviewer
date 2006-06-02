/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#ifndef UDGSTARVIEWERPROCESSIMAGE_H
#define UDGSTARVIEWERPROCESSIMAGE_H

#include <qobject.h>
#include <qwidget.h>
#include <QString>

#include "processimage.h"
#include "cachepacs.h"

namespace udg {

class Image;

/**
@author marc
*/
class StarviewerProcessImage: public QObject, public ProcessImage{

Q_OBJECT

public:

    ///Constructor de la classe
    StarviewerProcessImage();

    /** Processa la informacio de la imatge a descarregar, la guarda a la cache, si la imatge pertany a una nova s�rie tamb� guarda la informaci� de la s�rie
     * @param imatge a processar 
     */
    void process( Image* image );
    
    /// Retorna si s'ha produit algun error intentant guardar alguna de les imatges al disc dur
    bool getErrorRetrieving();

    ///Destructor de la classe    
    ~StarviewerProcessImage();

signals :

    /** signal que s'emet a QExcuteOperationThread per indica que s'ha descarregat una image
      *@param uid de l'estudi
      *@param n�mero d'imatge
      */
    void imageRetrieved( QString studyUID , int );

    /** signal que s'emet a QExcuteOperationThread per indica que s'ha descarregat una s�roe
      *@param n�mero d'imatge
      */
    void seriesRetrieved( QString studyUID );

    /** signal que s'emet a QExcuteOperationThread per indica que s'ha inicia la desc�rrega d'un estudi
      *@param n�mero d'imatge
      */
    void startRetrieving( QString );

    /** signal que s'emet a QExcuteOperationThread per indicar que hi ha una s�rie a punt per ser visualitzada
      *@para UID de l'estudi
      */
    void seriesView ( QString );
    
private :

    CachePacs *m_localCache;
    int m_downloadedImages , m_downloadedSeries;
    QString m_oldSeriesUID,m_studyUID;
    bool m_error;
    
    /** Crea el path de la imatge d'on obtenir la informaci� de les series
     * @param imatge de la que s'ha d'obtenir el path
     */
    QString createImagePath( Image* image );
    
    /** Retorna la informaci� de la s�rie de la imatge que es troba al path del par�metre
     * @param path de la imatge d'on obtenir la informaci� de la s�rie
     */
    Status getSeriesInformation( QString imagePath , Series &serie );

    /// Indica que s'ha produit algun error intentant guardar alguna de les imatges al disc dur
    void setErrorRetrieving();
};

};

#endif

