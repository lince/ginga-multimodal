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
 * @file MultimodalTest.h
 * @author Diogo de Carvalho Pedrosa
 * @author José Augusto Costa Martins Júnior
 * @date 29-01-10
 */

#ifndef MULTIMODALTEST_H
#define MULTIMODALTEST_H

#include "mmi/IMultimodalInputEventListener.h"
#include "mmi/IMultimodalInputEvent.h"
using namespace ::br::ufscar::lince::ginga::core::mmi;

#include "system/io/interface/input/IInputEventListener.h"
#include "system/io/interface/output/ISurface.h"
#include "system/io/interface/output/IWindow.h"
using namespace ::br::pucrio::telemidia::ginga::core::system::io;

#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;

using namespace std;

namespace br {
namespace usp {
namespace intermidia {
namespace ginga {
namespace multimodaltest {

class MultimodalTest : public IMultimodalInputEventListener,
		public IInputEventListener {

	protected:
		///Instância única de MultimodalTest.
		static MultimodalTest* _instance;

		/// Componente auxiliar para realização de logs.
		HLoggerPtr logger;

		IWindow* win;
		ISurface* surface;
		IColor* inkColor;

		/**
		 * Constrói um objeto MultimodalTest.
		 *
		 */
		MultimodalTest();

		/**
		 * Inicializa a janela que é usada para desenhar na tela.
		 */
		void initializeWindow();

	public:
		/**
		 * Acessa a instância única do objeto MultimodalTest.
		 * @return Instância de MultimodalTest.
		 */
		static MultimodalTest* getInstance();
			
		/**
		 * Destói o objeto MultimodalTest.
		 */
		~MultimodalTest();
		
		/**
		 * Chamada quando um evento multimodal é disparado.
		 * @param Evento Multimodal.
		 * @return true caso as notificações do evento multimodal devam
		 * continuar.
		 * @return false caso as notificações do evento multimodal devam parar.
		 */
		bool multimodalUserEventReceived(IMultimodalInputEvent* ev);

		/**
		 * Chamada quando um evento é disparado.
		 * @param Evento.
		 * @return true caso as notificações do evento devam continuar.
		 * @return false caso as notificações do evento devam parar.
		 */
		bool userEventReceived(IInputEvent* ev);
		
		/**
		 * Registra um observador.
		 *
		 */
		void registerListener();

		/**
		 * Usa métodos do DirectFB para mostrar dados de tinta na tela.
		 *
		 */
		void showInk(Ink *ink);
};

}
}
}
}
}

#endif //MULTIMODALTEST_H
