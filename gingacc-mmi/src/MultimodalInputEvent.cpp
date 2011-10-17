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
* @file MultimodalInputEvent.cpp
* @author Diogo de Carvalho Pedrosa
* @author José Augusto Costa Martins Júnior
* @date 29-01-10
*/
#include "../include/MultimodalInputEvent.h"

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{

	MultimodalInputEvent::MultimodalInputEvent(string id) {
		this->id = id;
		multimodalValuesMap = new map<string, void*>;
		strings = new vector<string>;
		binaries = new vector<File*>;
	}

	MultimodalInputEvent::~MultimodalInputEvent() {
		delete multimodalValuesMap;
		multimodalValuesMap = NULL;

		delete strings;
		strings = NULL;

		delete binaries;
		binaries = NULL;
	}

	string MultimodalInputEvent::getId() {
		return id;
	}

	void MultimodalInputEvent::setId(string id) {
		this->id = id;
	}

	void MultimodalInputEvent::addValue(string name, void* value) {
		// TODO Não permitir duas vezes o mesmo name.
		multimodalValuesMap->insert(pair<string,void*>(name, value));
	}

	bool MultimodalInputEvent::removeValue(string name) {
		//TODO
	}

	map<string, void*>* MultimodalInputEvent::getMultimodalValuesMap() {
		return multimodalValuesMap;
	}

	void MultimodalInputEvent::setMultimodalValuesMap(map<string, void*>* map){
		multimodalValuesMap = map;
	}

	vector<string>* MultimodalInputEvent::getStrings() {
		return strings;
	}

	void MultimodalInputEvent::addString(string str) {
		strings->push_back(str);
	}

	Ink* MultimodalInputEvent::getInk() {
		return this->ink;
	}

	void MultimodalInputEvent::setInk(Ink* ink) {
		this->ink = ink;
	}

	Acceleration* MultimodalInputEvent::getAcceleration() {
		return this->accel;
	}

	void MultimodalInputEvent::setAcceleration(Acceleration* accel) {
		this->accel = accel;
	}

	//TODO get e set para voice

	vector<File*>* MultimodalInputEvent::getBinaries() {
		return binaries;
	}

	void MultimodalInputEvent::addFile(File* f) {
		this->binaries->push_back(f);
	}
}
}
}
}
}
}
