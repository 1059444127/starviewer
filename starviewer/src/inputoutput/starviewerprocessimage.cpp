/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#include "starviewerprocessimage.h"
#include <string.h>
#include "imagedicominformation.h"
#include "series.h"
#include "starviewersettings.h"

namespace udg {

StarviewerProcessImage::StarviewerProcessImage()
{
        m_downloadedImages=0;
        m_localCache = CachePacs::getCachePacs();
        m_error = false;
        m_downloadedSeries = 0;
}


/** Enviar un signal de imatge descarregada a qretrievescreen, i quant es descarrega una s�rie nova envia signal de series descarregada
  */
void StarviewerProcessImage::process(Image *image)
{
    Status state;

    /*si es la primera imatge que es descarrega fem un signal indicant que comen�a la descarrega, inserim la primera
    serie, i ara que tenim la informaci� de la s�rie update la modalitat de l'estudi*/
    if (m_downloadedImages == 0)
    {  
        Series serie;
        
        //enviem un signal indicant que ha comen�at la descarrega de l'estudi
        emit(startRetrieving( image->getStudyUID().c_str()));
        
        //canviem l'estat de l'estudi de PENDING A RETRIEVING
        state = m_localCache->setStudyRetrieving( image->getStudyUID().c_str() );
        if ( !state.good() ) m_error = true;
        
        
        //inserim serie
        state = getSeriesInformation ( createImagePath( image ), serie );
        
        if (state.good())
        {
            state = m_localCache->insertSeries( &serie );
        }
        //si es produeix error no podem cancel�lar la descarregar, tirem endavant, quant finalitzi la descarregar avisarem de l'error
        if ( !state.good() ) m_error = true;
        
        state = m_localCache->setStudyModality( serie.getStudyUID() , serie.getSeriesModality() );
        
        //si es produeix error no podem cancel�lar la descarregar, tirem endavant, quant finalitzi la descarregar avisarem de l'error
        if ( !state.good() ) 
        {
            m_error = true;
        }
        else m_oldSeriesUID = image->getSeriesUID().c_str();
        
        m_studyUID = image->getStudyUID().c_str();
    }
    
    //inserim la nova s�rie que es descarrega
    if (m_oldSeriesUID != image->getSeriesUID().c_str())
    {
        Series serie;
    
        //inserim serie
        state = getSeriesInformation ( createImagePath( image ), serie );
        
        if (state.good())
        {
            state = m_localCache->insertSeries( &serie );
        }
        //si es produeix error no podem cancel�lar la descarregar, tirem endavant, quant finalitzi la descarregar avisarem de l'error
        //l'error 2019 no el tractem, perqu� indica serie duplicada, alguns pacs envien les imatges desornades per tant pot ser que les imatges d'una mateixa serie arribin separades, i intentem inserir altre vegada la serie
        if (!state.good() && state.code() != 2019 )
        {
            m_error = true;
        }
        //Nomes emitirem el signal quant la operacio s'hagi realitzat amb exit, pq hi ha alguns PACS que retornen les
        //imatges desordenades, i no venen agrupades per s�ries
        else  
        {
            emit(seriesRetrieved( image->getStudyUID().c_str() ) );
            
            if ( m_downloadedSeries == 0 )
            {
                emit( seriesView( image->getStudyUID().c_str() ) ); //aquest signal s'emet cap a qexecoperationthread, indicant que hi ha apunt una serie per ser visualitzada
            }
            m_downloadedSeries++;
        }

        m_oldSeriesUID = image->getSeriesUID().c_str();
    }
    
    //inserim imatge
    state = m_localCache->insertImage(image);
    //si es produeix error no podem cancel�lar la descarregar, tirem endavant, quant finalitzi la descarregar avisarem de l'error
    if (!state.good()) m_error = true;
   
    m_downloadedImages++;
    emit(imageRetrieved(image->getStudyUID().c_str(),m_downloadedImages));
    
}

/** Indica que s'ha produit algun error intentant guardar alguna de les imatges al disc dur
  */
/* Ara per ara per la configuraci� de les dcmtk no he descober com cancel�lar la descarrega d'imatges despres de produir-se un error,
  *l'�nic soluci� possible ara mateix i que m'han aconsellat als forums es matar el thread per� aquesta idea no m'agrada perqu� si matem
  *el thread no desconnectem del PACS, no destruim les senyals amb el QRetreiveScreen i no esborrem el thread de la llista retrieveThreads,
  *per tant de moment el que es far� es donar el error quant hagi finalitzat la descarrega
  */
void StarviewerProcessImage::setErrorRetrieving()
{
    m_error = true;
}

/** Retorna si s'ha produit algun error intentant guardar alguna de les imatges al disc dur
  */
bool StarviewerProcessImage::getErrorRetrieving()
{
    return m_error || m_downloadedImages == 0;
} 

/** Retorna la informaci� de la s�rie de la imatge que es troba al path del par�metre
  *     @param path de la imatge d'on obtenir la informaci� de la s�rie
  */
Status StarviewerProcessImage::getSeriesInformation( QString imagePath, Series &serie )
{
    Status state;
    QString path;
    
    ImageDicomInformation dInfo;
    
    state = dInfo.openDicomFile( imagePath.toAscii().constData() );
    
    serie.setStudyUID( dInfo.getStudyUID() );
    serie.setSeriesUID( dInfo.getSeriesUID() );
    serie.setSeriesNumber( dInfo.getSeriesNumber() );
    serie.setSeriesModality( dInfo.getSeriesModality() );
    serie.setSeriesDescription( dInfo.getSeriesDescription() );
    serie.setBodyPartExaminated( dInfo.getSeriesBodyPartExamined() );
    serie.setProtocolName( dInfo.getSeriesProtocolName() );
    
    //calculem el path de la serie
    path = dInfo.getStudyUID().c_str();
    path.append( "/" );
    path.append( dInfo.getSeriesUID().c_str() );
    path.append( "/" );
    
    serie.setSeriesPath( path.toAscii().constData());
    
    return state; 

}

/** Crea el path de la imatge d'on obtenir la informaci� de les series
  *     @param imatge de la que s'ha d'obtenir el path
  */
QString StarviewerProcessImage::createImagePath(Image *image)
{

    StarviewerSettings settings;
    Series serie;
    QString imagePath;   
    
    imagePath.insert( 0,settings.getCacheImagePath() );
    imagePath.append( image->getStudyUID().c_str() );
    imagePath.append( "/" );
    imagePath.append( image->getSeriesUID().c_str() );
    imagePath.append("/");
    imagePath.append( image->getImageName().c_str() );

    return imagePath;
}

StarviewerProcessImage::~StarviewerProcessImage()
{
}


};

