/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#include "qseriesiconview.h"
#include <q3iconview.h>
#include <QString>
#include <qpixmap.h>
#include <qimage.h>
//Added by qt3to4:
// #include <QImageIO>
#include <QImageReader>
#include "starviewersettings.h"

namespace udg {

/** Constructor de la classe
  */
QSeriesIconView::QSeriesIconView(QWidget *parent )
 : QWidget( parent )
{
    setupUi( this );
    QString className;    

    className.insert(0,this->name());
    //indiquem que despr�s d'inserir s'ha d'ordenar l'iconview ascendentment
    SeriesListV->setSorting(true,true);
    
}

/** Insereix l'informaci� d'una s�rie al ListICon
  *          @param descripci� de la s�rie
  *          @param Nombre d'imatges de la s�rie
  *          @param path de la imatge a mostrar de la s�rie
  */
void QSeriesIconView::insertSeries(Series *serie)
{
    QString text,num,scaledPathImage,nameClass;
    QPixmap image;
//     QImageIO iio;
    QImageReader iioReader;
    StarviewerSettings settings;
    
    text.insert(0,tr("Series "));
    text.append(serie->getSeriesNumber().c_str() );
    text.append('\n');
    
    if (serie->getImageNumber() > 0)
    {
        num.setNum(serie->getImageNumber() );
        text.append(num);
        text.append(" images");
        text.append('\n');
    }
    
    if (serie->getSeriesDescription().length() > 0)
    {//si hi ha descripci� la inserim
        text.append(serie->getSeriesDescription().c_str() );
        text.append('\n');
    }
        
    nameClass.insert(0,this->name());
    if (nameClass == "SeriesImViewCache")
    {
        scaledPathImage.insert(0,settings.getCacheImagePath());
        scaledPathImage.append(serie->getStudyUID().c_str() );
        scaledPathImage.append("/");
        scaledPathImage.append(serie->getSeriesUID().c_str() );
        scaledPathImage.append("/scaled.jpeg");
//         iio.setFileName(scaledPathImage);
//         if ( iio.read() ) image = iio.image();
        iioReader.setFileName(scaledPathImage);
        image = iioReader.read();
    }
    else
    {
        // \TODO lleig!!!!!!!!!!!!!!!!!
//         iio.setFileName( "/home/marc/starviewer-pacs/bin/images3.jpeg" );
//         if ( iio.read() ) image = iio.image(); // convert to pixmap
        iioReader.setFileName( "/home/marc/starviewer-pacs/bin/images3.jpeg" );
        image = iioReader.read();
    }
    Q3IconViewItem *series =  new Q3IconViewItem (SeriesListV, text, image );
    //inserim la clau per ordenar les series, la clau �s el n�mero de serie

    series->setKey(serie->getSeriesNumber().c_str() );
    
    SeriesListV->selectAll(true);
}

/** slot que s'activa quant es selecciona una serie, emiteix signal a QStudyListView, perqu� selecciona la mateixa serie que el QSeriesIconView
  *        @param serie Seleccionada
  */
void QSeriesIconView::clicked(Q3IconViewItem *item)
{

    if (item != NULL) emit(selectedSeriesIcon(item->index()));
    
}

/** slot que s'activa quant es fa doblec
  *        @param serie Seleccionada
  */
void QSeriesIconView::view(Q3IconViewItem *item)
{

    if (item != NULL) emit(viewSeriesIcon());
    
}

/** Slot que s'activa quant es selecciona una s�rie des del StudyListView,selecciona la serie del QStudyListView en el QSeriesIconView
  *        @param  clau (N�mero de la s�rie)
  */
void QSeriesIconView::selectedSeriesList(QString key)
{
    Q3IconViewItem *item;
    
    item = SeriesListV->firstItem();
    
    while (item!=0)
    {
        if (item->key() == key)
        {
            //Aquest tall no funciona, bug QT ? he provat d'altres maneres fer que assenyales nomes un iconitem, i quant el sectionMode �s single no ho fa  b�
            SeriesListV->setCurrentItem(item);
            SeriesListV->setSelected(item,true,false);
            item->setSelected(true);
            break;
        }
        else item = item->nextItem();
        
    }
    SeriesListV->selectAll(true);
    
}

/** Neteja el Icon View de s�ries
  */
void QSeriesIconView::clear()
{
    SeriesListV->clear();
}

/** Slot, que al rebre la senyal addSeries del del QStudyListView afegeix una s�rie al IconView
  *        @param [in] serie 
  */
void QSeriesIconView::addSeries(Series *serie)
{
    insertSeries(serie);
}

/** Slot, que al rebre la senya del QStudyListView neteja el IconView
  */
void QSeriesIconView::clearIconView()
{
    clear();
}

/** Destructor de la classe
  */
QSeriesIconView::~QSeriesIconView()
{
}


};
