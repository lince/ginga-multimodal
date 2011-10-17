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
* @file IMultimodalInputEvent.h
* @author Diogo de Carvalho Pedrosa
* @author José Augusto Costa Martins Júnior
* @date 29-01-10
*/
#ifndef IMULTIMODALINPUTEVENT_H
#define IMULTIMODALINPUTEVENT_H

#include <map>
#include <string>
using namespace std;

#include "File.h"
#include "Acceleration.h"

#include "Ink.h"
using namespace ::br::ufscar::lince::ginga::core::mmi::inkmllib;

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{

/**
 * Interface que representa um evento multimodal.
 */
class IMultimodalInputEvent {
	public:
		/**
		 * Destrói o IMultimodalInputEvent.
		 *
		 */
		virtual ~IMultimodalInputEvent(){};

		/**
		 * Acessa o id do evento multimodal.
		 * @return id do evento.
		 */
		virtual string getId() = 0;

		/**
		 * Define o id do evento multimodal.
		 * @param id do evento.
		 */
		virtual void setId(string id) = 0;

		/**
		 * Adiciona um valor ao evento multimodal.
		 * @param name do valor a ser adicionado.
		 * @param value conteúdo do valor a ser adicionado.
		 */
		virtual void addValue(string name, void* value) = 0;

		/**
		 * Remove um valor do evento multimodal.
		 * @param name Nome do valor a ser removido.
		 * @return true caso o valor tenha sido removido.
		 * @return false caso o valor não tenha sido removido.
		 */
		virtual bool removeValue(string name) = 0;

		/**
		 * Acessa os valores contidos no evento multimodal.
		 * @return mapa com os pares nome,valor do evento multimodal.
		 */
		virtual map<string, void*>* getMultimodalValuesMap() = 0;

		/**
		 * Define os valores contidos dentro do evento multimodal.
		 * @param map Mapa com o par <nome,valor>.
		 */
		virtual void setMultimodalValuesMap(map<string, void*>* map) = 0;

		/**
		 * Acessa as strings do evento multimodal.
		 * @return vetor de strings.
		 */
		virtual vector<string>* getStrings() = 0;

		/**
		 * Adiciona uma string ao evento multimodal.
		 * @param str String a ser adicionada no evento.
		 */
		virtual void addString(string str) = 0;

		/**
		 * Acessa o objeto ink do evento multimodal.
		 * @return Objeto com os dados de tinta do evento.
		 */
		virtual Ink* getInk() = 0;

		/**
		 * Define o objeto ink do evento multimodal.
		 * @param ink objeto com os dados de tinta do evento.
		 */
		virtual void setInk(Ink* ink) = 0;

		/**
		 * Acessa o objeto acceleration do evento multimodal.
		 * @return objeto Acceleration com os dados de aceleração do evento.
		 */
		virtual Acceleration* getAcceleration() = 0;

		/**
		 * Define o objeto acceleration do evento multimodal.
		 * @param accel objeto com os dados de aceleração do evento.
		 */
		virtual void setAcceleration(Acceleration* accel) = 0;

		//TODO get e set para voice

		/**
		 * Acessa os arquivos do evento multimodal.
		 * @return vetor de arquivos.
		 */
		virtual vector<File*>* getBinaries() = 0;

		/**
		 * Adiciona um arquivo ao evento multimodal.
		 * @param f Arquivo a ser adicionad no evento.
		 */
		virtual void addFile(File* f) = 0;
};

}
}
}
}
}
}

#endif /*IMULTIMODALINPUTEVENT_H*/
