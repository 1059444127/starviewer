#ifndef PACSPARAMETERS
#define PACSPARAMETERS

#define HAVE_CONFIG_H 1
#include <cond.h>
#include <list>

/** Aquesta classe ens permet configurar els par�metres necessaris que l'usuari ens ha d'entrar per connectar-nos en el pacs. A m�s d'altres dades d'inter�s
 * Aquests par�metres s�n :
 *            -IP o Adr. del Host
 *            - Port de connexi�
 *            - AE title de la m�quina local
 *            - AE title del PACS al que es vol connectar l'usuari
 *            - Instituci� al que pertany al PACS
 *            - Descripci� del Pacs
 *            - Localitzacio del Pacs
 *            - Default, estableix si aquest PACS �s el predeterminat per realitzar les cerques 
 */
namespace udg{

class string;
class Study;
class PacsParameters
{

public :
    
    /** Constructor r�pid de la classe. Aquest constructor inclou tots els par�mentres m�nim requerits per connectar-nos amb el PACS.
     *  IMPORTANT!!!! si l'objectiu de la connexi� �s descarregar imatges s'ha d'invocar la funci� setLocalPort
     * @param   adr Adre�a de pacs al qual ens volem connectar
     * @param   port Port en el qual el pacs esta escoltant les peticions          
     * @param   aet AETitle del PACS al qual ens volem connectar
     * @param   aec AETitle de la nostra m�quina. Cada m�quina ha de tenir un AETitle �nic. El PACS comprova al connectar-nos si la nostra IP correspont al nostre AETitle que li enviem
    */
    PacsParameters( std::string ,  std::string ,  std::string  , std::string  );
    
    /// Constructor buit de la classe
    PacsParameters();
    
    /** assigna l'adre�a al pacs al qual ens volem connectar
     * @param  Adre�a del PACS 
     */
    void setPacsAdr( std::string );

    /** assigna el port del pacs al qual ens volem connectar
     * @param port de connexi� del pacs
     */
    void setPacsPort( std::string );

    /** assigna l'aet title de la nostre m�quina local       
     * @param AE Title de la m�quina local
     */
    void setAELocal( std::string );
    
    /** assigna l'ae title del pacs al qual ens volem connectar
     * @param AE title del pacs al que es desitja connectar 
     */
    void setAEPacs( std::string );
            
    /** assigna el port pel qual nosaltres volem rebre les imatges del PACS
     * @param Port local
     */
    void setLocalPort( std::string );
    
    /** assigna la descripci� del PACS 
     * @param descripci� del PACS
     */
    void setDescription( std::string );
    
    /** assigna la institucio a la qual pertany el pacs                 
     * @param Institucio a la qual pertany al pcacs
     */
    void setInstitution( std::string );
    
    /** assigna la localitzaci� del PACS 
     * @param Localitzaci� del PACS
     */
    void setLocation( std::string );
        
    /** assigna al Pacs si �s el Predeterminat
     * @param indica si el PACS �s el predeterminat per defecte
     */
    void setDefault( std::string );
    
    /** assigna el timeout de la connexio
     * @param  time out en segons
     */
    void setTimeOut( int );
    
    /** assigna l'id del PACS, aquest �s un camp clau de la base de dades per diferenciar els PACS
     * @param id el pacs
     */
    void setPacsID( int );
    
    /** retorna l'adre�a del pacs 
     * @return Adre�a del pacs 
     */
     std::string  getPacsAdr();
    
    /** retorna el port del pacs 
     * @return Port del pacs 
     */
     std::string  getPacsPort();
    
    /** retorna l'AE Local
     * @return AE title local 
     */
     std::string  getAELocal();
    
    /** retorna l'AE del pacs 
     * @return Adre�a del pacs 
     */
     std::string  getAEPacs();
    
    /** retorna el port Local pel qual desitgem rebre les imatges
     * @return Port local 
     */
     std::string  getLocalPort();
    
    /** retorna la descripci� del PACS
     * @return descripci�
     */
     std::string  getDescription();
    
    /** retorna la instituci� a la que pertany el PACS
     * @return institucio
     */
     std::string  getInstitution();
    
    /** retorna la localitzaci� del PACS
     * @return localitzaci�
     */
     std::string  getLocation(); 
        
    /** retorna si �s el pacs predeterminat
     * @return si val 'S' voldr� di que �s el pacs predeterminat per realitzar les cerques
     */
     std::string  getDefault();
    
    /** retorna l'id del PACS, aquest es un camp clau de la taula PacsList, que l'assigna l'aplicacio
     * @return Id del pacs
     */
    int getPacsID();
    
    /** retorna el temps de time out en ms 
     * @return Time out
     */
    int getTimeOut();

private :

    std::string  m_aeCalled;
    std::string  m_aeTitle;
    std::string  m_Port;
    std::string  m_Adr;
    std::string  m_LocalPort; //especifica el port pel qual rebrem imatges
    std::string  m_Desc;
    std::string  m_Inst;
    std::string  m_Location;
    std::string  m_Default;
    int          m_PacsID; //camp clau de la taula PacsList, serveix per identificar els PACS, �s invisible per a l'usuari, aquest camp l'assigna l'aplicacio
    int m_TimeOut;

};
};
#endif
