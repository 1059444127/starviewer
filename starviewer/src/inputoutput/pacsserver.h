#ifndef PACS
#define PACS

#define HAVE_CONFIG_H 1

#include <assoc.h>
#include "pacsparameters.h"
#include "pacsnetwork.h"
#include "pacsconnection.h"

class string;
class Status;

/** Aquest classe �s la que ens ajuda interectuar amb el pacs.
 * La classe cont� les principals funcions i accions per connectar-nos en el pacs amb l'objectiu, de fer un echo, buscar informaci� o descarregar imatges. Alhora de connectar-nos al constructor passem els par�metres m�nim per connectar-nos. �s molt important llegir molt b� la documentaci� d'aquesta classe per saber quins par�metres utiltizar, si no voleu tenir problemes alhora de buscar informaci�,descarrega imatges
 */
namespace udg{

class PacsServer
{

public:
 
    enum modalityConnection { query , retrieveImages , echoPacs };
    
    //any it's only can be used with echoPacs modality
    enum levelConnection { patientLevel , studyLevel , seriesLevel,any , imageLevel };
   
    /** Constuctor de la classe. Se li ha de passar un objecte PacsParameters, amb els par�metres del pacs correctament especificats
     * @param Parametres del Pacs a connectar
     */
    PacsServer( PacsParameters );

    /// Constructor buit de la classe
    PacsServer();
    
    /** Aquesta funci� ens intenta connectar al PACS
     * @param Especifica en quina modalitat ens volem connectar, fer echo, busca informaci� o descarregar imatges
     * @param l Especifica a quin nivell durem a terme l'objectiu especificat al par�metre anterior. A nivell de  Pacient,estudi o s�rie. Per comprendre millor els nivells  consultar la documentaci� del dicom C�pitol 4. C.6
     * @return retorna l'estat de la connexi�
     */
    Status Connect( modalityConnection , levelConnection );
       
    /** Ens permet fer un echo al PACS. Per defecte per qualsevol modalitat de connexi� b� sigui busca informaci� o descarregar imatges per defecte permet fer un echo, per comprovar si els PACS est� viu  
     * @return retorna l'estatus del echo
     */   
    Status Echo();//This function makes an echo to the PACS
    
    /** Estableix un pacs per a la connexi�
     * @param Pacs amb els par�metres per a la connexi�
     */
    void setPacs( PacsParameters );
    
    /** Retorna una connexi� per a poder buscar informaci�, o descarregar imatges
     * @return retorna una connexi� de PACS
     */
    PacsConnection getConnection();
    
    /** Retorna una configuraci� de xarxa. Nom�s �s necess�ria quan l'objectiu de la connexi� sigui el de descarregar imatges
     * @return retorna la configuraci� de la xarxa          
     */
    T_ASC_Network * getNetwork();
    
    /// This action close the session with PACS's machine and release all the resources
    void Disconnect();

private:
    
   T_ASC_Network *m_net; // network struct, contains DICOM upper layer FSM etc.
   T_ASC_Parameters *m_params; // parameters of association request
   T_ASC_Association *m_assoc; // request DICOM association;
   PacsNetwork *m_pacsNetwork; //configures the T_ASC_Network
    
   PacsParameters m_pacs;
   
    /** Aquesta funci� �s privada. �s utilitzada per especificar en el PACS, que una de les possibles operacions que volem fer amb ell �s un echo. Per defecte en qualsevol modalitat de connexi� podrem fer un echo
     * @return retorna l'estat de la configuraci�
     */
   OFCondition ConfigureEcho(); 
   
    /** Aquesta funci� privada, configura els par�metres de la connexi� per especificar, que el motiu de la nostre connexi� �s buscar informaci�.
     * @param Especifiquem a quin nivell volem buscar la informaci�, de pacient, d'estudi o de s�rie
     * @return retorna l'estat de la configuraci�
     */
   OFCondition ConfigureFind( levelConnection );
   
    /** Aquesta funci� privada permet configurar la connexi� per a descarregar imatges al ordinador local. IMPORTANT!!! Abans de connectar s'ha d'invocar la funci� setLocalhostPort
     * @param Especifiquem a quin nivell volem descarregar les imatges, de pacient, d'estudi o de s�rie
     * @return retorna l'estat de la configuraci�
     */
   OFCondition ConfigureMove( levelConnection );
   
    /** Aquesta funci� privada, configura els par�metres pel tipus de connexi� per descarregat imatges segons la m�quina que tinguem
     * @param par�metres de la connexi�
     * @param Identificador de funcio
     * @param Objectiu de la funcio
     * @return retorna l'estat de la funci�
     */
   OFCondition addPresentationContextMove( T_ASC_Parameters * , T_ASC_PresentationContextID , const char* );
        
    /** Construeix l'adre�a del servidor en format ip:port, per connectar-se al PACS
     * @param adre�a del servidor
     * @param port del servidor
     */
   std::string ConstructAdrServer( std::string , std::string ); //construct PACS address
    
};
};
#endif
