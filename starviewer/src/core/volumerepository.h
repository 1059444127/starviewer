/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/

#ifndef UDGVOLUMEREPOSITORY_H
#define UDGVOLUMEREPOSITORY_H

#include "repository.h"
#include "volume.h"
#include "identifier.h"
#include <QObject>

namespace udg {


/**
    Aquesta classe �s el repositori de volums. En aquesta classe es guarden tots els volums que hi ha oberts durant
    l'execuci� del programa. Nom�s hi haura una sola inst�ncia en tota la vida del programa d'aquesta classe. 
    Per fer-ho s'aplica el patr� Singleton.
    
    Per poder obtenir una inst�ncia del repositori hem de fer un include del fitxer volumerepository.h i fer una crida 
    al m�tode VolumeRepository::getRepository(). Aquest ens retornar� un punter al repositori de volums.
    
    Exemple:
    
    \code
    #include "volumerepository.h"
    ...
    udg::VolumeRepository* m_volumeRepository;
    m_volumeRepository = VolumeRepository::getRepository();
    ...
    Volume* m_volume = m_volumeRepository->getVolume(id);    
    \endcode
    
    @author GGG
*/

class VolumeRepository : public Repository<Volume>
{
Q_OBJECT
public:

    /**
        Afegeix un volum al repositori.
        Ens retorna l'id del volum afegit per poder-lo obtenir m�s endavant.
    */    
    Identifier addVolume( Volume *model ); 
    
    /// Ens retorna un volum del repositori amb l'identificador que especifiquem.
    Volume *getVolume( Identifier id );
    
    /// Elimina un model del repositori
    void removeVolume( Identifier id );

    /// Retorna el nombre de volums que hi ha al repositori
    int getNumberOfVolumes();
    
    /// Ens retorna l'�nica inst�ncia del repositori.
    static VolumeRepository* getRepository()
    {   
        static VolumeRepository repository;
        return &repository;
    }

    /// El destructor allibera l'espai ocupat pels volums
    ~VolumeRepository(){};
    
signals:
    void itemAdded( Identifier id );
    void itemRemoved( Identifier id );
            
private:
    /// ha de quedar amagat perqu� no poguem crear inst�ncies
    VolumeRepository();                 
};

};  

#endif
