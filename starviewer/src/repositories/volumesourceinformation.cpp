/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#include "volumesourceinformation.h"

namespace udg {

VolumeSourceInformation::VolumeSourceInformation()
{
}

VolumeSourceInformation::~VolumeSourceInformation()
{
}

void VolumeSourceInformation::setPatientName( const char *name )
{
    m_patientName = name;
}

};  // end namespace udg 
