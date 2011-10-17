/*************************************************************************
 * Copyright (c) 2008 Hewlett-Packard Development Company, L.P.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 **************************************************************************/
/************************************************************************
 * SVN MACROS
 *
 * $Revision: 245 $
 * $Author: mnab $
 * $LastChangedDate: 2008-07-04 15:49:12 +0530 (Fri, 04 Jul 2008) $
 ************************************************************************************/
/************************************************************************
 * FILE DESCR: TraceFormat.h
 * AUTHOR:     Birendra
 * DATE:       24th July 2007
 * Author		Date			Description of change
 ************************************************************************/
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
/*******************************************************************************
As seguintes modificações foram feitas em cima do código original:
- Acréscimo dos namespaces.
- Acréscimo de comentários
- Remoção do método dependente do TinyXML
*******************************************************************************/
#ifndef TRACEFORMAT_H
#define TRACEFORMAT_H

#include <vector>
#include "Channel.h"
//#include "tinyxml.h"

using namespace std;

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{
namespace inkmllib{


enum CHANNEL{X,Y,F,S,UNKNOWN};

/**
 * Classe que representa o formato de um trace de um InkML.
 * Mais informações em http://sourceforge.net/apps/trac/inkmltk/wiki/InkMLLib
 */
class TraceFormat
{
public:
	/// Id do traceFormat
	char *id;

	/**
	* Desc - By default only X and Y channels are present
	*        This flag is to know whether pressure is present or not
	**/
	bool channelFPresent;

	/// Lista de channels
	vector<Channel> *vectChannel;

	/// Lista de channels
	vector<CHANNEL> *vectChannelOrder;

	/**
	* Desc - Constructor
	**/
	TraceFormat();

	/**
	 * Construtor
	 * @param id do traceFormat.
	 */
	TraceFormat(char *id);

	/**
	 * Adiciona um channel.
	 * @param c Channel a ser adicionado.
	 */
	void addChannelOrder(CHANNEL c)
	{
		this->vectChannelOrder->push_back(c);
	}

	/**
	 * Acessa o ID do traceFormat.
	 * @return id do traceFormat.
	 */
	char* getID()
	{
		return this->id;
	}

	/**
	 * Acessa um determinado channel
	 * @param index Índice do channel a ser acessado
	 * @return O channel
	 */
	CHANNEL getChannelAtIndex(int index);

	/**
	 * Adiciona um channel.
	 * @param c Channel a ser adicionado.
	 */
	void addChannelOrder(char *c);
//	void initializeFromXMLNode(TiXmlNode *node);
};

}
}
}
}
}
}
}

#endif // TRACEFORMAT_H
