/******************************************************************************
Este arquivo eh parte da implementacao do ambiente declarativo do middleware
Ginga (Ginga-NCL).

Copyright (C) 2009 UFSCar/Lince, Todos os Direitos Reservados.

Este programa eh software livre; voce pode redistribui-lo e/ou modificah-lo sob
os termos da Licenca Publica Geral GNU versao 2 conforme publicada pela Free
Software Foundation.

Este programa eh distribuido na expectativa de que seja util, porem, SEM
NENHUMA GARANTIA; nem mesmo a garantia implicita de COMERCIABILIDADE OU
ADEQUACAO A UMA FINALIDADE ESPECIFICA. Consulte a Licenca Publica Geral do
GNU versao 2 para mais detalhes.

Voce deve ter recebido uma copia da Licenca Publica Geral do GNU versao 2 junto
com este programa; se nao, escreva para a Free Software Foundation, Inc., no
endereco 59 Temple Street, Suite 330, Boston, MA 02111-1307 USA.

Para maiores informacoes:
lince@dc.ufscar.br
http://www.ncl.org.br
http://www.ginga.org.br
http://lince.dc.ufscar.br
http://www.icmc.usp.br/php/laboratorio.php?origem=icmc&id_lab=3&nat=icmc

/**
* @file EnhancedInputManager.h
* @author Caio Viel
* @author Diogo de Carvalho Pedrosa
* @author José Augusto Costa Martins Júnior
* @date 29-01-10
*/

#ifndef ENHANCEDINPUTMANAGER_H
#define ENHANCEDINPUTMANAGER_H

#include <ginga/system/io/interface/input/CodeMap.h>
using namespace ::br::pucrio::telemidia::ginga::core::system::io;

#include <ginga/linceutil/LoggerUtil.h>
#include <ginga/linceutil/BadArgumentException.h>
using namespace br::ufscar::lince::util;

#include <ginga/system/thread/Thread.h>
using namespace ::br::pucrio::telemidia::ginga::core::system::thread;

#include "util/functions.h"
using namespace ::br::pucrio::telemidia::util;

#include "IEnhancedInputManager.h"
#include "InputModeModifier.h"
#include "InputModeRedirecter.h"

#include "IMultimodalInputEvent.h"
#include "IMultimodalInputEventListener.h"
#include "DuplicateException.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
using namespace std;

/**
 * Estrutura que representa uma atualização da lista de observadores de eventos
 * multimodais. A ação de atualização pode ser de adição ou remoção da lista.
 */
typedef struct lockedMultimodalListenerAction {
	/// Observador a ser acrescentado ou removido.
	::br::ufscar::lince::ginga::core::mmi::IMultimodalInputEventListener* l;
	/// Ação a ser realizada.
	bool isAdd;
} LockedMultimodalAction;

/**
 * Estrutura que representa uma atualização da lista de observadores de eventos.
 * A ação de atualização pode ser de adição ou remoção da lista.
 */
typedef struct lockedListenerAction {
	/// Observador a ser acrescentado ou removido.
	::br::pucrio::telemidia::ginga::core::system::io::IInputEventListener* l;
	/// Ação a ser realizada.
	bool isAdd;
	/// Conjunto de códigos de tecla nos quais o observador está interessado.
	set<int>* events;
} LockedAction;


namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{

/**
 * Essa classe visa substituir o InputManager do componente Ginga-CC System IO.
 * Ela implementa a interface IEnhacedInputManager, que estende IInputManager.
 * Essa classe não estende a classe EnhacendInputManager pois assim evitamos:
 * <br>
 * - O problema do singleton estendido<br>
 * - O problema de ter que implementar no EIM métodos que simplesmente servem
 * para chamar os métodos já implementados do IM, pelo fato deles não estarem
 * acessíveis mesmo sendo herdados.<br>
 * - O problema de o InputManager.h não ser instalado junto com os demais
 * includes do Ginga no sistema. Apenas sua interface IInputManager.h é
 * instalada.<br>
 * Essa decisão é válida também porque, além de simplificar muito o entendimento
 * do código, quem usa a extensão, não tem mais o menor interesse em ter a
 * versão não estendida em paralelo.
 */
class EnhancedInputManager : public IEnhancedInputManager, public Thread {

	protected:
		/// Instância única do EnhancedInputManager
		static EnhancedInputManager* _instance;

		// Para tratamento do InputMode

		/// Código de tecla de um evento que indica mudança do modo de entrada.
		static int KEY_MODE_CHANGE;

		/// Conjunto de modos de entrada.
		vector<IInputMode*>* inputModes;

		/// Mapeamento entro o nome do modo de entrada e sua instância.
		map<string, IInputMode*>* inputModeMap;

		/// Indica o modo de entrada atualmente usado.
		IInputMode* currentInputMode;

		/// Indica o índice do modo de entrada atualmente usado.
		int inputModeIndex;
		
		/// Responsável pelo controle das mensagens de log
		HLoggerPtr logger;

		// Para tratamento de eventos multimodais.

		/// Conjunto de observadores de eventos multimodais.
		set<IMultimodalInputEventListener*>* multimodalEventListeners;

		/// Conjunto de observadores a serem acrescentados ou removidos em
		/// multimodalEventListeners.
		vector<LockedMultimodalAction*>* actionsToMultimodalListeners;

		/// Inidica se o componente está no meio do processo de notificação de
		/// eventos de entrada para os observadores.
		bool notifyingMultimodal;

		/// Controla o acesso concorrente ao multimodalEventListeners
		pthread_mutex_t multimodalMutex;

		/// Controla o acesso concorrente ao actionsToMultimodalListeners
		pthread_mutex_t actMultimodalMutex;

		// Para manipulação de teclas

		/// Instância utilizada para adicionar novos códigos de teclas
		CodeMap* codeMap;

		// Para manipulação dos eventos originais da InputManager

		/// Armazena as instâncias que aguardam por eventos e o conjuntos
		///	dos eventos que são aguardados por estas instâncias.
		map<IInputEventListener*, set<int>*>* eventListeners;

		/// Conjunto de observadores a serem acrescentados ou removidos em
		/// applicationListeners.
		vector<LockedAction*>* actionsToInpListeners;

		/// Conjunto de observadores de aplicações.
		set<IInputEventListener*>* applicationListeners;

		/// Conjunto de observadores de aplicações a serem acrescentados ou
		/// removidos em applicationListeners.
		vector<LockedAction*>* actionsToAppListeners;

		/// Denota que o InputManager está em execução
		bool running;

		/// Denota se o InputManager deve notificar seus listeners.
		bool notifying;

		/// Denota se o InputManager deve notificar as aplicações
		bool notifyingApp;

		/// Buffer que armazena os eventos que devem ser processados e despachados.
		IEventBuffer* eventBuffer;

		/// Registra o tempo de ocorrencia do último evento
		double lastEventTime;

		/// Utilizado para armazenar o tempo do evento atual
		double timeStamp;

		/// Armazena o intervalo minimo para detectar eventos imperativos
		double imperativeIntervalTime;

		/// Armazena o intervalo minimo para detectar eventos declarativos
		double declarativeIntervalTime;

		/// Posição do eixo X atual
		int currentXAxis;

		/// Posição do eixo Y atual
		int currentYAxis;

		/// Valor máximo do eixo X
		int maxX;

		/// Valor máximo do eixo Y
		int maxY;

		/// Controla acesso concorrente à lista actionsToAppListeners.
		pthread_mutex_t actAppMutex;

		/// Controla acesso concorrente à lista actionsToInpListeners.
		pthread_mutex_t actInpMutex;

		/// Controla acesso concorrente à lista applicationListeners.
		pthread_mutex_t appMutex;

		// Funções

		/**
		 * Constrói a instância única do EnhancedInputManager
		 */
		EnhancedInputManager();

		/**
		 * Destrói o EnhancedInputManager.
		 */
		~EnhancedInputManager();

		/**
		 * Lê do arquivo de configuração os intervalos de tempo mínimos para que
		 * dois eventos da mesma tecla sejam considerados.
		 */
		void initializeInputIntervalTime();


	public:
		/**
	 	 * Esvazia as listas de listeners e deleta o buffer de eventos.
	 	 */
		virtual void release();

		/**
	 	 * Acessa a instância única do EnhancedInputManager.
	 	 */
		static EnhancedInputManager* getInstance();

		/**
	 	 * Registra um observador de eventos.
	 	 * @param listener O observador a ser registrado.
	 	 * @param events Conjunto de eventos de interesse.
	 	 */
		virtual void addInputEventListener(
				IInputEventListener* listener, set<int>* events=NULL);

		/**
	 	 * Remove um observador de eventos.
	 	 * @param listener O observador a ser removido.
	 	 */
		virtual void removeInputEventListener(IInputEventListener* listener);

	private:
		/**
	 	 * Notifica os observadores da ocorrência de um evento.
	 	 * @param event Evento sendo notificado.
	 	 */
		virtual bool dispatchEvent(IInputEvent* event);

		/**
		 * Notifica os observadores da ocorrência de um evento de aplicação.
		 * @param event Evento sendo notificado.
	     */
		virtual bool dispatchApplicationEvent(IInputEvent* event);

		/**
		 * Atualiza o conjunto eventListeners com base na lista de ações de
		 * adição e remoção de listeners (actionsToInpListeners).
		 */
		virtual void performInputLockedActions();

		/**
	 	 * Atualiza o conjunto applicationListeners com base na lista de ações
		 * de adição e remoção de listeners (actionsToAppListeners).
		 */
		virtual void performApplicationLockedActions();

	public:
		/**
		 * Registra um observador de eventos de aplicação.
		 * @param listener O observador a ser registrado.
		 */
		virtual void addApplicationInputEventListener(
				IInputEventListener* listener);

		/**
		 * Remove um observador de eventos de aplicação.
		 * @param listener O observador a ser removido.
		 */
		virtual void removeApplicationInputEventListener(
				IInputEventListener* listener);

		/**
		 * Permite a notificação da ocorrência de um evento.
		 * @param event O evento ocorrido.
		 */
		virtual void postEvent(IInputEvent* event);

		/**
		 * Permite a notificação a ocorrência de um evento.
		 * @param keyCode Tecla pressionada.
		 */
		virtual void postEvent(int keyCode);

		/**
		 * Seta as coordenadas do ponteiro do mouse.
		 * @param x O valor da coordenada x.
		 * @param y O valor da coordenada y.
		 * @param z O valor da coordenada z.
		 */
		virtual void setAxisValues(int x, int y, int z);

		/**
		 * Seta as coordenadas que limitam a posição do ponteiro do mouse. Ou
		 * seja, as coordenadas do ponteiro do mouse nunca podem ser maiores do
		 * que as setadas por essa função.
		 * @param x O valor limite da coordenada x.
		 * @param y O valor limite da coordenada y.
		 * @param z O valor limite da coordenada z.
		 */
		virtual void setAxisBoundaries(int x, int y, int z);

		/**
		 * Acessa a posição atual do ponteiro do mouse na coordenada x.
		 * @return A posição atual do ponteiro do mouse na coordenada x.
		 */
		virtual int getCurrentXAxisValue();

		/**
		 * Acessa a posição atual do ponteiro do mouse na coordenada y.
		 * @return A posição atual do ponteiro do mouse na coordenada y.
		 */
		virtual int getCurrentYAxisValue();

	private:
		/**
		 * Laço infinito, enquanto o gerenciador de eventos estiver em execução,
		 * no qual os observadores são notificados da ocorrência de eventos.
		 */
		virtual void run();

	protected:
		// Para tratamento do InputMode

		/**
		 * Alterna para o próximo modo de entrada registrado, caso algum outro
		 * modo exista.
		 */
		void nextInputMode();

		// Para tratamento de eventos multimodais.

		/**
		 * Notifica os observadores da ocorrência de um evento multimodal.
		 * @param multimodalInputEvent Evento sendo notificado.
		 */
		virtual bool dispatchMultimodalEvent(
				IMultimodalInputEvent* multimodalInputEvent);

		/**
		 * Atualiza o conjunto multimodalEventListeners com base na lista de
		 * ações de adição e remoção de listeners (actionsToMultimodalListeners).
		 */
		virtual void performMultimodalInputLockedActions();

	public:

		// Para tratamento do InputMode

		/**
		 * Adicina um novo modo de entrada.
		 * @param inputModeId Id do novo modo de entrada.
		 * @param inputMode Novo modo de entrada.
		 * @throw BadArgumentException se inputMode for nulo.
		 * @throw DuplicateException se outro modo já adicionado possuir
		 * inputModeName como identificador.
		 */
		virtual void addInputMode(
				string inputModeId, IInputMode* inputMode);

		/**
		 * Remove um modo de entrada previamente adicionado.
		 * @param inputModeId Id do modo de entrada a ser removido.
		 * @return O modo de entrada removido ou NULL caso o modo não seja
		 * encontrado.
		 * @throw BadArgumentException inputModeName for o identificador do modo
		 * de entrada atual.
		 */
		virtual IInputMode* removeInputMode(string inputModeId);

		/**
		 * Acessa um modo de entrada previamente adicionado.
		 * @param inputModeId Id do modo de entrada a ser acessado.
		 * @return O modo de entrada ou NULL caso o modo não seja encontrado.
		 */
		virtual IInputMode* getInputMode(string inputModeId);

		/**
		 * Atvia um modo de entrada previamente adicionado.
		 * @param inputModeId Id do modo de entrada a ser ativado.
		 */
		virtual void gotoInputMode(string inputModeId);


		// Para tratamento de eventos multimodais.

		/**
		 * Registra um observador de eventos multimodais.
		 * @param listener O observador a ser registrado.
		 */
		virtual void addMultimodalInputEventListener(
				IMultimodalInputEventListener* listener);

		/**
		 * Remove um observador de eventos multimodais.
		 * @param listener O observador a ser removido.
		 */
		virtual void removeMultimodalInputEventListener(
				IMultimodalInputEventListener* listener);

		/**
		 * Permite a notificação  a ocorrência de um evento multimodal.
		 * @param multimodalInputEvent O evento multimodal ocorrido.
		 */
		virtual void postMultimodalEvent(
				IMultimodalInputEvent* multimodalInputEvent);

		/**
		 * Permite a notificação da ocorrência de um evento multimodal na forma
		 * de uma string contendo um xml. Essa função faz o parser do xml, monta
		 * um objeto de evento multimodal e chama o método void
		 * postMultimodalEvent(IMultimodalInputEvent* multimodalInputEvent)
		 * @param xml O evento multimodal ocorrido em formato xml.
		 */
		virtual void postMultimodalEvent(string xml);

		// Para manipulação de teclas

		/**
		 * Adiciona um novo código de tecla.
		 * @param codeStr Identificador da nova tecla.
		 * @return Código atribuido à tecla.
		 */
		virtual int addCode(string codeStr);

		/**
		 * Remove uma tecla do conjunto de teclas mapeadas.
		 * @param code Código da tecla a ser removida.
		 * @return true se a tecla foi removida com sucesso; false caso contrário.
		 */
		virtual bool removeCode(int code);

		/**
		 * Remove uma tecla do conjunto de teclas mapeadas.
		 * @param codeStr Identificador a ser removida.
		 * @return true se a tecla foi removida com sucesso; false caso contrário.
		 */
		virtual bool removeCode(string codeStr);

		/**
		 * Verifica se uma dada tecla existe no conjunto de teclas mapeadas.
		 * @param code Código da tecla a ser chegada.
		 * @return true se a tecla existir; false caso contrário.
		 */
		virtual bool hasCode(int code);

		/**
		 * Verifica se uma dada tecla existe no conjunto de teclas mapeadas.
		 * @param codeStr Identificador da tecla a ser chegada.
		 * @return true se a tecla existir; false caso contrário.
		 */
		virtual bool hasCode(string codeStr);

		/**
		 * Para permitir que a main fique travada mesmo sem estar recebendo
		 * um fluxo de dados ou sem existir uma aplicação ncl rodando
		 */
		virtual void waitForUnlockCondition();

};

}
}
}
}
}
}

#endif /* ENHANCEDINPUTMANAGER_H */
