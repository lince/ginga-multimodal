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
******************************************************************************
This file is part of the declarative environment of middleware Ginga (Ginga-NCL)

Copyright (C) 2009 UFSCar/Lince, Todos os Direitos Reservados.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 2 as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License version 2 for more
details.

You should have received a copy of the GNU General Public License version 2
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

For further information contact:
lince@dc.ufscar.br
mgp@icmc.usp.br
http://www.ncl.org.br
http://www.ginga.org.br
http://lince.dc.ufscar.br
*******************************************************************************/

/**
* @file IEnhancedInputManager.h
* @author Caio Viel
* @author Diogo de Carvalho Pedrosa
* @author José Augusto Costa Martins Júnior
* @date 29-01-10
*/
#ifndef IENHANCEDINPUTMANAGER_H_
#define IENHANCEDINPUTMANAGER_H_

#include <ginga/system/io/IInputManager.h>
using namespace ::br::pucrio::telemidia::ginga::core::system::io;

#include "IInputMode.h"

#include "IMultimodalInputEvent.h"
#include "IMultimodalInputEventListener.h"

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{

/**
 * Interface do gerenciador de eventos estendido para dar suporte a eventos
 * multimodais.
 */
class IEnhancedInputManager : public IInputManager {

	public:
		~IEnhancedInputManager() {};

		// Para tratamento do InputMode

		/**
		 * Adicina um novo modo de entrada.
		 * @param inputModeName Nome do novo modo de entrada.
		 * @param inputMode Novo modo de entrada.
		 * @throw BadArgumentException se inputMode for nulo.
		 * @throw DuplicateException se outro modo já adicionado possuir
		 * inputModeName como identificador.
		 */
		virtual void addInputMode(string inputModeName, 
				IInputMode* inputMode)=0;

		/**
		 * Remove um modo de entrada previamente adicionado.
		 * @param inputModeName Nome do modo de entrada a ser removido.
		 * @return O modo de entrada removido ou NULL caso o modo não seja
		 * encontrado.
		 * @throw BadArgumentException inputModeName for o identificador do modo
		 * de entrada atual.
		 */
		virtual IInputMode* removeInputMode(string inputModeName)=0;

		/**
		 * Acessa um modo de entrada previamente adicionado.
		 * @param inputModeName Nome do modo de entrada a ser acessado.
		 * @return O modo de entrada ou NULL caso o modo não seja encontrado.
		 */
		virtual IInputMode* getInputMode(string inputModeName)=0;

		/**
		 * Atvia um modo de entrada previamente adicionado.
		 * @param inputModeName Nome do modo de entrada a ser ativado.
		 */
		virtual void gotoInputMode(string inputModeName)=0;

		/**
		 * Permite a notificação  a ocorrência de um evento multimodal.
		 * @param multimodalInputEvent O evento multimodal ocorrido.
		 */
		virtual void postMultimodalEvent(
				IMultimodalInputEvent* multimodalInputEvent)=0;

		/**
		 * Registra um observador de eventos multimodais.
		 * @param listener O observador a ser registrado.
		 */
		virtual void addMultimodalInputEventListener(
				IMultimodalInputEventListener* listener)=0;

		/**
		 * Remove um observador de eventos multimodais.
		 * @param listener O observador a ser removido.
		 */
		virtual void removeMultimodalInputEventListener(
				IMultimodalInputEventListener* listener)=0;

		/**
		 * Adiciona um novo código de tecla.
		 * @param codeStr Identificador da nova tecla.
		 * @return Código atribuido à tecla.
		 */
		virtual int addCode(string codeStr)=0;

		/**
		 * Remove uma tecla do conjunto de teclas mapeadas.
		 * @param code Código da tecla a ser removida.
		 * @return true se a tecla foi removida com sucesso; false caso contrário.
		 */
		virtual bool removeCode(int code)=0;

		/**
		 * Remove uma tecla do conjunto de teclas mapeadas.
		 * @param codeStr Identificador a ser removida.
		 * @return true se a tecla foi removida com sucesso; false caso contrário.
		 */
		virtual bool removeCode(string codeStr)=0;

		/**
		 * Verifica se uma dada tecla existe no conjunto de teclas mapeadas.
		 * @param code Código da tecla a ser chegada.
		 * @return true se a tecla existir; false caso contrário.
		 */
		virtual bool hasCode(int code)=0;

		/**
		 * Verifica se uma dada tecla existe no conjunto de teclas mapeadas.
		 * @param codeStr Identificador da tecla a ser chegada.
		 * @return true se a tecla existir; false caso contrário.
		 */
		virtual bool hasCode(string codeStr)=0;


		//Funções Herdadas de IInputManager.h
		/*virtual void release()=0;
		virtual void addInputEventListener(
				IInputEventListener* listener, set<int>* events=NULL)=0;

		virtual void removeInputEventListener(IInputEventListener* listener)=0;

		virtual void addApplicationInputEventListener(
				IInputEventListener* listener)=0;

		virtual void removeApplicationInputEventListener(
				IInputEventListener* listener)=0;

		virtual void postEvent(IInputEvent* event)=0;
		virtual void postEvent(int keyCode)=0;

		virtual void setAxisValues(int x, int y, int z)=0;
		virtual void setAxisBoundaries(int x, int y, int z)=0;
		virtual int getCurrentXAxisValue()=0;
		virtual int getCurrentYAxisValue()=0;*/

		/**
		 * Permite a notificação da chegada de um evento multimodal através
		 * do protocolo em xml.
		 * @param xml Conteúdo do xml representando o evento multimodal, ou
		 * caminho para o arquivo. Caso a string comece com o caracter '<', será
		 * considerado que trata-se do conteúdo, caso contrário, que trata-se
		 * do caminho para o arquivo.
		 */
		virtual void postMultimodalEvent(string xml)=0;

		/**
		 * Para permitir que a main fique travada mesmo sem estar recebendo
		 * um fluxo de dados ou sem existir uma aplicação ncl rodando
		 */
		virtual void waitForUnlockCondition() = 0;

};

}
}
}
}
}
}

/**
 * Função que permite a criação de uma instância de IEnhancedInputManager.
 */
typedef ::br::ufscar::lince::ginga::core::mmi::IEnhancedInputManager*
		EnhancedInputManagerCreator();

/**
 * Função que permite a destruição de uma instância de IEnhancedInputManager.
 */
typedef void EnhancedInputManagerDestroyer(
		::br::ufscar::lince::ginga::core::mmi::IEnhancedInputManager* eim);

#endif /*IENHANCEDINPUTMANAGER_H_*/
