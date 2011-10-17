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
mgp@icmc.usp.br
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
* @file CommunicationManager.h
* @author Diogo de Carvalho Pedrosa
* @author José Augusto Costa Martins Júnior
* @date 27-05-10
*/
#ifndef COMMUNICATIONMANAGER_H
#define COMMUNICATIONMANAGER_H

#include <ginga/system/thread/Thread.h>
using namespace ::br::pucrio::telemidia::ginga::core::system::thread;

#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;

#include <avahi-client/client.h>
#include <avahi-client/publish.h>

#include <avahi-common/alternative.h>
#include <avahi-common/simple-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>
#include <avahi-common/timeval.h>

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{

/**
 * Classe responsável pelo recebimento via rede, usando o protocolo ZeroConf,
 * de mensagens XML representando eventos multimodais.
 */
class CommunicationManager : public Thread {
	
	protected:
		/// Instância única
		static CommunicationManager* _instance;

		/// Responsável pelo controle das mensagens de log
		HLoggerPtr logger;

		/// TODO Comentar
		AvahiEntryGroup *group;

		/// TODO Comentar
		AvahiSimplePoll *simplePoll;

		/// TODO Comentar
		char *name;

	protected:
		/**
		 * Constrói a instância única
		 */
		CommunicationManager();

		/**
		 * Destrói o CommunicationManager.
		 */
		~CommunicationManager();

	public:
		/**
	 	 * Acessa a instância única.
	 	 */
		static CommunicationManager* getInstance();

		/**
		 * Laço infinito em que dados representando um evento são esperados.
		 * Sempre que um fluxo de dados com o conteúdo de um xml chega, o método
		 * postMultimodalEvent(string xml) é chamado.
		 */
		virtual void run();

		/**
		 * Inicializa um socket e espera por conexões de dispositivos externos.
		 * @return 0 em caso de sucesso e um valor diferente em caso de erro.
		 */
		int startSocket();

		/**
		 * TODO Comentar
		 */
		static void clientCallback(AvahiClient* c, AvahiClientState state,
				AVAHI_GCC_UNUSED void* userdata);

		/**
		 * TODO Comentar
		 */
		static void entryGroupCallback(AvahiEntryGroup* g,
				AvahiEntryGroupState state, AVAHI_GCC_UNUSED void* userdata);

		/**
		 * Só é chamada "internamente", mas não pode ser protected porque é
		 * chamada pelas funções de callback, que não pertencem a classe.
		 *
		 * TODO Terminar de comentar
		 */
		void createServices(AvahiClient* c);

};

}
}
}
}
}
}

#endif /*COMMUNICATIONMANAGER_H*/
