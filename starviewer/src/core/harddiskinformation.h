/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef HARDDISKINFORMATION_H
#define HARDDISKINFORMATION_H

#include <QString>

namespace udg {

/**
Classe que ens permet obtenir informaci� sobre un disc dur. M�s concretament ens permet obtenir els Byte, MB, GB d'una partici� i quins d'aquests s�n lliures. Cal tenir present que quan es vol saber l'espai lliure d'un disc dur s'ha d'especificar de quina partici�. En windows s'hauria d'especificar la unitat (c:, d:, ...) i un directori i en linux s'ha d'especificar la ruta sensera del directori per saber la partici�. Actualment aquesta classe nom�s suporta els sistemes operatius basats en Unix (Linux, Mac OS X ...).

	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class HardDiskInformation{
public:
    /**
     * Constructor de la classe
     */
    HardDiskInformation();

    /**
     * Destructor de la classe
     */
    ~HardDiskInformation();

    /**
     * M�tode que ens serveix per saber el n�mero de Bytes total (lliures+ocupats) que hi ha en una partici� concreta. Per indicar la
     * partici� s'ha d'especificar el path absolut a un fitxer o directori que estigui en aquesta.
     *
     * El m�tode no comprova que existeixi el path.
     *
     * Si hi ha qualsevol error el m�tode retornar� 0.
     * @param path Indica el path a un directori/fitxer dintre de la partici�
     * @return El nombre total de bytes que t� una partici�
     */
    unsigned long getTotalNumberOfBytes(QString path);

    /**
     * Retorna el n�mero de Bytes d'espai lliure que ens queden en una partici� concreta i que poden ser utilitzats per l'usuari. Cal fer
     * notar que aquest n�mero pot ser diferent del nombre real de bytes lliures (per exemple en casos que hi hagi quotes per usuari). Per
     * indicar la partici� s'ha d'especificar el path absolut a un fitxer o directori que estigui en aquesta.
     *
     * El m�tode no comprova que existeixi el path.
     *
     * Si hi ha qualsevol error el m�tode retornar� 0.
     * @param path Indica el path a un directori/fitxer dintre de la partici�
     * @return El nombre de bytes lliures que t� una partici� i poden ser utilitzats per l'usuari que executa el programa
     */
    unsigned long getNumberOfFreeBytes(QString path);

    /**
     * Es comporta exactament igual que getTotalNumberOfBytes() per� retorna MBytes en comptes de Bytes. Cal tenir en compte, per�, que
     * aquest no �s un m�tode per saber el n�mero de MBytes amb absoluta precissi� (per aix� fer servir getTotalNumberOfBytes() )
     * @param path Indica el path a un directori/fitxer dintre de la partici�
     * @return El nombre de MBytes truncats (ex.: si �s 1,9MBytes reals retornar� 1Mbytes)
     */
    unsigned long getTotalNumberOfMBytes(QString path);
    
    /**
     * Es comporta exactament igual que getNumberOfFreeBytes() per� retorna MBytes en comptes de Bytes. Cal tenir en compte, per�, que
     * aquest no �s un m�tode per saber el n�mero de MBytes amb absoluta precissi� (per aix� fer servir getNumberOfFreeBytes() )
     * @param path Indica el path a un directori/fitxer dintre de la partici�
     * @return El nombre de MBytes lliures truncats (ex.: si �s 1,9MBytes reals retornar� 1MByte)
     */
    unsigned long getNumberOfFreeMBytes(QString path);

private:
    quint64 getTotalBytesPlataformEspecific(QString path);
    quint64 getFreeBytesPlataformEspecific(QString path);

};

};  //  end  namespace udg

#endif
