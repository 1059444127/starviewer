/***************************************************************************
 *   Copyright (C) 2005-2007 by Grup de Gràfics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/

#include "thumbnailcreator.h"

#include <QObject>
#include <QImage>
#include <QString>
#include <QPainter>

#include "series.h"
#include "image.h"
#include "logging.h"
#include "dicomtagreader.h"
#include "dicomdictionary.h"

// fem servir dcmtk per l'escalat de les imatges dicom
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/ofstd/ofbmanip.h"

namespace udg
{

    ///Crea un thumbnail a partir de les imatges de la sèrie
QImage ThumbnailCreator::getThumbnail(const Series *series, int resolution)
{
    QImage thumbnail;

    if(series->getModality() == "KO")
    {
        thumbnail.load(":/images/kinThumbnail.png");
    }
    else if(series->getModality() == "PR")
    {
        thumbnail.load(":/images/presentationStateThumbnail.png");
    }
    else if(series->getModality() == "SR")
    {
        thumbnail.load(":/images/structuredReportThumbnail.png");
    }
    else
    {
        int numberOfImages = series->getImages().size();

        if (numberOfImages > 0)
        {
            thumbnail = createImageThumbnail(series->getImages()[numberOfImages / 2 ]->getPath());
        }
        else
        {
            // si la sèrie no conté imatges en el thumbnail ho indicarem
            thumbnail = QImage(resolution, resolution, QImage::Format_RGB32);
            thumbnail.fill(Qt::black);

            QPainter painter(&thumbnail);
            painter.setPen(Qt::white);
            painter.drawText(0, 0, 100, 100, Qt::AlignCenter | Qt::TextWordWrap, QObject::tr("No Images Available"));
        }
    }

    return thumbnail;
}

QImage ThumbnailCreator::getThumbnail(const Image *image, int resolution)
{
    return createImageThumbnail(image->getPath(), resolution);
}

QImage ThumbnailCreator::createImageThumbnail(QString imageFileName, int resolution)
{
    QImage thumbnail;

    DICOMTagReader reader( imageFileName );
    if( isSuitableForThumbnailCreation(&reader) )
    {
        //carreguem el fitxer dicom a escalar
        DicomImage *dicomImage = new DicomImage( qPrintable(imageFileName) );
        thumbnail = createThumbnail(dicomImage,resolution);
        
        // Cal esborrar la DicomImage per no tenir fugues de memòria
        if ( dicomImage )
            delete dicomImage;
    }

    return thumbnail;
}

QImage ThumbnailCreator::createThumbnail(DicomImage *dicomImage, int resolution)
{
    QImage thumbnail;
    bool ok = false;

    if(dicomImage == NULL)
    {
        ok = false;
        DEBUG_LOG("Memòria insuficient per carregar l'imatge DICOM al fer el thumbnail o punter nul");
    }
    else if(dicomImage->getStatus() == EIS_Normal)
    {
        dicomImage->hideAllOverlays();
        dicomImage->setMinMaxWindow(1);
        //escalem l'imatge
        DicomImage *scaledImage;
        //Escalem pel cantó més gran
        unsigned long width, height;
        if(dicomImage->getWidth() < dicomImage->getHeight())
        {
            width = 0;
            height = resolution;
        }
        else
        {
            width = resolution;
            height = 0;
        }
        scaledImage = dicomImage->createScaledImage(width,height, 1, 1);
        if( scaledImage == NULL)
        {
            ok = false;
            DEBUG_LOG("La imatge escalada s'ha retornat com a nul");
        }
        else if(scaledImage->getStatus() == EIS_Normal)
        {
            // el següent codi crea una imatge pgm a memòria i carreguem aquest buffer directament al pixmap
            // obtingut de http://forum.dcmtk.org/viewtopic.php?t=120&highlight=qpixmap
            // get image extension
            const int width = (int)(scaledImage->getWidth());
            const int height = (int)(scaledImage->getHeight());
            char header[32];
            // create PGM header
            sprintf(header, "P5\n%i %i\n255\n", width, height);
            const int offset = strlen(header);
            const unsigned int length = width * height + offset;
            // create output buffer for DicomImage class
            Uint8 *buffer = new Uint8[length];
            if (buffer != NULL)
            {
                // copy PGM header to buffer
                OFBitmanipTemplate<Uint8>::copyMem((const Uint8 *)header, buffer, offset);
                if (scaledImage->getOutputData((void *)(buffer + offset), length, 8))
                {
                    if(thumbnail.loadFromData((const unsigned char *)buffer, length, "PGM"))
                    {
                        ok = true;
                    }
                    else
                        DEBUG_LOG(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Ha fallat :(");

                }
                // delete temporary pixel buffer
                delete[] buffer;
                // Cal esborrar la DicomImage per no tenir fugues de memòria
                delete scaledImage;
            }
        }
        else
        {
            ok = false;
            DEBUG_LOG(QString( "La imatge escalada té errors. Error: %1 ").arg( DicomImage::getString( scaledImage->getStatus())));
        }

    }
    else
    {
        ok = false;
        DEBUG_LOG(QString("Error en carregar la DicomImage. Error: %1 ").arg(DicomImage::getString( dicomImage->getStatus())));
    }

    // Si no hem pogut generar el thumbnail, creem un de buit
    if(!ok)
    {
        thumbnail = QImage(resolution, resolution, QImage::Format_RGB32);
        thumbnail.fill(Qt::black);

        QPainter painter( &thumbnail);
        painter.setPen(Qt::white);
        painter.drawText(0, 0, resolution, resolution, Qt::AlignCenter | Qt::TextWordWrap, QObject::tr("Preview image not available"));
    }

    return thumbnail;
}

bool ThumbnailCreator::isSuitableForThumbnailCreation(DICOMTagReader *reader) const
{
    if( !reader )
    {
        DEBUG_LOG("El DICOMTagReader donat és NUL!");
        return false;
    }
    
    bool suitable = true;

    // Ens hem trobat que per algunes imatges que contenen Overlays, la DICOMImage no es pot crear. 
    // Els casos que hem trobat estan descrits al ticket #1121
    // La solució adoptada ara mateix és que si trobem que la imatge conté algun dels tags següents, 
    // descartem la creació del thumbnail i en creem un de "neutre" indicant que no s'ha pogut crear aquest
    // En quant siguem capaços de tornar a llegir aquestes imatges sense problema, aquesta comprovació desapareixerà
    QList<DICOMTag> tags;
    tags << DICOMOverlayRows << DICOMOverlayColumns << DICOMOverlayType << DICOMOverlayOrigin << DICOMOverlayBitsAllocated << DICOMOverlayBitPosition <<  DICOMOverlayData;
    foreach( DICOMTag tag, tags )
    {
        if( reader->tagExists(tag) )
        {
            suitable = false;
            DEBUG_LOG( QString("Found Tag: %1,%2. Overlay restriction applied. Preview image won't be available.").arg(tag.getGroup(),0,16).arg(tag.getElement(),0,16) );
        }
    }

    // TODO Comprovar la modalitat també?
    return suitable;
}

};
