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
 * FILE DESCR: InkMLParser.h - interface of the InkMLParser class
 * CONTENTS:   
 * AUTHOR:     Birendra
 * DATE:       16th June 2007
 * CHANGE HISTORY:
 * Author		Date			Description of change
 * Balaji M N A 18-June-08      Moved class definition methods in .cpp file.
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
- Troca dos código do TinyXML por código do Xerces
- Acrécimo do logger
- Acréscimo dos namespaces.
*******************************************************************************/
#ifndef INKMLPARSER_H
#define INKMLPARSER_H

#include "Ink.h"

#include <xercesc/dom/DOM.hpp>
XERCES_CPP_NAMESPACE_USE

#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;


namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{
namespace inkmllib{

/**
 * Classe responsável por realizar o parser de um InkML, utilizando a biblioteca
 * xerces.
 * Mais informações em http://sourceforge.net/apps/trac/inkmltk/wiki/InkMLLib
 */
class InkMLParser
{
protected:
	/// Responsável pelo controle das mensagens de log
	HLoggerPtr logger;

	/**
	 * Faz o parser de uma tag "definitions" e acrescenta os dados no objeto
	 * da Classe Ink recebido.
	 *
	 * @param e DOMElement a ser parseado
	 * @param objInk Instância de Ink que receberá os dados parseados
	 */
	void parseDefinitions(DOMElement *e, Ink *objInk);

public:
	/**
	 * Construtor da classe InKMLParser.
	 */
	InkMLParser();

	/**
	 * Faz o parser de uma tag "ink" e retorna um objeto da Classe Ink.
	 *
	 * @param inkMLData DOMElement a ser parseado
	 * @param retCode Código de erro, caso ocorra, ou NoError em caso de sucesso
	 */
	Ink* parse(DOMElement *inkMLData, InkMLError *retCode);

	/**
	 * Faz o parser de uma tag "trace" e acrescenta os dados no objeto da Classe
	 * Ink recebido.
	 *
	 * @param child DOMElement a ser parseado
	 * @param objInk Instância de Ink que receberá os dados parseados
	 */
	void insertTrace(DOMElement *child, Ink *objInk);
};

}
}
}
}
}
}
}

#endif // INKMLPARSER_H
