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
* @file EventParser.h
* @author Diogo de Carvalho Pedrosa
* @date 14-05-10
*/

#ifndef EVENTPARSER_H
#define EVENTPARSER_H

#include "IMultimodalInputEvent.h"

#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;

#include <xercesc/dom/DOM.hpp>
XERCES_CPP_NAMESPACE_USE


namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{

/**
 * Classe que faz o parser de um XML e gera um evento multimodal.
 * Deve ser singleton??? Tomar cuidado com as chamadas de Initialize e Terminate
 */
class EventParser {

	protected:
		/// Evento
		IMultimodalInputEvent* multimodalEvent;

		/// Responsável pelo controle das mensagens de log
		HLoggerPtr logger;

	public:

		/**
		 * Construtor
		 */
		EventParser();

		/**
		 * Destrutor
		 */
		~EventParser();

		/**
		 * Faz o parser de um xml.
		 * @param xml Conteúdo do xml.
		 */
		virtual IMultimodalInputEvent* parse(string xml);

		/**
		 * Faz o parser do elemento raiz do xml.
		 * @param e Elemento raiz do xml.
		 */
		virtual void parseRoot(DOMElement* e);

		/**
		 * Faz o parser do cabeçalho do xml.
		 * @param e Cabeçalho do xml.
		 */
		virtual void parseHead(DOMElement* e);

		/**
		 * Faz o parser do corpo do xml.
		 * @param e Corpo do xml.
		 */
		virtual void parseBody(DOMElement* e);
};

}
}
}
}
}
}

#endif /*EVENTPARSER_H*/
