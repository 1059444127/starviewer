/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGCREATEDICOMDIR_H
#define UDGCREATEDICOMDIR_H

#include "dcddirif.h"     /* for class DicomDirInterface */

class string;

namespace udg {

class Status;

/** Converteix un directori determinat a dicomdir. Per a que un directori es pugui convertir a dicomdir, els fitxer dicom han de tenir un nom com a molt de 8 car�cters, i han d'estar guardats en little endian
	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class CreateDicomdir
{
public:

    enum Device { harddisk , cdDvd , usb };

    CreateDicomdir();

    /** Especifica a quin dispositiu crearem el dicomdir. �s important especificar el dispositiu correctament, sin� ens podem trobar, que no ens crei correctament el dicomdir
     * @param deviceToCreateDicomdir Dispositiu on crearem el dicomdir (harddisk,cdDvd,usb)
     */
    void setDevice( Device deviceToCreateDicomdir );
        
    /** Crea el fitxer DicomDir amb l'estructura dels estudis del directori passat per par�metre
     * @param dicomdirPath directori a convertir a dicomdir
     * @return estat de finalitzaci� del m�tode
     */
    Status create( std::string dicomdirPath );

    ~CreateDicomdir();

private:

    DicomDirInterface::E_ApplicationProfile m_optProfile;

};

}

#endif
