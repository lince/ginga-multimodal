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
* @file MultimodalInputEvent.h
* @author Diogo de Carvalho Pedrosa
* @author José Augusto Costa Martins Júnior
* @date 29-01-10
*/
#ifndef MULTIMODALINPUTEVENT_H
#define MULTIMODALINPUTEVENT_H

#include "IMultimodalInputEvent.h"

#include "File.h"

#include <map>
#include <vector>
#include <string>
using namespace std;

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{

/**
 * Classe que representa um evento multimodal.
 */
class MultimodalInputEvent : public IMultimodalInputEvent {
	
	protected:
		/// Variável para definição do id do evento ocorrido
		string id;

		/// Objeto que contém um mapa de chaves e valores
		map<string, void*>* multimodalValuesMap;

		/// Vetor de strings
		vector<string>* strings;

		/// Ponteiro par um objeto que guarda os dados de tinta
		Ink* ink;

		/// Ponteiro par um objeto que guarda os dados de acelerômetro
		Acceleration* accel;

		/// Ponteiro par um objeto que guarda os dados de voz
//		Voice* voice;

		/// Vetor de arquivos
		vector<File*>* binaries;

	public:
	
		/**
		 * Cria um evento do tipo multimodal apenas definindo seu id
		 * @param id do evento
		 */
		MultimodalInputEvent(string id);

		/**
		 * Destrói o objeto do evento multimodal
		 */
		~MultimodalInputEvent();
	
		/**
		 * Acessa o id do evento multimodal.
		 * @return id do evento.
		 */
		virtual string getId();

		/**
		 * Define o id do evento multimodal.
		 * @param id do evento.
		 */
		virtual void setId(string id);

		/**
		 * Adiciona novos valores ao evento multimodal
		 * @param name Nome do valor
		 * @param value O valor
		 */
		void addValue(string name, void* value);
	
		/**
		 * Remove valores do evento multimodal
		 * @param name Nome do valor
		 * @return true caso o valor tenha sido removido
		 * @return false caso o valor não tenha sido removido
		 */
		bool removeValue(string name);
	
		/**
		 * Acessa os valores contidos no evento multimodal
		 * @return mapa com todos os valores contidos no evento multimodal
		 */
		map<string, void*>* getMultimodalValuesMap();

		/**
		 * Define um mapa com valores ao evento multimodal
		 * @param map Mapa com todos os valores a serem inseridos no evento multimodal
		 */
		void setMultimodalValuesMap(map<string, void*>* map);

		/**
		 * Acessa as strings do evento multimodal.
		 * @return vetor de strings.
		 */
		virtual vector<string>* getStrings();

		/**
		 * Adiciona uma string ao evento multimodal.
		 * @param str String a ser adicionada no evento.
		 */
		virtual void addString(string str);

		/**
		 * Acessa o objeto ink do evento multimodal.
		 * @return ink objeto com os dados de tinta do evento.
		 */
		virtual Ink* getInk();

		/**
		 * Define o objeto ink do evento multimodal.
		 * @param ink objeto com os dados de tinta do evento.
		 */
		virtual void setInk(Ink* ink);

		/**
		 * Acessa o objeto acceleration do evento multimodal.
		 * @return objeto Acceleration com os dados de aceleração do evento.
		 */
		virtual Acceleration* getAcceleration();

		/**
		 * Define o objeto acceleration do evento multimodal.
		 * @param accel objeto com os dados de aceleração do evento.
		 */
		virtual void setAcceleration(Acceleration* accel);

		//TODO get e set para voice

		/**
		 * Acessa os arquivos do evento multimodal.
		 * @return vetor de arquivos.
		 */
		virtual vector<File*>* getBinaries();

		/**
		 * Adiciona um arquivo ao evento multimodal.
		 * @param f Arquivo a ser adicionad no evento.
		 */
		virtual void addFile(File* f);
};

}
}
}
}
}
}

#endif /*MULTIMODALINPUTEVENT_H*/
